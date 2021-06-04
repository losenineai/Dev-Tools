//
// Created by amlloc on 18-7-11.
//

#ifndef DX_ANTI_EMULATOR_SIM_UTILS_H
#define DX_ANTI_EMULATOR_SIM_UTILS_H

#include <jni.h>
#include "anti_config.h"

#define ACTION_CONFIGURE_VOICEMAIL  "android.telephony.action.CONFIGURE_VOICEMAIL"
#define ACTION_PHONE_STATE_CHANGED  "android.intent.action.PHONE_STATE"
#define ACTION_RESPOND_VIA_MESSAGE  "android.intent.action.RESPOND_VIA_MESSAGE"
#define ACTION_SHOW_VOICEMAIL_NOTIFICATION  "android.telephony.action.SHOW_VOICEMAIL_NOTIFICATION"
#define APPTYPE_CSIM  4
#define APPTYPE_ISIM  5
#define APPTYPE_RUIM  3
#define APPTYPE_SIM  1
#define APPTYPE_USIM  2
#define AUTHTYPE_EAP_AKA  129
#define AUTHTYPE_EAP_SIM  128
#define CALL_STATE_IDLE  0
#define CALL_STATE_OFFHOOK  2
#define CALL_STATE_RINGING  1
#define DATA_ACTIVITY_DORMANT  4
#define DATA_ACTIVITY_IN  1
#define DATA_ACTIVITY_INOUT  3
#define DATA_ACTIVITY_NONE  0
#define DATA_ACTIVITY_OUT  2
#define DATA_CONNECTED  2
#define DATA_CONNECTING  1
#define DATA_DISCONNECTED  0
#define DATA_SUSPENDED 3
#define EXTRA_CALL_VOICEMAIL_INTENT "android.telephony.extra.CALL_VOICEMAIL_INTENT"
#define EXTRA_HIDE_PUBLIC_SETTINGS  "android.telephony.extra.HIDE_PUBLIC_SETTINGS"
#define EXTRA_INCOMING_NUMBER "incoming_number"
#define EXTRA_IS_REFRESH  "android.telephony.extra.IS_REFRESH"
#define EXTRA_LAUNCH_VOICEMAIL_SETTINGS_INTENT  "android.telephony.extra.LAUNCH_VOICEMAIL_SETTINGS_INTENT"
#define EXTRA_NOTIFICATION_COUNT  "android.telephony.extra.NOTIFICATION_COUNT"
#define EXTRA_PHONE_ACCOUNT_HANDLE  "android.telephony.extra.PHONE_ACCOUNT_HANDLE"
#define EXTRA_STATE  "state"
#define EXTRA_STATE_IDLE  null
#define EXTRA_STATE_OFFHOOK  null
#define EXTRA_STATE_RINGING  null
#define EXTRA_VOICEMAIL_NUMBER  "android.telephony.extra.VOICEMAIL_NUMBER"
#define METADATA_HIDE_VOICEMAIL_SETTINGS_MENU  "android.telephony.HIDE_VOICEMAIL_SETTINGS_MENU"
#define NETWORK_TYPE_1xRTT  7
#define NETWORK_TYPE_CDMA  4
#define NETWORK_TYPE_EDGE  2
#define NETWORK_TYPE_EHRPD  14
#define NETWORK_TYPE_EVDO_0  5
#define NETWORK_TYPE_EVDO_A 6
#define NETWORK_TYPE_EVDO_B  12
#define NETWORK_TYPE_GPRS 1
#define NETWORK_TYPE_GSM 16
#define NETWORK_TYPE_HSDPA 8
#define NETWORK_TYPE_HSPA 10
#define NETWORK_TYPE_HSPAP 15
#define NETWORK_TYPE_HSUPA 9
#define NETWORK_TYPE_IDEN 11
#define NETWORK_TYPE_IWLAN  18
#define NETWORK_TYPE_LTE  13
#define NETWORK_TYPE_TD_SCDMA  17
#define NETWORK_TYPE_UMTS 3
#define NETWORK_TYPE_UNKNOWN  0
#define PHONE_TYPE_CDMA 2
#define PHONE_TYPE_GSM 1
#define PHONE_TYPE_NONE 0
#define PHONE_TYPE_SIP 3
#define SIM_STATE_ABSENT 1
#define SIM_STATE_CARD_IO_ERROR  8
#define SIM_STATE_CARD_RESTRICTED  9
#define SIM_STATE_NETWORK_LOCKED  4
#define SIM_STATE_NOT_READY 6
#define SIM_STATE_PERM_DISABLED 7
#define SIM_STATE_PIN_REQUIRED 2
#define SIM_STATE_PUK_REQUIRED 3
#define SIM_STATE_READY 5
#define SIM_STATE_UNKNOWN 0
#define USSD_ERROR_SERVICE_UNAVAIL -2
#define USSD_RETURN_FAILURE -1
#define VVM_TYPE_CVVM "vvm_type_cvvm"
#define VVM_TYPE_OMTP "vvm_type_omtp"

