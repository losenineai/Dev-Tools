package com.xxlib.utils;

import android.annotation.SuppressLint;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

@SuppressLint("SimpleDateFormat")
public class TimeUtils {

    public static final SimpleDateFormat DEFAULT_DATE_FORMAT = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    public static final SimpleDateFormat DATE_FORMAT_DATE = new SimpleDateFormat("yyyy-MM-dd");
    public static final SimpleDateFormat DATE_FORMAT_HOUR = new SimpleDateFormat("HH:mm:ss");
    public static final SimpleDateFormat DATE_FORMAT_HOUR2 = new SimpleDateFormat("HH:mm");
    public static final SimpleDateFormat DATE_FORMAT_DATE2 = new SimpleDateFormat("MM/dd HH:mm:ss");
    public static final SimpleDateFormat DATE_FORMAT_MIN = new SimpleDateFormat("yyyy-MM-dd HH:mm");
    public static final SimpleDateFormat DATE_FORMAT_MIN2 = new SimpleDateFormat("MM/dd/yyyy HH:mm");
    public static final SimpleDateFormat DATE_FORMAT_DATE3 = new SimpleDateFormat("MM-dd HH:mm");
    public static final SimpleDateFormat DATE_FORMAT_DATE4 = new SimpleDateFormat("MM/dd/yyyy");
    public static final SimpleDateFormat DATE_FORMAT_DATE5 = new SimpleDateFormat("yyyy/MM/dd");

    /**
     * long time to string
     *
     * @param timeInMillis
     * @param dateFormat
     * @return
     */
    public static String getTime(long timeInMillis, SimpleDateFormat dateFormat) {
        return dateFormat.format(new Date(timeInMillis * 1000));
    }

    /**
     * int time to string
     *
     * @param timeInSecond
     * @return
     */
    public static String getDate(int timeInSecond) {
        return DATE_FORMAT_DATE.format(new Date((long) timeInSecond * 1000));
    }
    /**
     * int time to string
     *
     * @return
     */
    public static String getDate(long timeInMillis) {
        return DATE_FORMAT_DATE.format(new Date(timeInMillis));
    }


    /**
     * int time to string
     *
     * @return
     */
    public static String getDateBySecond(long timeInSecond) {
        return DATE_FORMAT_DATE.format(new Date(timeInSecond * 1000));
    }

    /**
     * wxj, int time to string
     *
     * @param timeInSecond
     * @return
     */
    public static String getDate(SimpleDateFormat dataFormat, int timeInSecond) {
        return dataFormat.format(new Date((long) timeInSecond * 1000));
    }

    public static String getOpenModuleDate(long seconds, boolean isDetail) {
        String time = DATE_FORMAT_HOUR2.format(new Date(seconds * 1000));
        Calendar calendar = Calendar.getInstance();
        Calendar tempCalendar = (Calendar) calendar.clone();
        tempCalendar.setTimeInMillis(seconds * 1000);
        if (calendar.get(Calendar.DAY_OF_YEAR) == tempCalendar.get(Calendar.DAY_OF_YEAR))
            time = "今天" + (isDetail ? (" " + time) : "");
        else if (calendar.get(Calendar.DAY_OF_YEAR) - tempCalendar.get(Calendar.DAY_OF_YEAR) == 1)
            time = "昨天" + (isDetail ? (" " + time) : "");
        else if (calendar.get(Calendar.DAY_OF_YEAR) - tempCalendar.get(Calendar.DAY_OF_YEAR) == -1)
            time = "明天" + (isDetail ? (" " + time) : "");
        else
            time = new SimpleDateFormat("MM月dd日").format(new Date(seconds * 1000)) + (isDetail ? (" " + time) : "");
        return time;
    }

    /**
     * 果盘游戏开服开测时间格式化
     * chenrenzhan 添加
     *
     * @param seconds
     * @param isDetail
     * @return
     */
    public static String getGPGameOpenModuleDate(long seconds, boolean isDetail) {
        String time = DATE_FORMAT_HOUR2.format(new Date(seconds * 1000));
        Calendar calendar = Calendar.getInstance();
        Calendar tempCalendar = (Calendar) calendar.clone();
        tempCalendar.setTimeInMillis(seconds * 1000);
        if (calendar.get(Calendar.DAY_OF_YEAR) == tempCalendar.get(Calendar.DAY_OF_YEAR))
            time = "今天" + (isDetail ? (" " + time) : "");
        else if (calendar.get(Calendar.DAY_OF_YEAR) - tempCalendar.get(Calendar.DAY_OF_YEAR) == 1)
            time = "昨天" + (isDetail ? (" " + time) : "");
        else if (calendar.get(Calendar.DAY_OF_YEAR) - tempCalendar.get(Calendar.DAY_OF_YEAR) == -1)
            time = "明天" + (isDetail ? (" " + time) : "");
        else
            time = new SimpleDateFormat("MM-dd").format(new Date(seconds * 1000)) + (isDetail ? (" " + time) : "");
        return time;
    }

    /**
     * int time to string
     *
     * @param timeInMillis
     * @return
     * @description
     */
    public static String getTimeForMin(int timeInMillis) {
        return DATE_FORMAT_MIN.format(new Date(timeInMillis * 1000L));
    }

    /**
     * int time to string
     *
     * @param timeInMillis
     * @return
     * @description
     */
    public static String getTimeForMin(long timeInMillis) {
        return DATE_FORMAT_MIN.format(new Date(timeInMillis));
    }

    /**
     * long time to string, format is {@link #DEFAULT_DATE_FORMAT}
     *
     * @param timeInMillis
     * @return
     */
    public static String getTime(long timeInMillis) {
        return getTime(timeInMillis, DATE_FORMAT_DATE2);
    }

