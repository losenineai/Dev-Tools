package com.auric.intell.commonlib.tv.connect;

import com.auric.intell.commonlib.connectivity.ap.TcpClient;
import com.auric.intell.commonlib.connectivity.ap.TcpServer;
import com.auric.intell.commonlib.connectivity.ap.TcpServerObsv;
import com.auric.intell.commonlib.connectivity.udp.UdpMode;
import com.auric.intell.commonlib.connectivity.udp.UdpReceiveCallBack;
import com.auric.intell.commonlib.connectivity.udp.UdpReceiver;
import com.auric.intell.commonlib.connectivity.udp.UdpServer;
import com.auric.intell.commonlib.tv.connect.callback.TVAPPServerCallBack;

import java.util.List;
import java.util.Queue;

/**
 * Created by sh on 2018/1/30.
 */

public class TVAppServer {
    private int TCP_PORT=32191;
    private int UDP_SERVER_PORT =32190;
    private int UDP_RECEIVER_PORT =32190;

    private TcpServer mTcpServer;
    private TVAPPServerCallBack mTvappServerCallBack;
    private UdpReceiver udpReceiver;
    private UdpMode udpMode=UdpMode.BROADCAST;
    private String UDP_MULTICASTGROUP="224.119.2.0";
    private UdpServer udpServer;

    public TVAppServer(){

    }

    public void setTVAPPServerCallBack(TVAPPServerCallBack tvappServerCallBack){
        mTvappServerCallBack=tvappServerCallBack;
    }

    public void setTCP_PORT(int tcp_port){
        TCP_PORT=tcp_port;
    }

    public void setUDP_SERVER_PORT(int udp_server_port){
        UDP_SERVER_PORT=udp_server_port;
    }

    public void setUDP_RECEIVER_PORT(int udp_receiver_port){
        UDP_RECEIVER_PORT=udp_receiver_port;
    }

    public void setUdpMode(UdpMode udpMode){
        this.udpMode=udpMode;
    }

    public void setUDP_MULTICASTGROUP(String ip){
        UDP_MULTICASTGROUP=ip;
    }

    public void build(){
        startUdpReceiver();
        startUdpServer();
        startTcpServer();
    }

    private void startTcpServer(){
        mTcpServer=new TcpServer(TCP_PORT, new TcpServerObsv() {
            @Override
            public void onStateChange(TcpServer.TcpServerState state) {
                switch (state){
                    case IDLE:
                        break;
                    case BUILDING:
                        break;
                    case BUILDED:
                        break;
                    case BUILDED_FAILED:
                        break;
                    case CLIENT_CONNECT:
                        mTvappServerCallBack.onConnect();
                        break;
                    case CLIENT_DISCONNECTED:
                        mTvappServerCallBack.onDisConnect();
                        break;
                    case RECEIVED_CLIENT:
                        break;
                    case CLOSED:
                        break;
                }
            }

            @Override
            public void onReceiveData(TcpClient client, String data) {
                mTvappServerCallBack.onReceivedTcpData(data);
            }
        });
        mTcpServer.buildServer();
    }

    private void startUdpReceiver(){
        udpReceiver=new UdpReceiver(UDP_RECEIVER_PORT, new UdpReceiveCallBack() {
            @Override
            public void onReciverMessage(String serverIp, String message) {
                mTvappServerCallBack.onReceivedUdpData(serverIp,message);
            }
        });
        udpReceiver.setUdpMode(udpMode);
        udpReceiver.setMulticastGroup(UDP_MULTICASTGROUP);
    }

    private void startUdpServer(){
        udpServer=new UdpServer(UDP_SERVER_PORT);
        udpServer.setUdpMode(udpMode);
        udpServer.setMulticastGroup(UDP_MULTICASTGROUP);
    }

    public void stop(){
        udpReceiver.stop();
        mTcpServer.closeServer();
    }

    public void stopUdpReceiver(){
        udpReceiver.stop();
    }

    //向指定ip单播数据
    public void sendUdpData(String ip,String data){
        udpServer.sendMessage(ip,data);
    }

    //向ip所在子网段广播数据
    public void broadcastMessage(String ip,String data){
        udpServer.broadcastMessage(ip,data);
    }

    //向指定IP客户端发送TCP数据
    public void sendTcpData(String ip,String data){
        mTcpServer.sendData(ip,data);
    }

    //向所有客户端发送TCP数据
    public void sendTcpData(String data){
        mTcpServer.sendData(data);
    }
}
