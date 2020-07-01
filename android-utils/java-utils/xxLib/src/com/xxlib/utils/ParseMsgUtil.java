package com.xxlib.utils;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.text.Html;
import android.text.Html.ImageGetter;
import android.text.Spannable;
import android.text.SpannableStringBuilder;
import android.text.style.ImageSpan;

import com.xxlib.R;
import com.xxlib.config.DanMuChattConfig;

/**
 * 消息解析显示工具
 * 1.java使用UTF-16格式存储Unicode编码
 * 
 * @author linbin
 * 
 */
public class ParseMsgUtil {

	/**
	 * 将本地显示内容转换成emoji表情码。将所有[exxxx]转换成unicode编码,供发送到服务器
	 * 
	 * @param cs ：原始内容串，包含各种格式化。不用转换成纯String，如若为EditText中的内容，则直接取EditText.getEditableText()输入，
	 * 	不必使用Html.toHtml()来转换。
	 * @param mContext
	 * @return
	 */
	public static String convertToMsg(CharSequence cs, Context mContext) {
		
		SpannableStringBuilder ssb = new SpannableStringBuilder(cs);
//		Log.e("convertToMsg之前", ssb.toString());
		Pattern sinaPatten = Pattern.compile(DanMuChattConfig.EXPRESSION_PATTERN, Pattern.CASE_INSENSITIVE);
		Matcher matcher = sinaPatten.matcher(ssb);
		while (matcher.find()) {
			String key = matcher.group();
			int trueStartPosition = key.lastIndexOf("[e");
			
			key = key.substring(key.lastIndexOf("[e"),key.length());
			// [f000]
			String filedname = "emoji_" + key.substring(2, key.length() - DanMuChattConfig.EXPRESSION_CODE_SUFFIX.length());
			String c = filedname;
			int a = matcher.start() + trueStartPosition;
			int b = matcher.start() + trueStartPosition + key.length();
			if (c.contains("emoji")) {  //如果是Emoji表情，即sic=emoji_2600的形式
//				if(!convertUnicode(c).equals("")){
//				}

				ssb.replace(a, b, convertUnicode(c)); // 替换[exxxx]标签
				matcher = sinaPatten.matcher(ssb);
			}
		}
		ssb.clearSpans(); // 去除所有标签
//		Log.e("convertToMsg之后", ssb.toString());

		return ssb.toString();
	}

	/**
	 * 将emo(本地表情文件名)转换成对应Emoji表情unicode编码,每个UTF-16码占两个字节 
	 * 
	 * @param emo 本地表情文件名 例如:emoji_1f1e8_1f1f3.png
	 * @return
	 */
	public static String convertUnicode(String emo) {
		String emo1 = emo;
		emo = emo.replace(DanMuChattConfig.EXPRESSION_CODE_PREFIX, "");
		emo = emo.replace(DanMuChattConfig.EXPRESSION_CODE_SUFFIX, "");
		try{
			emo = emo.substring(emo.indexOf("_") + 1);
			if (emo.length() < 6) {
				return new String(Character.toChars(Integer.parseInt(emo, 16)));
			}
			String[] emos = emo.split("_");
			char[] char0 = Character.toChars(Integer.parseInt(emos[0], 16)); //字符串("2600") -> 整型 -> UTF-16 encoded char sequence(java使用UTF-16格式存储Unicode编码)
			char[] char1 = Character.toChars(Integer.parseInt(emos[1], 16));
			char[] emoji = new char[char0.length + char1.length];
			for (int i = 0; i < char0.length; i++) {
				emoji[i] = char0[i];
			}
			for (int i = char0.length; i < emoji.length; i++) {
				emoji[i] = char1[i - char0.length];
			}
			return new String(emoji);
		}catch(Exception er){
		}
		return DanMuChattConfig.EXPRESSION_CODE_PREFIX+emo1.replace("emoji_", "")+"& ]";
	}

	/**
	 * 将EmojiParser解析出的"[e]xxxxx[/e]"代码，渲染成图片
	 * @param content
	 * @param mContext
	 * @return
	 */
	public static SpannableStringBuilder convetToHtml(String content,Context mContext) {
//		Log.e("convetToHtml", content);
		String regex = "\\[e\\](.*?)\\[/e\\]";
		Pattern pattern = Pattern.compile(regex);
		String emo = "";
		Resources resources = mContext.getResources();
		String unicode = EmojiParser.getInstance(mContext).parseEmoji(content);
		Matcher matcher = pattern.matcher(unicode);
		SpannableStringBuilder sBuilder = new SpannableStringBuilder(unicode);
		Drawable drawable = null;
		ImageSpan span = null;
		while (matcher.find()) {
			emo = matcher.group();
			try {
				int id = resources.getIdentifier(
						"emoji_"+ emo.substring(emo.indexOf("]") + 1,emo.lastIndexOf("[")), "drawable",
						mContext.getPackageName());
				if (id != 0) {
					drawable = resources.getDrawable(id);
					drawable.setBounds(0, 0, 24, 24);
					span = new ImageSpan(drawable);
					sBuilder.setSpan(span, matcher.start(), matcher.end(),Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
				}
			} catch (Exception e) {
				break;
			}
		}
		return sBuilder;
	}

	/**
	 * 过滤掉Emoji表情，转换成 [表情] 给予显示
	 * @param content
	 * @param mContext
	 * @return
	 */
	public static String convetToNotice(String content, Context mContext) {
		return EmojiParser.getInstance(mContext).convertEmoji(content);
	}

	/**
	 * 过滤掉<img>标签，转换成一个“心”表情给予显示
	 * @param content
	 * @param mContext
	 * @return
	 */
	public static SpannableStringBuilder convetToHeart(SpannableStringBuilder content, final Context mContext) {
		
		ImageGetter imageGetter = new ImageGetter() {
			public Drawable getDrawable(String source) {
				Drawable d = mContext.getResources().getDrawable(
						R.drawable.emoji_1f55a);
				d.setBounds(0, 0, 24, 24);
				return d;
			}
		};
		CharSequence cs1 = Html.fromHtml("<img src=\"heart\"/>", imageGetter,null);
		content.append(cs1);
		return content;
	}

	/**
	 * 转换表情名称(未实现)
	 * @param content
	 * @param mContext
	 * @return
	 */
	public static CharSequence convetToEmo(String content, Context mContext) {
//		if (content == null) {
//			return "表情";
//		}
//		try {
//			int index = Integer.parseInt(content.substring(content.indexOf("/") + 1, content.indexOf(".")));
//			String[] emos = mContext.getResources().getStringArray(R.array.nn_emo);
//			return emos[index - 1];
//		} catch (Exception e) {
//			return "表情";
//		}
		
		return "表情";
	}
}