    /**
     * get current time in milliseconds
     *
     * @return
     */
    public static long getCurrentTimeInLong() {
        return System.currentTimeMillis();
    }

    /**
     * get current time in milliseconds, format is {@link #DEFAULT_DATE_FORMAT}
     *
     * @return
     */
    public static String getCurrentTimeInString() {
        return getTime(getCurrentTimeInLong());
    }

    /**
     * get current time in milliseconds
     *
     * @return
     */
    public static String getCurrentTimeInString(SimpleDateFormat dateFormat) {
        return getTime(getCurrentTimeInLong(), dateFormat);
    }

    /**
     * int time to string
     * @param time
     * @return
     */
    public static String getTimeForSec(int time) {
        return DEFAULT_DATE_FORMAT.format(new Date(time * 1000L));
    }

    /**
     * int time to string
     * @param time
     * @return
     */
    public static String getTimeForSec(long time) {
        return DEFAULT_DATE_FORMAT.format(new Date(time));
    }

    /**
     * @param time
     * @return
     * @description 将秒数转成标志格式的 分:秒 格式
     */
    public static String transferMinuteFromInt(int time) {
        StringBuffer result = new StringBuffer();
        int minute = time / 60;
        int second = time % 60;
        result.append(minute);
        result.append(":");
        if (second < 10) {
            result.append("0");
        }
        result.append(second);
        return result.toString();
    }

    /**
     * @param time
     * @return
     * @description 将秒数转成标志格式的 分:秒 格式
     */
    public static String transferMinuteFromInt(long time) {
        StringBuffer result = new StringBuffer();
        int minute = (int) (time / (60 * 1000));
        int second = (int) (time % (60 * 1000)) / 1000;
        result.append(minute);
        result.append(":");
        if (second < 10) {
            result.append("0");
        }
        result.append(second);
        return result.toString();
    }

    /**
     * @param time
     * @return
     * @description 将秒数转成标志格式的 分:秒 格式
     */
    public static String transferMinuteFromTime(int time) {
        StringBuffer result = new StringBuffer();
        int minute = time / 60;
        int second = time % 60;
        result.append(minute);
        result.append("分");
        if (second < 10) {
            result.append("0");
        }
        result.append(second);
        result.append("秒");
        return result.toString();
    }

    /**
     * @param time
     * @return
     * @description 将秒数转成标志格式的 分:秒 格式
     */
    public static String transferMinuteFromTime(long time) {
        StringBuffer result = new StringBuffer();
        int minute = (int) (time / (60 * 1000));
        int second = (int) (time % (60 * 1000)) / 1000;
        result.append(minute);
        result.append("分");
        if (second < 10) {
            result.append("0");
        }
        result.append(second);
        result.append("秒");
        return result.toString();
    }

    /**
     * @param secondTime 秒
     * @return
     * @description 将秒数转成标志格式的 时:分:秒 格式
     */
    public static String transferHourMinuteFromInt(int secondTime) {
        StringBuffer result = new StringBuffer();
        int hour = secondTime / 3600;
        int lastMinute = secondTime % 3600;
        int minute = lastMinute / 60;
        int second = lastMinute % 60;
        if (hour < 10) {
            result.append("0");
        }
        result.append(hour);
        result.append(":");
        if (minute < 10) {
            result.append("0");
        }
        result.append(minute);
        result.append(":");
        if (second < 10) {
            result.append("0");
        }
        result.append(second);
        return result.toString();
    }

    /**
     * @param secondTime 秒
     * @return
     * @description 将秒数转成标志格式的 时:分:秒 格式
     */
    public static String transferHourMinuteFromInt(long secondTime) {
        StringBuffer result = new StringBuffer();
        int hour = (int) (secondTime / (60 * 60 * 1000));
        int lastMinute = (int) (secondTime % (60 * 60 * 1000));
        int minute = lastMinute / 60;
        int second = lastMinute % 60;
        if (hour < 10) {
            result.append("0");
        }
        result.append(hour);
        result.append(":");
        if (minute < 10) {
            result.append("0");
        }
        result.append(minute);
        result.append(":");
        if (second < 10) {
            result.append("0");
        }
        result.append(second);
        return result.toString();
    }

    public static int calculateDays(long date1, long date2) {
        return (int) (Math.abs(date1 - date2) / 24L / 60L / 60L / 1000L);
    }

    public static int calculateHour(long date1, long date2) {
        return (int) (Math.abs(date1 - date2) / 60L / 60L / 1000L) % 24;
    }

    public static int calculateMin(long date1, long date2) {
        return (int) (Math.abs(date1 - date2) / 60L / 1000L) % 60;
    }

    public static int calculateSecond(long date1, long date2) {
        return (int) (Math.abs(date1 - date2) / 1000L) % 60;
    }

    /**
     * 获取两个日期之间相隔的天数
     * @param time1
     * @param time2
     * @return 返回相隔天数
     */
    public static int getDaysBetween(long time1, long time2) {
        Calendar day1 = Calendar.getInstance();
        day1.setTimeInMillis(time1);
        Calendar day2 = Calendar.getInstance();
        day2.setTimeInMillis(time2);
        if (day1.after(day2)) {
            Calendar swap = day1;
            day1 = day2;
            day2 = swap;
        }
        int days = day2.get(Calendar.DAY_OF_YEAR) - day1.get(Calendar.DAY_OF_YEAR);
        int y2 = day2.get(Calendar.YEAR);
        if (day1.get(Calendar.YEAR) != y2) {
            day1 = (Calendar) day1.clone();
            do {
                days += day1.getActualMaximum(Calendar.DAY_OF_YEAR);//得到当年的实际天数
                day1.add(Calendar.YEAR, 1);
            } while (day1.get(Calendar.YEAR) != y2);
        }
        return days;
    }
}