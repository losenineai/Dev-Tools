package com.xxlib.utils;

import android.annotation.SuppressLint;

import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

/**
 * 数值表现的工具类，如：字节大小的int转化成MB单位，long时间转化文字 linbin 加入聊天时间显示规则
 *
 * @author wxj
 */
public class NumberDisplayUtil {

    /**
     * 转换文件大小到合适的单位（保留n位小数）
     * @param size 文件大小
     * @param decimalCount 保留的小数位数
     * @return
     */
    public static String formatFileSize(long size, int decimalCount) {
        if (decimalCount < 0) {
            throw new IllegalStateException("decimalCount can not be the negative number");
        } else {
            float result = size;
            String suffix = "B";
            if (result > 1024.0f) {
                suffix = "K";
                result = result / 1024.0f;
            }
            if (result > 1024) {
                suffix = "M";
                result = result / 1024.0f;
            }
            if (result > 1024) {
                suffix = "G";
                result = result / 1024.0f;
            }
            if (result > 1024) {
                suffix = "T";
                result = result / 1024.0f;
            }
            if (result > 1024) {
                suffix = "P";
                result = result / 1024.0f;
            }

            if (decimalCount == 0) {
                return (int)(result + 0.5) + suffix;
            } else {
                StringBuilder builder = new StringBuilder(".#");
                for (int i = 1; i < decimalCount; ++i) {
                    builder.insert(1, "0");
                }
                DecimalFormat format = new DecimalFormat(builder.toString());
                return format.format(result) + suffix;
            }
        }
    }

    /**
     * 转换文件大小到合适的单位（保留1位小数）
     * @param size 文件大小
     * @return
     */
    public static String formatFileSizeInOne(long size) {
        return formatFileSize(size, 1);
    }

    /**
     * 转换文件大小到合适的单位（保留2位小数）
     * @param size 文件大小
     * @return
     */
    public static String formatFileSize(long size) {
        return formatFileSize(size, 2);
    }

    /**
     * 转化MB大小单位的字符串
     *
     * @param size
     * @return
     */
    public static String toMB(int size) {
        // 大小
//        String sizeStr = String.valueOf(size / 1000000.0);
//        sizeStr = sizeStr.substring(0, sizeStr.indexOf(".") + 2) + "M";
//        return sizeStr;
        return formatFileSize(size);
    }

    /**
     * 转化MB大小单位的字符串
     *
     * @param size 单位为B
     * @return
     */
    public static String toMbOrKb(long size) {
//        if (size < 1024) {
//            return String.valueOf(size) + "B";
//        }
//        DecimalFormat format = new DecimalFormat(".0#");
//        if (size < 1048576) {
//            String kbSize = format.format(size / 1024.0);
//            return kbSize + "K";
//        }
//        if(size < 1073741824){
//            String sizeStr = format.format(size / 1048576.0);
//            return sizeStr + "M";
//        }
//
//        String sizeStr = format.format(size / 1073741824.0);
//        return sizeStr + "G";
        return formatFileSize(size);
    }

    /**
     * 转化MB大小单位的字符串
     * 除以1024
     * @param size 单位为B
     * @return
     */
    public static String toMbOrKbBy1024(long size) {
//        if (size < 1024) {
//            return String.valueOf(size) + "B";
//        }
//        DecimalFormat format = new DecimalFormat(".0#");
//        if (size < 1048576) {
//            String kbSize = format.format(size / 1024.0);
//            return kbSize + "K";
//        }
//        if(size < 1073741824){
//            String sizeStr = format.format(size / 1048576.0);
//            return sizeStr + "M";
//        }
//
//        String sizeStr = format.format(size / 1073741824.0);
//        return sizeStr + "G";
        return formatFileSize(size);
    }

    /**
     * 获得下载速度的正确显示值
     */
    public static String toDownLoadSpeedNumber(double speed) {
        if ((speed / 1024) > 1024) {
            return StringUtils.format("%.2f", (double) (speed / 1024 / 1024)) + "MB/S";
        } else {
            return (int) (speed / 1024) + "KB/S";
        }
    }

    /**
     * 转化成带单位的数量，万，亿
     *
     * @return
     */
    public static String toUnit(int number) {
        return toUnit(number, 0);
    }

