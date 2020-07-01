package com.xxlib.utils;

import android.content.Context;
import android.content.SharedPreferences;
import android.text.TextUtils;
import android.util.Base64;
import android.util.Log;

import com.xxlib.utils.SPCenter.XXLibSpkey;
import com.xxlib.utils.base.LogTool;
import com.xxlib.utils.base.XXTea;

import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;
import java.io.RandomAccessFile;


/**
 * Created by jammy on 16/1/21.
 */
public class ApkCommentChannelIdUtils {

    private static String TAG = "ApkCommentChannelIdUtils";

    private static final String CHANNEL_JSON_KEY = "channel";
    // 加解密密匙
    public static final String KEY = "#%$*)&*M<><vance";

    private static final int BUFFER_SMALL = 1024;
    private static final int BUFFER_LARGE = 8096;


    private static String readChannelByApkComment(Context context) {
        String apkPath = context.getApplicationInfo().sourceDir;
        if (TextUtils.isEmpty(apkPath)) {
            return null;
        }
        LogTool.i(TAG, "apk path: " + apkPath);

        String messageString = extractZipComment(apkPath);
        LogTool.i(TAG, "读取apk的结果: " + messageString);
        if (TextUtils.isEmpty(messageString)) {
            return null;
        }

        try {
            // 解密
            byte[] b64Result = Base64.decode(messageString, Base64.DEFAULT);

            // 返回的数据进行XXTea解密
            byte[] xxResult = XXTea.XXTeaDecrypt(b64Result, b64Result.length, KEY.getBytes());

            // 获取到channel渠道值
            String jsonStr = new String(xxResult);
            JSONObject jsonObj = new JSONObject(jsonStr);
            if (jsonObj.has(CHANNEL_JSON_KEY)) {
                return jsonObj.getString(CHANNEL_JSON_KEY);
            }
        } catch (Exception e) {
        }
        return null;
    }

    /**
     * 获取接入sdk的channel
     *
     * 优先读取 SharedPreferences
     * 再读取 apk的后缀
     * 最后 读取文件
     */
    public static String getChannelId(Context context, String defaultChannel) {
        String id = "";
        SharedPreferences sPreferences = context.getSharedPreferences(XXLibSpkey.XX_LIB_SP_NAME, Context.MODE_PRIVATE);
        String spId = sPreferences.getString(XXLibSpkey.XXLIB_SP_CHANNEL_ID, "");

        if (TextUtils.isEmpty(spId)) {
            id = readChannelByApkComment(context);
            if (TextUtils.isEmpty(id)) {
                id = defaultChannel;
            }
            // 保存到sp
            LogTool.i(TAG, "取loacal Id：" + id);
            SharedPreferences.Editor editor = sPreferences.edit();
            editor.putString(XXLibSpkey.XXLIB_SP_CHANNEL_ID, id);
            editor.apply();
        } else {
            LogTool.i(TAG, "取SharePreference Id：" + spId);
            id = spId;
        }

        Log.i(TAG, "获取到的channelId：" + id + ", pkg="+context.getPackageName());
        return id;
    }

