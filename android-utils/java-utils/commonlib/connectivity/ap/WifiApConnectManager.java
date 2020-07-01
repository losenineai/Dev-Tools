package com.auric.intell.commonlib.connectivity.ap;

import android.content.Context;
import android.text.TextUtils;

import com.auric.intell.commonlib.connectivity.base.IConnectivityCallback;
import com.auric.intell.commonlib.connectivity.base.IConnectivityManager;
import com.auric.intell.commonlib.utils.ContextFinder;
import com.auric.intell.commonlib.utils.DeviceUtils;
import com.auric.intell.commonlib.utils.LogUtils;
import com.auric.intell.commonlib.utils.RandomUtils;

/**
 * @author white
 * @description：Wifi AP 连接方案的实现
 * @date 2017/9/5
 */

public class WifiApConnectManager implements IConnectivityManager<WifiApConnectParams, WifiApResult, String>{

    private Context mContext;
    private WifiApConnector mApConnector;
    private WifiConnector mWifiConnnector;
    private TcpClient mTcpClient;
    private TcpServer mTcpServer;
    private int mServerBuildCount;
    private WifiApConnectParams mParams;

    private IConnectivityCallback<WifiApResult> mCallback;
    private WifiConnectReceiver.WifiConnectStateListener mListenr;

    private static final int DEFAULT_SOCKET_PORT = 55555;
    private static final String DEFAULT_CONNECT_IP = "192.168.43.1";

    public WifiApConnectManager() {

    }

    @Override
    public void init(Context context) {
        mContext = context;
    }


    @Override
    public void buildServerAndReceive(WifiApConnectParams params, IConnectivityCallback<WifiApResult> callback) {
        if (params == null) {
            throw new IllegalArgumentException("WifiApConnectParams params is null.");
        }
        if (callback == null) {
            throw new IllegalArgumentException("IConnectivityCallback params is callback.");
        }

        release();
        mCallback = callback;
        mParams = params;

        if (mApConnector == null) {
            mApConnector = new WifiApConnector(mContext);
        }

        mApConnector.openWifiAp(mParams.getSSID(), mParams.getPassword(), WifiBase.WifiCipherType.NONE, new WifiApConnector.WifiApStateListener() {
            @Override
            public void onStateChange(int state) {
                LogUtils.d("state:" + state);
                if (state == WifiApConnector.WIFI_AP_STATE_ENABLED) {
                    if (mApConnector != null) {
                        mApConnector.release();
                        mApConnector = null;
                    }
                    if (mCallback != null) {
                        mCallback.onResult(new WifiApResult(WifiApResult.AP_OPEN_SUCCESS, null));
                    }
                    mServerBuildCount = 0;
                    doBuildServer();
                }
            }
        });
    }

    public void connectWifi(String ssid, String pwd, WifiConnectReceiver.WifiConnectStateListener listener) {
//        release();
        mCallback = null;
        mListenr = null;
        if (mWifiConnnector == null) {
            mWifiConnnector = new WifiConnector(mContext);
        }
        mListenr = listener;
        mWifiConnnector.connectWifi(ssid, pwd, mListenr);
    }

    @Override
    public void connectServer(final WifiApConnectParams params, IConnectivityCallback<WifiApResult> callback) {
        if (params == null) {
            throw new IllegalArgumentException("WifiApConnectParams params is null.");
        }
        if (callback == null) {
            throw new IllegalArgumentException("IConnectivityCallback params is callback.");
        }

        release();
        mCallback = callback;
        if (mWifiConnnector == null) {
            mWifiConnnector = new WifiConnector(mContext);
        }
        LogUtils.e("Wifi Test connectWifi() start ");
        LogUtils.e("params.getSSID()" + params.getSSID() + " params.getPassword()" + params.getPassword());
        mWifiConnnector.connectWifi(params.getSSID(), params.getPassword(), new WifiConnectReceiver.WifiConnectStateListener() {
            @Override
            public void onDisabed() {
                LogUtils.d("");
            }

            @Override
            public void onEnabling() {
                LogUtils.d("");
            }

            @Override
            public void onEnabled() {
                LogUtils.d("");

            }

            @Override
            public void onAuthenFail(String SSID, int error) {
                if (SSID == null) {
                    return;
                }
                LogUtils.d("SSID:" + SSID + " error:" + error);
                if (params.getSSID().equals(SSID)) {
                    if (mCallback != null) {
                        mCallback.onResult(new WifiApResult(WifiApResult.WIFI_CONNECTED_ERROR_AUTHENTICATING, null));
                    }
                }
            }

            @Override
            public void onConnecting(String SSID) {
                LogUtils.d("onConnecting SSID = " + SSID);
                if (!params.getSSID().equals(SSID))
                    reconnectWiFi(params);
            }

            @Override
            public void onConnected(String SSID) {
                LogUtils.d("onConnected SSID:" + SSID);
                if (TextUtils.isEmpty(SSID)) {
                    return;
                }
                LogUtils.d("onConnected");
                if (params.getSSID().equals(SSID)) {
                    if (mWifiConnnector != null) {
                        mWifiConnnector.release();
                    }
                    if (mCallback != null) {
                        mCallback.onResult(new WifiApResult(WifiApResult.WIFI_CONNECTED_SUCCESS, null));
                    }
                    doConnectServer(params.getPort());
                } else {
                    reconnectWiFi(params);
                }
            }

            @Override
            public void onDisconnect(String SSID) {
                LogUtils.d("onDisconnect + SSID = " + SSID);
            }
        });
    }

