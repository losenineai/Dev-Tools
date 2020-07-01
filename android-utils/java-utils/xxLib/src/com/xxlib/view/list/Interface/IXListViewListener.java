package com.xxlib.view.list.Interface;

/**
 * implements this interface to get refreshData/load more event.
	 */
	public interface IXListViewListener {
		public void onRefresh();

		public void onLoadMore();
	}