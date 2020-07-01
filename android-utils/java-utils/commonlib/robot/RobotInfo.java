package com.auric.intell.commonlib.robot;

import com.auric.intell.commonlib.manager.retrofit.TResponse;

/**
 * Created by zhangxiliang on 2017/4/26.
 */

public class RobotInfo extends TResponse{
    private String serial_no;
    private int id;
    /**
     * model : ZN22101
     * os_version : 1.0.0
     * total_flash_space : 300000
     * free_flash_space : 290000
     * android_version : 4.2
     * nim : {"token":"IxnTjgypVe2WUAQKl1Ehp0prkyPPEj8M","accid":"161500006"}
     */

    private String wifiMac;
    private String blueMac;

    private String model;
    private String os_version="1.0.0";
    private String total_flash_space;
    private String free_flash_space;
    private String android_version;
    private NimBean nim;

    public String getModel() {
        return model;
    }

    public void setModel(String model) {
        this.model = model;
    }

    public String getOs_version() {
        return os_version;
    }

    public void setOs_version(String os_version) {
        this.os_version = os_version;
    }

    public String getTotal_flash_space() {
        return total_flash_space;
    }

    public void setTotal_flash_space(String total_flash_space) {
        this.total_flash_space = total_flash_space;
    }

    public String getFree_flash_space() {
        return free_flash_space;
    }

    public void setFree_flash_space(String free_flash_space) {
        this.free_flash_space = free_flash_space;
    }

    public String getAndroid_version() {
        return android_version;
    }

    public void setAndroid_version(String android_version) {
        this.android_version = android_version;
    }

    public String getSerial_no() {
        // TODO: 2017/8/21 手机debug
//        return "322311000000003";
        return serial_no;
    }

    public void setSerial_no(String serial_no) {
        this.serial_no = serial_no;
    }

    public String getWifiMac() {
        return wifiMac;
    }

    public void setWifiMac(String wifiMac) {
        this.wifiMac = wifiMac;
    }

    public String getBlueMac() {
        return blueMac;
    }

    public void setBlueMac(String blueMac) {
        this.blueMac = blueMac;
    }

    public NimBean getNim() {
        return nim;
    }

    public void setNim(NimBean nim) {
        this.nim = nim;
    }

    public static class NimBean {
        /**
         * token : IxnTjgypVe2WUAQKl1Ehp0prkyPPEj8M
         * accid : 161500006
         */

        private String token;
        private String accid;

        public String getToken() {
            return token;
        }

        public void setToken(String token) {
            this.token = token;
        }

        public String getAccid() {
            return accid;
        }

        public void setAccid(String accid) {
            this.accid = accid;
        }
    }

    @Override
    public String toString() {
        return "RobotInfo{" +
                "serial_no='" + serial_no + '\'' +
                ", id=" + id +
                ", wifiMac='" + wifiMac + '\'' +
                ", blueMac='" + blueMac + '\'' +
                ", model='" + model + '\'' +
                ", os_version='" + os_version + '\'' +
                ", total_flash_space='" + total_flash_space + '\'' +
                ", free_flash_space='" + free_flash_space + '\'' +
                ", android_version='" + android_version + '\'' +
                ", nim=" + nim +
                '}';
    }
}
