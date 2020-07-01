package com.auric.intell.commonlib.utils;

import android.bluetooth.BluetoothAdapter;

import net.vidageek.mirror.dsl.Mirror;

/**
 * Created by sh on 2018/2/5.
 */

public class BluetoothUtil {

    //android6.0以上获取蓝牙地址的方式，需要添加对包net.vidageek:mirror的引用
    public static String getBtMac_6p(){
        BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        Object bluetoothManagerService = new Mirror().on(bluetoothAdapter).get().field("mService");
        if (bluetoothManagerService == null) {
            LogUtils.d("couldn't find bluetoothManagerService");
            return null;
        }
        Object address = new Mirror().on(bluetoothManagerService).invoke().method("getAddress").withoutArgs();
        if (address != null && address instanceof String) {
            LogUtils.d("using reflection to get the BT MAC address: " + address);
            return (String) address;
        } else {
            return null;
        }
    }
}
