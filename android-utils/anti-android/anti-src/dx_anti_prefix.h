//
// Created by white on 2018/6/27.
//

#ifndef DANTI_NO_PREFIX

#ifndef ANTI_PREFIX_H
#define ANTI_PREFIX_H

#include "sockets_prefix.h"

#define dx_go_hell                                                      l9gO1IOL99lIo0IoLolI

// anti_debug
#define DXAnti_detect_debug                                             l1991gLol999liOlLl9i
#define anti_debug_detectByPort23946ByTcp                               Il1OLOIlggil9g9iIo90
#define anti_debug_detectByObjProcess                                   OI9IL9i9ilO0o099ilLO
#define anti_debug_detectByParents                                      oO1Lgig9OgIio9gIiLio
#define anti_debug_detectByTaskCount                                    lgLI0gL1111gOioiLLl0
#define anti_debug_detectByNativeIsDBGConnected                         IggolIO9LO1Li9LI1iLg
#define anti_debug_detectByTrace                                        i0o11lo9o9iO99i1Ogli
#define anti_debug_detectByApi                                          lg9liOlL1ILI99loIgi1
#define anti_debug_catch_SIGTRAP                                        O01o9li1999i0lgILLo9
#define anti_debug_handle_exception_sig                                 O01OlOIigoi9li0llgio
#define anti_debug_handle_catched_signal                                Ig0oioL0IgOOgILIlIO1
#define anti_debug_replace_ptrace                                       goLg91Ooio010O9OigIo
#define anti_debug_dwp                                                  g9I0glOIgLgg1ll00ILg
#define anti_debug_keep_pipe_connect                                    Iiiil0OL1l0o0l1LOIlL
#define anti_debug_replace_dvmDbgActive                                 g9igILo0gi9g119gI0LO
#define DXAnti_detect_debug_inner                                       LLL11LOIoOOIOi9ggioo


// anti_flaw_janus
#define DXAnti_detect_flaw_janus                                        OgiooILIgIO19LoL0g9g

// anti_inject
#define DXAnti_detect_inject                                            iOL1I1l0i9iL90g9o1Ll
#define anti_inject_is_xposed_att                                       l0Io9iol1Lig1OO11loI
#define anti_inject_jar_filter                                          i0Lioil9g0o9goLLLi0l
#define anti_inject_so_filter                                           g0LoOO0g19O10oLiOlOI
#define anti_inject_find_hook_feature                                   IOII11i1oL90oI0L91OL
#define anti_inject_get_cmdline                                         I90L000L9ilL999iO0i0
#define anti_inject_detect_inject_libs                                  oiiLoLO01OO0oogLIO1O
#define anti_inject_detect_inject_jars                                  gl1il9oIOL109O10lig9

// anti_log
#define DXAnti_detect_log                                               OlO9i9LIlI01L0LLIooo
#define anti_log_replace_write                                          I010g9Io01ILO19gLg09
#define anti_log_replace_buf_write                                      IOOIiO9LoL1ILL0LigO1

// anti_memdump
#define DXAnti_detect_memdump                                           loi1l00IIiOLLgig09L9
#define anti_memdump_add_process_watch                                  lio9ll1ig191giIgoiIg
#define anti_memdump_add_thread_watch                                   I1000OOOo1oI0lI9gLol
#define anti_memdump_monitor_tid                                        ooiOlgIO0LIoiIogLL0l
#define anti_memdump_thread_exit_handler                                I91oI019OIoIo0oolo01
#define anti_memdump_check                                              OL9OL0I0g1gilLO00g1I
#define anti_memdump_monitor_pid                                        ollIig0oIOoi9Lii0LI0

// anti_multirun
#define DXAnti_detect_multirun                                          Ill9gLggiii9IiiO9oLL
#define anti_multirun_start_server                                      LLg01g91OI1L0ogIi1Io
#define anti_multirun_start_client                                      LIL1Oi1ilgllIl9Lg1ll
#define anti_multirun_socket_check                                      lIil9LOgg09go0Ol0Loi
#define anti_multirun_xiaomi_check                                      lIil9LOgg09go0Ol0Lio
#define anti_multirun_pkg_check                                         oioooog001OLoILOI0Og

// anti_thread
#define DXAnti_detect_thread                                            i9lliiLoi9IIOoO1i1iI
#define anti_thread_scan_process_threads                                lLgo090lo1lo9l0i9LLo
#define anti_thread_check_process_stopped                               Il0gLl9o0LLLLlgOil9g
#define anti_thread_check_body                                          LIo9ggO0lOL0LIllilgo

