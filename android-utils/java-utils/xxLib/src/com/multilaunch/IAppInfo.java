package com.multilaunch;


import android.graphics.drawable.Drawable;

/**
 * Created by white on 15-12-28.
 */
public interface IAppInfo {

    Drawable getIcon();

    String getAppname();

    String getPackname();

    String getVersion();

}
