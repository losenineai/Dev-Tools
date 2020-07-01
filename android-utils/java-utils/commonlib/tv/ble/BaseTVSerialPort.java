package com.auric.intell.commonlib.tv.ble;

import com.auric.intell.commonlib.serialport.SerialPort;
import com.auric.intell.commonlib.tv.ble.callback.ReadDataCallBack;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created by sh on 2018/1/29.
 */

public class BaseTVSerialPort {
    private static final String TAG="BaseTVSerialPort";
    private String  PORTNAME= "/dev/ttyMT1";
    private int BAUDRATE = 115200;
    private int feedbackDataLength=10;
    private ReadThread readThread;

    private SerialPort serialPort;
    private FileOutputStream writeInstructionStream;
    private FileInputStream readStatusSteam;

    private byte[] readDatas;
    private ReadDataCallBack readDataCallBack;

    public void setPORTNAME(String portname){
        PORTNAME=portname;
    }
    public String getPORTNAME(){
        return PORTNAME;
    }

    public void setBAUDRATE(int baudrate){
        BAUDRATE=baudrate;
    }

    public int getBAUDRATE(){
        return BAUDRATE;
    }

    public void setFeedbackDataLength(int feedbackDataLength){
        this.feedbackDataLength=feedbackDataLength;
    }

    public void setReadDataCallBack(ReadDataCallBack readDataCallBack){
        this.readDataCallBack=readDataCallBack;
    }

    public void init(){
        try {
            serialPort = new SerialPort(new File(PORTNAME), BAUDRATE);
            writeInstructionStream = (FileOutputStream) serialPort.getOutputStream();
            readStatusSteam = (FileInputStream) serialPort.getInputStream();
            readDatas=new byte[feedbackDataLength];
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void writeInstruction(byte[] instruction){
        try {
            if(writeInstructionStream!=null){
                writeInstructionStream.write(instruction);

            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void readData(){
        if(readThread!=null){
            readThread.stopRead();
        }
        readThread=new ReadThread();
        readThread.start();
    }

    public void stopReadData(){
        if(readThread!=null){
            readThread.stopRead();
        }
    }

    class ReadThread extends Thread{
        private boolean stop=false;

        public void stopRead(){
            stop=true;
        }
        @Override
        public void run() {
            super.run();
                try {
                    while (!stop) {
                        if(readStatusSteam!=null){
                            int size = readStatusSteam.read(readDatas, 0, feedbackDataLength);
                            if(size!=0){
                                if(readDataCallBack!=null){
                                    readDataCallBack.onReadData(readDatas);
                                }
                            }
                        }

                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
        }
    }

}
