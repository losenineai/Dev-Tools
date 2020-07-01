package com.xxlib.utils;

import com.xxlib.utils.base.LogTool;

import java.util.concurrent.BlockingDeque;
import java.util.concurrent.Executor;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 * Created by rosejames on 16/3/12.
 * 具体的线程实现
 */
public class XXLibJobExecutor implements Executor {

    private static final String TAG = "JobExecutor";

    private ThreadPoolExecutor mThreadPoolExecutor;

    private static final int CORE_THREAD_POOL = 3;
    private static final int MAX_THREAD_POOL = 5;
    private static final int MAX_ALIVE_TIME = 120;
    private static final TimeUnit MAX_ALIVE_TIME_UINT = TimeUnit.SECONDS;

    private BlockingDeque<Runnable> mRunnableQueue;
    private ThreadFactory mThreadFactory;

    private static XXLibJobExecutor mSingleton;

    public static XXLibJobExecutor getExecutor() {
        if (mSingleton == null) {
            synchronized (XXLibJobExecutor.class) {
                if (mSingleton == null)
                    mSingleton = new XXLibJobExecutor();
            }
        }
        return mSingleton;
    }


    private XXLibJobExecutor() {
        this.mRunnableQueue = new LinkedBlockingDeque<Runnable>();
        this.mThreadFactory = new JobThreadFactory();
        this.mThreadPoolExecutor = new ThreadPoolExecutor(CORE_THREAD_POOL, MAX_THREAD_POOL,
                MAX_ALIVE_TIME, MAX_ALIVE_TIME_UINT, this.mRunnableQueue, this.mThreadFactory);
    }

    @Override
    public void execute(Runnable runnable) {
        if (runnable == null) {
            throw new IllegalArgumentException("Runnable to execute cannot be null");
        }
        try {
            this.mThreadPoolExecutor.execute(runnable);
        } catch (Exception e) {
            LogTool.w(TAG, e);
        }
    }

    private class JobThreadFactory implements ThreadFactory {

        private static final String THREAD_PROFIX = "GPGAME_THREAD_";
        private int mCount = 0;

        @Override
        public Thread newThread(Runnable runnable) {
            return new Thread(runnable, THREAD_PROFIX + mCount++);
        }
    }


}
