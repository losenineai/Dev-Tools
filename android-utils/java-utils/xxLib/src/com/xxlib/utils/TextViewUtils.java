package com.xxlib.utils;

import android.text.Spannable;
import android.text.SpannableStringBuilder;
import android.text.style.ClickableSpan;
import android.text.style.ForegroundColorSpan;
import android.text.style.URLSpan;
import android.view.View;
import android.widget.TextView;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by white on 15-9-22.
 */
public class TextViewUtils {


    /**
     * textview指定内容颜色设置
     * @param textView
     * @param name
     * @param color
     */
    public static void setTextColor(TextView textView, String name, int color){
        if(textView == null){
            return;
        }

        SpannableStringBuilder textBuilder = new SpannableStringBuilder(textView.getText());
        Pattern pattern = Pattern.compile(name);
        Matcher matcher = pattern.matcher(textBuilder.toString());
        while(matcher.find()){
            int start = matcher.start();
            int end = start +  matcher.group().length();
            if(start == -1 || end == -1){
                break;
            }
            textBuilder.setSpan(new ForegroundColorSpan(color), start, end, Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
            textView.setText(textBuilder);
        }
    }

    /**
     * 处理textview内的url链接
     * @param textView 需要处理的textview
     * @param clickListener 点击事件处理
     */
    public static void handleTextViewUrl(TextView textView, View.OnClickListener clickListener, int tagId) {
        CharSequence text = textView.getText();
        if (text instanceof Spannable) {
            int end = text.length();
            Spannable sp = (Spannable) textView.getText();
            URLSpan[] spans = sp.getSpans(0, end, URLSpan.class);
            SpannableStringBuilder style = new SpannableStringBuilder(text);
            style.clearSpans();// should clear old spans
            for (final URLSpan span : spans) {
                if (span.getURL() == null) {
                    continue;
                }
                TextViewClickableSpan clickableSpan = new TextViewClickableSpan(span.getURL(), clickListener, tagId);
                style.setSpan(clickableSpan, sp.getSpanStart(span), sp.getSpanEnd(span), Spannable.SPAN_EXCLUSIVE_INCLUSIVE);
            }
            textView.setText(style);
        }
    }

    private static class TextViewClickableSpan extends ClickableSpan {
        private String mSpan;
        private View.OnClickListener mClickListener;
        private int mTagId;

        TextViewClickableSpan(String span, View.OnClickListener clickListener, int tagId) {
            mSpan = span;
            mClickListener = clickListener;
            mTagId = tagId;
        }

        @Override
        public void onClick(View view) {
            if (view.getTag(mTagId) != null) {
                view.setTag(mTagId, null);
                return;
            }
            view.setTag(mSpan);
            mClickListener.onClick(view);
        }
    }
}