// anti_pkg_signature
#define DXAnti_verify_pkg                                               go0O9o10LLgiOIIILilL
#define DXAnti_verify_signature                                         gL1LIL1lo0iil0Il9Oo0
#define anti_do_verify_signature_by_java                                oIlO1OLoglo9Lg9iLLO0
#define anti_do_verify_signature_by_c                                   lI01OI9Ii0OLli0I9Io0
#define get_signature_md5_list_by_java                                  LO0i0go9O0IILO0Lgl00
#define DXAnti_JNIEnvGuard                                              I9lOol9i9lOgI1i1Ogg1

// anti_root
#define DXAnti_detect_root                                              liIoOOgiIlOIiLL0ogi0
#define anti_root_detectByWhich                                         giI9I191Li1OOl0i0Li9
#define anti_root_detectBySu                                            IO01i01i1iIoOg0L11oo
#define anti_root_detectByApk                                           OOLI01lg9I9iloi0g1i1
#define anti_root_detectByProps                                         ogl0gi1I9OILglLLlo9O

// anti_emulator
#define DXAnti_detect_emulator                                          lIIoil0OI1OgO0I9i011
#define anti_emu_basic_checkEmuBasicAll                                 iig99o9lOoliOlo0iIi9
#define anti_emu_basic_checkPhoneNumber                                 gOLi91lol019L91Oglg0
#define anti_emu_basic_checkDeviceId                                    LILl100i01oiLL11Io19
#define anti_emu_basic_checkImsi                                        Ogo91gIiI1oIlLgl9gLi
#define anti_emu_basic_checkVoiceMail                                   oO10i09lol1Oill1lOi9
#define anti_emu_basic_checkOperatorNameAndroid                         igI9IioggliLl1LoLoiL
#define anti_emu_basic_checkIp                                          Ioi0ioOggg1giOOiO919
#define anti_emu_basic_checkEth0                                        loOgI0IIO0l1i9go1LL1
#define anti_emu_basic_checkBuildInfo                                   IL1LiI9O09g19oLOLoi9
#define anti_emu_basic_checkPackageName                                 LgI9gg0OIi1O9loO0i0I
#define anti_emu_basic_checkBattery                                     gO9LoLgLgO9gLOigLigl
#define anti_emu_basic_checkTaint                                       oillL0gO9I9L1OOo19oi
#define anti_emu_debug_check_adb                                        Lo0LIo9l0ILoOi91o9Li
#define anti_emu_debug_check_UserAMonkey                                gLig1lLLi1LOOO1gIOIg
#define anti_emu_debug_check_DebuggerConnected                          ilL19lL9LOglII0lglg9
#define anti_emu_debug_check_TracerPid                                  I00l1OI9gLLgLlg9O1il
#define anti_emu_file_checkFiles                                        g0i9I9Ilg9lIL1IlLIig
#define anti_emu_file_checkFeatures                                     LoiILOi9giIIL9gi0ggl
#define anti_emulator_get_str                                           ogOgI1IoiiOgLl1gooIl
#define anti_detect_emulator                                            OO9LiiO91O0ILILi9igl
#define anti_emu_getItemValueMap                                        OlilI900g91gO9oIIoIg
#define anti_emu_calculateByPercent                                     O9O990I1oi0O0oIgoI9l
#define anti_emu_qemu_checkArm_pc                                       iiO0I1oiii91091goIgo
#define anti_emu_handler_signal                                         iOo1OIlOIiI1igO19lgL
#define anti_emu_qemu_checkArm_cache                                    oggiOg0O9L9OgoLgiiig
#define anti_emu_qemu_checkArm_bkpt                                     I11II0g9g0l99Ooooi0O
#define anti_emu_setupSigTrap                                           l0OIggO0ogIi9oLI9oLI
#define anti_emu_setupSigSegv                                           l1lgLoIgoIi9igi909L1
#define anti_emu_setupSigBus                                            iolO1g90li1L0ooIiIlL
#define anti_emu_tryBKPT                                                lgi1i10LLi9oiL00llgg
#define anti_emu_doCheckQemuArm_bkpt                                    oo1L1olOOO0i01o1Llli
#define anti_emu_checkQemuArm_exec_thread                               L1O1l1i9iIlOOgl9Il1O
#define anti_emu_checkQemuArm_get_thread                                O00i9Io9lgoiiiOiLLo0
#define anti_emu_doCheckQemuArm_pc                                      IgoL1gILO0099ILoI1g1
#define anti_emu_x86_support                                            OoLLogiggI1ILO00L0Ol
#define anti_emu_x86_by_hw                                              lliL1oilLIL0ol9Ol1lI


