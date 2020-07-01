package com.auric.intell.commonlib.floatview.main;


import android.content.Context;
import android.os.Handler;

import java.util.ArrayList;
import java.util.Hashtable;

import com.auric.intell.commonlib.utils.LogTool;

/**
 * 悬浮窗主题
 * @author white
 * @date 16/7/27
 */
public class FloatViewSubject {

    private Hashtable<Integer, ArrayList<IFloatViewObserver>> mObserversHashtable;
    private Context mContext;
    private static FloatViewSubject mFloatViewSubject;

    private FloatViewSubject() {
        mObserversHashtable = new Hashtable<Integer, ArrayList<IFloatViewObserver>>();
    }

    public static FloatViewSubject getInstance() {
        synchronized (FloatViewSubject.class) {
            if (mFloatViewSubject == null) {
                mFloatViewSubject = new FloatViewSubject();
            }
            return mFloatViewSubject;
        }
    }

    public void init(Context context) {
        mContext = context;
    }

    public void register(int type, IFloatViewObserver floatViewObserver) {
        LogTool.d("Sub-Obs", "registerSubject : " + type);
        if (mObserversHashtable.containsKey(type)) {
            LogTool.d("Sub-Obs", "registerSubject add observer ");
            if (!mObserversHashtable.get(type).contains(floatViewObserver))
                mObserversHashtable.get(type).add(floatViewObserver);
        } else {
            LogTool.d("Sub-Obs", "registerSubject add array ");
            ArrayList<IFloatViewObserver> arrayList = new ArrayList<IFloatViewObserver>();
            arrayList.add(floatViewObserver);
            mObserversHashtable.put(type, arrayList);
        }
    }

    public void unRegister(int type, IFloatViewObserver floatViewObserver) {
        if (mObserversHashtable.containsKey(type)) {
            mObserversHashtable.get(type).remove(floatViewObserver);
            if (mObserversHashtable.get(type).size() <= 0) {
                mObserversHashtable.remove(type);
            }
        }
    }

    public boolean notifyUI(final int type, final Object... objects) {
        LogTool.d("Sub-Obs", "type: " + type);
        boolean notifySuc = false;
        if (!mObserversHashtable.containsKey(type)) {
            return notifySuc;
        }
        LogTool.d("Sub-Obs", "type get!: " + type);
        final ArrayList<IFloatViewObserver> observers = mObserversHashtable.get(type);
        new Handler(mContext.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                for (IFloatViewObserver floatViewObserver : observers) {
                    floatViewObserver.refreshUI(type, objects);
                }
            }
        });
        return true;
    }
}
