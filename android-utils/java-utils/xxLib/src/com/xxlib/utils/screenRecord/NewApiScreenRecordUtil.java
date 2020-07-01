package com.xxlib.utils.screenRecord;

import android.annotation.TargetApi;
import android.content.Context;
import android.hardware.display.DisplayManager;
import android.hardware.display.VirtualDisplay;
import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaFormat;
import android.media.MediaMuxer;
import android.media.MediaRecorder;
import android.media.MediaRecorder.OnErrorListener;
import android.media.projection.MediaProjection;
import android.view.Surface;

import com.xxlib.utils.base.LogTool;

import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * @author linbin 2015-4-20 -- 下午5:21:09
 * @Description 给5.0及以上的系统版本使用的录屏工具类
 * @modify xiaQing 2015-5-28
 */

@TargetApi(21)
public class NewApiScreenRecordUtil {

    private static final String TAG = "NewApiScreenRecordUtil";

    private ScreenRecordThread mRecordThread;
    private int mWidth;
    private int mHeight;
    private int mBitRate;
    private int mDpi;
    private String mDstPath;
    public String mUnCompleteMediaPath;
    public String mUnCompleteAudioPath;
    private MediaProjection mMediaProjection;
    private MediaRecorder mediaRecorder;
    private boolean isRecording;
    private Context mContext;

    private static final String VIDEO_TYPE = "video/avc"; // H.264 视频编码格式

    private static final int FRAME_RATE = 30; // 30帧每秒
    private static final int IFRAME_INTERVAL = 10;
    private static final int TIMEOUT = 10000;

    private MediaCodec mEncoder;
    private Surface mSurface;
    private MediaMuxer mMuxer;
    private boolean mMuxerStarted = false;
    private int mVideoTrackIndex = -1;
    private AtomicBoolean mQuit = new AtomicBoolean(false);
    private MediaCodec.BufferInfo mBufferInfo = new MediaCodec.BufferInfo();
    private VirtualDisplay mVirtualDisplay;
    private ComposeListener mComposeListener;

    /**
     * 录屏工具类 5.0以上适用
     *
     * @param dstPath 存放录屏mp4的路径
     * @param width   宽
     * @param height  高
     * @param bitrate 比特率
     * @param dpi     密度
     * @param mp      录屏数据入口：MediaProjection
     */
    public NewApiScreenRecordUtil(String dstPath, int width, int height, int bitrate, int dpi, MediaProjection mp, Context context) {
        mContext = context;
        mWidth = width;
        mHeight = height;
        mBitRate = bitrate;
        mDpi = dpi;
        mMediaProjection = mp;
        mDstPath = dstPath;
        mRecordThread = new ScreenRecordThread();
        mUnCompleteMediaPath = dstPath.replace(".mp4", "_uncomplete.mp4");
        mUnCompleteAudioPath = dstPath.replace(".mp4", "_uncomplete.acc");
    }


    /**
     * 视频地址
     */
    public String getVideoPath() {
        return mDstPath;
    }

    /**
     * 停止录屏
     */
    public void stopRecord(ComposeListener composeListener) {
        if (isRecording) {
            mComposeListener = composeListener;
            mQuit.set(true);
        }
    }

    /**
     * 开始录屏
     */
    public void startRecord() {
        if (mRecordThread != null && !isRecording) {
            mRecordThread.start();
        }
    }

    // 录屏在该线程中启动
    class ScreenRecordThread extends Thread {
        @Override
        public void run() {
            super.run();
            isRecording = true;
            try {
                recordAudio();
                recordVirtualDisplay();
            } catch (Exception e) {
                LogTool.e(TAG, "ScreenRecordThread :" + e.getMessage());
            } finally {
                release();
                composeMediaAndAudio();
                if (mComposeListener != null) {
                    mComposeListener.finish();
                    mComposeListener = null;
                }
                isRecording = false;
            }
        }
    }

    /**
     * 重设输出格式
     */
    private void resetOutputFormat() {
        if (mMuxerStarted) {
            throw new IllegalStateException("output format already changed!");
        }
        MediaFormat newFormat = mEncoder.getOutputFormat();

        mVideoTrackIndex = mMuxer.addTrack(newFormat);
        mMuxer.start();
        mMuxerStarted = true;
    }

    /**
     * 对视频输出做编码
     *
     * @param index
     */
    private void encodeToVideoTrack(int index) {
        ByteBuffer encodedData = mEncoder.getOutputBuffer(index);

        if ((mBufferInfo.flags & MediaCodec.BUFFER_FLAG_CODEC_CONFIG) != 0) {
            mBufferInfo.size = 0;
        }
        if (mBufferInfo.size == 0) {
            encodedData = null;
        }
        if (encodedData != null) {
            encodedData.position(mBufferInfo.offset);
            encodedData.limit(mBufferInfo.offset + mBufferInfo.size);
            mMuxer.writeSampleData(mVideoTrackIndex, encodedData, mBufferInfo);
        }
    }

