package com.auric.intell.commonlib.connectivity.base;

/**
 * @author white
 * @description：连接结果
 * @date 2017/9/5
 */

public class ConnectivityResult<D> {

    public static final int BASE_BUILD_SERVER = 1000;
    public static final int BASE_CONNECT_CLIENT = 2000;
    public static final int BASE_DATA = 3000;

    protected int code;
    protected D data;

    public ConnectivityResult(int code, D data) {
        this.code = code;
        this.data = data;
    }

    public D getData() {
        return data;
    }

    public ConnectivityResult setData(D data) {
        this.data = data;
        return this;
    }

    public int getCode() {
        return code;
    }

    public ConnectivityResult setCode(int code) {
        this.code = code;
        return this;
    }
}
