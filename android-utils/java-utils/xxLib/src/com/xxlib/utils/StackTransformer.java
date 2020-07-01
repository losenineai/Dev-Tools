package com.xxlib.utils;

import android.annotation.SuppressLint;
import android.view.View;


public class StackTransformer extends ABaseTransformer {

	
	@SuppressLint("NewApi")
	@Override
	protected void onTransform(View page, float position) {
		// TODO Auto-generated method stub
//		Log.e("position_itn ", position + "");
		if(android.os.Build.VERSION.SDK_INT> android.os.Build.VERSION_CODES.GINGERBREAD_MR1){
			page.setAlpha(position < 0 ? 1+position : 1-position);
		}
		
		
	}
	@Override
	public boolean isPagingEnabled() {
		if(android.os.Build.VERSION.SDK_INT> android.os.Build.VERSION_CODES.GINGERBREAD_MR1){
			return false;
		}else{
			return true;
		}
	}

//	@SuppressLint("NewApi")
//	@Override
//	protected void onTransform(View view, float position) {
//		view.setTranslationX(position < 0 ? 0f : -view.getWidth() * position);
//	}

	
}
