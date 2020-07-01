package com.xxlib.utils;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;

/**
 * Created by white.
 */
public class TaobaoUtils {

    private static final String TAG = "TaobaoUtils";

    public static boolean openTaobaoUrl(Context context, String url){
        // 不是淘宝链接不跳转
        if(!url.contains("item.taobao.com")){
            return false;
        }
        try{
            Intent intent= new Intent();
            intent.setAction(Intent.ACTION_VIEW);
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            Uri content_url = Uri.parse(url);
            intent.setData(content_url);
            context.startActivity(intent);
            return true;
        }catch(Exception er){
            er.printStackTrace();
        }
        return false;
    }

}
