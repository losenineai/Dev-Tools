package com.auric.intell.commonlib.utils;

import android.os.Environment;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;

/**
 * Created by Administrator on 2016-03-25.
 */
public class TokenUtil {
    public static String getToken(){
        StringBuffer sb = new StringBuffer();
        String path = Environment.getExternalStorageDirectory().getPath()+"/auric";
        File file2 = new File(path);
        if (!file2.exists()){
            file2.mkdirs();
        }
        File file = new File(file2,"token");
        if (!file.exists()){
            return null;
        }else {
            try {
                FileInputStream inputStream = new FileInputStream(file);
                InputStreamReader reader = new InputStreamReader(inputStream);
                int len = 0;
                char[] buf = new char[1024];

                while ((len = reader.read(buf)) != -1)
                {
                    sb.append(new String(buf, 0, len));
                }
                reader.close();
                return sb.toString();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return null;
    }

    public static void setToken(String token){
        String path = Environment.getExternalStorageDirectory().getPath()+"/auric";
        File file2 = new File(path);
        if (!file2.exists()){
            file2.mkdirs();
        }
        File file = new File(file2,"token");
        if (!file.exists()){
            try {
                file.createNewFile();
                FileOutputStream outputStream = new FileOutputStream(file);
                OutputStreamWriter writer = new OutputStreamWriter(outputStream);
                writer.write(token,0,token.length());
                writer.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

    }
}
