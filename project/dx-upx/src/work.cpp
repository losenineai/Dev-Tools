/* work.cpp -- main driver

   This file is part of the UPX executable compressor.

   Copyright (C) 1996-2018 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1996-2018 Laszlo Molnar
   All Rights Reserved.

   UPX and the UCL library are free software; you can redistribute them
   and/or modify them under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.
   If not, write to the Free Software Foundation, Inc.,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   Markus F.X.J. Oberhumer              Laszlo Molnar
   <markus@oberhumer.com>               <ezerotven+github@gmail.com>
 */

#include "conf.h"
#include "file.h"
#include "packmast.h"
#include "packer.h"
#include "ui.h"
#include "report_builder.h"

#include <string>

#include "elf_utils.h"
#include "sign_verify.h"
#include "anti_debug.h"
#include "log.h"
#include "elf_shellcode.h"
#include "integrity_check.h"
#include "anti_debugging.h"
#include "anti_dump.h"
#include "expand_elf.h"

#if (ACC_OS_DOS32) && defined(__DJGPP__)
#define USE_FTIME 1
#elif (ACC_OS_WIN32 && ACC_CC_MWERKS) && defined(__MSL__)
#include <utime.h>
#define USE_UTIME 1
#elif ((ACC_OS_WIN32 || ACC_OS_WIN64) && (ACC_CC_INTELC || ACC_CC_MSC))
#define USE__FUTIME 1
#elif (HAVE_UTIME)
#define USE_UTIME 1
#endif

#if !defined(SH_DENYRW)
#define SH_DENYRW (-1)
#endif
#if !defined(SH_DENYWR)
#define SH_DENYWR (-1)
#endif

// ignore errors in some cases and silence __attribute__((__warn_unused_result__))
#define IGNORE_ERROR(var) ACC_UNUSED(var)

/*************************************************************************
// process one file
**************************************************************************/