#define anti_emu_qemu_checkVbox                                         oll0I0gg90OIlioO0O99
#define anti_emu_qemu_checkQemuProps                                    I9igg1l09iO9i9lLli0L
#define anti_emu_qemu_checkQemuDriver                                   Lg1IiIooLi09IolLOg11
#define anti_emu_qemu_checkQemuFiles                                    LIgLliiO9Il901ILLLog

// anti_virtualapp
#define DXAnti_detect_virtual_app                                       Ig01iOOiiI9L9I00gOO9
#define readMaps                                                        I9O0iO19gIlg11LooOg1
#define readProcessName                                                 iOOiLl0gi9o0oI0igil9
#define findSoPathByFilter                                              iLliIl90LOo1ll9gIigL
#define countVAPathBySo                                                 Il91LlLOgOllLl1gOOII
#define countVAPathByPkg                                                IgOlOiOOi0llgoi9oL00
#define checkVAByJava                                                   I91O1Ll1LIo09ollILOI
#define checkVABySo                                                     g0iLOlgoOl090l0I90g9
#define checkVAByPkgName                                                Il0o1i1l909iglIg1iOO

// dx_anti
#define DXAnti_monitor_init                                             I9lL00LO0ig0OI0iligI
#define DXAnti_monitor_start                                            OiilIgl9lil0O1iIIlO0
#define DXAnti_monitor_check                                            g1il91L09l91IL9O9Ll9
#define DXAnti_detect_log_entry                                         i1ool1OO11iig1g9000i
#define DXAnti_detect_root_entry                                        l0O1g01llI1LIl0o9Oli
#define DXAnti_detect_inject_entry                                      gl0oiL9IgIg1Li9igOli
#define DXAnti_detect_emulator_entry                                    ggoO0oiILiO19IoOii1l
#define DXAnti_detect_multirun_entry                                    LOOOOg90IgOl9gglIO91
#define DXAnti_detect_flaw_janus_entry                                  IOO9I0iOo0iIlgigi91i
#define DXAnti_detect_memdump_entry                                     lLiLIIiiio0Oo0LgIIli
#define DXAnti_detect_debug_entry                                       OlOiioi09oo0IlglIolL
#define DXAnti_detect_thread_entry                                      l1i9I1IoLg0o01i910Lg
#define DXAnti_detect_virtual_app_entry                                 gLi1Oo09Oo99l1iI1LiI
#define DXAnti_detect_debug_inner_entry                                 iIll119LiiI1gliLgOgi
#define DXAnti_detect_jni_hook_entry									iIll119LiiI1gliLgG1l


#define DXAnti_detect_log_async                                         LiLo9li9g9gl100I101i
#define DXAnti_detect_root_async                                        lg1gOlliggO1iiOll9OI
#define DXAnti_detect_inject_async                                      glILigll1oi90Lo10liI
#define DXAnti_detect_emulator_async                                    g19liI9iogOLl0OO0O1O
#define DXAnti_detect_multirun_async                                    oL9LggIi0oio00l9Ioio
#define DXAnti_detect_flaw_janus_async                                  OIo1i10i9iil1glog010
#define DXAnti_detect_memdump_async                                     I1g019l0LO9OILo900l1
#define DXAnti_detect_debug_async                                       I0O01IO9o0loLl110O01
#define DXAnti_detect_thread_async                                      I1g9LLLOLLOIiLli9iLL
#define DXAnti_detect_virtual_app_async                                 iliLL9i010OLOI9Igl91
#define DXAnti_detect_debug_inner_async                                 L9oLliIOiO0i90l9Lio9
#define DXAnti_detect_jni_hook_async									L9oLliIOiO0i90l9L1l9

#define DXAnti_set_monitor_callback                                     O0i0Og9l0OOl0LlLiO11
#define DXAnti_ResultCallback                                           io9ogg0o01Ilg9iILi19
#define anti_monitor_thread                                             IgooolO1ioI1OiIIoIoI
#define handle_result_by_hell                                           gI0LO1I9LiO9giL0ioIl
#define do_start_monitor_thread                                         g1looli1g9oil090g99g
#define antiResultCallbackImp                                           o9i9g091ggli1OlLlll0


// dx_jni_hook
#define DXAnti_detect_jni_hook_init                                     o9i9g091ggli1OlLll11
#define DXAnti_detect_jni_hook_add_methods                              o9i9g091ggli1OlLll1l
#define DXAnti_detect_jni_hook_setup                                    o9i9g091ggli1OlLlll1


