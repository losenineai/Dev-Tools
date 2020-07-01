package com.xxlib.view.list.Interface;

import android.widget.ListAdapter;

import java.util.List;

/**
 * Created by mushroom0417 on 9/22/15.
 */
public interface IListAdapter extends ListAdapter {
    void setData(List list, Object... extraData);
    void appendData(List list, Object... extraData);
    void notifyDataSetChanged();
}
