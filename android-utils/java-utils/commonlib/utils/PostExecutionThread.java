package com.auric.intell.commonlib.utils;

interface PostExecutionThread {
    /**
     * Causes the {@link Runnable} to be added to the message queue of the Main UI Thread
     * of the application.
     *
     * @param runnable {@link Runnable} to be executed.
     */
    void post(Runnable runnable);

    void post(Runnable runnable, long deley);
  }