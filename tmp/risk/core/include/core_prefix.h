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
#define http_request dx_risk_http_request
#define async_http_request dx_risk_async_http_request
#define get_res_by_name dx_risk_get_res_by_name

#define flow_log dx_risk_flow_log
#define err_log dx_risk_err_log
#define info_log dx_risk_info_log
#define sys_log_v dx_risk_sys_log_v
    
#define StringUtils DXRiskStringUtils
#define OthersUtils DXRiskOthersUtils
#define ZipFile DXRiskZipFile
#define FileUtils DXRiskFileUtils
#define CryptUtils DXRiskCryptUtils
#define JSonUtils DXRiskJSonUtils

#define dingxiang dingxiangrisk
#define google dxriskgoogle

#endif

#endif /* prefix_h */
