package com.auric.intell.commonlib.uikit;

import java.io.Serializable;

/**
 * Created by zhangxiliang on 2017/3/23.
 */

public class BaseEvent<T> implements Serializable{

    public static final int EVENT_BIND_OK=1;
    public static final int EVENT_BIND_FAIL=2;
    public static final int EVENT_BIND_TIMEOUT = 10;
    public static final int EVENT_CANCEL=4;
    public static final int EVENT_START=3;
    public static final int EVENT_FINISH_ACTIVITY = 5;
    public static final int EVENT_NET_GUIDE_SHOW_HOME = 6;
    public static final int EVENT_NET_GUIDE_ENTER_OFFLINE_SCENE = 7;
    public static final int EVENT_NET_GUIDE_FIRST = 8;

    public static final int EVENT_ORDER_EXIT = 101;
    public static final int EVENT_ORDER_SHOW_LIST = 102;
    public static final int EVENT_ORDER_SHOW_ITEM = 103;
    public static final int EVENT_ORDER_SHOW_QRCODE = 104;
    public static final int EVENT_ORDER_SHOW_PUBLIC_QRCODE = 105;

    int id;
    T obj;

    public BaseEvent(){}

    public BaseEvent(int id,T t){
        this.id=id;
        this.obj=t;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public T getObj() {
        return obj;
    }

    public void setObj(T obj) {
        this.obj = obj;
    }
}
