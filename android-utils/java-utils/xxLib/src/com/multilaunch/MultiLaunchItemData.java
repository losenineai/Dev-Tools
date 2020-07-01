package com.multilaunch;

/**
 * Created by chenrenzhan on 15/12/1.
 */
public class MultiLaunchItemData {
    public enum CopyState {
        STATE_NORMAL,    // 按钮状态：普通
        STATE_PROCESS,   // 按钮状态：进度中
        STATE_COMPLETE,  // 按钮状态：进度完成
        STATE_FAILED     // 按钮状态：进度失败
    }

    private CopyState copyState;
    private IAppInfo appInfo;

    public MultiLaunchItemData(IAppInfo appInfo) {
        this.appInfo = appInfo;
        copyState = CopyState.STATE_NORMAL;
    }

    public CopyState getCopyState() {
        return copyState;
    }

    public void setCopyState(CopyState copyState) {
        this.copyState = copyState;
    }

    public IAppInfo getAppInfo() {
        return appInfo;
    }

    public void setAppInfo(IAppInfo appInfo) {
        this.appInfo = appInfo;
    }
}