#define INVOKE_TELEPHONY_METHOD_RET_(rettype) invokeTelephonyMethodRet##rettype

/*获取当前手机sim卡对应的网络类型*/
/*需要权限ACCESS_NETWORK_STATE*/
#define  NETWORK_TYPE_UNAVAILABLE  -1
// #define  NETWORK_TYPE_MOBILE  -100
#define  NETWORK_TYPE_WIFI  -101

#define  NETWORK_CLASS_WIFI  -101
#define  NETWORK_CLASS_UNAVAILABLE  -1
/**
 * Unknown network class.
 */
#define  NETWORK_CLASS_UNKNOWN  0
/**
 * Class of broadly defined "2G" networks.
 */
#define  NETWORK_CLASS_2_G  1
/**
 * Class of broadly defined "3G" networks.
 */
#define  NETWORK_CLASS_3_G  2
/**
 * Class of broadly defined "4G" networks.
 */
#define  NETWORK_CLASS_4_G  3


// 可以适配低版本手机
/**
 * Network type is unknown
 */
#define  NETWORK_TYPE_UNKNOWN  0
/**
 * Current network is GPRS
 */
#define  NETWORK_TYPE_GPRS  1
/**
 * Current network is EDGE
 */
#define  NETWORK_TYPE_EDGE  2
/**
 * Current network is UMTS
 */
#define  NETWORK_TYPE_UMTS  3
/**
 * Current network is CDMA: Either IS95A or IS95B
 */
#define  NETWORK_TYPE_CDMA  4
/**
 * Current network is EVDO revision 0
 */
#define  NETWORK_TYPE_EVDO_0  5
/**
 * Current network is EVDO revision A
 */
#define  NETWORK_TYPE_EVDO_A  6
/**
 * Current network is 1xRTT
 */
#define  NETWORK_TYPE_1xRTT  7
/**
 * Current network is HSDPA
 */
#define  NETWORK_TYPE_HSDPA  8
/**
 * Current network is HSUPA
 */
#define  NETWORK_TYPE_HSUPA  9
/**
 * Current network is HSPA
 */
#define  NETWORK_TYPE_HSPA  10
/**
 * Current network is iDen
 */
#define  NETWORK_TYPE_IDEN  11
/**
 * Current network is EVDO revision B
 */
#define  NETWORK_TYPE_EVDO_B  12
/**
 * Current network is LTE
 */
#define  NETWORK_TYPE_LTE  13
/**
 * Current network is eHRPD
 */
#define  NETWORK_TYPE_EHRPD  14
/**
 * Current network is HSPA+
 */
#define  NETWORK_TYPE_HSPAP  15


/*
 * Permission
 */
#define ACCESS_NETWORK_STATE "android.permission.ACCESS_NETWORK_STATE"
#define READ_PHONE_STATE "android.permission.READ_PHONE_STATE"
#define READ_SMS "android.permission.READ_SMS"

jboolean getTelephonyStateBySloIdx(int slotIdx);
jobject invokeTelephonyMethodRetObj(const char *method, int param, const char *sig);
jobject invokeTelephonyMethodRetObj(const char *method, const char *sig);
jint invokeTelephonyMethodRetInt(const char *method, int param, const char *sig);
jint invokeTelephonyMethodRetInt(const char *method, const char *sig);
jobject invokeSubscriptionManagerMethod(const char* method, int param, const char *sig);
jboolean getTelephonyStateBySloIdx(int slotIdx);
jint getSubidBySlotId(int slotIdx);
jstring getSubscriberid(int slotIdx);
jstring getDeviceId(int slotIdx);
jstring getSimSerialNumber(int slotIdx);

jstring getPhoneNumber();
jint getSimCardCount();
jint getNetworkClassByType(int networkType);
jint getNetworkClass(int slotId);
jstring getCurrentNetworkType(int slotId);
jstring getVoiceMailNumber();

#endif //DX_ANTI_EMULATOR_SIM_UTILS_H
