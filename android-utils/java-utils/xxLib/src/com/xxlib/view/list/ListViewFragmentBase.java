package com.xxlib.view.list;

import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.xxlib.utils.ToastUtils;
import com.xxlib.utils.base.LogTool;
import com.xxlib.utils.floatview.CheckAppRunning;
import com.xxlib.view.list.Interface.ExBaseAdapter;
import com.xxlib.view.list.Interface.IExHttpListener;
import com.xxlib.view.list.Interface.IListView;
import com.xxlib.view.list.Interface.IStateView;
import com.xxlib.view.list.Interface.IXListViewListener;
import com.xxlib.view.list.Interface.OnStateViewClickListener;

import java.util.List;

/**
 * Created by jammy on 15/9/16.
 * Modified by mushroom on 15/9/23.
 */
public abstract class ListViewFragmentBase extends Fragment implements IXListViewListener, IFloatFragment {
    private static final String TAG = "ListViewFragmentBase";

    protected Context mContext;

    // 声明状态常量
    protected static final int LV_STATE_DONE = 100;
    protected static final int LV_STATE_LOAD_MORE = 101;
    protected static final int LV_STATE_REFRESH = 102;
    protected static final int LV_STATE_INIT = 103;
    protected int mUserAction = 0;

    protected IExHttpListener mListDataHttpListener;
    protected ExBaseAdapter mListAdapter;
    protected IListView mExListView;
    protected IStateView mUniversalViewStateWidget;
    protected View mRootView;
    protected MessageHandler mMessageHandler;
    protected static int POST_DELAY_TO_LOAD_TIME = 1000;
    // 是否自动初始化数据
    protected boolean mIsAutoInitData = true;
    // 上一次请求的数量
    protected int mLastRequestCount;

    // handler的消息定义
    protected static final int MSG_INIT_SUCCESS_WITH_DATA = 200;
    protected static final int MSG_INIT_NO_DATA = 201;
    protected static final int MSG_INIT_NO_NET = 202;
    protected static final int MSG_INIT_LOADING = 209;
    protected static final int MSG_REFRESH_SUCCESS_WITH_DATA = 203;
    protected static final int MSG_REFRESH_NO_DATA = 204;
    protected static final int MSG_REFRESH_NO_NET = 205;
    protected static final int MSG_LOAD_MORE_SUCCESS_WITH_DATA = 206;
    protected static final int MSG_LOAD_MORE_NO_DATA = 207;
    protected static final int MSG_LOAD_MORE_NO_NET = 208;
    protected static final int MSG_REFRESH_FAILURE = 210;
    /**
     * 子类还有handler的消息的话，就在这个base上面递增
     */
    protected static final int MSG_SUB_BASE = 250;

    private Handler mHandler = new Handler() {
        public void handleMessage(Message msg) {
            if (mMessageHandler != null)
                mMessageHandler.beforeMessageHandled(msg);
            switch (msg.what) {
                case MSG_INIT_SUCCESS_WITH_DATA:
                case MSG_REFRESH_SUCCESS_WITH_DATA:
                    if (mUniversalViewStateWidget != null)
                        mUniversalViewStateWidget.hideAllView();
                    if (mExListView != null)
                        mExListView.setVisibility(View.VISIBLE);
                    if (mListAdapter != null) {
                        mListAdapter.setData((List) msg.obj);
                        mListAdapter.notifyDataSetChanged();
                    }
                    if (mExListView != null)
                        mExListView.setPullLoadEnable(isCanLoadMore());
                    break;
                case MSG_REFRESH_NO_DATA:
                case MSG_INIT_NO_DATA:
                    if (mUniversalViewStateWidget != null)
                        mUniversalViewStateWidget.showNoDataView(getNoDataStrResId());
                    if (mExListView != null)
                        mExListView.setVisibility(View.GONE);
                    break;
                case MSG_INIT_NO_NET:
                    if (mUniversalViewStateWidget != null)
                        mUniversalViewStateWidget.showNoNetView();
                    if (mExListView != null)
                        mExListView.setVisibility(View.GONE);
                    break;
                case MSG_REFRESH_FAILURE:
                    if (mExListView == null || mListAdapter == null || mListAdapter.getCount() == 0)
                        if (mUniversalViewStateWidget != null)
                            mUniversalViewStateWidget.showNoNetView();
                    break;
                case MSG_REFRESH_NO_NET:
                    showNoNetToast();
                    break;
                case MSG_LOAD_MORE_SUCCESS_WITH_DATA:
                    if (mListAdapter != null) {
                        if (msg.obj != null && msg.obj instanceof List)
                            mListAdapter.appendData((List) msg.obj);
                        mListAdapter.notifyDataSetChanged();
                    }
                    mExListView.setPullLoadEnable(isCanLoadMore());
                    break;
                case MSG_LOAD_MORE_NO_DATA:
//                    showNoMoreToast();
                    if (mListAdapter != null) {
                        if (msg.obj != null && msg.obj instanceof List)
                            mListAdapter.appendData((List) msg.obj);
                        mListAdapter.notifyDataSetChanged();
                    }
                    mExListView.setPullLoadEnable(false);
                    break;
                case MSG_LOAD_MORE_NO_NET:
                    if(mExListView != null){
                        mExListView.onLoadMoreNoNet(null);
                    }
                    showNoNetToast();
                    break;
                case MSG_INIT_LOADING:
                    break;
                default:
                    if (mMessageHandler != null)
                        mMessageHandler.onMessageReceived(msg);
                    return;
            }
            stopRefresh();
            stopLoadMore();
            if (mMessageHandler != null)
                mMessageHandler.afterMessageHandled(msg);
        }
    };

