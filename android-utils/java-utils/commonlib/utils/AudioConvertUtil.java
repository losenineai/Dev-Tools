package com.auric.intell.commonlib.utils;

import android.media.AudioFormat;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * @author white
 * @description：
 * @date 2016/11/2
 */

public class AudioConvertUtil {

    public class Constants {

        final static public int RECORDER_SAMPLERATE = 16000;
        final static public int RECORDER_CHANNELS = AudioFormat.CHANNEL_IN_MONO;
        final static public int RECORDER_AUDIO_ENCODING = AudioFormat.ENCODING_PCM_16BIT;

        final static public int BufferElements2Rec = 1024; // want to play 2048 (2K) since 2 bytes we use only 1024
        final static public int BytesPerElement = 2; // 2 bytes in 16bit format


    }

    public static boolean pcmToWave(final File rawFile, final File waveFile) {

        try {
            byte[] rawData = new byte[(int) rawFile.length()];
            DataInputStream input = null;
            try {
                input = new DataInputStream(new FileInputStream(rawFile));
                input.read(rawData);
            } finally {
                if (input != null) {
                    input.close();
                }
            }

            DataOutputStream output = null;
            try {
                output = new DataOutputStream(new FileOutputStream(waveFile));
                // WAVE header
                writeString(output, "RIFF"); // chunk id
                writeInt(output, 36 + rawData.length); // chunk size
                writeString(output, "WAVE"); // format
                writeString(output, "fmt "); // subchunk 1 id
                writeInt(output, 16); // subchunk 1 size
                writeShort(output, (short) 1); // audio format (1 = PCM)
                writeShort(output, (short) 1); // number of channels
                writeInt(output, Constants.RECORDER_SAMPLERATE); // sample rate
                writeInt(output, Constants.RECORDER_SAMPLERATE * 2); // byte rate
                writeShort(output, (short) 2); // block align
                writeShort(output, (short) 16); // bits per sample
                writeString(output, "data"); // subchunk 2 id
                writeInt(output, rawData.length); // subchunk 2 size
                // Audio data (conversion big endian -> little endian)
                short[] shorts = new short[rawData.length / 2];
                ByteBuffer.wrap(rawData).order(ByteOrder.LITTLE_ENDIAN).asShortBuffer().get(shorts);
                ByteBuffer bytes = ByteBuffer.allocate(shorts.length * 2);
                for (short s : shorts) {
                    bytes.putShort(s);
                }

                output.write(fullyReadFileToBytes(rawFile));

                return true;
            } finally {
                if (output != null) {
                    output.close();
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return false;

    }

    private static byte[] fullyReadFileToBytes(File f) throws IOException {
        int size = (int) f.length();
        byte bytes[] = new byte[size];
        byte tmpBuff[] = new byte[size];
        FileInputStream fis= new FileInputStream(f);
        try {

            int read = fis.read(bytes, 0, size);
            if (read < size) {
                int remain = size - read;
                while (remain > 0) {
                    read = fis.read(tmpBuff, 0, remain);
                    System.arraycopy(tmpBuff, 0, bytes, size - remain, read);
                    remain -= read;
                }
            }
        }  catch (IOException e){
            throw e;
        } finally {
            fis.close();
        }

        return bytes;
    }
    private static void writeInt(final DataOutputStream output, final int value) throws IOException {
        output.write(value >> 0);
        output.write(value >> 8);
        output.write(value >> 16);
        output.write(value >> 24);
    }

    private static void writeShort(final DataOutputStream output, final short value) throws IOException {
        output.write(value >> 0);
        output.write(value >> 8);
    }

    private static void writeString(final DataOutputStream output, final String value) throws IOException {
        for (int i = 0; i < value.length(); i++) {
            output.write(value.charAt(i));
        }
    }

}
