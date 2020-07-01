package com.auric.intell.commonlib.connectivity.ap;

import android.text.TextUtils;

import com.auric.intell.commonlib.connectivity.base.IClient;
import com.auric.intell.commonlib.utils.LogUtils;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.charset.Charset;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


/**
 * @author white
 * @description：TCP客户端
 * @date 2017/9/6
 */

public class TcpClient implements IClient<String> {

    private static final String TAG = "TcpClient";


    /**
     * TCP状态
     */
    public enum TcpClientState {
        IDLE,
        CONNECTING,
        CONNECTED,
        CONNECTED_FAILED,
        CLOSED
    }

    public enum TcpDataReceiveMode {
        WITHNEWLINE,
        WITHOUTNEWLINE
    }

    private TcpDataReceiveMode tcpDataReceiveMode = TcpDataReceiveMode.WITHNEWLINE;

    /**
     * 最多尝试建立连接次数
     */
    private static final int MAX_COUNT_RETRY_CONNECT = 5;
    /**
     * 尝试连接间隔时间
     */
    private static final int RETRY_INTERNEL_MS = 2000;

    private TcpClientState mState = TcpClientState.IDLE;
    private TcpClientObsv mObsv;
    private Socket mSocket;
    private InputStream mInStream;
    private OutputStream mOutStream;
    private String mHostName;
    private int mPort;
    private volatile boolean mRunning;

    private static final ExecutorService sOperExecutor = Executors.newFixedThreadPool(50);


    public TcpClient(Socket socket, TcpClientObsv obsv) {
        this.mSocket = socket;
        mObsv = obsv;
    }

    public TcpClient(String hostName, int port, TcpClientObsv obsv) {
        LogUtils.d("TcpClient:Hostname:"+hostName);
        mHostName = hostName;
        mPort = port;
        mObsv = obsv;
    }

    @Override
    public void connect() {
        LogUtils.d("tcp connect() start");
        LogUtils.d("TCPSTATEconnect:"+mState);
        if (mState == TcpClientState.CONNECTING || mState == TcpClientState.CONNECTED) {
            return;
        }
        new Thread(mClientMainTask).start();
    }