#define DXFindClass                                                     JlAALFF19goOIgSODJ1J
#define DXGetStaticMethodID                                             JJSWoAAgiiIIOFglD0O9
#define DXGetMethodID                                                   SOIIAIWOIiOg0J9FF1Hi
#define DXGetObjectClass                                                g9KA9ioH9IKiLSKSIKIg
#define DXDeleteGlobalRef                                               O10WlOSIlDFJl11JHDIo
#define DXGetObjectMethodID                                             gAFDLFL1iWWLF0l9OOFS
#define DXCallBooleanMethodByName                                       HiAlOIWg0SLKHKDSFOIi
#define DXCallBooleanMethodV                                            KSL1iKDJ0099gFJ9DKL9
#define DXCallIntMethodByName                                           iLoAK9FWHlAAJKiF1Hl0
#define DXCallIntMethodV                                                LJJgIgOglAF1o1LKKI1W
#define DXCallObjectMethodByName                                        ILJISo0W1OlSAAogSiiH
#define DXCallObjectMethodV                                             ggi1oi1l1AiHlIIIl0AA
#define DXCallVoidMethodV                                               IHW0FF09WILJigiJ0FoO
#define DXCallVoidMethodByName                                          K1lOSKK1gHD10SiFKiAJ
#define DXCallStaticBooleanMethodV                                      HK9SDOHIIgWDWl1J01gH
#define DXCallStaticBooleanMethodByName                                 L9LIKJSWS1LgALHi1JLS
#define DXNewObjectV                                                    JAloLF1i1lJAAHL90FOJ
#define DXNewObjectByName                                               ogH0iIg1LSAOgoiD9Io9
#define DXToReflectedMethod                                             OIglL1lKoOLgSo9gSJ9O



// comm_utils
#define anti_util_executeCMD                                            LLL1Ig1LiO9oOgLgiL90
#define anti_util_createRandom                                          l0Oil0iIIigl9ILi9L01
#define anti_util_sleepMS                                               I1Llg9l9lIo9II9g9l9i
#define anti_util_sleepSEC                                              OOg9ggo0iLlI0olgLO91

// file_utils
#define anti_file_exist                                                 oL0LO99g09ig0O000ogg
#define anti_file_readLines                                             Lg11OLL1LO19LLg0llIi
#define anti_file_writeAll                                              g9O1lig9LlOg0ig0Oglo
#define anti_file_readAll                                               OO91goOoo1OO0gIi9III
#define anti_file_deleteIfExist                                         ggi0Oi99011101looIgo
#define anti_file_makeDir                                               LgoL0oOI0iI0090LggLo
#define anti_file_deleteDir                                             O0i0OIi0O9lOLiI9I1O0
#define anti_file_list                                                  IiL09lg00O9oOi0O19lI

// jni_comm_utils
#define anti_jni_findClassByName                                        IlIgLl0OgO9ol09Ol9LO
#define anti_jni_getStaticMethodID                                      o1OI9o9lo0oO9L119i00
#define anti_jni_getMethodID                                            ig90i19L01i0olLil9II
#define anti_jni_findClassByObj                                         Il9iOLL10O1l1L0Ii19l
#define anti_jni_deleteGlobalRef                                        lgIo9911Ll19i00LLlII
#define anti_jni_getMethodIDByObj                                       OLiiLg9ollL0oi0l9LiO
#define anti_jni_callStaticMethodReturnObj                              I1IL011g90oL10Li0g0o
#define anti_jni_callStaticMethodReturnBool                             ooOLoiO00o9ilioi1g0o
#define anti_jni_callMethodByName                                       Ig0lLolgOoOggoOI0i1g
#define anti_jni_callMethodById                                         I99gi9o0Iio19l99901O
#define anti_jni_getGlobalContext                                       ioglgO9OI00IIOiiOOlL
#define anti_jni_getStaticObjectField                                   oIg0O9goIolLLlOgOLo1
#define anti_jni_getObjectField                                         g09oiO99I9g0LLglIOII
#define anti_jni_callStaticMethodReturnInt                              OigLlIILIg1lIL9I1gi0
#define anti_jni_callBoolMethodByName                                   Li9io1goIg1l0gO1LLlL
#define anti_jni_callBoolMethodById                                     iLoLi1LL1o19goOiiI9l
#define anti_jni_callIntMethodByName                                    oi10LIIoOilOLii9O119
#define anti_jni_callIntMethodById                                      i1ILIOLIooILgol09gI9
#define anti_jni_getStaticFieldRetIntByName                             lgo1Ll9Ooo99llLg01L1
#define anti_jni_NewObject                                              IL90Oio0Llg0i1LOIloo
#define anti_jni_stringArray2jobjectArray                               LoggL0giiOIl0OlO99Lo
#define anti_jni_setContext                                             LoggL0giiOIloggL0ogg