void do_one_file(const char *iname, char *oname) {
    int r;
    struct stat st;
    memset(&st, 0, sizeof(st));
#if (HAVE_LSTAT)
    r = lstat(iname, &st);
#else
    r = stat(iname, &st);
#endif
    if (r != 0) {
        if (errno == ENOENT)
            throw FileNotFoundException(iname, errno);
        else
            throwIOException(iname, errno);
    }
    if (!(S_ISREG(st.st_mode)))
        throwIOException("not a regular file -- skipped");
#if defined(__unix__)
    // no special bits may be set
    if ((st.st_mode & (S_ISUID | S_ISGID | S_ISVTX)) != 0)
        throwIOException("file has special permissions -- skipped");
#endif
    if (st.st_size <= 0)
        throwIOException("empty file -- skipped");
    if (st.st_size < 512)
        throwIOException("file is too small -- skipped");
    if (!mem_size_valid_bytes(st.st_size))
        throwIOException("file is too large -- skipped");
    if ((st.st_mode & S_IWUSR) == 0) {
        bool skip = true;
        if (opt->output_name)
            skip = false;
        else if (opt->to_stdout)
            skip = false;
        else if (opt->backup)
            skip = false;
        if (skip)
            throwIOException("file is write protected -- skipped");
    }

    InputFile fi;
    fi.st = st;
    fi.sopen(iname, O_RDONLY | O_BINARY, SH_DENYWR);

#if (USE_FTIME)
    struct ftime fi_ftime;
    memset(&fi_ftime, 0, sizeof(fi_ftime));
    if (opt->preserve_timestamp) {
        if (getftime(fi.getFd(), &fi_ftime) != 0)
            throwIOException("cannot determine file timestamp");
    }
#endif

    // open output file
    OutputFile fo;
    if (opt->cmd == CMD_COMPRESS || opt->cmd == CMD_DECOMPRESS) {
        if (opt->to_stdout) {
            if (!fo.openStdout(1, opt->force ? true : false))
                throwIOException("data not written to a terminal; Use '-f' to force.");
        } else {
            char tname[ACC_FN_PATH_MAX + 1];
            if (opt->output_name)
                strcpy(tname, opt->output_name);
            else {
                if (!maketempname(tname, sizeof(tname), iname, ".upx"))
                    throwIOException("could not create a temporary file name");
            }
            if (opt->force >= 2) {
#if (HAVE_CHMOD)
                r = chmod(tname, 0777);
                IGNORE_ERROR(r);
#endif
                r = unlink(tname);
                IGNORE_ERROR(r);
            }
            int flags = O_CREAT | O_WRONLY | O_BINARY;
            if (opt->force)
                flags |= O_TRUNC;
            else
                flags |= O_EXCL;
            int shmode = SH_DENYWR;
#if defined(__MINT__)
            flags |= O_TRUNC;
            shmode = O_DENYRW;
#endif
            // cannot rely on open() because of umask
            // int omode = st.st_mode | 0600;
            int omode = 0600;
            if (!opt->preserve_mode)
                omode = 0666;
            fo.sopen(tname, flags, shmode, omode);
            // open succeeded - now set oname[]
            strcpy(oname, tname);
        }
    }

    // handle command
    PackMaster pm(&fi, opt);
    if (opt->cmd == CMD_COMPRESS)
        pm.pack(&fo);
    else if (opt->cmd == CMD_DECOMPRESS)
        pm.unpack(&fo);
    else if (opt->cmd == CMD_TEST)
        pm.test();
    else if (opt->cmd == CMD_LIST)
        pm.list();
    else if (opt->cmd == CMD_FILEINFO)
        pm.fileInfo();
    else
        throwInternalError("invalid command");

    // copy time stamp
    if (opt->preserve_timestamp && oname[0] && fo.isOpen()) {
#if (USE_FTIME)
        r = setftime(fo.getFd(), &fi_ftime);
        IGNORE_ERROR(r);
#elif (USE__FUTIME)
        struct _utimbuf u;
        u.actime = st.st_atime;
        u.modtime = st.st_mtime;
        r = _futime(fo.getFd(), &u);
        IGNORE_ERROR(r);
#endif
    }

    // close files
    fo.closex();
    fi.closex();

    // rename or delete files
    if (oname[0] && !opt->output_name) {
        // FIXME: .exe or .cof etc.
        if (!opt->backup) {
#if (HAVE_CHMOD)
            r = chmod(iname, 0777);
            IGNORE_ERROR(r);
#endif
            File::unlink(iname);
        } else {
            // make backup
            char bakname[ACC_FN_PATH_MAX + 1];
            if (!makebakname(bakname, sizeof(bakname), iname))
                throwIOException("could not create a backup file name");
            File::rename(iname, bakname);
        }
        File::rename(oname, iname);
    }

    // copy file attributes
    if (oname[0]) {
        oname[0] = 0;
        const char *name = opt->output_name ? opt->output_name : iname;
        UNUSED(name);
#if (USE_UTIME)
        // copy time stamp
        if (opt->preserve_timestamp) {
            struct utimbuf u;
            u.actime = st.st_atime;
            u.modtime = st.st_mtime;
            r = utime(name, &u);
            IGNORE_ERROR(r);
        }
#endif
#if (HAVE_CHMOD)
        // copy permissions
        if (opt->preserve_mode) {
            r = chmod(name, st.st_mode);
            IGNORE_ERROR(r);
        }
#endif
#if (HAVE_CHOWN)
        // copy the ownership
        if (opt->preserve_ownership) {
            r = chown(name, st.st_uid, st.st_gid);
            IGNORE_ERROR(r);
        }
#endif
    }

    UiPacker::uiConfirmUpdate();
}

/*************************************************************************
// process all files from the commandline
**************************************************************************/

static void unlink_ofile(char *oname) {
    if (oname && oname[0]) {
#if (HAVE_CHMOD)
        int r;
        r = chmod(oname, 0777);
        IGNORE_ERROR(r);
#endif
        if (unlink(oname) == 0)
            oname[0] = 0;
    }
}

static void unhook(const char* iname, long dt_init_val){
    if(dt_init_val == -1){
        return;
    }
    LOGD("unhook DT_INIT, dt_init_val:%d", dt_init_val);
    elf_utils elf(iname);
    if(dt_init_val == 0){
        elf.set_dt_type(DT_INIT, DT_NULL);
    }else{
        elf.set_dt_func_val(DT_INIT, dt_init_val);
    }
    elf.update_file();
    return;
}

extern void rand_string(char *s, int len);

char shellcode_key[10];// = {'d','x','_','1','2','3','4','5','\0','\0'};

long dt_init_val = -1;

