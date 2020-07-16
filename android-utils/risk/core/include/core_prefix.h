//
//  prefix.h
//  STEEForiOS
//
//  Created by xelz on 2017/7/19.
//  Copyright © 2017年 echonn. All rights reserved.
//

#ifndef core_prefix_h
#define core_prefix_h

#ifdef __APPLE__

#define get_work_dir dx_risk_get_work_dir
#define get_sdcard_dir dx_risk_get_sdcard_dir
#define get_exsdcard_dir dx_risk_get_exsdcard_dir
#define get_sdcard_status dx_risk_get_sdcard_status
#define get_os_version dx_risk_get_os_version
#define get_app_version dx_risk_get_app_version
#define get_os_arch dx_risk_get_os_arch
#define get_os_type dx_risk_get_os_type
#define get_pkg_name dx_risk_get_pkg_name
#define get_app_version_code dx_risk_get_app_version_code
#define sync_http_request dx_risk_sync_http_request
#define async_http_request dx_risk_async_http_request
#define get_res_by_name dx_risk_get_res_by_name

#define LOGR dx_risk_release_log
#define LOGE dx_risk_err_log
#define LOGD dx_risk_info_log
#define sys_log_v dx_risk_sys_log_v
    
#define StringUtils DXRiskStringUtils
#define OthersUtils DXRiskOthersUtils
#define ZipFile DXRiskZipFile
#define FileUtils DXRiskFileUtils
#define CryptUtils DXRiskCryptUtils
#define JSonUtils DXRiskJSonUtils

#define dingxiang dingxiangrisk
#define google dxriskgoogle


#define dxFILE FILE
#define dx_errno errno
#define dx_execl execl
#define dx_execvpe execvpe
#define dx_execvp execvp
#define dx_fexecve fexecve
#define dx_pclose pclose
#define dx_popen popen
#define dx_vfork vfork
#define dx_write write
#define dx_read read
#define dx_lseek lseek
#define dx_close close
#define dx_fflush fflush
#define dx_fclose fclose
#define dx_feof feof
#define dx_ferror ferror
#define dx_fgets fgets
#define dx_fread fread
#define dx_fwrite fwrite
#define dx_fseek fseek
#define dx_ftell ftell
#define dx_fopen fopen
#define dx_open open
#define dx_fdopen fdopen
#define dx_dlopen dlopen
#define dx_dlsym dlsym
#define dx_get_symbols get_symbols
#define dx_dlclose dlclose
#define dx_strlen strlen
#define dx_strcat strcat
#define dx_strcmp strcmp
#define dx_strcpy strcpy
#define dx_strncat strncat
#define dx_strncmp strncmp
#define dx_strstr strstr
#define dx_strrchr strrchr
#define  dx_strnlen strnlen
#define dx_fcntl fcntl
#define dx_nanosleep nanosleep
#define dx_usleep usleep
#define dx_sleep sleep
#define dx_sprintf sprintf
#define dx_sscanf sscanf
#define dx_pthread_create pthread_create
#define dx_clone clone
#define dx_pthread_kill pthread_kill
#define dx_pthread_detach pthread_detach
#define dx_pthread_join pthread_join
#endif

#endif /* prefix_h */
