package com.auric.intell.commonlib.tv.connect;

import android.content.Context;
import android.util.Log;

import com.auric.intell.commonlib.connectivity.ap.TcpClient;
import com.auric.intell.commonlib.connectivity.ap.TcpClientObsv;
import com.auric.intell.commonlib.connectivity.udp.UdpMode;
import com.auric.intell.commonlib.connectivity.udp.UdpReceiveCallBack;
import com.auric.intell.commonlib.connectivity.udp.UdpReceiver;
import com.auric.intell.commonlib.connectivity.udp.UdpServer;
import com.auric.intell.commonlib.tv.connect.callback.TVConnectedCallBack;
import com.auric.intell.commonlib.utils.ContextFinder;
import com.auric.intell.commonlib.utils.LogUtils;
import com.auric.intell.commonlib.utils.ProcessUtil;
import com.auric.intell.commonlib.utils.net.NetWorkUtil;

import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;


/**
 * Created by sh on 2018/1/10.
 */

public class TvConnector{
    private static List<TcpClient> clientList=new ArrayList<TcpClient>();
    private int TCP_PORT=9688;
    private int UDP_SERVER_PORT =9687;
    private int UDP_RECEIVER_PORT=9687;
    private String UDP_MULTICASTGROUP="224.119.2.0";
    private UdpMode udpMode=UdpMode.BROADCAST;

    public boolean tvConnected=false;

    private TcpClient mTcpClient;
    private UdpServer udpServer;
    private UdpReceiver udpReceiver;

    private TVConnectedCallBack mTvConnectedCallBack;

    public TvConnector(){
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

    public void setUDP_MULTICASTGROUP(String udp_multicastgroup){
        UDP_MULTICASTGROUP=udp_multicastgroup;
    }

    public void setTvConnectedCallBack(TVConnectedCallBack tvConnectedCallBack){
        mTvConnectedCallBack=tvConnectedCallBack;
        LogUtils.d("TCPSTATE:setTvConnectedCallBack"+(mTvConnectedCallBack==null));
    }

    private void startUdpServer(){
        udpServer=new UdpServer(UDP_SERVER_PORT);
        udpServer.setUdpMode(udpMode);
        udpServer.setMulticastGroup(UDP_MULTICASTGROUP);
    }

    public void startUdpReceiver(){
        udpReceiver=new UdpReceiver(UDP_RECEIVER_PORT, new UdpReceiveCallBack() {
            @Override
            public void onReciverMessage(String serverIp, String message) {
                LogUtils.d("TvConnector:onReciverMessage:"+udpReceiver.getPort());
                if(TvConnector.this.mTvConnectedCallBack!=null){
                    TvConnector.this.mTvConnectedCallBack.onReceiveUdpData(serverIp,message);
                }
            }
        });
        udpReceiver.setUdpMode(udpMode);
        udpReceiver.setMulticastGroup(UDP_MULTICASTGROUP);
    }


    public void build(){
        startUdpServer();
    }

    public void stop(){
        udpReceiver.stop();
        if(mTcpClient!=null) {
            mTcpClient.closeConnection();
        }
    }

    public void stopUdpReceiver(){
        udpReceiver.stop();
    }

    /*
    * 连接电视
    * Tcp
    * */
    public void tcpConnect(String tv_Ip) {
        int clientNum=clientList.size();
        for(int i=0;i<clientNum;i++ ){
            TcpClient client=clientList.get(i);
            if(client.getPort()==TCP_PORT){
                client.closeConnection();
                clientList.remove(client);
                clientNum--;
            }
        }
        TcpClientObsv tcpClientObsv=new TcpClientObsv() {
            @Override
            public void onStateChange(TcpClient client, TcpClient.TcpClientState state) {
                LogUtils.d("TCPSTATE:"+state);
                switch (state){
                    case CLOSED:
                        tvConnected=false;
                        LogUtils.d("TCPSTATE1:"+tvConnected);
                        if(TvConnector.this.mTvConnectedCallBack!=null){
                            LogUtils.d("TCPSTATE2:mTvConnectedCallBack");
                            mTcpClient.closeConnection();
                            TvConnector.this.mTvConnectedCallBack.onClosed();
                        }
                        break;
                    case CONNECTING:
                        break;
                    case CONNECTED:
                        tvConnected=true;

                        if(TvConnector.this.mTvConnectedCallBack!=null){
                            TvConnector.this.mTvConnectedCallBack.onSuccess();
                        }
                        break;
                    case CONNECTED_FAILED:
                        tvConnected=false;
                        if(TvConnector.this.mTvConnectedCallBack!=null){
                            TvConnector.this.mTvConnectedCallBack.onFail();
                        }
                        break;
                }
            }

            @Override
            public void onReceiveData(TcpClient client, String data) {
                LogUtils.d("onReceiveData"+data);
                if(TvConnector.this.mTvConnectedCallBack!=null){
                    TvConnector.this.mTvConnectedCallBack.onReceiveTcpData(data);
                }
            }
        };

        mTcpClient=new TcpClient(tv_Ip, TCP_PORT, tcpClientObsv);
        mTcpClient.setTcpDataReceiveMode(TcpClient.TcpDataReceiveMode.WITHOUTNEWLINE);
        mTcpClient.connect();
        LogUtils.d("TCPSTATE:"+ProcessUtil.getCurProcessName(ContextFinder.getApplication()));
        LogUtils.d("command1"+(this.mTcpClient==null));
        clientList.add(mTcpClient);
    }

    public void removeTvConnectedCallBack(){
        TvConnector.this.mTvConnectedCallBack=null;
        LogUtils.d("TCPSTATE:removeTvConnectedCallBack"+(mTvConnectedCallBack==null));

    }
    public void sendTcpCommand(String command){
        LogUtils.d(command);
        LogUtils.d("command"+(this.mTcpClient==null));
        LogUtils.d("TCPSTATE:"+ProcessUtil.getCurProcessName(ContextFinder.getApplication()));
        if(this.mTcpClient!=null) {
            this.mTcpClient.sendData(command);
        }
    }

    public void sendUdpCommand(String ip,String command){
        udpServer.sendMessage(ip,command);
    }

    public void broadcastUdpCommand(String ip,String command){
        udpServer.broadcastMessage(ip,command);
    }

}
