#include <jni.h>
#include <string>
#include "log.h"

#include "dx_xxtea.h"
#include "dx_base64.h"
#include "dx_md5.h"
#include "dx_sha1.h"
#include "dx_sha256.h"
#include "dx_rc4.h"
#include "dx_aes.h"
#include "dx_wbaes.h"
#include "dx_sm3.h"
#include "dx_sm4.h"
#include "dx_mtkey.h"

using namespace std;

void check();

extern "C"
JNIEXPORT jstring

JNICALL
Java_com_dingxiang_mobile_crypto_1as_1project_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    check();

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

void check_xxtea() {

    err_log("- - - check_xxtea - - -");

    string data ="this is a xxtea text";
    string key = "12345678901234567890123456789012";

    string dx_enc = DX_XXTeaEncrypt(key, data);
    string dx_dec = DX_XXTeaDecrypt(key, dx_enc);

    err_log("xxtea enc: %s", dx_enc.c_str());
    err_log("xxtea dec: %s", dx_dec.c_str());

}

void check_base64() {

    err_log("- - - check_base64 - - -");

    string enc = DX_Base64Encode("this is a test text");

    string dec = DX_Base64Decode(enc);

    err_log("check data enc: %s", enc.c_str());
    err_log("check data dec: %s", dec.c_str());

}

void check_md5() {

    err_log("- - - check_md5 - - -");

    string data ="this is a md5 text";
    string key = "12345678901234567890123456789012";

    string dx_enc = DX_MD5(data);
    string dx_enc_key = DX_MD5WithKey(data, key);

    err_log("dx_enc: %s", dx_enc.c_str());
    err_log("dx_enc_key: %s", dx_enc_key.c_str());

}


void check_sha1() {

    err_log("- - - check_sha1 - - -");

    string data ="this is a sha1 text";
    string key = "12345678901234567890123456789012";

    string dx_enc = DX_SHA1(data);
    string dx_dec = DX_SHA1WithKey(key, data);

    err_log("sha1 enc: %s", dx_enc.c_str());
    err_log("sha1 key: %s", dx_dec.c_str());

}

void check_sha256() {

    err_log("- - - check_sha256 - - -");

    string data ="this is a sha256 text";
    string key = "12345678901234567890123456789012";

    string dx_enc = DX_SHA256(data);
    string dx_dec = DX_SHA256WithKey(key, data);

    err_log("sha256 enc: %s", dx_enc.c_str());
    err_log("sha256 key: %s", dx_dec.c_str());

}

void check_rc4() {

    err_log("- - - check_rc4 - - -");

    string data ="this is a rc4 text";
    string key = "12345678901234567890123456789012";

    string dx_enc = DX_RC4(key, data);

    err_log("rc4 enc: %s", dx_enc.c_str());

}

void check_aes() {

    err_log("- - - check_aes - - -");

    string data ="this is a aes text";
    string key = "12345678901234567890123456789012";

    string dx_aes_ecb_enc = DX_AesEcbEncrypt(key, data);
    string de_aes_ecb_dec = DX_AesEcbDecrypt(key, dx_aes_ecb_enc);

    string dx_aes_cbc_enc = DX_AesCbcEncrypt(key, "test", data);
    string dx_aes_cbc_dec = DX_AesCbcDecrypt(key, "test", dx_aes_cbc_enc);

    err_log("aes ecb enc1: %s", dx_aes_ecb_enc.c_str());
    err_log("aes ecb dec1: %s", de_aes_ecb_dec.c_str());

    err_log("aes cbc enc2: %s", dx_aes_cbc_enc.c_str());
    err_log("aes ecb dec2: %s", dx_aes_cbc_dec.c_str());
}

void check_wbox() {

    err_log("- - - check_wbox - - -");

    string data ="this is a wbox text";
    string key = "12345678901234567890123456789012";

    string dx_enc = DX_WBAesEncrypt(key, data);
    string dx_dec = DX_WBAesDecrypt(key, dx_enc);

    err_log("wbox enc: %s", dx_enc.c_str());
    err_log("wbox key: %s", dx_dec.c_str());

}

void check_sm3() {

    err_log("- - - check_sm3 - - -");

    string data ="this is a sm3 text";
    string key = "12345678901234567890123456789012";

    string dx_enc = DX_SM3Encrypt(key, data);

    err_log("sm3 enc: %s", dx_enc.c_str());

}

void check_sm4() {

    err_log("- - - check_sm4 - - -");

    string data ="this is a sm4 text";
    string key = "12345678901234567890123456789012";

    string dx_enc = DX_SM4Encrypt(key, data);
    string dx_dec = DX_SM4Decrypt(key, dx_enc);

    err_log("sm4 enc: %s", dx_enc.c_str());
    err_log("sm4 key: %s", dx_dec.c_str());
}

void check_mtkey() {

    err_log("- - - check_mtkey - - -");

    string data ="this is a mt text";
    string key = "12345678901234567890123456789012";

    string dx_enc = DX_MTKeyEncrypt(key, data);
    string dx_dec = DX_MTKeyDecrypt(key, dx_enc);

    err_log("mtt enc: %s", dx_enc.c_str());
    err_log("mtt key: %s", dx_dec.c_str());

}

void check() {

    err_log("- - - - check - - - -");

    check_xxtea();

    check_base64();

    check_md5();

    check_sha1();

    check_sha256();

    check_rc4();

    check_aes();

    // 未验证
    // check_wbox();

    check_sm3();

    check_sm4();

    check_mtkey();
}
