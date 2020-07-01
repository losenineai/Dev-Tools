package com.auric.intell.commonlib.utils.thread;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Created by zhangxiliang on 2016/12/30.
 */
public class TaskPoolExecutor {

    private final static int POOL_CORE_SIZE = 2;

    private final static int POOL_MAX__SIZE = 10;

    private final static int KEEP_ALIVIE_TIME = 30 * 1000;

    private ExecutorService mExcutor;

    private BlockingQueue<Runnable> mBlockingQueue;


    private TaskPoolExecutor() {
        mBlockingQueue = new LinkedBlockingDeque<Runnable>();
        mExcutor = new ThreadPoolExecutor(POOL_CORE_SIZE, POOL_MAX__SIZE, KEEP_ALIVIE_TIME, TimeUnit.MILLISECONDS, mBlockingQueue, new TaskThreadFactory(TaskThreadFactory.FACTORY_NAME), new ThreadPoolExecutor.DiscardPolicy());
    }

    public static TaskPoolExecutor getInstance() {
        return TaskPoolHolder.taskPoolExecutor;
    }


    public void execute(Runnable runnable) {
        mExcutor.execute(runnable);
    }

    public Future<?> submit(Runnable runnable) {
        synchronized (this) {
            if (mExcutor == null || mExcutor.isShutdown()) {
                return null;
            }
            return mExcutor.submit(runnable);
        }
    }


    static class TaskThreadFactory implements ThreadFactory {
        static String FACTORY_NAME = "TaskThreadFactory";
        private final ThreadGroup mThreadGroup;

        private final AtomicInteger mThreadNumber = new AtomicInteger(1);

        private final String mNamePrefix;

        TaskThreadFactory(String name) {
            SecurityManager s = System.getSecurityManager();

            mThreadGroup = (s != null) ? s.getThreadGroup() : Thread.currentThread().getThreadGroup();

            mNamePrefix = name + "#";
        }

        public Thread newThread(Runnable r) {
            Thread t = new Thread(mThreadGroup, r, mNamePrefix + mThreadNumber.getAndIncrement(), 0);

            // no daemon
            if (t.isDaemon())
                t.setDaemon(false);

            // normal priority
            if (t.getPriority() != Thread.NORM_PRIORITY)
                t.setPriority(Thread.NORM_PRIORITY);

            return t;
        }
    }

    static class TaskPoolHolder {
        static TaskPoolExecutor taskPoolExecutor = new TaskPoolExecutor();
    }


}