// jni_utils
#define anti_jni_convert_to_global_ref                                  L0911ooLL00Iii1oigLi
#define anti_jni_delete_ref                                             gliLL0g1lOi9L9lLOoLg
#define anti_jni_jstring_to_string                                      iI0glLlll1091olLO1ig
#define anti_jni_string_to_jstring                                      I0L9i1I011l01OO9lL0O
#define anti_jni_jbyteArray_to_cbytes                                   OIilgg0gLL0lil9OO0go
#define anti_jni_jbyteArray_to_string                                   I0g91lo9ilg11011IiL0
#define anti_jni_CheckException                                         olO9gI9o9ogoglo0og1o
#define anti_get_jni_env                                                olO9gI9o9ogoglo0og11

// platform
#define anti_platform_getPkgName                                        iLggl0LglgLO1OOOlglo
#define anti_platform_getWorkDir                                        O1gLil9gOlO11L1iiLoI
#define anti_platform_getResPath                                        L111oigl9ggigL0LIggL
#define anti_platform_isDebuggerConnected                               olOoOoOIO0oO09lLilg1

// properties
#define anti_property_get                                               IO0O01oolIlIg0OolIlI
#define anti_property_set                                               lglIOg1ogIoL19lgg1ol

// string_utils
#define anti_str_replaceAll                                             OOIlggL0Og9igo0gLOlI
#define anti_str_splitString                                            gliLgo09oOl1Ill0gg11
#define anti_str_format                                                 L1O1OlO0Lg9ggl9loO01
#define anti_str_MD5                                                    ii0oloOL09LgO1IO0Oo1
#define anti_str_toHexString                                            l0gLOIO1I1iI1LgoOIO0
#define anti_str_trim                                                   l0L0IiooiOLg9Olo1iL0
#define anti_str_start_with                                             gIliiLLO9o0g1g9IOLgL
#define anti_str_end_with                                               O0ggllL1loLi9OlgO0Ii
#define anti_str_containWithFilter                                      o0LgLio0Ll19lo101l1i
#define anti_str_charToHexString                                        O111OoL9OLOIllLOlI01
#define anti_str_trim_c                                                 ol9I09L9lliiloLiIL0g


// utils_anti_log
#define anti_err_log                                                    ioli9lILIliO1IOOgol1

// dx_watcher
#define watcher_clear                                                   OoLIIoilI1oiIIoLO0ig
#define watcher_read                                                    Ooi91LL010g1OiliooLO
#define watcher_init                                                    gL9l1lI1giolOILO19OL
#define watcher_uninit                                                  iIoOoL1iLILi1oIL9I1l
#define watcher_add                                                     lLl1II9lgOli0OlO9100
#define watcher_auto_remove                                             O1LlIOiggllig0g0og1I
#define watcher_dispose                                                 o9gIgO00o9OOiigOlOi1

// dx_libc_anti_utils
#define dx_set_errno                                                    g1IgL0LIioloililIOoI
#define dx_kill                                                         iI0gg11g19I0L1l0iI90
#define bb_gettimeofday                                                 oLOLL0o10IL9io1ggLLg
#define bb_timer_delete                                                 lgil1l00iO0l9LgO0Ol0
#define malloc_wraper                                                   L0Io9ooLilog0iL9IlLo
#define dx_hook2                                                        L1Og1o1g1lil0IOL0loI
#define dx_select                                                       g0Oil00Lg9iOgo1li9I0
#define dx_get_sys_lib_path                                             L10iloloo9gI91IOiI0O

// utils_int_map
#define map_allocate                                                    i0iiLo91ioI0OiI1IIOO
#define map_set                                                         ioO9LO0i1oi1ioI9g1lI
#define map_contains_key                                                o19IL1igLLloLO01L9OO
#define map_get                                                         oloo0l1IgOig1lgliIl1
#define map_delete                                                      gLo0I1Ig0IIgg0IglOo1
#define map_trunk                                                       Ii9L0olo1oII9Ig9900I

// utils_common
#define anti_jni_get_sdk_int                                            IloO91IOOgo11Llgolio

// utils_permissions
#define checkPermissions                                                gOo0ii0lO9I0o90ogI0i