    private void reconnectWiFi(WifiApConnectParams params) {
        connectServer(params, mCallback);
    }


    @Override
    public void sendMessage(String msg) {
        if (mTcpClient != null) {
            mTcpClient.sendData(msg);
        }
        if (mTcpServer != null) {
            mTcpServer.sendData(msg);
        }
    }

    @Override
    public void release() {
        mCallback = null;
        mListenr = null;
        if (mApConnector != null) {
            mApConnector.release();
            mApConnector = null;
        }
        if (mWifiConnnector != null) {
            mWifiConnnector.release();
            mWifiConnnector = null;
        }
        if (mTcpServer != null) {
            mTcpServer.closeServer();
            mTcpServer = null;
        }
        if (mTcpClient != null) {
            mTcpClient.closeConnection();
            mTcpClient = null;
        }
    }

    private void doBuildServer(){
        if (mTcpServer != null || mParams == null) {
            return;
        }
        mServerBuildCount++;
        LogUtils.d("mServerBuildCount:"+ mServerBuildCount + " mServerPort:"+mParams.getPort());
        mTcpServer = new TcpServer(mParams.getPort(), new TcpServerObsv() {
            @Override
            public void onStateChange(TcpServer.TcpServerState state) {
                LogUtils.d("TcpServer WifiApConnectManager state:"+state);
                switch (state) {
                    case BUILDED:
                        if (mCallback != null) {
                            mCallback.onResult(new WifiApResult(WifiApResult.SERVER_BUILD_SUCCESS, null));
                        }
                        break;
                    case BUILDED_FAILED:
                        // 3次重新建立
                        if (mServerBuildCount < 3) {
                            // 随机端口号
                            int port = RandomUtils.getRandom(1024, 0xFFFF);
                            mParams.setPort(port);
                            mTcpServer = null;
                            doBuildServer();
                        }
                        else{
                            if (mCallback != null) {
                                mCallback.onResult(new WifiApResult(WifiApResult.SERVER_BUILD_ERROR_OTHERS, null));
                            }
                        }
                        break;
                    case CLOSED:
                        if (mCallback != null) {
                            mCallback.onResult(new WifiApResult(WifiApResult.SERVER_BUILD_ERROR_OTHERS, null));
                        }
                        break;
                }
            }

            @Override
            public void onReceiveData(TcpClient client, String data) {
                LogUtils.d("TcpServer WifiApConnectManager client:"+client+" data:"+data);
                WifiApResult result = new WifiApResult(WifiApResult.DATA_RECEIVED, data);
                if (mCallback != null) {
                    mCallback.onResult(result);
                }
            }
        });
        mTcpServer.setClientCount(1);
        mTcpServer.buildServer();
    }


    private void doConnectServer(int port) {
        if (mTcpClient != null) {
            return;
        }
        port = port <= 0 ? DEFAULT_SOCKET_PORT : port;
        mTcpClient = new TcpClient(DEFAULT_CONNECT_IP, port, new TcpClientObsv() {
            @Override
            public void onStateChange(TcpClient client, TcpClient.TcpClientState state) {
                LogUtils.d("doConnectServer onStateChange state:"+state);
                switch (state) {
                    case CONNECTED:
                        if (mCallback != null) {
                            mCallback.onResult(new WifiApResult(WifiApResult.CLIENT_CONNECTED_SUCCESS, null));
                        }
                        break;
                    case CONNECTED_FAILED:
                    case CLOSED:
                        if (mCallback != null) {
                            mCallback.onResult(new WifiApResult(WifiApResult.CLIENT_CONNECTED_ERROR, null));
                        }
                        break;
                }
            }

            @Override
            public void onReceiveData(TcpClient client, String data) {
                LogUtils.d("doConnectServer onReceiveData data:"+data);
                WifiApResult result = new WifiApResult(WifiApResult.DATA_RECEIVED, data);
                if (mCallback != null) {
                    mCallback.onResult(result);
                }
            }
        });
        mTcpClient.connect();
        LogUtils.d("mTcpClient.connect()");
    }

    /**
     * 获取当前参数
     * @return
     */
    public WifiApConnectParams getParams(){
        return mParams;
    }


    /**
     * 获取默认的AP配合
     * @return
     */
    public static WifiApConnectParams getDefaultParams(){
        WifiApConnectParams params = new WifiApConnectParams();

        try {
            String androidID = DeviceUtils.getAndroidID(ContextFinder.getApplication());
            String ssid = "Auric-" + androidID.substring(androidID.length() - 3, androidID.length());
            params.setSSID(ssid);
            params.setPort(DEFAULT_SOCKET_PORT);
        } catch (Exception e) {
            e.printStackTrace();
            // 报错随机id
            params.setSSID("Auric-" + RandomUtils.getRandom(100, 999));
            params.setPort(DEFAULT_SOCKET_PORT);
        }
        return params;
    }
}