    /**
     * 循环读取，进行录屏
     */
    private void recordVirtualDisplay() {
        while (!mQuit.get()) {
            int index = mEncoder.dequeueOutputBuffer(mBufferInfo, TIMEOUT);
            if (index == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                resetOutputFormat();
            } else if (index == MediaCodec.INFO_TRY_AGAIN_LATER) {
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    LogTool.e(TAG, "recordVirtualDisplay:" + e.getMessage());
                }
            } else if (index >= 0) {
                if (!mMuxerStarted) {
                    throw new IllegalStateException(
                            "MediaMuxer dose not call addTrack(format) ");
                }
                encodeToVideoTrack(index);

                mEncoder.releaseOutputBuffer(index, false);
            }
        }
    }

    /**
     * 准备录屏的一些参数初始化
     *
     * @throws IOException
     */
    public boolean prepareForRecord() {
        LogTool.i(TAG, "prepareForRecord");
        try {
            MediaFormat format = MediaFormat.createVideoFormat(VIDEO_TYPE,
                    mWidth, mHeight);
            format.setInteger(MediaFormat.KEY_COLOR_FORMAT,
                    MediaCodecInfo.CodecCapabilities.COLOR_FormatSurface);
            format.setInteger(MediaFormat.KEY_BIT_RATE, mBitRate);
            format.setInteger(MediaFormat.KEY_FRAME_RATE, FRAME_RATE);
            format.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, IFRAME_INTERVAL);

            mEncoder = MediaCodec.createEncoderByType(VIDEO_TYPE);
            mEncoder.configure(format, null, null,
                    MediaCodec.CONFIGURE_FLAG_ENCODE);
            mSurface = mEncoder.createInputSurface();
            mEncoder.start();
        } catch (Exception e) {
            LogTool.w(TAG, "prepare mEncoder ex " + e.getMessage());
            release();
            return false;
        }

        try {
            mMuxer = new MediaMuxer(mUnCompleteMediaPath,
                    MediaMuxer.OutputFormat.MUXER_OUTPUT_MPEG_4);
        } catch (Exception e) {
            LogTool.w(TAG, "prepare MediaMuxer ex " + e.getMessage());
            release();
            return false;
        }

        try {
            mVirtualDisplay = mMediaProjection.createVirtualDisplay(
                    "ScreenRecorder-display", mWidth, mHeight, mDpi,
                    DisplayManager.VIRTUAL_DISPLAY_FLAG_PUBLIC, mSurface, null,
                    null);
        } catch (Exception e) {
            LogTool.w(TAG, "prepare VirtualDisplay ex " + e.getMessage());
            release();
            return false;
        }

        LogTool.w(TAG, "prepareForRecord succ succ");
        return true;
    }

    /**
     * 释放资源
     */
    private void release() {
        try {

            if (mEncoder != null) {
                mEncoder.stop();
                mEncoder.release();
                mEncoder = null;
            }
            if (mVirtualDisplay != null) {
                mVirtualDisplay.release();
            }
            if (mMediaProjection != null) {
                mMediaProjection.stop();
            }
            if (mMuxer != null) {
                mMuxer.stop();
                mMuxer.release();
                mMuxer = null;
            }
            if (mediaRecorder != null) {
                mediaRecorder.stop();
                mediaRecorder.release();
                mediaRecorder = null;
            }

        } catch (Exception e) {

        }
    }

    /**
     * 开始录音
     */
    private void recordAudio() {
        try {
            File file = new File(mUnCompleteAudioPath);
            if (file.exists()) {
                // 如果文件存在，删除它，演示代码保证设备上只有一个录音文件
                file.delete();
            }
            mediaRecorder = new MediaRecorder();
            // 设置音频录入源
            mediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
            // 设置录制音频的输出格式
            mediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.AAC_ADTS);
            // 设置音频的编码格式
            mediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AAC);
            // 设置录制音频文件输出文件路径
            mediaRecorder.setOutputFile(file.getAbsolutePath());

            mediaRecorder.setOnErrorListener(new OnErrorListener() {

                @Override
                public void onError(MediaRecorder mr, int what, int extra) {
                    // 发生错误，停止音频录制
                    mediaRecorder.stop();
                    mediaRecorder.release();
                    mediaRecorder = null;
                    LogTool.e(TAG, "recordAudio on error:");
                }
            });

            // 准备、开始
            mediaRecorder.prepare();
            mediaRecorder.start();

        } catch (Exception e) {
            LogTool.e(TAG, "recordAudio:" + e.getMessage());
            File file = new File(mUnCompleteAudioPath);
            if (file.exists())
                file.delete();
            e.printStackTrace();
        }
    }

    /**
     * 合成音频和视频
     */
    private void composeMediaAndAudio() {
        boolean isSucc = PluginApkLoader.getInstance().getIRecordCompose().composeVideoAudio(mUnCompleteMediaPath, mUnCompleteAudioPath, mDstPath);
        LogTool.i(TAG, "compose result " + isSucc);

        File unCompleteMedia = new File(mUnCompleteMediaPath);
        File unCompleteAudio = new File(mUnCompleteAudioPath);
        LogTool.e(TAG, "删除");
        unCompleteMedia.delete();
        unCompleteAudio.delete();

    }
}
