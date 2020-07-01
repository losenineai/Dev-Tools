package com.auric.intell.commonlib.manager.retrofit;

import java.io.Serializable;

/**
 * Created by zhangxiliang on 2017/6/20.
 */

public class TResponse implements Serializable{

    /**
     * status_code : 404
     * message : user_not_found
     */

    private int status_code;
    private String message;



    public int getStatus_code() {
        return status_code;
    }

    public void setStatus_code(int status_code) {
        this.status_code = status_code;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }
}
