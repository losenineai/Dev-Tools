package com.auric.intell.commonlib.robot;

/**
 * Created by zhangxiliang on 2017/4/26.
 */

public class Token {
    private String token;
    private String status_code;
    private String message;

    public String getToken() {
        return token;
    }

    public void setToken(String token) {
        this.token = token;
    }

    public String getStatus_code() {
        return status_code;
    }

    public void setStatus_code(String status_code) {
        this.status_code = status_code;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    @Override
    public String toString() {
        return "Token{" +
                "token='" + token + '\'' +
                ", status_code='" + status_code + '\'' +
                ", message='" + message + '\'' +
                '}';
    }
}