// sim_utils
#define isSupportTelePhony                                              i9l0LLgLgog0iIOgI9Li
#define invokeTelephonyMethodRetObj                                     iLILI1I0o9i01iO1ioO0
#define invokeTelephonyMethodRetInt                                     l1L19LgI1L990LLLOg9L
#define invokeSubscriptionManagerMethod                                 l1119Io00OIgi0iLl1g0
#define invokeStaticSubscriptionManagerMethod                           IooggoLO1o10o9L1LI1L
#define getTelephonyStateBySloIdx                                       L1i0giOlI10Log9oO1g0
#define getSubidBySlotId                                                Li9lO1g011iOLggI0iL9
#define getSubscriberid                                                 Ll9ooO110i0I9099l00l
#define getDeviceId                                                     oII9go1gO0o1i9i9iilO
#define getSimSerialNumber                                              L1ioO9gil1L9ol9gilLI
#define getPhoneNumber                                                  ogOLi9Llo90O0glI00gL
#define getVoiceMailNumber                                              LOi191OL1l0iOoiiOoIo
#define getSimCardCount                                                 lLog1ol1O0L00gIiiO09
#define getNetworkClassByType                                           gOi0l991Li1LiO1ol0oO
#define getNetworkClass                                                 O11Ol009loOIoi1iLgLL
#define getCurrentNetworkType                                           gOO090ILg1Ii10ooLOii

// AntiResultHandler
#define AntiResultHandler                                               lgOgIl0o9lg0glgi9OLl
#define getInstance                                                     l9LII1LLi9LO9O099i19
#define dx_receive_sub_process_thread                                   L11Igogo90O1I0oO1ll1
#define HandleResult                                                    ilL0g911g00iIgo1g0Il
#define SetResultCallback                                               i9ogo1IIoglgIlO1Oigi

// crypto
#define CryptUtils                                                      o99O10O1O0IILOiOo0I1
#define MD5                                                             O0go91loi9g0o0LL99go
#define MD5WithKey                                                      o9I9iiol9li0g11lg9ig
#define HexStringToChar                                                 OLiloi0O1loi9gIigOlo
#define CharToHexString                                                 lg1Oolg9I10L1LO1L9l9

// pkcs7
#define pkcs7                                                           Oog1o1I1L119LLLgg0LO
#define open_file                                                       i0LoLO1o91goLlLioL99
#define get_content                                                     Loi901g9OlOl9OLo0ig0
#define parse_pkcs7                                                     oLgIloL10g1iI9LiL09l
#define get_from_apk                                                    oo1g10g9ioL10OOoi9LO
#define len_num                                                         I1100OL90L1Olg1L9lo9
#define num_from_len                                                    LLOooi19llOgLOIii9lg
#define tag_offset                                                      oOgOII19lgI9LLll0oLi
#define get_length                                                      iiooLig0l9o1go1oLL0O
#define create_element                                                  L1909go9Ool9o0IligLL
#define parse_certificate                                               IgIogOl909l11II9II19
#define parse_signerInfo                                                lllLLlio109iOogLolO0
#define parse_content                                                   iliILOIIgL10o1oOgOoi
#define get_count                                                       LIOlO1L9gLL900g99900
#define get_MD5_hex                                                     iOll19IL1l0i1LoLI0iL

// zip
#define print_buf_internal                                              go0LL0IiIoiL00ilI01g
#define fflush_buf                                                      gO0oOL1o9I0OloOI900l
#define fill_buffer_filefunc                                            iIg0gllIOL9O1IOoOgI1
#define fopen_buf_internal_func                                         iiOggIl0llo99olIO0Oi
#define ftell_buf_internal_func                                         l1lIO0g91gIo11igiogi
#define fseek_buf_internal_func                                         igg10li9Ii9oIllOlLO0
#define ferror_buf_func                                                 g0oglL00L11l9o1li19O
#define fopen_buf_func                                                  LLIlgggo0O90OI1O11Ol
#define ftell_buf_func                                                  ggOo0og10IilILg99OOo
#define fopendisk_buf_func                                              gOg1o1iOolOLl9Oli9gL
#define fseek_buf_func                                                  gLig00LligLLoOiiiLg1
#define fwrite_buf_func                                                 ooloL9Oi0Li1L0iLiOIg
#define fclose_buf_func                                                 ILo9IO01g010io1191oi
#define fread_buf_func                                                  g19Ol9oI1IgLg1O1Lgoo
#define fopen64_buf_func                                                lILoligloOo9L1o1iI9L
#define ftell64_buf_func                                                iiI1LIL0LOoL9OIi0LOl
#define fopendisk64_buf_func                                            LLg90g1Oo910i0LOg0Io
#define fseek64_buf_func                                                Ogo11igoI1L0lO09OO00
#define fill_buffer_filefunc64                                          go1gg0g0oiilggiioogO
#define fill_fopen_filefunc                                             iggliig0999iiLil99Og
#define fill_fopen64_filefunc                                           OigIiO91911g0L0Lgg1i
#define call_zopen64                                                    ggL0lIiliI1I0o99g0L0
#define call_ztell64                                                    glglogI00Io1g9g9gl9g
#define call_zopendisk64                                                g0Iio1Lil1i1igl191Oi
#define call_zseek64                                                    iliilLgOiLoo9iO99Og0
#define fill_zlib_filefunc64_32_def_from_filefunc32                     I0IlIl0LII19L1o1gLgo
#define fill_memory_filefunc                                            O1OlIL9glLL9giogg1lO
#define ferror_mem_func                                                 l0oIgoI9LllOLli19lL9
#define fopen_mem_func                                                  OLoLoOILoL1lliLI9o0i
#define ftell_mem_func                                                  gL9igololloOL9i990go
#define fopendisk_mem_func                                              LL09Llg10IiglLogoIg1
#define fseek_mem_func                                                  IgO0OIoI1I1gillgL19g
#define fwrite_mem_func                                                 IooLlii0ggi9Og11iOL1
#define fclose_mem_func                                                 lLOLiggig9091l9iOiLl
#define fread_mem_func                                                  L0Iol0oLi1loiooI90i9

