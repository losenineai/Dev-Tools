package com.auric.intell.commonlib.robot;

/**
 * Created by zhangxiliang on 2017/6/12.
 */

public class SyncClock {
    /**
     * today : 星期一
     * datetime : 2016-04-18T01:01:29+0800
     * date : 2016-04-18
     */

    private String today;
    private String datetime;
    private String date;

    public String getToday() {
        return today;
    }

    public void setToday(String today) {
        this.today = today;
    }

    public String getDatetime() {
        return datetime;
    }

    public void setDatetime(String datetime) {
        this.datetime = datetime;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }
}
