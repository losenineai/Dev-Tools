//
// Created by boyliang on 2017/7/7.
//

#ifndef STEE_CLIENT_SDK_STEE_CONFIG_H
#define STEE_CLIENT_SDK_STEE_CONFIG_H

#include <string>

using namespace std;

//#define DX_RISK_SECURITY "d11260edf481c5b334fc3e83b6940835";

#define KEY_UUID "34db3d779a79e77796da5bec045bd782" // md5 of dx_uuid

#define LAST_FETCH_TIME "eca0f8197edb14b1f0054232fe919e5c" // md5 of stee_last_fetch_time

#define TOKEN "e150beba69983d909657d9003c0a4378" // md5 of stee_const_id

#define LAST_URL "5eba5318aeaef3b850a9e604f354358e" // md5 of stee_last_url

#define FETCH_INTERVAL 24 * 60 * 60 * 1000 // 8 hours

#define KEY_HARD_ID "0d1ec0a6c11e8f031176bf238f940122" // md5 of dx_hardid

#define KEY_IS_LIMIT "4c17167d2181a741efc86cfd746b6164" // md5 of dx_limit

// 请求异步上报API成功时间
#define KEY_ASYNC_REPORT_TIME "38ab4e3c0f30ddc594b6dd77a229e162"
// 异步配置API的JSON内容
#define KEY_ASYNC_REPORT_CONFIG_JSON "6f546dc57f308352d7d406d7f7c19bc7"


/**
 * 唯一字段文件路径, sdcard信息
 */
#define FILE_SDCARD_CID "/sys/block/mmcblk0/device/cid"
#define FILE_SDCARD_CSD "/sys/block/mmcblk0/device/csd"
#define FILE_SDCARD_RAW_ID "/sys/block/mmcblk0/device/raw_id"
#define FILE_SDCARD_TYPE "/sys/block/mmcblk0/device/type"
#define FILE_SDCARD_NAME "/sys/block/mmcblk0/device/name"
#define FILE_SDCARD_DATE "/sys/block/mmcblk0/device/date"
#define FILE_MMC_TYPE "/sys/class/mmc_host/mmc0/mmc0:0001/type"
#define FILE_MMC_DATE "/sys/class/mmc_host/mmc0/mmc0:0001/date"
#define FILE_MMC_NAME "/sys/class/mmc_host/mmc0/mmc0:0001/name"
#define FILE_MMC_CSD "/sys/class/mmc_host/mmc0/mmc0:0001/csd"
#define FILE_MMC_CID "/sys/class/mmc_host/mmc0/mmc0:0001/cid"
#define FILE_MMC_RAW_ID "/sys/class/mmc_host/mmc0/mmc0:0001/raw_id"
/**
 * 唯一字段文件路径, 系统芯片信息
 */
#define FILE_SOC_FAMILY "/sys/devices/soc0/family"
#define FILE_SOC_MACHINE "/sys/devices/soc0/machine"
#define FILE_SOC_BUILD_ID "/sys/devices/soc0/build_id"
#define FILE_SOC_NUMBER "/sys/devices/soc0/serial_number"
#define FILE_SOC_RAW_ID "/sys/devices/soc0/raw_id"
#define FILE_BUS_NUMBER "/sys/bus/soc/devices/soc0/serial_number"
#define FILE_BUS_MACHINE "/sys/bus/soc/devices/soc0/machine"
#define FILE_BUS_BUILD_ID "/sys/bus/soc/devices/soc0/build_id"
#define FILE_BUS_RAW_ID "/sys/bus/soc/devices/soc0/raw_id"

#define ID_BOOT_ID "/proc/sys/kernel/random/boot_id"
#define ID_UUID "/proc/sys/kernel/random/uuid"
#define ID_OSRELEASE "/proc/sys/kernel/osrelease"


#endif //STEE_CLIENT_SDK_STEE_CONFIG_H
