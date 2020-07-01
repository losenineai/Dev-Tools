package com.xxlib.view.list.Interface;

import android.view.View;

/**
 * Created by M on 2015/9/20.
 */
public interface IStateView {
    void setOnViewClickListener(OnStateViewClickListener onStateViewClickListener);
    void removeOnViewClickListener();
    void showNoNetView(int textResId, int imageResId);
    void showNoNetView(int textResId);
    void showNoNetView();
    void showNoDataView(int textResId, int imageResId);
    void showNoDataView(int textResId);
    void showNoDataView();
    void showLoadingView();
    void hideNoNetView();
    void hideNoDataView();
    void hideLoadingView();
    void hideInstallingView();
    void hideAllView();
    void showInstallingView();
    void setNoDataText(int resId);
    void setNoNetText(int resId);
    void setNoDataView(View view);
    void setNoNetView(View view);
    void showNotLoginView(int tipsResId, View.OnClickListener listener);
    void showNotLoginView(String tipsStr, View.OnClickListener listener);
    void hideNotLoginView();
    boolean isLoading();
    boolean isNoNet();
    boolean isNoData();
}
