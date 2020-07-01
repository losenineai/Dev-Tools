package com.xxAssistant.DanMuKu.plugin.apk;

import android.net.LocalServerSocket;
import android.net.LocalSocket;

import com.xxlib.utils.base.LogTool;

import java.io.IOException;

/**
 * Created by white on 16/1/22.
 */
public class XXIpcServer {

    private static final String TAG = "XXIpcServer";

    private String mServerName = "";
    private LocalServerSocket mServerSocket;
    /**
     * 最多尝试建立连接次数
     */
    private static final int MAX_COUNT_TRY_BUILD = 3;
    /**
     * 接受最多的客户端数量
     */
    private static final int MAX_COUNT_RECEIVE_CLIENT = 100;
    private static final int TRY_INTERNEL_MS = 500;

    public enum LocalSocketServerState {
        /**
         * 建立中
         */
        SS_BUILDING,
        /**
         * 建立成功
         */
        SS_BUILDED,
        /**
         * 建立失败
         */
        SS_BUILD_FAIL,
        /**
         * 关闭状态
         */
        SS_CLOSED,
    }

    private LocalSocketServerState mState = LocalSocketServerState.SS_CLOSED;
    private boolean mRun;
    private Thread mMainThread;
    private IXXIpcServerObsv mServerObsv;


    public XXIpcServer(String serverName, IXXIpcServerObsv obsv){
        mServerName = serverName;
        mServerObsv = obsv;
    }


    public void beginBuildServer(){
        if(mState == LocalSocketServerState.SS_BUILDING || mState == LocalSocketServerState.SS_BUILDED){
            return;
        }

        mState = LocalSocketServerState.SS_BUILDING;
        mMainThread = new Thread(mServerMainTask);
        mMainThread.start();
    }

    public void closeServer(){
        mState = LocalSocketServerState.SS_CLOSED;
        mRun = false;
        mMainThread = null;
        if(mServerSocket != null){
            try {
                mServerSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            mServerSocket = null;
        }
    }


    private boolean doBuildServer(int maxCount){
        for(int i = 0; i < maxCount; i++){
            try {
                mServerSocket = new LocalServerSocket(mServerName);
                return true;
            } catch (IOException e) {
                e.printStackTrace();
                try {
                    Thread.currentThread().sleep(TRY_INTERNEL_MS);
                } catch (InterruptedException e1) {
                    e1.printStackTrace();
                }
            }
        }
        return false;
    }


    private Runnable mServerMainTask = new Runnable() {
        @Override
        public void run() {
            // 建立Server
            if(doBuildServer(MAX_COUNT_TRY_BUILD)){
                mState = LocalSocketServerState.SS_BUILDED;
                mServerObsv.onBuildSucc();
            }
            else{
                mServerObsv.onBuildFailed();
                closeServer();
                return;
            }

            // 监听Client
            mRun = true;
            int count = 0;
            while(mRun && count < MAX_COUNT_RECEIVE_CLIENT){
                LogTool.i(TAG, "正在监听Client... " + count);
                try {
                    final LocalSocket socket = mServerSocket.accept();
                    if(socket == null){
                        break;
                    }
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            mServerObsv.onReceiveClient(socket);
                        }
                    }).start();
                    count++;
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            // 销毁服务
            mState = LocalSocketServerState.SS_CLOSED;
            mServerObsv.onServerClosed();
            closeServer();

        }
    };

}