    /**
     * 转化成带单位的数量，万，亿
     * @param number    数值
     * @param decimalDigit  保留小数点后面几位
     * @return
     */
    public static String toUnit(int number, int decimalDigit){
        decimalDigit = Math.max(decimalDigit, 0);
        decimalDigit = Math.min(decimalDigit, 7);

        String numStr = number + "";
        if (number >= 10000 * 10000) {
            numStr = String.format("%." + decimalDigit + "f", number / 10000F / 10000F) + "亿";
        }
        else if (number >= 10000) {
            numStr = String.format("%." + decimalDigit + "f", number / 10000F) + "万";
        }

        return numStr;
    }



    /**
     * 转化成人性化的时间提示,最小单位是毫秒
     *
     * @param time
     * @return
     */
    @SuppressLint("SimpleDateFormat")
    public static String toPersonalTime(long time) {
        // 现在时间
        Calendar now = Calendar.getInstance();
        // 指定时间
        Calendar calendar = Calendar.getInstance();
        calendar.setTimeInMillis(time);

        String timeStr = new SimpleDateFormat("yy-MM-dd").format(new Date(time));

        // 相差时间戳，秒最低单位
        int diffValue = (int) (now.getTimeInMillis() / 1000 - time / 1000);

        // 一样时间
        if (diffValue <= 3) {
            timeStr = "刚刚";
        }
        // 未来时间
        // else if(diffValue < 0){
        // timeStr = new SimpleDateFormat("yy-MM-dd").format(new Date(time));
        // }
        // 以前时间
        else {
            // 不是同一年
            if (calendar.get(Calendar.YEAR) != now.get(Calendar.YEAR)) {
                return timeStr;
            } else {
                int days = diffValue / 60 / 60 / 24;
                int hours = diffValue / 60 / 60;
                int minutes = diffValue / 60;
                int seconds = diffValue;
                if (seconds > 0 && seconds < 60) {
                    timeStr = seconds + "秒前";
                } else if (minutes > 0 && minutes < 60) {
                    timeStr = minutes + "分钟前";
                } else if (hours > 0 && hours < 24) {
                    timeStr = hours + "小时前";
                } else if (days > 0 && days <= 7) {
                    timeStr = days + "天前";
                } else {
                    timeStr = new SimpleDateFormat("MM-dd").format(new Date(time));
                }
            }
        }

        return timeStr;
    }

    /**
     * 转化为聊天时间显示：今天内的显示时间，今年内的显示日月时，非今年的显示年月日时
     *
     * @param time
     * @return
     */
    @SuppressLint("SimpleDateFormat")
    public static String toChatTime(long time) {
        // 现在时间
        Calendar now = Calendar.getInstance();
        // 指定时间
        Calendar calendar = Calendar.getInstance();
        calendar.setTimeInMillis(time);

        String timeStr = new SimpleDateFormat("yyyy-MM-dd HH:mm").format(new Date(time));
        // 非今年,显示年月日
        if (calendar.get(Calendar.YEAR) != now.get(Calendar.YEAR)) {
            return timeStr;
        }
        // 今年内，非今天，显示月日
        else if (now.get(Calendar.DAY_OF_MONTH) != calendar.get(Calendar.DAY_OF_MONTH)) {
            return new SimpleDateFormat("MM-dd HH:mm").format(new Date(time));
        }
        // 同一年，同一月，同一日
        else if (now.get(Calendar.DAY_OF_MONTH) == calendar.get(Calendar.DAY_OF_MONTH)) {

            return new SimpleDateFormat("HH:mm").format(new Date(time));
        }
        return timeStr;
    }

    /**
     * 转化为聊天列表时间显示：今天内的显示时间，今年内的显示日月，非今年的显示年月日
     *
     * @param time
     * @return
     */
    @SuppressLint("SimpleDateFormat")
    public static String toChatListTime(long time) {
        // 现在时间
        Calendar now = Calendar.getInstance();
        // 指定时间
        Calendar calendar = Calendar.getInstance();
        calendar.setTimeInMillis(time);

        String timeStr = new SimpleDateFormat("yy-MM-dd").format(new Date(time));
        // 非今年,显示年月日
        if (calendar.get(Calendar.YEAR) != now.get(Calendar.YEAR)) {
            return timeStr;
        }
        // 今年内，非今天，显示月日
        else if (now.get(Calendar.DAY_OF_MONTH) != calendar.get(Calendar.DAY_OF_MONTH)) {
            return new SimpleDateFormat("MM-dd").format(new Date(time));
        }
        // 同一年，同一月，同一日
        else if (now.get(Calendar.DAY_OF_MONTH) == calendar.get(Calendar.DAY_OF_MONTH)) {

            return new SimpleDateFormat("HH:mm").format(new Date(time));
        }
        return timeStr;
    }

