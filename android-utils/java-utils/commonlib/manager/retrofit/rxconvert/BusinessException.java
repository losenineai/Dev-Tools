package com.auric.intell.commonlib.manager.retrofit.rxconvert;

import com.auric.intell.commonlib.manager.retrofit.TResponse;
import com.auric.intell.commonlib.utils.JSONUtils;
import com.auric.intell.commonlib.utils.StringUtil;

/**
 * Created by zhangxiliang on 2017/12/26.
 */

public class BusinessException extends Exception {

    int code;
    String message;

    public int getCode() {
        return code;
    }

    public void setCode(int code) {
        this.code = code;
    }

    @Override
    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public BusinessException() {
        super();
    }


    public BusinessException(String detailMessage) {
        super(detailMessage);

    }

    public BusinessException(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public BusinessException(Throwable throwable) {
        super(throwable);
        if (!StringUtil.isEmpty(throwable.getMessage())) {
            TResponse tResponse = JSONUtils.parseJsonObj(throwable.getMessage(), TResponse.class);
            if(tResponse==null) {
                tResponse=new TResponse();
                tResponse.setMessage("当前网络不可用");
            }
            this.code = tResponse.getStatus_code();
            this.message = tResponse.getMessage();
        }
    }

    @Override
    public String toString() {
        return "BusinessException{" +
                "code=" + code +
                ", message='" + message + '\'' +
                '}';
    }
}
