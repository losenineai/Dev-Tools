package com.auric.intell.commonlib.framework.mvp;

/**
 * @author white
 * @description：MVPS 的Status (自创新框架MVPS)
 * @date 2016/12/5
 */

public interface IMvpStatus<S extends IMvpStatus> {

    S setParams(Object... objs);

    Object[] getParams();

}
