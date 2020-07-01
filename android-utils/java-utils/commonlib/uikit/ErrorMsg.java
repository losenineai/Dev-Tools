package com.auric.intell.commonlib.uikit;

/**
 * Created by zhangxiliang on 2018/1/8.
 */

public class ErrorMsg {

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
