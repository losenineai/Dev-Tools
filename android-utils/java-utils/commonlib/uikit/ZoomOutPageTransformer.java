package com.auric.intell.commonlib.uikit;

import android.support.v4.view.ViewPager;
import android.view.View;

/**
 * Created by sh on 2018/3/12.
 */

public class ZoomOutPageTransformer implements ViewPager.PageTransformer {
    private  float MAX_SCALE = 1f;
    private  float MIN_SCALE = 0.7f;//0.85f

    public void setMaxScale(float maxScale){
        MAX_SCALE=maxScale;
    }
    public void setMinScale(float minScale){
        MIN_SCALE=minScale;
    }

    @Override
    public void transformPage(View view, float position) {
        //setScaleY只支持api11以上
        if (position < -1){
            view.setScaleX(MIN_SCALE);
            view.setScaleY(MIN_SCALE);
        } else if (position <= 1) //a页滑动至b页 ； a页从 0.0 -1 ；b页从1 ~ 0.0
        { // [-1,1]
//              Log.e("TAG", view + " , " + position + "");
            float scaleFactor =  MIN_SCALE+(1-Math.abs(position))*(MAX_SCALE-MIN_SCALE);
            view.setScaleX(scaleFactor);
            //每次滑动后进行微小的移动目的是为了防止在三星的某些手机上出现两边的页面为显示的情况
            if(position>0){
                view.setTranslationX(-scaleFactor*2);
            }else if(position<0){
                view.setTranslationX(scaleFactor*2);
            }
            view.setScaleY(scaleFactor);

        } else
        { // (1,+Infinity]

            view.setScaleX(MIN_SCALE);
            view.setScaleY(MIN_SCALE);
        }
    }

}