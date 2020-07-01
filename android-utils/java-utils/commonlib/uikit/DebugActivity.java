package com.auric.intell.commonlib.uikit;


import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.auric.intell.commonlib.BuildConfig;
import com.auric.intell.commonlib.R;
import com.auric.intell.commonlib.utils.AppStateUtil;
import com.auric.intell.commonlib.utils.AppUtil;
import com.auric.intell.commonlib.utils.ContextFinder;
import com.auric.intell.commonlib.utils.DeviceUtils;

import butterknife.Bind;
import butterknife.ButterKnife;


/**
 * Created by zhangxiliang on 2016/8/15.
 */
public class DebugActivity extends UI {


    TextView mVersionCode;
    TextView mVersionName;
    TextView mSnTv;
    TextView mWifiMacTv;
    TextView mBlueMacTv;
    TextView mTvCustom;

    public static boolean sIsShowing;
    /**
     * 外部自定义内容
     */
    public static final String KEY_CUSTOM_CONTENT = "KEY_CUSTOM_CONTENT";

    @Override
    protected void initView() {
        mVersionCode=findView(R.id.version_code);
        mVersionName=findView(R.id.version_name);
        mSnTv=findView(R.id.sn_tv);
        mWifiMacTv=findView(R.id.wifimac_tv);
        mBlueMacTv=findView(R.id.bluemac_tv);
        mTvCustom = findView(R.id.tv_custom);

        mVersionCode.setText("版本号:" +AppUtil.getAPPVersionCode(ContextFinder.getApplication()));
        mVersionName.setText("版本名称:" + AppUtil.getAPPVersionNameFromAPP(ContextFinder.getApplication()));
        mSnTv.setText("SN号："+ DeviceUtils.getIMEI(this));
        mWifiMacTv.setText("Wifi mac address:"+DeviceUtils.getMacAddr());
        mBlueMacTv.setText("Blue mac address:"+DeviceUtils.getBlueAddress());

        // 自定义内容
        if (getIntent().hasExtra(KEY_CUSTOM_CONTENT)) {
            String custom = getIntent().getStringExtra(KEY_CUSTOM_CONTENT);
            mTvCustom.setText(custom);
            mTvCustom.setVisibility(View.VISIBLE);
        }
        else{
            mTvCustom.setVisibility(View.GONE);
        }

    }

    @Override
    protected void onStart() {
        super.onStart();
        sIsShowing = true;
    }

    @Override
    protected void onStop() {
        super.onStop();
        sIsShowing = false;
    }

    @Override
    protected int getLayoutView() {

        return R.layout.activity_debug;
    }

    @Override
    public void onEvent(BaseEvent event) {
        if(event.getId()==BaseEvent.EVENT_CANCEL){
            finish();
        }
    }
}
