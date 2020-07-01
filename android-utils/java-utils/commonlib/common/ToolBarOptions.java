package com.auric.intell.commonlib.common;


import com.auric.intell.commonlib.R;

/**
 *
 * Created by hzxuwen on 2016/6/16.
 */
public class ToolBarOptions {
    /**
     * toolbar的title资源id
     */
    public int titleId = 0;
    /**
     * toolbar的title
     */
    public String titleString;
    /**
     * toolbar的subTitle
     */
    public String subTitleString;
    /**
     * toolbar的logo资源id
     */
    public int logoId = 0;
    /**
     * toolbar的返回按钮资源id，默认开启的资源nim_actionbar_dark_back_ico     */

   public int navigateId = R.drawable.icon_back;

    /**
     * toolbar的返回按钮，默认开启
     */
    public boolean isNeedNavigate = true;

    public int bgColor=0;
    public int titleColor=0;

}
