package com.xxAssistant.DanMuKu.plugin.apk;

import android.net.LocalSocket;
import android.net.LocalSocketAddress;

import com.xxlib.utils.StringUtils;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;

/**
 * xx ipc socket连接
 */
public class XXIpcClient {
	private static final String TAG = "XXIpcClient";
	
	private String socketName = "";
	private static final int MaxTryCount = 3;
	private static final int TryInternelInMS = 500;
	
	public enum MiniSocketClientState {
		/**正在连接中*/
		Connecting,
		/**连接成功*/
		Connected,
		/**连接失败*/
		ConnectFail,
		/**连接断开。从连接成功的状态，然后过了一段时间，断开*/
		Closed,
	}
	
	private MiniSocketClientState state = MiniSocketClientState.Closed;
	private Boolean run;
	private Thread connectThread;
	
	private LocalSocket socket;
	private InputStream socketIn;
	private OutputStream socketOut;
	private PrintWriter socketLineWriter;
	
	private XXIpcController mController;

	
	/**
	 * @param controller socket连接状态、接受内容，通知controller
	 */
	public XXIpcClient(XXIpcController controller, String socketName) {
		state = MiniSocketClientState.Closed;
		this.mController = controller;
		this.socketName = socketName;
	}
	
	
	public void beginConnect() {
        beginConnect(null);
	}

    /**
     * 开始连接
     * @param socket null表示需要连接server，非null表示已经从server获取socket，直接work
     */
    public void beginConnect(LocalSocket socket){
        if (state == MiniSocketClientState.Closed || state == MiniSocketClientState.ConnectFail) {
            XXPLog.i(TAG, "beginConnect socket");
            state = MiniSocketClientState.Connecting;
            run = true;

            if(socket != null){
                // begin to work
                connectThread = new Thread(threadMainRunnable);
                connectThread.start();
            }
            else{
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        // try connect
                        connectToSocketServer(MaxTryCount);
                        // begin to work
                        connectThread = new Thread(threadMainRunnable);
                        connectThread.start();
                    }
                }).start();
            }

        } else {
            notifyControllerSocketState(state);
        }
    }

	
	/**
	 * 断开socket连接
	 */
	public void closeConnect() {
		XXPLog.i(TAG, "closeConnect");
		run = false;
		releaseSocket();
	}
	
	
	private void notifyControllerSocketState(MiniSocketClientState socketState) {
		XXPLog.i(TAG, "socketState " + socketState);
		if (null == mController) { 
			return;
		}
		switch (socketState) {
		case Closed :
			mController.onClosed();
			break;
		case Connected :
			mController.onConnected();
			break;
		case Connecting :
			mController.onConnecting();
			break;
		case ConnectFail:
			mController.onConnectFail();
			break;
		default:
			break;
		}
	}
	
	
	private boolean connectToSocketServer(int retryTimes) {
		for (int i=0; i<retryTimes; ++i) {
			try {
				LocalSocketAddress.Namespace namespace = LocalSocketAddress.Namespace.ABSTRACT;
				LocalSocketAddress address = new LocalSocketAddress(socketName, namespace);
				XXPLog.i(TAG, "Connecting to address name : " + address.getName());
				socket = new LocalSocket();
				socket.connect(address);
				return true;
			} catch (Exception e) {
				XXPLog.e(TAG, "connectToSocketServer " + e);
				socket = null;
				XXPLog.i(TAG, StringUtils.format("Connection failed. Will retry after %dms... (%d)", TryInternelInMS, retryTimes));
				try {
					Thread.sleep(TryInternelInMS);
				} catch (InterruptedException e1) {
					e1.printStackTrace();
				}
			}
		}
		return false;
	}
	
	
	private void releaseSocket() {
		try {
			if (null != socketLineWriter) {
				socketLineWriter.close();
			}
		} catch (Exception e) {
			XXPLog.i(TAG, e.toString());
		}
		try {
			if (null != socket) {
				socket.shutdownInput();
				socket.shutdownOutput();
				socket.close();
			}
		} catch (Exception e) {
			XXPLog.i(TAG, e.toString());
		}
	}
	
	
	// thread main
	Runnable threadMainRunnable = new Runnable() {
		@Override
		public void run() {
			
			// failed
			if (socket == null) {
				XXPLog.i(TAG, "failed, because socket == null");
				state = MiniSocketClientState.ConnectFail;
				notifyControllerSocketState(state);
				return;
			}
			
			// 获取输入流，输出流
			try {
				socketIn = socket.getInputStream();
				socketOut = socket.getOutputStream();
			} catch (Exception e) {
				releaseSocket();
				state = MiniSocketClientState.ConnectFail;
				notifyControllerSocketState(state);
				return;
			}
			
			// 连接成功
			BufferedReader lineReader = new BufferedReader(new InputStreamReader(socketIn));
			socketLineWriter = new PrintWriter(socketOut, true);
			state = MiniSocketClientState.Connected;
			notifyControllerSocketState(state);
			
			// 进入循环监听socket接受数据
			while (run) {
				String newLine = null;
				try {
					XXPLog.i(TAG, "begin lineReader.readLine");
					newLine = lineReader.readLine();
					XXPLog.i(TAG, "end lineReader.readLine " + newLine);
				} catch (Exception e) {
					XXPLog.e(TAG, "error occurs: " + e.getMessage());
					e.printStackTrace();
				}
				
				if (newLine != null && mController != null) {
					mController.onReceive(newLine);
				} else if (null == newLine) {
					XXPLog.i(TAG, "failed, because newLine == null || mController == null");
					state = MiniSocketClientState.Closed;
					run = false;
					break;
				}
			}
			
			// 断开连接从while循环出来
			XXPLog.i(TAG, "start releaseSocket");
			state = MiniSocketClientState.Closed;
			notifyControllerSocketState(state);
			releaseSocket();	// 要释放资源
		}
	};
	
	
	// send string to remote endpoint
	public void sendString(String str) {
		if (state == MiniSocketClientState.Connected) {
			XXPLog.i(TAG, "line send: " + str);
			socketLineWriter.print(str);
			socketLineWriter.flush();
		}
	}
}
