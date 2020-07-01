package com.auric.intell.commonlib.utils;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;

/**
 * Created by zhangxiliang on 2017/9/18.
 */

public class DateUtils {
    /**
     * 传入Data类型日期，返回字符串类型时间（ISO8601标准时间）
     * @param date
     * @return
     */
    public static String getISO8601Timestamp(Date date){
        TimeZone tz = TimeZone.getTimeZone("Asia/Shanghai");
        DateFormat df = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss'+08:00'");
        df.setTimeZone(tz);
        String nowAsISO = df.format(date);
        return nowAsISO;
    }
}