#define fopen_file_func                                                 i09Li9l9iOoOLg9g1glg
#define fopendisk_file_func                                             og9IIiIgOiooO9OgLoLI
#define fread_file_func                                                 LIOOIL0O9Io1olIggloL
#define fwrite_file_func                                                OOo90ILool9Il0IiglLL
#define ftell_file_func                                                 Lig0OLIiI00Lgig1I9oL
#define fseek_file_func                                                 IoOOO1OLiLIiiLL9LLiI
#define fclose_file_func                                                i9IlIol0lIIl1O00gOO1
#define ferror_file_func                                                ligLigOgggllOiIIglL0
#define fopen64_file_func                                               L1OIOlggOO1O1L0g1lg0
#define fopendisk64_file_func                                           oLI1LlioioiolO0IO0L1
#define ftell64_file_func                                               I9l90ILgii0IoioILg1o
#define fseek64_file_func                                               oigIli9oOolg9I90OOg1
#define file_build_ioposix                                              g1L9gOiiOi9ii0gg1L9O

#define zipCloseFileInZipRaw                                            iOlOoOIo00OIloglo09I
#define zip_copyright                                                   il0olO1L00g1lg9lIiig
#define zipOpenNewFileInZip                                             gLLOgLgLlIgIiI9O9o9g
#define zipWriteInFileInZip                                             I999gO00l9OoLIgLi90g
#define zipCloseFileInZip                                               OolgIoiiO10I9Oo1liOg
#define zipOpen                                                         o09ilgoOoli1Oo0o10gI
#define zipClose                                                        LoOIgIgl0IgLiogi0I9i
#define zipOpenNewFileInZip4                                            IL0lO9I0OIg1l19i09lo
#define zipOpen4                                                        lgg01oOL91LLO19Il99O
#define zipCloseFileInZipRaw64                                          IIO09Oo9o011o1I1oo0l
#define zipOpenNewFileInZip64                                           illgOi9olLLg999IIo19
#define zipOpen64                                                       IgiIIiILi01g9ILL0Io1
#define zipOpenNewFileInZip4_64                                         Li19iL99L9Lo1LI9119l
#define zipOpenNewFileInZip3_64                                         OlOO9IL9go00Ol9gLogI
#define zipOpen3_64                                                     L1oLoL1IiiI9Ig0illI9
#define zipOpenNewFileInZip2_64                                         Ig0i0901g9Lg9Olg9l10
#define zipOpen2_64                                                     ogOoL9o1goi0OIgLLLo1
#define zipOpenNewFileInZip3                                            iLoOO01IL1oII911iliL
#define zipOpen3                                                        Oolo19o0I1OLOi0O0gol
#define zipOpenNewFileInZip2                                            Ol09I9l10l0LIO1oILIi
#define zipOpen2                                                        IL1oig9gil9l0ILOg1LO

