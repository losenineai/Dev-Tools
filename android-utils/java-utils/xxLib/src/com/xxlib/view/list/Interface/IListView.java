package com.xxlib.view.list.Interface;

import android.graphics.drawable.Drawable;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListAdapter;

/**
 * Created by M on 2015/9/20.
 */
public interface IListView {
    int getMaxCountPerRequest();
    boolean getPullLoadMoreEnable();
    void stopLoadMore();
    void stopRefresh();
    void setRefreshTime();
    void setAdapter(ListAdapter listAdapter);
    void setVisibility(int visibility);
    void setXListViewListener(IXListViewListener listViewListener);
    void setPullLoadEnable(boolean enable);
    void setPullRefreshEnable(boolean enable);
    void setOnItemClickListener(AdapterView.OnItemClickListener listener);
    int getHeaderViewsCount();
    void addHeaderView(View view);
    void setDivider(Drawable drawable);

    // 新增接口，from xx工程, by xiaojun 2016/4/26
    void onLoadMoreNoNet(View.OnClickListener listener);
}
