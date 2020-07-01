package com.auric.intell.commonlib.connectivity.ap;

import android.os.SystemClock;
import android.text.TextUtils;

import com.auric.intell.commonlib.connectivity.base.IServer;
import com.auric.intell.commonlib.utils.LogTool;
import com.auric.intell.commonlib.utils.LogUtils;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.LinkedList;
import java.util.Queue;


/**
 * @author white
 * @description：TCP协议server端
 * 目前结构是：1个Server管理一组Client，并且发送数据统一发送到接收到的所有client连接
 * @date 2017/9/6
 */

public class TcpServer implements IServer<String> {

    private static final String TAG = "TcpServer";
    /**
     * TCP状态
     */
    public enum TcpServerState{
        IDLE,
        BUILDING,
        BUILDED,
        BUILDED_FAILED,
        RECEIVED_CLIENT,
        CLIENT_CONNECT,
        CLIENT_DISCONNECTED,
        CLOSED

    }

    /**
     * 最多尝试建立连接次数
     */
    private static final int MAX_COUNT_RETRY_BUILD = 3;
    /**
     * 默认接受最多的客户端数量
     */
    private static final int DEFUALT_MAX_COUNT_RECEIVE_CLIENT = 3;
    /**
     * 尝试连接间隔时间
     */
    private static final int RETRY_INTERNEL_MS = 500;


    private TcpServerState mState = TcpServerState.IDLE;
    private ServerSocket mServerSocket;
    private TcpServerObsv mObsv;
    private Queue<TcpClient> mClientList;
    private int mPort;
    private int mClientCount = DEFUALT_MAX_COUNT_RECEIVE_CLIENT;
    private volatile boolean mRunning;


    public TcpServer(int port, TcpServerObsv obsv) {
        mPort = port;
        mObsv = obsv;
        mClientList = new LinkedList<>();
    }

    @Override
    public void buildServer() {
        LogUtils.d("");
        if (mState == TcpServerState.BUILDED || mState == TcpServerState.BUILDING) {
            return;
        }
        new Thread(mServerMainTask).start();
    }


    @Override
    public void closeServer() {
        if (mState == TcpServerState.CLOSED) {
            return;
        }
        LogUtils.d("closeServer");
        onStateChange(TcpServerState.CLOSED);
        // close client list
//        for (TcpClient client : mClientList) {
//            client.closeConnection();
//        }
//
//        mClientList.clear();

        TcpClient client;
        while ((client=mClientList.poll())!=null){
            client.closeConnection();
        }

        // close server
        mRunning = false;
        if (mServerSocket != null) {
            try {
                mServerSocket.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        mServerSocket = null;
    }

    @Override
    public void sendData(String data) {
        LogUtils.d("data:"+data);
        LogUtils.d("mClientList.size():"+mClientList.size());
        if (TextUtils.isEmpty(data)) {
            return;
        }
        for (TcpClient client : mClientList) {
            client.sendData(data);
        }

    }

    public void sendData(String ip,String data){
        for (TcpClient client : mClientList) {
            if(client.getIP().equals(ip)){
                client.sendData(data);
            }
        }
    }

    public Queue<TcpClient> getClients(){
        return mClientList;
    }

    public void setClientCount(int count) {
        mClientCount = count;
    }

    public int getClientNum(){
        return mClientList.size();
    }
    private void onStateChange(TcpServerState state) {
        LogUtils.d("state:"+state);
        mState = state;
        if (mObsv != null) {
            mObsv.onStateChange(mState);
        }
    }

    private boolean doBuildServer(int retryCount) {
        LogUtils.d("retryCount:"+retryCount);
        for (int i = 0; i < retryCount; i++) {
            try {
                mServerSocket = new ServerSocket(mPort);
                return true;
            } catch (Exception e) {
                e.printStackTrace();
                SystemClock.sleep(RETRY_INTERNEL_MS);
            }
        }
        return false;
    }

    /**
     * 核心线程
     */
    private Runnable mServerMainTask = new Runnable() {
        @Override
        public void run() {
            // 建立Server
            onStateChange(TcpServerState.BUILDING);
            boolean result = doBuildServer(MAX_COUNT_RETRY_BUILD);
            if (!result) {
                onStateChange(TcpServerState.BUILDED_FAILED);
                return;
            }

            // 建立成功
            onStateChange(TcpServerState.BUILDED);
            // 监听Client
            mRunning = true;
            int count = 0;
            while(mRunning && mClientList.size() < mClientCount){
                LogTool.i(TAG, "正在监听Client... " + count);
                try {
                    final Socket socket = mServerSocket.accept();

                    if(socket == null){
                        break;
                    }

                    // 接受到Client连接
                    TcpClient client = new TcpClient(socket, new TcpClientObsv() {
                        @Override
                        public void onStateChange(TcpClient client, TcpClient.TcpClientState state) {
                            LogTool.d(TAG, "onReceiveData client state:"+state);
                            if (state != TcpClient.TcpClientState.CONNECTING
                                    && state != TcpClient.TcpClientState.CONNECTED) {
//                                client.closeConnection();
                                try {
                                    mClientList.remove(client);
                                } catch (Exception e) {
                                    e.printStackTrace();
                                }
                                TcpServer.this.onStateChange(TcpServerState.CLIENT_DISCONNECTED);
//                                if (mClientList.size() == 0) {
//                                    closeServer();
//                                }
                            }
                        }

                        @Override
                        public void onReceiveData(TcpClient client, String data) {
                            LogTool.d(TAG, "onReceiveData client data:"+data);
                            if (mObsv != null) {
                                mObsv.onReceiveData(client, data);
                            }
                        }
                    });
                    mClientList.add(client);
                    client.connect();
                    count++;
                    TcpServer.this.onStateChange(TcpServerState.CLIENT_CONNECT);
                    LogTool.i(TAG, "接收到Client... " + count);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            LogTool.i(TAG, "关闭Server ");
            // 关闭Server
            mRunning = false;
            if (mServerSocket != null) {
                try {
                    mServerSocket.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
                mServerSocket = null;
            }
        }
    };
}
