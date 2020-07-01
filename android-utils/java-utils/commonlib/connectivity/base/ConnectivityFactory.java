package com.auric.intell.commonlib.connectivity.base;

import android.content.Context;

import com.auric.intell.commonlib.connectivity.ap.WifiApConnectManager;
import com.auric.intell.commonlib.connectivity.ap.WifiApConnectParams;
import com.auric.intell.commonlib.connectivity.ap.WifiApResult;
import com.auric.intell.commonlib.utils.ToastUtil;

/**
 * @author white
 * @description：连接工厂类。用于Android中两端设备连接
 * 包括ap热点，Wifi直连，蓝牙3.0，蓝牙BLE，NFC等等
 * @date 2017/9/1
 */

public class ConnectivityFactory {

    public static final int TAG_WIFI_AP = 1;
    public static final int TAG_WIFI_P2P = 2;
    public static final int TAG_BT_SOCKET = 3;
    public static final int TAG_BT_BLE = 4;

    public static IConnectivityManager buildManager(int tag) {
        IConnectivityManager manager = null;
        switch (tag) {
            case TAG_WIFI_AP:
                manager = new WifiApConnectManager();
                break;
            default:
                break;
        }

        return manager;
    }

    // TODO: 2017/9/26 以下是TAG_WIFI_AP 类型的使用demo
    public static void demoForWifiAp(Context context) {
        /**
         * mob 代码
         */
        final WifiApConnectManager wifiApConnectManager = (WifiApConnectManager) ConnectivityFactory.buildManager(ConnectivityFactory.TAG_WIFI_AP);
        // 初始化
        wifiApConnectManager.init(context);
        // 连接参数
        WifiApConnectParams params = WifiApConnectParams.build().setSSID("123");
        // 开始连接ap和连接tcp
        wifiApConnectManager.connectServer(params, new IConnectivityCallback<WifiApResult>() {
            @Override
            public void onResult(WifiApResult result) {
                switch (result.getCode()) {
                    case WifiApResult.CLIENT_CONNECTED_ERROR:
                        ToastUtil.show("收到服务端msg："+result.getData());
                        // todo 最后发送wifi信息成功之后，调用释放资源接口
                        wifiApConnectManager.release();
                        break;
                    case WifiApResult.CLIENT_CONNECTED_SUCCESS:
                        ToastUtil.show("socket连接成功");

                        // todo 建立连接Update之后，可以发送数据，具体协议后续再定
                        wifiApConnectManager.sendMessage("");
                        break;
                }
            }
        });

       /* wifiApConnectManager.buildServerAndReceive(params, new IConnectivityCallback<WifiApResult>() {
            @Override
            public void onResult(WifiApResult result) {

            }
        });*/



        /**
         * RBT 端代码
         */
        WifiApConnectParams params1 = WifiApConnectParams.build().setSSID("");
        wifiApConnectManager.buildServerAndReceive(params1, new IConnectivityCallback<WifiApResult>() {
            @Override
            public void onResult(WifiApResult result) {
                switch (result.getCode()) {
                    case WifiApResult.DATA_RECEIVED:
                        ToastUtil.show("收到客户端msg："+result.getData());
                        break;
                    case WifiApResult.SERVER_BUILD_SUCCESS:
                        ToastUtil.show("socket建立成功");
                        // 连接成功之后，可以发送数据，具体协议后续再定
                        wifiApConnectManager.sendMessage("");
                        break;
                }
            }
        });
    }

}
