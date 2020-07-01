package com.xxAssistant.DanMuKu.plugin.apk;

import android.net.LocalSocket;

/**
 * Created by white on 16/1/22.
 */
public interface IXXIpcServerObsv {

    void onBuildSucc();

    void onBuildFailed();

    void onServerClosed();

    void onReceiveClient(LocalSocket socket);
}