#define init_keys                                                       L901g09oiLiOlOL0I0I1
#define add_data_in_datablock                                           l0g1gL0g01OOlL19ili0
#define update_keys                                                     I1IOig01Lll0il0LOo1i
#define zip64FlushWriteBuffer                                           I1iL0gL1OOL10ILgi1gi
#define zip64local_getLong                                              iOo99gOlI9iigioioOg1
#define zip64local_getLong64                                            ogOg0lOi11Ogg9ggg9iO
#define zip64local_getShort                                             g0Lg0Oi9giLiiiOo9OIL
#define zip64local_putValue                                             Ogoig9oIliLIli0o0oIg
#define zipGoToNextDisk                                                 lIi1golOgl1O1gOioOO1
#define zipGoToSpecificDisk                                             lgIOiIiILOl91I99LOgo

#define unzGetGlobalComment                                             oOlL9LLlilOlol1oI9Lg
#define unz_copyright                                                   o9giO9OO1Og1090I0OiO
#define unzSetOffset                                                    iIOig01L0gI10oigOOl1
#define unzGetOffset                                                    goLI0gOilioogLOiOiII
#define unzGetFilePos                                                   iiog1ooLOO0gi1i0LllL
#define unzGoToFilePos                                                  lggIOl1i0gioOl00Ig90
#define unzGetGlobalInfo                                                i9og1LLgOlLogllLolLi
#define unzGetCurrentFileInfo                                           lL0LIlOIogOOggILg99L
#define unzOpen                                                         OLIIiLiI01i09oiio009
#define unztell                                                         L9II1g0IO11oIgo9g9Lg
#define unzeof                                                          igOg0O9lI1191ILi1gLL
#define unzClose                                                        O1oi9goOi0LgL1i0gglL
#define unzGoToNextFile                                                 oLIIIoOi10Og0LgilIgg
#define unzGoToFirstFile                                                o9gll1OLLiI9IlLOiI00
#define unzOpenCurrentFile                                              Lol9g9I9I19llooooiOg
#define unzCloseCurrentFile                                             LOLL1IIOO0o001oi00LO
#define unzReadCurrentFile                                              igiiL9io9gi1OIiigoli
#define unzLocateFile                                                   O0IiIOIgiOIoLoLiL190
#define unzOpenCurrentFilePassword                                      oIL011L1lLIoOg90Oo0l
#define unzGetLocalExtrafield                                           ogLlIgII0I90IL1L1O0g
#define unzSetOffset64                                                  iOi1g1i10011l0ilO1l9
#define unzGetOffset64                                                  Olol9oli101Lg1I1ollg
#define unzGetCurrentFileZStreamPos64                                   lII9iOLi9LlO9oOL1lgo
#define unzGetFilePos64                                                 OLOIiilI1olI0Io119il
#define unzGoToFilePos64                                                LLL1iL1iOl9LL0oLiL0l
#define unzGetGlobalInfo64                                              l99goI1g00Ii9Oo1ol0I
#define unzGetCurrentFileInfo64                                         g1Il1OL0ioOgl90oioI0
#define unzOpen64                                                       loIOio0l9IO1i00olOog
#define unztell64                                                       LIII0OLIIoOIi1ig9lgi
#define unzOpen2_64                                                     oglIoOgloO1IIoLi9Og1
#define unzOpenCurrentFile3                                             ogglgLLloL9gIiIl1Ioo
#define unzOpen2                                                        gOg10IogO0lLgIOL9gIl
#define unzGoToNextFile2                                                iLlL10O9iL901l9Ilooo
#define unzGoToFirstFile2                                               g0OII0lLL1Ol0lllL9Oi
#define unzOpenCurrentFile2                                             lO09i0I0l9101Ol1I091
#define unzOpenInternal                                                 gO1ilLoo0glll1g9l01O
#define unzGoToNextDisk                                                 oLlg1O9III9LIiig0Igo
#define unz64local_getLong                                              l9Ol9l91Lo0ol1LgOoiL
#define unz64local_getShort                                             illloIiL0iloooil0L9l
#define unz64local_getLong64                                            OgOLii1oI1l90olg9gO9
#define unz64local_GetCurrentFileInfoInternal                           iiLg1IIoLgol1iLOL0IO

#define MD5Init                                                         i111OgLgilg1ooLL0gli
#define MD5Final                                                        gILO9OiOLlgI0Li1o90i
#define MD5Update                                                       gIO1iI1LgLIgO1gIgIIo
#define MD5Transform                                                    LI9L0I0I10L1loO1Oo0I
#define md5                                                             Lilg9Og019iI91iII9Og
#define hmac_sha1                                                       o0I19L0O1090lgILigoI
#define hmac_md5                                                        ig990IgILiO1OOOoOog0
#define Encode                                                          oL0loloo9l9g91i0o919

#endif //ANTI_PREFIX_H

#endif //DANTI_NO_PREFIX