    protected ResIdPackage mResIdPackage = new ResIdPackage();

    protected abstract void setResIdToThisPackage(ResIdPackage resIdPackage);

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        setResIdToThisPackage(mResIdPackage);
        return inflater.inflate(mResIdPackage.getLayoutResId(), null);
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        mContext = view.getContext();
        mRootView = view;
        mUserAction = LV_STATE_DONE;
        beforeParentInitWidget();
        initWidget();
        afterParentInitWidget();
        beforeParentSetWidget();
        setWidget();
        afterParentSetWidget();
        if (mIsAutoInitData) {
            onRefresh();
        }
    }

    @Override
    public View getView(ViewGroup container) {
        Context context = container.getContext();
        View view = onCreateView(LayoutInflater.from(context), null, null);
        onViewCreated(view, null);
        return view;
    }

    private void initWidget() {
        setResIdToThisPackage(mResIdPackage);
        if (mExListView == null)
            mExListView = (IListView) mRootView.findViewById(mResIdPackage.getListViewResId());
        if (mUniversalViewStateWidget == null)
            mUniversalViewStateWidget = (IStateView) mRootView.findViewById(mResIdPackage.getStateViewResId());
        if (mListDataHttpListener == null)
            mListDataHttpListener = new IExHttpListener() {

                private Object mExtraData;

                public void setExtraData(Object object) {
                    mExtraData = object;
                }

                public Object getExtraData() {
                    return mExtraData;
                }

                @Override
                public void onSuccess(int cmd, Object obj) {
                    if (cmd == 0 && obj != null) {
                        boolean isRefresh = false;
                        if (mUserAction == LV_STATE_REFRESH)
                            isRefresh = true;
                        List list = parseListData(obj, isRefresh, mExtraData);
                        mLastRequestCount = list == null ? 0 : list.size();
                        if (mUserAction == LV_STATE_LOAD_MORE) {
                            if (list == null || list.size() == 0)
                                mHandler.sendEmptyMessage(MSG_LOAD_MORE_NO_DATA);
                            else
                                mHandler.obtainMessage(MSG_LOAD_MORE_SUCCESS_WITH_DATA, list).sendToTarget();
                        } else if (mUserAction == LV_STATE_REFRESH)
                            if ((list == null || list.size() == 0))
                                mHandler.sendEmptyMessage(MSG_REFRESH_NO_DATA);
                            else
                                mHandler.obtainMessage(MSG_REFRESH_SUCCESS_WITH_DATA, list).sendToTarget();

                    } else {
                        onFailure(cmd, obj);
                    }
                }

                @Override
                public void onFailure(int cmd, Object obj) {
//                    if (mListAdapter.getCount() != 0)
//                        if (mUserAction == LV_STATE_LOAD_MORE)
//                            mHandler.sendEmptyMessage(MSG_LOAD_MORE_NO_NET);
//                        else
//                            mHandler.sendEmptyMessage(MSG_REFRESH_NO_NET);
//                    else
//                        mHandler.sendEmptyMessage(MSG_INIT_NO_NET);
                    mHandler.sendEmptyMessage(MSG_REFRESH_FAILURE);
                }

                @Override
                public void onNetException() {
                    if (mListAdapter.getCount() != 0)
                        if (mUserAction == LV_STATE_LOAD_MORE)
                            mHandler.sendEmptyMessage(MSG_LOAD_MORE_NO_NET);
                        else
                            mHandler.sendEmptyMessage(MSG_REFRESH_NO_NET);
                    else
                        mHandler.sendEmptyMessage(MSG_INIT_NO_NET);
                }
            };
    }

    private void setWidget() {
        mExListView.setXListViewListener(this);
        mUniversalViewStateWidget.showLoadingView();
        mUniversalViewStateWidget.setOnViewClickListener(new OnStateViewClickListener() {
            @Override
            public void onNoNetViewClick() {
                if (mUserAction == LV_STATE_DONE) {
                    mUniversalViewStateWidget.showLoadingView();
                    onRefresh();
//                    loadRefreshData(mExListView.getMaxCountPerRequest(), mListDataHttpListener);
                }
            }

            @Override
            public void onNoDataViewClick() {
            }
        });
        mExListView.setVisibility(View.GONE);
        mListAdapter = getAdapter();
        if (mListAdapter != null)
            mExListView.setAdapter(mListAdapter);
        mMessageHandler = getMessageHandler();
        setBaseWidget(mExListView);
    }

    protected MessageHandler getMessageHandler() {
        return null;
    }

    // 供外部调用内部的Handler来发自定义的消息，最终会回调到MessageHandler的实现类中
    protected void sendMessage(int what, long delayTime, Object obj, int arg1, int arg2, Bundle bundle) {
        Message message = mHandler.obtainMessage();
        message.what = what;
        message.obj = obj;
        message.arg1 = arg1;
        message.arg2 = arg2;
        message.setData(bundle);
        mHandler.sendMessageDelayed(message, delayTime);
    }

    protected void removeMessage(int what) {
        mHandler.removeMessages(what);
    }

    /**
     * 外部调用的刷新,无延迟操作
     */
    public final void refreshData() {
        if (null == mExListView) {
            LogTool.i(TAG, "refreshData mExListView is null");
            return;
        }
        if (mUserAction == LV_STATE_DONE) {
            mUserAction = LV_STATE_REFRESH;
            loadRefreshData(mExListView.getMaxCountPerRequest(), mListDataHttpListener);
        }
    }


    @Override
    public final void onRefresh() {
        if (null == mExListView) {
            LogTool.i(TAG, "onRefresh mExListView is null");
            return;
        }
        if (mUserAction == LV_STATE_DONE) {
            mUserAction = LV_STATE_REFRESH;
            mHandler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    loadRefreshData(mExListView.getMaxCountPerRequest(), mListDataHttpListener);
                }
            }, POST_DELAY_TO_LOAD_TIME);
        }
    }

    @Override
    public final void onLoadMore() {
        if (null == mExListView) {
            return;
        }
        if (mUserAction == LV_STATE_DONE) {
            if (isCanLoadMore()) {
                mUserAction = LV_STATE_LOAD_MORE;
                mHandler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        loadMoreData(mListAdapter, mExListView.getMaxCountPerRequest(), mListDataHttpListener);
                    }
                }, POST_DELAY_TO_LOAD_TIME);
            } else {
                stopLoadMore();
            }
        }
    }

    private void stopRefresh() {
        mExListView.stopRefresh();
        mExListView.setRefreshTime();
        mUserAction = LV_STATE_DONE;
    }

    private void stopLoadMore() {
        mExListView.stopLoadMore();
        mUserAction = LV_STATE_DONE;
    }

    protected void showNoMoreToast() {
        ToastUtils.show(mContext, mContext.getString(mResIdPackage.getNoMoreTextId()), !CheckAppRunning.isXxRunningOnForeground(mContext, mContext.getPackageName()));
    }

    protected void showNoNetToast() {
        // 这里原来是用application的context不知道为什么
        ToastUtils.show(mContext, mContext.getString(mResIdPackage.getNetErrorTextId()), !CheckAppRunning.isXxRunningOnForeground(mContext, mContext.getPackageName()));
    }

    public void showNotLoginView(int tipsResId, View.OnClickListener listener) {
        if (mUniversalViewStateWidget != null)
            mUniversalViewStateWidget.showNotLoginView(tipsResId, listener);
    }

    public void showNotLoginView(String tipsStr, View.OnClickListener listener) {
        if (mUniversalViewStateWidget != null)
            mUniversalViewStateWidget.showNotLoginView(tipsStr, listener);
    }

    public void hideNotLoginView() {
        mUniversalViewStateWidget.hideNotLoginView();
    }

    public void setNoDataView(View view) {
        mUniversalViewStateWidget.setNoDataView(view);
    }

    public void setNoNetView(View view) {
        mUniversalViewStateWidget.setNoNetView(view);
    }

    // 没有特殊需求无需重写此方法
    protected boolean isCanLoadMore() {
        boolean isCanLoadMore = false;
//        if (mExListView != null) {
//            isCanLoadMore = mExListView.isCanLoadMore();
//        }

        int readCount = mListAdapter.getCount();

        if (readCount > 0 && mLastRequestCount >= mExListView.getMaxCountPerRequest()) {
            // 下拉刷新
            if (mUserAction == LV_STATE_REFRESH) {
                isCanLoadMore = true;
            }
            // 加载更多，或者notifyDataSetChanged
            else {
                isCanLoadMore = mExListView.getPullLoadMoreEnable();
            }
        } else {
            isCanLoadMore = false;
        }

        return isCanLoadMore;
    }

    @Override
    public void onResume() {
        super.onResume();
        LogTool.i(TAG, "onResume!!!");
        onResume(mListAdapter);
    }

    /**
     * 子类想在onResume处理刷新，可以重写此方法
     *
     * @param adapter
     */
    protected void onResume(ExBaseAdapter adapter) {
    }


    public void setAutoInitData(boolean auto) {
        mIsAutoInitData = auto;
    }

    @Override
    public void refreshUI(int type, Object... objects) {
    }

    /**
     * 以下4个函数是供子类对父类行为进行注入的接口
     */
    protected void beforeParentInitWidget() {
    }

    protected void afterParentInitWidget() {
    }

    protected void beforeParentSetWidget() {
    }

    protected void afterParentSetWidget() {
    }

    /****************************************** 子类必须实现部分 ****************************************/
    /**
     * 设置listview属性，子类的ui赋值可以在此进行，此方法会在初始化父类所有ui后才调用
     *
     * @param listView
     */
    protected abstract void setBaseWidget(IListView listView);

    /**
     * 解析数据返回显示在ListView的list
     *
     * @param object    数据返回的object
     * @param isRefresh
     * @param extraData
     * @return
     */
    protected abstract List parseListData(Object object, boolean isRefresh, Object extraData);

    /**
     * 返回Adapter数据
     *
     * @return
     */
    protected abstract ExBaseAdapter getAdapter();

    /**
     * 异步刷新数据，必须使用listener回调
     *
     * @param loadCount
     * @param listener
     */
    protected abstract void loadRefreshData(int loadCount, IExHttpListener listener);

    /**
     * 异步加载更多数据，必须使用listener回调
     *
     * @param adapter
     * @param loadCount
     * @param listener
     */
    protected abstract void loadMoreData(ExBaseAdapter adapter, int loadCount, IExHttpListener listener);

    /**
     * 返回无数据页面的wording resId
     *
     * @return
     */
    protected abstract int getNoDataStrResId();

    /**
     * 如需自定义更多类型的消息，实现这个接口然后set进来就好了，对于无法识别的数据类型我们会分发给实现这个接口的类去做
     * 这里出现回调是能确保在主线程的了~外部无需再用handler来处理
     */
    public interface MessageHandler {
        void onMessageReceived(Message message);

        void beforeMessageHandled(Message message);

        void afterMessageHandled(Message message);
    }

}
