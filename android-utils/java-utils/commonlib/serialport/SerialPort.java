/*
 * Copyright 2009 Cedric Priscal
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.auric.intell.commonlib.serialport;

import android.util.Log;

import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class SerialPort {

	private static final String TAG = "SerialPort";

	/*
	 * Do not remove or rename the field mFd: it is used by native method close();
	 */
	private FileDescriptor mFd;
	private FileInputStream mFileInputStream;
	private FileOutputStream mFileOutputStream;

	public SerialPort(File device, int baudrate) throws SecurityException, IOException {

		Log.i("chw", "SerialPort ---> SerialPort");
		/* Check access permission */
		if (!device.canRead() || !device.canWrite()) {
			/*try {
			//	 Missing read/write permission, trying to chmod the file
				Process su;
				su = Runtime.getRuntime().exec("/system/bin/su");
				//String cmd = "chmod 777 " + device.getAbsolutePath() + "\n"
						//+ "exit\n";
				String cmd = "chmod 777 /dev/s3c_serial0" + "\n"
				+ "exit\n";
				su.getOutputStream().write(cmd.getBytes());
				if ((su.waitFor() != 0) || !device.canRead()
						|| !device.canWrite()) {
					throw new SecurityException();
				}
			} catch (Exception e) {
				e.printStackTrace();
				throw new SecurityException();
			}*/

			try {
				String command = "chmod 777 " + device.getAbsolutePath();
				Log.i("chw", "command = " + command  + "\ndevice.getAbsolutePath = " + device.getAbsolutePath());
				Runtime runtime = Runtime.getRuntime();

				Process proc = runtime.exec(command);
			} catch (IOException e) {
				Log.i("chw","chmod fail!!  !!");
				e.printStackTrace();
				throw new SecurityException();
			}
			Log.i("chw", "Get serial port read/write   permission");
		}

		mFd = open(device.getAbsolutePath(), baudrate);
		if (mFd == null) {
			Log.e(TAG, "native open returns null ");
			throw new IOException();
		}
		mFileInputStream = new FileInputStream(mFd);
		mFileOutputStream = new FileOutputStream(mFd);


	}


	// Getters and setters
	public InputStream getInputStream() {
		Log.i("chw", "SerialPort ---> getInputStream");
		return mFileInputStream;
	}

	public OutputStream getOutputStream() {
		Log.i("chw", "SerialPort ---> getOutputStream");
		return mFileOutputStream;
	}




	// JNI
	private native static FileDescriptor open(String path, int baudrate);
	public native void close();


	static {
		Log.i("chw", "SerialPort ---> loadLibrary");
		System.loadLibrary("mserial_port");
	}
}