    /**
     * 转化成人性化的时间提示,最小单位是秒
     *
     * @param time
     * @return
     */
    public static String toPersonalTime(int time) {
        return toPersonalTime((long) time * 1000);
    }

    /**
     * 转化成橄榄邮对话时间,最小单位是秒
     *
     * @param time
     * @return
     */
    public static String toChatTime(int time) {
        return toChatTime((long) time * 1000);
    }

    /**
     * 转化成橄榄邮对话列表时间,最小单位是秒
     *
     * @param time
     * @return
     */
    public static String toChatListTime(int time) {
        return toChatListTime((long) time * 1000);
    }


    /**
     * 时间段格式化
     *
     * @param days 天数
     * @return
     */
    public static String toCardByDay(int days) {
        String str = "";
        if (days <= 0) {
            str = "无套餐";
        }

        // 永久
        if (days >= 36600) {
            str = "永久";
        }
        // 1年以上
        else if (days >= 366) {
            str = "年卡(366天)";
        }
        // 1季以上
        else if (days >= 90) {
            str = "季卡(90天)";
        }
        // 1个月以上
        else if (days >= 30) {
            str = "月卡(30天)";
        }
        // 一周以上
        else if (days >= 7) {
            str = "周卡(7天)";
        }
        // 1天以上
        else {
            str = "日卡(1天)";
        }
        return str;
    }


    /**
     * 剩余时间格式化
     * 剩余时间的统一调整为： 大于1天的 直接显示天数  小于1天的 显示小时及分钟
     *
     * @param min 分钟单位
     * @return
     */
    public static String toRemainTimeUnit(int min) {
        String str = "0";
        if (min <= 0) {
            return str;
        }

        // 永久
        if (min >= 52704000) {
            str = "永久";
        }
        // 1天以上
        else if (min >= 60 * 24) {
            int hours = min % (60 * 24) / 60;
            int mins = min % (60 * 24) % 60;
            str = min / (60 * 24) + "天" + hours + "小时" + mins + "分钟";

        }
        // 一小时以上
        else if (min >= 60) {
            str = min / 60 + "小时" + min % 60 + "分钟";
        }
        // 一分钟以上
        else if (min >= 1) {
            str = min + "分钟";
        }
        // 0分钟
        else {
            str = "0";
        }

        return str;
    }


    /**
     * 脚本购买类型
     *
     * @param buyPrice
     * @param buyDays
     * @return
     */
    public static String toScriptPriceType(float buyPrice, int buyDays) {
        String str = "暂无";
        if (buyPrice <= 0 && buyDays <= 0) {
            return str;
        }
        // 永久
        if (buyDays >= 36600) {
            str = "永久";
        } else {
            String dayStr = "";
            // 永久
            if (buyDays >= 36600) {
                dayStr = "永久";
            }
            // 1年以上
            else if (buyDays >= 366) {
                dayStr = "年";
            }
            // 1季以上
            else if (buyDays >= 90) {
                dayStr = "季";
            }
            // 1个月以上
            else if (buyDays >= 30) {
                dayStr = "月";
            }
            // 一周以上
            else if (buyDays >= 7) {
                dayStr = "周";
            }
            // 1天以上
            else {
                dayStr = "日";
            }

            // 保留小数点后两位
            DecimalFormat df = new DecimalFormat("0.00");
            str = df.format(buyPrice) + "/" + dayStr;
        }

        return str;
    }

    public static String toEventTime(long time) {
        String timeDate = new SimpleDateFormat("MM-dd").format(new Date(time));
        String timeHour = new SimpleDateFormat("HH:mm").format(new Date(time));
        return timeDate + " " + timeHour;
    }

    /**
     * 时间戳转化为年月日
     * @param time
     * @return
     */
    public static String toYMD(long time) {
        Calendar calendar = Calendar.getInstance();
        calendar.setTimeInMillis(time);
        String ymdStr = calendar.get(Calendar.YEAR) + "年"
                + (calendar.get(Calendar.MONTH) + 1) + "月"
                + calendar.get(Calendar.DATE) + "日";
        return ymdStr;
    }

}