    @Override
    public void closeConnection() {
        LogUtils.d("");
        if (mState == TcpClient.TcpClientState.CLOSED) {
            return;
        }
        mRunning = false;
        onStateChange(TcpClientState.CLOSED);
        try {
            if (mInStream != null) {
                mInStream.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        try {
            if (mOutStream != null) {
                mOutStream.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        try {
            if (mSocket != null) {
                mSocket.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        mInStream = null;
        mOutStream = null;
        mSocket = null;
    }


    @Override
    public void sendData(final String data) {
        LogUtils.d("data:" + data);
        if (mState != TcpClientState.CONNECTED
                || TextUtils.isEmpty(data)) {
            return;
        }

        sOperExecutor.execute(new Runnable() {
            @Override
            public void run() {
                try {
//                    PrintWriter printWriter = new PrintWriter(mOutStream, true);
//                    printWriter.print(data+"\n");
//                    printWriter.flush();
                    BufferedWriter buffWriter = new BufferedWriter(new OutputStreamWriter(mOutStream, Charset.forName("UTF-8")));
                    buffWriter.write(data + "\r\n");
                    buffWriter.flush();
                    LogUtils.d("sendData success");
                } catch (Throwable t) {
                    LogUtils.e(t);
                }
            }
        });
    }


    public TcpClientState getState() {
        return mState;
    }

    public void setTcpDataReceiveMode(TcpDataReceiveMode mode) {
        tcpDataReceiveMode = mode;
    }

    private void onStateChange(TcpClientState state) {
        LogUtils.d("state:" + state);
        mState = state;
        if (mObsv != null) {
            mObsv.onStateChange(this, state);
        }
    }

    /**
     * 重连机制连接服务端
     *
     * @param retryCount
     * @return
     */
    private boolean doConnectServer(int retryCount) {
        LogUtils.e("retryCount:" + retryCount);
        for (int i = 0; i < retryCount; i++) {
            try {
                LogUtils.e("retryCount:" + i);
                InetSocketAddress address = new InetSocketAddress(mHostName, mPort);
                LogUtils.e("myHostName:"+mHostName);
                LogUtils.e("hostName:" + address.getHostName());
                LogUtils.e("port:" + address.getPort());
                mSocket = new Socket();
                mSocket.connect(address, 1000 * 2);
                return true;
            } catch (Exception e) {
                LogUtils.e(e);
                mSocket = null;
                try {
                    Thread.sleep(RETRY_INTERNEL_MS);
                } catch (InterruptedException e1) {
                    e1.printStackTrace();
                }
            }
        }
        return false;
    }

    public String getIP() {
        return mSocket.getInetAddress().toString();
    }

    // thread main
    private Runnable mClientMainTask = new Runnable() {
        @Override
        public void run() {
            LogUtils.e("tcp mClientMainTask start");
            onStateChange(TcpClientState.CONNECTING);
            // 连接Server
            if (mSocket == null) {
                if (!doConnectServer(MAX_COUNT_RETRY_CONNECT)) {
                    // 连接失败
                    onStateChange(TcpClientState.CONNECTED_FAILED);
                    return;
                }
            }


            // 获取输入流，输出流
            try {
                // 读超时，0表示无限
                mSocket.setSoTimeout(0);
                mSocket.setKeepAlive(true);
                mInStream = mSocket.getInputStream();
                mOutStream = mSocket.getOutputStream();
            } catch (Exception e) {
                onStateChange(TcpClientState.CONNECTED_FAILED);
                closeConnection();
                return;
            }
            LogUtils.e("tcp mClientMainTask success");
            // 连接成功
            onStateChange(TcpClientState.CONNECTED);

            if (tcpDataReceiveMode == TcpDataReceiveMode.WITHNEWLINE) {
                BufferedReader lineReader = new BufferedReader(new InputStreamReader(mInStream));
                // TODO: 2017/10/21
                mRunning = true;
//            mRunning = false;
                // 进入循环监听socket接受数据
                while (mRunning) {
                    String readLine = null;

                    try {
                        LogUtils.e("TcpClient receiving line...");
                        readLine = lineReader.readLine();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                    LogUtils.e("TcpClient received line:" + readLine);

                    if (readLine != null && mObsv != null) {
                        final String resultLine = readLine;

                        // 子线程回调，避免阻塞
                        sOperExecutor.execute(new Runnable() {
                            @Override
                            public void run() {
                                mObsv.onReceiveData(TcpClient.this, resultLine);
                            }
                        });

                    } else if (null == readLine) {
                        LogUtils.e("null == readLine");
                        onStateChange(TcpClientState.CONNECTED_FAILED);
                        break;
                    }


                }
                try {
                    lineReader.close();
                } catch (Exception e) {
                }
            }else if(tcpDataReceiveMode==TcpDataReceiveMode.WITHOUTNEWLINE){

                DataInputStream dis = new DataInputStream(new BufferedInputStream(
                        mInStream));
                byte[] bytes = new byte[1]; // 一次读取一个byte
                String data = "";
                try {
                    while (dis.read(bytes) != -1) {
                        data += (char)bytes[0];
                        if (dis.available() == 0) { //一个请求
                            mObsv.onReceiveData(TcpClient.this,data);
                            data="";
                        }
                    }
                    onStateChange(TcpClientState.CLOSED);
                } catch (IOException e) {
                    e.printStackTrace();
                    onStateChange(TcpClientState.CONNECTED_FAILED);
                }
            }


            // 断开连接从while循环出来
            closeConnection();
        }
    };

    public int getPort(){
        return mPort;
    }
}
