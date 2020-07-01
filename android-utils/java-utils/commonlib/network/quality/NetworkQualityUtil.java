package com.auric.intell.commonlib.network.quality;

import android.os.SystemClock;

import com.auric.intell.commonlib.utils.LogTool;
import com.auric.intell.commonlib.utils.net.HttpGetUtil;


/**
 * @author white
 * @description：网络质量管理器
 * @date 2017/7/6
 */

public class NetworkQualityUtil {

    private static final String TAG = "NetworkQualityUtil";

    /**
     * 开始监听网络质量
     * @param listener
     */
    public static void startNetworkQualityListen(final NetworkQualityListener listener) {
        if (listener == null) {
            return;
        }
        NetworkQualityManager.getInstance().register(new NetworkQualityManager.ConnectionClassStateChangeListener() {
            @Override
            public void onBandwidthStateChange(NetworkQuality networkQuality) {
                LogTool.d(TAG, "onBandwidthStateChange "+networkQuality);
                listener.onQualityChange(networkQuality);
            }
        });
        DeviceBandwidthSampler.getInstance().startSampling();
        new Thread(new Runnable() {
            @Override
            public void run() {
                while (true) {
                    LogTool.d(TAG, "start download");
                    boolean result = HttpGetUtil.downloadFile("https://github.com/facebook/network-connection-class/archive/master.zip", "/sdcard/baidu.web");
                    LogTool.d(TAG, "end download result:"+ result);
//                    SystemClock.sleep(10000);
                }
            }
        }).start();
        new Thread(new Runnable() {
            @Override
            public void run() {
                while (true) {
                    LogTool.d(TAG, "cur: " + NetworkQualityManager.getInstance().getCurrentBandwidthQuality());
                    SystemClock.sleep(1000);
                }
            }
        }).start();


    }

    /**
     * 停止监听
     */
    public static void stopNetworkQualityListen() {
        DeviceBandwidthSampler.getInstance().stopSampling();
    }

    public interface NetworkQualityListener{
        void onQualityChange(NetworkQuality quality);
    }

}
