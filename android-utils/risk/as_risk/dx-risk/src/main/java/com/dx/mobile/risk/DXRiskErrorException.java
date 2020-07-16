package com.dx.mobile.risk;

/**
 * Created by blue on 2017/10/20.
 */

/**
 * DX安全组件异常
 */
public final class DXRiskErrorException extends RuntimeException {

    public DXRiskErrorException(String message) {
        super(message);
    }

    public DXRiskErrorException(String message, Throwable throwable){
        super(message, throwable);
    }
}