    private static String extractZipComment(String filename) {
        String retStr = null;
        try {
            File file = new File(filename);
            int fileLen = (int) file.length();

            FileInputStream in = new FileInputStream(file);

			/*
			 * The whole ZIP comment (including the magic byte sequence) MUST
			 * fit in the buffer otherwise, the comment will not be recognized
			 * correctly You can safely increase the buffer size if you like
			 */
            byte[] buffer = new byte[Math.min(fileLen, BUFFER_LARGE)];
            int len;

            in.skip(fileLen - buffer.length);

            if ((len = in.read(buffer)) > 0) {
                retStr = getZipCommentFromBuffer(buffer, len);
            }

            in.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return retStr;
    }

    private static String getZipCommentFromBuffer(byte[] buffer, int len) {
        byte[] magicDirEnd = { 0x50, 0x4b, 0x05, 0x06 };
        int buffLen = Math.min(buffer.length, len);
        // Check the buffer from the end
        for (int i = buffLen - magicDirEnd.length - 22; i >= 0; i--) {
            boolean isMagicStart = true;
            for (int k = 0; k < magicDirEnd.length; k++) {
                if (buffer[i + k] != magicDirEnd[k]) {
                    isMagicStart = false;
                    break;
                }
            }
            if (isMagicStart) {
                // Magic Start found!
                int commentLen = buffer[i + 20] + buffer[i + 21] * 256;
                int realLen = buffLen - i - 22;
                LogTool.i(TAG, "ZIP comment found at buffer position "
                        + (i + 22) + " with len=" + commentLen + ", good!");
                if (commentLen != realLen) {
                    LogTool.i(TAG, "WARNING! ZIP comment size mismatch: directory says len is "
                            + commentLen
                            + ", but file ends after "
                            + realLen + " bytes!");
                }
                String comment = new String(buffer, i + 22, Math.min(
                        commentLen, realLen));
                return comment;
            }
        }
        LogTool.i(TAG, "ERROR! ZIP comment NOT found!");
        return null;
    }


    public static void addChannelInfo(String path, int channel) {
        try {
            String channelStr = getChannelJson(channel);
            LogTool.w(TAG, "channelStr " + channelStr);
            if (TextUtils.isEmpty(channelStr)) {
                return;
            }
            byte[] channelEncode = encodeChannelString(channelStr);
            if (null == channelEncode || channelEncode.length == 0) {
                LogTool.w(TAG, "encode fail, null == channelEncode || channelEncode.length == 0 ");
                return;
            }
            int commentLengthPos = findCommentLengthStartPosition(new File(path));
            LogTool.i(TAG, "commentLengthPos " + commentLengthPos);
            if (commentLengthPos < 0) {
                return;
            }
            RandomAccessFile mOut = new RandomAccessFile(path, "rwd");
            int commentLength = channelEncode.length;
            // 注释的长度在apk的最后两个字节，新长度要加上“渠道注释长度”
            mOut.setLength(commentLengthPos + 2 + commentLength);
            mOut.seek(commentLengthPos);
            LogTool.i(TAG, "commentLength " + commentLength);
            mOut.write(commentLength);      // 小端
            mOut.write(commentLength/256);  // 大端
            mOut.write(channelEncode);      // 渠道注释内容
            mOut.close();
            LogTool.w(TAG, "write channel succ");
        } catch (Exception e) {
            LogTool.w(TAG, "write channel fail, fail, fail");
            LogTool.w(TAG, e);
        }
    }

    private static String getChannelJson(int channelValue) {
        String str = "";
        try {
            JSONObject jsonObject = new JSONObject();
            jsonObject.put(CHANNEL_JSON_KEY, "" + channelValue);
            str = jsonObject.toString();
        } catch (Exception e) {
            LogTool.w(TAG, e);
        }
        return str;
    }

    public static byte[] encodeChannelString(String channelJson) {
        // XXTea加密
        byte[] xxDst = XXTea.XXTeaEncrypt(channelJson.getBytes(), channelJson.getBytes().length, KEY.getBytes());
        if (xxDst == null) {
            LogTool.i(TAG, "XX Encrypt fail");
            return null;
        }
        // Base64加密
        byte[] b64Dst = Base64.encode(xxDst, Base64.DEFAULT);
        if (b64Dst == null) {
            LogTool.i(TAG, "Base64 Encrypt fail");
            return null;
        }
        return b64Dst;
    }


    private static int findMagicByteStartPosition(byte[] content) {
        byte[] MAGIC_BYTES = new byte[]{
                (byte) 0x50,
                (byte) 0x4b,
                (byte) 0x05,
                (byte) 0x06
        };
        int magicStartPos = -1;
        if (null == content || content.length < 4) {
            return magicStartPos;
        }

        for (int i = 0; i < content.length-4; ++i) {
            if (content[i] == MAGIC_BYTES[0]
                    && content[i+1] == MAGIC_BYTES[1]
                    && content[i+2] == MAGIC_BYTES[2]
                    && content[i+3] == MAGIC_BYTES[3]) {
                // 匹配到magic bytes
                magicStartPos = i;
                break;
            }
        }
        return magicStartPos;
    }

    public static int findCommentLengthStartPosition(File file) {
        int pos = doFindCommentLengthStartPosition(file, BUFFER_SMALL); // 一般comment很少，读取1k就可以读取到
        if (pos < 0) {
            pos = doFindCommentLengthStartPosition(file, BUFFER_LARGE); // 特殊情况，就读取8K来查找，如果还读取不到，放弃算了。
        }
        return pos;
    }

    public static int doFindCommentLengthStartPosition(File file, int bufferLength) {
        int magicByteStartPos = -1;
        if (null == file) {
            return magicByteStartPos;
        }

        byte[] buffer = new byte[bufferLength];
        int dataLenth = ((int) file.length());
        LogTool.i(TAG, "file.length(): " + file.length());

        int startPos = dataLenth > bufferLength ? (dataLenth-bufferLength) : 0;
        try {
            FileInputStream stream = new FileInputStream(file);
            stream.skip(startPos);
            if ((stream.read(buffer)) != -1) {
                int posInBuffer = findMagicByteStartPosition(buffer);
                magicByteStartPos = startPos + posInBuffer;
                LogTool.i(TAG, "magicByteStartPos " + magicByteStartPos);
            }
            stream.close();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
        }

        if (dataLenth < magicByteStartPos + 20 + 2) {
            // 往后移动20字节，就是注释长度的字节位置，注释长度的占2个字节。
            // 文件如果少于这个长度，表示注释位置可能错误
            return -1;
        }
        return magicByteStartPos + 20;
    }
}
