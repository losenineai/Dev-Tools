package com.auric.intell.commonlib.manager.retrofit;

import android.net.ParseException;

import com.auric.intell.commonlib.manager.retrofit.rxconvert.BusinessException;
import com.auric.intell.commonlib.utils.JSONUtils;
import com.auric.intell.commonlib.utils.StringUtil;
import com.google.gson.JsonParseException;

import org.apache.http.conn.ConnectTimeoutException;
import org.json.JSONException;

import java.net.ConnectException;
import java.util.HashMap;

import retrofit2.adapter.rxjava.HttpException;

/**
 * Created by Tamic on 2016-08-12.
 */
public class ExceptionHandle {

    private static final int UNAUTHORIZED = 401;
    private static final int FORBIDDEN = 403;
    private static final int NOT_FOUND = 404;
    private static final int REQUEST_TIMEOUT = 408;
    private static final int INTERNAL_SERVER_ERROR = 500;
    private static final int BAD_GATEWAY = 502;
    private static final int SERVICE_UNAVAILABLE = 503;
    private static final int GATEWAY_TIMEOUT = 504;

    private static HashMap<String,String> errorTipMap=new HashMap<>();

    static {
        errorTipMap.put("account_exists","帐号已经存在无法发送注册验证码");
        errorTipMap.put("mobile_in_sms_blacklist","手机号被列入短信提供商的号码黑名单中");
        errorTipMap.put("sms_request_too_frequently","短信请求太过频繁");
        errorTipMap.put("sms_request_times_limited","手机号请求短信次数超过日限制");
        errorTipMap.put("incorrect_mobile_format","手机号格式有误");
        errorTipMap.put("sms_code_expired","验证码已失效");
        errorTipMap.put("require_different_password","新旧密码相同");
        errorTipMap.put("invalid_credentials","验证码输入错误");
        errorTipMap.put("login_fail","账号或密码错误");
        errorTipMap.put("user_not_found","用户不存在");
        errorTipMap.put("token_invalid","Token过期请重新登录");
        errorTipMap.put("validation_error","验证码错误");
        errorTipMap.put("baby_not_found","未找到绑定设备");
        errorTipMap.put("invalid_binding_code","验证码无效");
        errorTipMap.put("binding_service_unavailable","绑定服务不可用");
        errorTipMap.put("unauthorized_media_url","未授权的媒体资源链接");
        errorTipMap.put("robot_not_match","机器人不属于该用户");
        errorTipMap.put("item_not_found","找不到对应的媒体");
        errorTipMap.put("robot_not_found","找不到对应的机器人");
        errorTipMap.put("robot_command_not_found","找不到对应的机器人操作");
        errorTipMap.put("net_or_service_disable","网络异常或服务不可用");
    }

    public static ResponeThrowable handleException(Throwable throwable) {


        return new ResponeThrowable(throwable.getMessage());
      /*  ResponeThrowable ex;
        if (e instanceof HttpException) {
            HttpException httpException = (HttpException) e;
            ex = new ResponeThrowable(e, ERROR.HTTP_ERROR);
            switch (httpException.code()) {
                case UNAUTHORIZED:
                case FORBIDDEN:
                case NOT_FOUND:
                case REQUEST_TIMEOUT:
                case GATEWAY_TIMEOUT:
                case INTERNAL_SERVER_ERROR:
                case BAD_GATEWAY:
                case SERVICE_UNAVAILABLE:
                default:
                    ex.message =httpException.response().message();
                    break;
            }
            return ex;
        } else if (e instanceof ServerException) {
            ServerException resultException = (ServerException) e;
            ex = new ResponeThrowable(resultException, resultException.status_code);
            ex.message = resultException.message;
            return ex;
        } else if (e instanceof JsonParseException
                || e instanceof JSONException
                || e instanceof ParseException) {
            ex = new ResponeThrowable(e, ERROR.PARSE_ERROR);
            ex.message = "paresefail";
            ex.errorTips = "解析错误";
            return ex;
        } else if (e instanceof ConnectException) {
            ex = new ResponeThrowable(e, ERROR.NETWORD_ERROR);
            ex.message = "connectfail";
            ex.errorTips = "连接失败";

            return ex;
        } else if (e instanceof javax.net.ssl.SSLHandshakeException) {
            ex = new ResponeThrowable(e, ERROR.SSL_ERROR);
            ex.message = "verfiyfail";
            ex.errorTips = "证书验证失败";

            return ex;
        } else if (e instanceof ConnectTimeoutException){
            ex = new ResponeThrowable(e, ERROR.TIMEOUT_ERROR);
            ex.message = "手机网络不太稳定";
            ex.errorTips = "手机网络不太稳定";

            return ex;
        } else if (e instanceof java.net.SocketTimeoutException) {
            ex = new ResponeThrowable(e, ERROR.TIMEOUT_ERROR);
            ex.message = "手机网络不太稳定";
            ex.errorTips = "连接超时";

            return ex;
        }else if(e instanceof java.net.UnknownHostException){
            ex = new ResponeThrowable(e, ERROR.UNKNOWN_HOST);
            ex.message = "网络异常";
            ex.errorTips = "网络异常";
        } else {
            ex = new ResponeThrowable(e, ERROR.UNKNOWN);
            ex.message = "手机网络不太稳定";
            ex.errorTips="手机网络不太稳定";
            return ex;
        }
        return  ex;*/
    }


    /**
     * 约定异常
     */
    class ERROR {

        /**
         * 网络异常
         */
        public static final int UNKNOWN_HOST = 1007;

        /**
         * 未知错误
         */
        public static final int UNKNOWN = 1000;
        /**
         * 解析错误
         */
        public static final int PARSE_ERROR = 1001;
        /**
         * 网络错误
         */
        public static final int NETWORD_ERROR = 1002;
        /**
         * 协议出错
         */
        public static final int HTTP_ERROR = 1003;

        /**
         * 证书出错
         */
        public static final int SSL_ERROR = 1005;

        /**
         * 连接超时
         */
        public static final int TIMEOUT_ERROR = 1006;
    }

    public static class ResponeThrowable extends Exception {
        public int status_code;
        public String message;
        public String errorTips;


        public ResponeThrowable(String errorMsg){
            super(errorMsg);

            if (!StringUtil.isEmpty(errorMsg)) {
                TResponse tResponse = JSONUtils.parseJsonObj(errorMsg, TResponse.class);
                if(tResponse==null) {
                    tResponse=new TResponse();
                    tResponse.setMessage("net_or_service_disable");
                }
                this.status_code=tResponse.getStatus_code();
                this.message=tResponse.getMessage();
                this.errorTips=errorTipMap.get(this.message);
            }
        }

        public ResponeThrowable(Throwable throwable, int code) {
            super(throwable);
            this.status_code = code;
        }

        public ResponeThrowable(int code,String message) {
            this.message=message;
            this.status_code = code;
            this.errorTips=errorTipMap.get(message);
        }

    }

    public class ServerException extends RuntimeException {
        public int status_code;
        public String message;
    }





}

