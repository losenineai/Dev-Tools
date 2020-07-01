/**
 * Copyright (C) 2014 android10.org. All rights reserved.
 * @author Fernando Cejas (the android10 coder)
 */
package com.auric.intell.commonlib.utils;

import android.os.Handler;
import android.os.Looper;

/**
 * MainThread (UI Thread) implementation based on a Handler instantiated with the main
 * application Looper.
 */
public class UIThread implements PostExecutionThread {

  private static class LazyHolder {
    private static final UIThread INSTANCE = new UIThread();
  }

  public static UIThread getInstance() {
    return LazyHolder.INSTANCE;
  }

  private final Handler handler;

  private UIThread() {
    this.handler = new Handler(Looper.getMainLooper());
  }

  /**
   * Causes the Runnable r to be added to the message queue.
   * The runnable will be run on the main thread.
   *
   * @param runnable {@link Runnable} to be executed.
   */
  @Override
  public void post(Runnable runnable) {
    handler.post(runnable);
  }

  @Override
  public void post(Runnable runnable, long deley) {
    handler.postDelayed(runnable,deley);
  }

    /**
     * remove all handle message
     */
    public void removeAll() {
        if (handler != null) {
            handler.removeCallbacksAndMessages(null);
        }
    }


}
