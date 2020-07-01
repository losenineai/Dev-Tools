package com.auric.intell.commonlib.tv.connect;

import com.auric.intell.commonlib.connectivity.udp.UdpMode;
import com.auric.intell.commonlib.tv.connect.callback.TVConnectedCallBack;

/**
 * Created by sh on 2018/1/31.
 */

public class TvConnectorBuilder {
    private TvConnector tvConnector;

    public TvConnectorBuilder (){
        tvConnector=new TvConnector();
    }

    public TvConnectorBuilder setTCP_PORT(int tcp_port){
        tvConnector.setTCP_PORT(tcp_port);
        return this;
    }

    public TvConnectorBuilder setUDP_SERVER_PORT(int udp_server_port){
        tvConnector.setUDP_SERVER_PORT(udp_server_port);
        return this;
    }

    public TvConnectorBuilder setUDP_RECEIVER_PORT(int udp_receiver_port){
        tvConnector.setUDP_RECEIVER_PORT(udp_receiver_port);
        return this;
    }

    public TvConnectorBuilder setUdpMode(UdpMode udpMode){
        tvConnector.setUdpMode(udpMode);
        return this;
    }

    public TvConnectorBuilder setUDP_MULTICASTGROUP(String udp_multicastgroup)
    {
        tvConnector.setUDP_MULTICASTGROUP(udp_multicastgroup);
        return this;
    }

    public TvConnectorBuilder setTvConnectedCallBack(TVConnectedCallBack tvConnectedCallBack){
        tvConnector.setTvConnectedCallBack(tvConnectedCallBack);
        return this;
    }

    public TvConnector toBuild(){
        tvConnector.build();
        return tvConnector;
    }
}
