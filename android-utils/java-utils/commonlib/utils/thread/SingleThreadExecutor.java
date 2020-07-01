package com.auric.intell.commonlib.utils.thread;

import android.os.Handler;
import android.os.Looper;

import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

/**
 * Created by zhangxiliang on 2016/12/29.
 */
public class SingleThreadExecutor {

    private Handler uiHander;

    private Executor executor;


    public static SingleThreadExecutor getInstance(){
        return InstanceHodler.singleThreadExecutor;

    }

    private SingleThreadExecutor(){
        executor=Executors.newSingleThreadExecutor();
        uiHander= new Handler(Looper.getMainLooper());
    }


    static class InstanceHodler{
       static SingleThreadExecutor singleThreadExecutor=new SingleThreadExecutor();
    }


    public <T> void excuteTask(SyncTask<T> task){
        executor.execute(new TaskRunable(task));
    }


    public void excuteTask(Runnable runnable){
        executor.execute(runnable);
    }

     public interface SyncTask<T>{
        public T doInBackGround();
        public void onCompleted(T result);
    }


    private  class  TaskRunable<T> implements Runnable{

        SyncTask<T> syncTask;

        public TaskRunable(SyncTask<T> task){

            this.syncTask=task;
        }
        @Override
        public void run() {
          final   T t=syncTask.doInBackGround();
            if (uiHander != null) {
                uiHander.post(new Runnable() {
                    @Override
                    public void run() {
                        syncTask.onCompleted(t);
                    }
                });
            }
        }
    }





}
