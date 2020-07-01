package com.xxlib.utils;

/**
 * Created by rosejames on 15/7/14.
 */
public class StringTypeCheckUtils {

    /**
     * @description 是否合法的QQ号
     */
    public static boolean isLegalQQ(String qq) {
        if (StringUtils.isEmpty(qq)) {
            return false;
        }

        return qq.matches("[0-9]{5,11}");
    }

    /**
     * @description 是否合法的手机号
     */
    public static boolean isLegalPhoneNum(String phoneNum) {
        if (StringUtils.isEmpty(phoneNum)) {
            return false;
        }
        return phoneNum.matches("^[1][3|4|5|7|8]+\\d{9}$");
    }

    /**
     * @description 是否合法的email格式
     */
    public static boolean isLegalEmail(String email) {
        if (StringUtils.isEmpty(email)) {
            return false;
        }

        return email.matches("^(\\w)+@(\\w)+\\.(\\w+)+$");
    }
}
