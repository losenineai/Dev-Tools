package com.auric.intell.commonlib.connectivity.udp;

import android.util.Log;

import com.auric.intell.commonlib.utils.LogUtils;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.MulticastSocket;
import java.net.SocketException;

/**
 * Created by sh on 2017/11/13.
 */

public class UdpReceiver {
    private UdpMode mUdpMode = UdpMode.BROADCAST;
    private String multicastGroup = "230.0.0.1";

    private int myPort;
    UdpReceiveCallBack mUdpRecieveCallBack;
    private boolean monitorUDP = true;
    MulticastSocket mServer = null;
    DatagramSocket dServer = null;

    public UdpReceiver(int port, UdpReceiveCallBack udpReceiveCallBack) {
        myPort = port;
        mUdpRecieveCallBack = udpReceiveCallBack;
        new ReceiverThread().start();
    }
    public int getPort(){
        return myPort;
    }
    public void setUdpMode(UdpMode udpMode) {
        mUdpMode = udpMode;
    }

    public UdpMode getUdpMode() {
        return mUdpMode;
    }

    public void setMulticastGroup(String ip) {
        multicastGroup = ip;
    }

    public String getMulticastGroup() {
        return multicastGroup;
    }

    public void stop() {
        monitorUDP = false;
        if(mServer!=null){
            mServer.close();
        }
        if(dServer!=null){
            dServer.close();
        }
    }

    class ReceiverThread extends Thread {
        @Override
        public void run() {
            byte[] buffer = new byte[1024];
        /*在这里同样使用约定好的端口*/
            switch (mUdpMode) {
                case MULTICAST:

                    try {
                        mServer = new MulticastSocket(null);
                        mServer.setReuseAddress(true);
                        mServer.bind(new InetSocketAddress(myPort));
                        InetAddress bcAddr = InetAddress.getByName(multicastGroup);
                        mServer.joinGroup(bcAddr);
                        mServer.setReceiveBufferSize(4096);
                        DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                        while (monitorUDP) {
                                mServer.receive(packet);
                                String message = new String(packet.getData(), 0, packet.getLength(), "UTF-8");
                                Log.d("UdpReceived:", message);
                                String serverIp = packet.getAddress().getHostAddress();

                                mUdpRecieveCallBack.onReciverMessage(serverIp, message);
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    finally {
                        if (mServer != null)
                            mServer.close();
                    }


                    break;
                case SINGLECAST:
                case BROADCAST:
                default:

                    try {
                        LogUtils.d("");
                        dServer =  new DatagramSocket(null);
                        dServer.setReuseAddress(true);
                        dServer.bind(new InetSocketAddress(myPort));
                        dServer.setReceiveBufferSize(4096);
                        DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                        while (monitorUDP) {
                                dServer.receive(packet);
                                String message = new String(packet.getData(), 0, packet.getLength(), "UTF-8");
                                Log.d("UdpReceived:", message);
                                String serverIp = packet.getAddress().getHostAddress();
                                mUdpRecieveCallBack.onReciverMessage(serverIp, message);
                        }

                    } catch (SocketException e) {
                        e.printStackTrace();

                    } catch (IOException e) {
                        e.printStackTrace();
                    }finally {
                        if (dServer != null)
                            dServer.close();
                    }
                    break;

            }


            super.run();
        }
    }

}
