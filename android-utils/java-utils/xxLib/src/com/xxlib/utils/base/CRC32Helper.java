package com.xxlib.utils.base;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.util.zip.CRC32;

/**
 * crc32转换器，目前提供两种转换格式：字节数组 字符串
 * 
 * @author linbin 2013
 */
public class CRC32Helper {

	public static long translate(byte[] data) {
		CRC32 crc = new CRC32();
		crc.update(data);
		return crc.getValue();
	}

	public static long translate(String data) throws IOException {
		CRC32 crc = new CRC32();
		ObjectOutputStream outputStream = null;
		byte[] bytes = null;
		try {

			ByteArrayOutputStream byteArray = new ByteArrayOutputStream();

			outputStream = new ObjectOutputStream(byteArray);

			outputStream.writeObject(data);

			outputStream.flush();

			bytes = byteArray.toByteArray();

		}

		finally {
			try {
				if (outputStream != null)
					outputStream.close();
			} catch (Exception e) {
				e.printStackTrace();
			}

		}

		crc.update(bytes);
		return crc.getValue();
	}
}