void do_files(int i, int argc, char *argv[]) {
    if (opt->verbose >= 1) {
        show_head();
        UiPacker::uiHeader();
    }

    report_builder builder(opt->report, opt->pic, opt->orig, "vendor.json", opt->arg0);

    for (; i < argc; i++) {
        infoHeader();

        const char *iname = argv[i];
        char oname[ACC_FN_PATH_MAX + 1];
        oname[0] = 0;
        // chmod(iname, 0777);

        builder.store_u_len_info(iname);
        builder.store_filenames(iname);
        int protected_info = 0;
        
        bool signverify_added = false;
        bool integrity_added = false;
        bool antiida_added = false;
        bool antidebugging_added = false;
        bool antidump_added = false;
        
        bool hasupx = false;
        
        try {
            
            if(opt->o_unix.expand_space){
                expand_elf expand_space(iname, iname);
                expand_space.expand(0x5000);
            }else if(opt->o_unix.android_shlib){
                FILE* fp = fopen(iname, "rb");
                if(fp != NULL){
                    fseek(fp, 0, SEEK_END);
                    int length = ftell(fp);
                    fseek(fp, 0, SEEK_SET);
                    fclose(fp);
                    if(length < 51200){
                        expand_elf expand_space(iname, iname);
                        expand_space.expand(0x4000);
                    }
                }
            }

            bool need_anti_ida = false;
            if (opt->o_unix.anti_ida) {
                anti_debug antidbg(iname);
                need_anti_ida = antidbg.add_empty_sections(50);
                if (need_anti_ida) {
                    LOGD("add empty section ok");
                } else {
                    LOGD("add empty section fail!");
                }
            }
            
            {
                elf_utils elf(iname);
                dt_init_val = elf.get_dt_func_val(DT_INIT);
            }
            
            //generate random key
            rand_string(shellcode_key, 8);
            int expand_count = 0;
            if(opt->o_unix.android_anti_debug){
                expand_count++;
                expand_count++;
            }
            if(opt->o_unix.android_anti_dump){
                expand_count++;
            }
            if(opt->o_unix.android_debug_sighash || opt->o_unix.android_signhash){
                expand_count++;
                expand_count++;
            }
            if(opt->o_unix.android_check_integrity){
                expand_count++;
            }

            if(expand_count>0){
                expand_elf shellcode_expend(iname, iname);
                shellcode_expend.expand(0x1000*expand_count);

                //patching shellcode...
                elf_shellcode shellp(iname, shellcode_expend.get_xct_vaddr(), shellcode_expend.get_xct_offset());

                if ((opt->o_unix.android_signhash != 0 || opt->o_unix.android_debug_sighash != 0)) {
                    //LOGD("adding shellcode...");
                    if (shellp.add_sign_verify_shellcode(opt->o_unix.android_signhash, opt->o_unix.android_debug_sighash)) {
                        signverify_added = true;
                        LOGI("add sign verify ok");
                    }else{
                        LOGE("add sifn verify shellcode fail...");
                    }
                }

                if (opt->o_unix.android_check_integrity) {
                    LOGD("=======integrity checking");
                    if (shellp.add_integerity_check_shellcode()) {
                        integrity_added = true;
                        LOGI("add integrity check ok");
                    }else{
                        LOGE("add integrity check shellcode fail...");
                    }
                }

                if(opt->o_unix.android_anti_dump){
                    if(shellp.add_anti_dump_shellcode()){
                        antidump_added = true;
                        LOGI("add anti dump shellcode ok");
                    }else{
                        LOGD("add anti dump shellcode fail...");
                    }
                }
                
                if(opt->o_unix.android_anti_debug){
                    if(shellp.add_anti_debugging_shellcode()){
                        antidebugging_added = true;
                        LOGI("add anti debugging ok");
                    }else{
                        LOGD("add anti debugging shellcode fail...");
                    }
                }

                if (signverify_added || integrity_added || antidump_added || antidebugging_added) {
                    if(shellp.update()){
                        LOGI("patch shellcode ok");
                    }
                }
            }

            if (opt->o_unix.anti_ida && opt->o_unix.fix_sdk_build) {
                LOGE("anti_ida is against fix_sdk_build.");
                throwNotPacked("anti_ida is against fix_sdk_build, please rm '--anti-ida' and try again.");
                continue;
            }


            //adding anti ida...
            //LOGD("anti_ida...");
            if (opt->o_unix.anti_ida) {
                anti_debug antidbg(iname);
                if (need_anti_ida && antidbg.anti_ida()) {
                    antiida_added = true;
                    LOGD("anti ida ok");
                } else {
                    LOGD("anti ida fail!");
                }
            }

            LOGD("doing one file");
            //adding upx...
            do_one_file(iname, oname);
            hasupx = true;
                        
            if (opt->o_unix.fix_sdk_build) {
                anti_debug antidbg(iname);
                if (antidbg.fix_elf()) {
                    LOGD("fix elf section header ok");
                } else {
                    LOGD("fix elf section header fail!!");
                }
            }
            
            //adding hide symbol...
            if (opt->o_unix.anti_ida) {
                anti_debug antidbg(iname);
                if (antidbg.hide_symbols()) {
                    antiida_added = true;
                    LOGD("hide symbol ok");
                } else {
                    LOGD("hide symbol fail!!");
                }
            }

            if(integrity_added && opt->o_unix.android_check_integrity){
                integrity_check::update_hashcode(iname); 
            }

        } catch(const CantPackException &e){
            unlink_ofile(oname);
            unhook(iname, dt_init_val);
            if (opt->verbose >= 1 || (opt->verbose >= 0 && !e.isWarning()))
                printErr(iname, &e);
            if(protected_info == 0){
                protected_info = 2;
            }
            set_exit_code(EXIT_WARN);
        }catch (const Exception &e) {
            unlink_ofile(oname);
            unhook(iname, dt_init_val);
            if (opt->verbose >= 1 || (opt->verbose >= 0 && !e.isWarning()))
                printErr(iname, &e);
            //set_exit_code(e.isWarning() ? EXIT_WARN : EXIT_ERROR);
            if(protected_info == 0){
                protected_info = 3;
            }
            
            set_exit_code(EXIT_WARN);
        } catch (const Error &e) {
            unlink_ofile(oname);
            unhook(iname, dt_init_val);
            printErr(iname, &e);
            e_exit(EXIT_ERROR);
        } catch (std::bad_alloc *e) {
            unlink_ofile(oname);
            unhook(iname, dt_init_val);
            printErr(iname, "out of memory");
            UNUSED(e);
            // delete e;
            e_exit(EXIT_ERROR);
        } catch (const std::bad_alloc &) {
            unlink_ofile(oname);
            unhook(iname, dt_init_val);
            printErr(iname, "out of memory");
            e_exit(EXIT_ERROR);
        } catch (std::exception *e) {
            unlink_ofile(oname);
            unhook(iname, dt_init_val);
            printUnhandledException(iname, e);
            // delete e;
            e_exit(EXIT_ERROR);
        } catch (const std::exception &e) {
            unlink_ofile(oname);
            unhook(iname, dt_init_val);
            printUnhandledException(iname, &e);
            e_exit(EXIT_ERROR);
        } catch (...) {
            unlink_ofile(oname);
            unhook(iname, dt_init_val);
            printUnhandledException(iname, NULL);
            e_exit(EXIT_ERROR);
        }

        builder.store_c_len_info(iname);
        builder.store_protected_info(protected_info);
        builder.store_tasks(hasupx, signverify_added, integrity_added, antiida_added, antidebugging_added, antidump_added);
    }
    
    int ret = builder.make_report();
    LOGD("report builder ret:%d", ret);
    if(ret == 0){
        LOGD("build md ok");
    }else if(ret == 1){ //没有报告生成。
        LOGD("no so is protected");
        set_exit_code(3);
    }else if(ret == -1){
        LOGD("report gen error");
        set_exit_code(1);
    }
    

    if (opt->cmd == CMD_COMPRESS)
        UiPacker::uiPackTotal();
    else if (opt->cmd == CMD_DECOMPRESS)
        UiPacker::uiUnpackTotal();
    else if (opt->cmd == CMD_LIST)
        UiPacker::uiListTotal();
    else if (opt->cmd == CMD_TEST)
        UiPacker::uiTestTotal();
    else if (opt->cmd == CMD_FILEINFO)
        UiPacker::uiFileInfoTotal();
}


/* vim:set ts=4 sw=4 et: */