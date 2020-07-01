package com.auric.intell.commonlib.common.view;


import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;

import com.auric.intell.commonlib.R;


public class ProgressDialog extends Dialog {

	private AlertDialog ad;
	private Context context;
	private TextView mTipTv;



	
	public ProgressDialog(Context context) {
		super(context, R.style.progress_dialog);
		this.context = context;
		setCanceledOnTouchOutside(false);
		View dialogView= LayoutInflater.from(context).inflate(R.layout.layout_progress, null);

		setContentView(dialogView);
		getWindow().getAttributes().gravity= Gravity.CENTER;
//		getWindow().getAttributes().width=(int)(DensityUtil.getDeviceWidth((Activity) context)*0.4);
		mTipTv=(TextView)getWindow().findViewById(R.id.tip_tv);

	}
	
	
	public void setTips(String tips){
		mTipTv.setText(tips);
	}
	
	public void setTipsGone(){
		mTipTv.setVisibility(View.GONE);
	}

	public void showDialog(){
		if(!((Activity)context).isFinishing()){
			show();
		}
	}
	
}
