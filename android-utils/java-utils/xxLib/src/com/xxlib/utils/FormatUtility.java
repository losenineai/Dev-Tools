package com.xxlib.utils;

public class FormatUtility {

    /**
     * 用户数量转为适用格式
     *
     * @param number 用户数量
     * @return
     */
    public static String usernum_format(int number) {
        StringBuffer usernumber = new StringBuffer();
        String usernum = Integer.toString(number);
        if (usernum.length() >= 5 && usernum.length() <= 8) {
            usernum = usernum.substring(0, usernum.length() - 4);
            usernumber.append(usernum);
            usernumber.append("万人使用");
        } else if (usernum.length() > 8) {
            usernum = usernum.substring(0, usernum.length() - 8);
            usernumber.append(usernum);
            usernumber.append("忆人使用");
        } else {
            usernumber.append(usernum);
            usernumber.append("人使用");
        }

        return usernumber.toString();
    }

    /**
     * 用户数量转为适用格式2
     *
     * @param number 用户数量
     * @return
     */
    public static String usernum_format2(int number) {
        StringBuffer usernumber = new StringBuffer();
        String usernum = Integer.toString(number);
        if (usernum.length() >= 5 && usernum.length() <= 8) {
            usernum = usernum.substring(0, usernum.length() - 4);
            usernumber.append(usernum);
            usernumber.append("万+");
        } else if (usernum.length() > 8) {
            usernum = usernum.substring(0, usernum.length() - 8);
            usernumber.append(usernum);
            usernumber.append("忆+");
        } else {
            usernumber.append(usernum);
            usernumber.append("人+");
        }

        return usernumber.toString();
    }


    /**
     * 用户数量转为适用格式3
     *
     * @param number 用户数量
     * @return
     */
    public static String usernum_format3(int number) {
        StringBuffer usernumber = new StringBuffer();
        String usernum = Integer.toString(number);
        if (usernum.length() >= 5 && usernum.length() <= 8) {
            usernum = usernum.substring(0, usernum.length() - 4);
            usernumber.append(usernum);
            usernumber.append("万");
        } else if (usernum.length() > 8) {
            usernum = usernum.substring(0, usernum.length() - 8);
            usernumber.append(usernum);
            usernumber.append("亿");
        } else {
            usernumber.append(usernum);
            usernumber.append("");
        }

        return usernumber.toString();
    }

    /**
     * 工具大小转为适用格式
     *
     * @param size 工具大小
     * @return
     */
    public static String size_format(int size) {
        String size_t = String.valueOf(size / 1000000.0);
        size_t = size_t.substring(0, size_t.indexOf(".") + 2);
        return size_t;
    }
    /**
     * 工具大小转为适用格式
     *
     * @param size 工具大小
     * @return
     */
    public static String size_format(long size) {
        String size_t = String.valueOf(size / 1000000.0);
        size_t = size_t.substring(0, size_t.indexOf(".") + 2);
        return size_t;
    }

    /**==============================start xmod method===========================**/
    /**
     * size格式转换
     *
     * @param number size
     * @return
     */
    public static String formatInstalledSize(int number) {
        if (number < 1000) {
            return number + "";
        } else if (number < 1000 * 1000) {
            return (number / 1000) + "k";
        } else if (number < 1000 * 1000 * 1000) {
            return (number / 1000 / 1000) + "m";
        } else {
            return (number / 1000 / 1000 / 1000) + "00m";
        }
    }
    /**==============================end xmod method===========================**/
}
