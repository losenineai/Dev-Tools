package com.xxlib.utils;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;

public class StringtoBytes {

	public static byte[] getBytes(String data) throws IOException
	{
		ObjectOutputStream outputStream = null;
		
        try {

            ByteArrayOutputStream byteArray = new ByteArrayOutputStream();

            outputStream = new ObjectOutputStream(byteArray);

            outputStream.writeObject(data);

            outputStream.flush();

            return byteArray.toByteArray();

        }

        finally {

            try {
				outputStream.close();
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

        }


	}
}
