package com.auric.intell.commonlib.tv.connect;

import com.auric.intell.commonlib.connectivity.udp.UdpMode;
import com.auric.intell.commonlib.tv.connect.callback.TVAPPServerCallBack;

/**
 * Created by sh on 2018/1/31.
 * setTCP_PORT、setUDP_SERVER_PORT、setUDP_RECEIVER_PORT为必须
 * setUdpMode、setUDP_MULTICASTGROUP非必须，默认为广播模式
 * TVAPPServerCallBack回调设备端连接与断开，收到TCP数据、收到UDP数据
 */

public class TVAppServerBuilder {
    private TVAppServer tvAppServer;

    public TVAppServerBuilder(){
        tvAppServer=new TVAppServer();
    }

    public TVAppServerBuilder setTCP_PORT(int tcp_port){
        tvAppServer.setTCP_PORT(tcp_port);
        return this;
    }

    public TVAppServerBuilder setUDP_SERVER_PORT(int udp_server_port){
        tvAppServer.setUDP_SERVER_PORT(udp_server_port);
        return this;
    }

    public TVAppServerBuilder setUDP_RECEIVER_PORT(int udp_receiver_port){
        tvAppServer.setUDP_RECEIVER_PORT(udp_receiver_port);
        return this;
    }

    public TVAppServerBuilder setUdpMode(UdpMode udpMode){
        tvAppServer.setUdpMode(udpMode);
        return this;
    }

    public TVAppServerBuilder setUDP_MULTICASTGROUP(String udp_multicastgroup){
        tvAppServer.setUDP_MULTICASTGROUP(udp_multicastgroup);
        return this;
    }

    public TVAppServerBuilder setTVAPPServerCallBack(TVAPPServerCallBack tvappServerCallBack) {
        tvAppServer.setTVAPPServerCallBack(tvappServerCallBack);
        return this;
    }

    public TVAppServer toBuild(){
        tvAppServer.build();
        return tvAppServer;
    }
}
