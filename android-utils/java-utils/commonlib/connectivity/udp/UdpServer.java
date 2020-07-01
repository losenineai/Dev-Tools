package com.auric.intell.commonlib.connectivity.udp;

import android.content.BroadcastReceiver;
import android.util.Log;

import com.auric.intell.commonlib.utils.LogUtils;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.concurrent.Executor;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created by sh on 2017/11/13.
 */

public class UdpServer {
    private UdpMode mUdpMode=UdpMode.BROADCAST;
    private String multicastGroup="230.0.0.1";


    private   int myPort;

    public void setUdpMode(UdpMode udpMode){
        mUdpMode=udpMode;
    }

    public UdpMode getUdpMode(){
        return mUdpMode;
    }

    public void setMulticastGroup(String ip){
        multicastGroup=ip;
    }
    public String getMulticastGroup(){
        return multicastGroup;
    }

    private static DatagramSocket socket;
    static {
        try {
            socket = new DatagramSocket();
        } catch (SocketException e) {
            e.printStackTrace();
        }
    }

    public UdpServer(int port){
        myPort=port;
    }
    public void sendMessage(final String ip,final String message) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {

                    byte data[] = message.getBytes();
                    InetAddress serverAddress;
                    serverAddress = InetAddress.getByName(ip);
                    DatagramPacket packet = new DatagramPacket(data, data.length,
                            serverAddress, myPort);
                    socket.send(packet);
                } catch (UnknownHostException e1) {
                    // TODO Auto-generated catch block
                    e1.printStackTrace();
                } catch (SocketException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                } catch (IOException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
        }).start();
    }

    public void multicastMessage(final String message) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                MulticastSocket mServer = null;
                try {
                    byte data[] = message.getBytes();
                    mServer = new MulticastSocket(myPort);
                    InetAddress bcAddr = InetAddress.getByName(multicastGroup);
                    mServer.joinGroup(bcAddr);
                    mServer.setLoopbackMode(false);
                    mServer.setReceiveBufferSize(4096);
                    DatagramPacket packet = new DatagramPacket(data, data.length,
                            bcAddr, myPort);
                    mServer.send(packet);
                } catch (IOException e) {
                    e.printStackTrace();
                } finally {
                    if (mServer != null)
                        mServer.close();
                }
            }
        }).start();
    }


    public void broadcastMessage(final String ip, String message){

//        for(int i=0;i<255;i++){
//            String broadcastIp=ip.substring(0,ip.lastIndexOf("."))+"."+i;
//            UdpServer.this.sendMessage(broadcastIp,message);
//        }
        LogUtils.d("broadcastMessage:"+message);
        switch (mUdpMode){
            case MULTICAST:
                multicastMessage(message);
                break;
            case SINGLECAST:
            case BROADCAST:
            default:
                String broadcastIp=ip.substring(0,ip.lastIndexOf("."))+"."+255;//"255.255.255.255";//
                UdpServer.this.sendMessage(broadcastIp,message);
                break;
        }



    }

}
