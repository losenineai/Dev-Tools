package com.xxlib.utils;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;

import com.xxlib.config.DanMuChattConfig;


import android.content.Context;
import android.util.Log;
/**
 * Emoji表情过滤解析器,要点
 * 1.本地表情文件存储名结构：emoji_2600.png 或者 emoji_0034_2013.png 形式
 * 2.映射表以XML文件格式存储在assets文件夹
 * 3.java使用UTF-16格式存储Unicode编码,1个Char表示一个字符=2个Byte即2个字节
 * 4.一个完整的Unicode字符叫"代码点/CodePoint"，而1个Java char叫“代码单元/code unit"。 
 * 5.一个 代码点/CodePoint，可能需要一个也可能需要两个 代码单元/code unit表示
 * 
 *
 */
public class EmojiParser {
	
	private EmojiParser(Context mContext) {
		readMap(mContext);
	}

	/**存 unicode编码 和 对应string字符 的映射。因本地文件名为小写，映射表里保存小写**/
	private HashMap<List<Integer>, String> convertMap = new HashMap<List<Integer>, String>();
	
	/**emoji表情分组映射。因本地文件名为小写，映射表里保存小写**/
	private HashMap<String, ArrayList<String>> emoMap = new HashMap<String, ArrayList<String>>();
	private static EmojiParser mParser;

	public static EmojiParser getInstance(Context mContext) {
		if (mParser == null) {
			mParser = new EmojiParser(mContext);
		}
		return mParser;
	}

	/**
	 * 获取表情分组映射表
	 * @return
	 */
	public HashMap<String, ArrayList<String>> getEmoMap() {
		return emoMap;
	}

	/**
	 * 构造所需映射表：convertMap（<List<Integer>, String>）  、
	 * emoMap（HashMap<String, ArrayList<String>>）
	 * @param mContext
	 */
	public void readMap(Context mContext) {
		if (convertMap == null || convertMap.size() == 0) {
			convertMap = new HashMap<List<Integer>, String>();
			XmlPullParser xmlpull = null;
			String fromAttr = null;
			String key = null;
			ArrayList<String> emos = null;
			try {
				XmlPullParserFactory xppf = XmlPullParserFactory.newInstance();
				xmlpull = xppf.newPullParser();
				InputStream stream = mContext.getAssets().open("emojiMap.xml");
				xmlpull.setInput(stream, "UTF-8");
				int eventCode = xmlpull.getEventType();
				while (eventCode != XmlPullParser.END_DOCUMENT) {
					switch (eventCode) {
					case XmlPullParser.START_DOCUMENT: {
						break;
					}
					case XmlPullParser.START_TAG: {
						if (xmlpull.getName().equals("key")) {
							emos = new ArrayList<String>();
							key = xmlpull.nextText();
						}
						if (xmlpull.getName().equals("e")) {
							fromAttr = xmlpull.nextText();
							emos.add(fromAttr.toLowerCase());//因本地文件名为小写，映射表里保存小写
							List<Integer> fromCodePoints = new ArrayList<Integer>();
							if (fromAttr.length() > 6) {  //如果emoji表情是两个unicode组成的
								String[] froms = fromAttr.split("\\_");
								for (String part : froms) {
									fromCodePoints.add(Integer.parseInt(part,
											16));
								}
							} else {
								fromCodePoints.add(Integer.parseInt(fromAttr,
										16));
							}
//							System.out.println("add one emoji unicode ="+fromCodePoints);
							convertMap.put(fromCodePoints, fromAttr.toLowerCase()); //因本地文件名为小写，映射表里保存小写
						}
						break;
					}
					case XmlPullParser.END_TAG: {
						if (xmlpull.getName().equals("dict")) {
							System.out.println("add one key  ="+key);
							emoMap.put(key, emos); //因本地文件名为小写，映射表里保存小写
						}
						break;
					}
					case XmlPullParser.END_DOCUMENT: {
//						Log.i("EmojiParser", "parse emoji complete");
						break;
					}
					}
					eventCode = xmlpull.next();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	/**
	 * 解析字符串，将Emoji表情编码转换成"[exxxxx&e]"的形式，供下一步解析使用，但是要本地emoji编码表支持的才可以转换，不包含的将不做处理
	 * @param input
	 * @return
	 */
	public String parseEmoji(String input) {
		if (input == null || input.length() <= 0) {
			return "";
		}
		
		StringBuilder result = new StringBuilder();
		int[] codePoints = toCodePointArray(input);
		List<Integer> key = null;
		for (int i = 0; i < codePoints.length; i++) {
			key = new ArrayList<Integer>();
			
			//尝试检查emoji表情是否为两个unicode码组成的
			if (i + 1 < codePoints.length) {
				key.add(codePoints[i]);
				key.add(codePoints[i + 1]);
				if (convertMap.containsKey(key)) {  
					String value = convertMap.get(key);
					if (value != null) {
						result.append(DanMuChattConfig.EXPRESSION_CODE_PREFIX + value + DanMuChattConfig.EXPRESSION_CODE_SUFFIX);
					}
					i++;
					continue;
				}
			}
			
			//如果emoji表情是单个unicode组成的
			key.clear();
			key.add(codePoints[i]);
			if (convertMap.containsKey(key)) {
				String value = convertMap.get(key);
				if (value != null) {
					result.append(DanMuChattConfig.EXPRESSION_CODE_PREFIX + value + DanMuChattConfig.EXPRESSION_CODE_SUFFIX);
				}
				continue;
			}
			
			result.append(Character.toChars(codePoints[i])); //如果不是表情，则直接转换成字符加入
		}
		return result.toString();
	}

	/**
	 * 获取 str 转换成 Unicode code 格式编码集,java中使用UTF-16来保存unicode编码
	 * 
	 * @param str
	 * @return
	 */
	private int[] toCodePointArray(String str) {
		char[] ach = str.toCharArray();
		int len = ach.length;
		int[] acp = new int[Character.codePointCount(ach, 0, len)];//计算char数组中unicode码数量，1个unicode代码点可包含1-2个代码单元
		int j = 0;
		for (int i = 0, cp; i < len; i += Character.charCount(cp)) { //Character.charCount(cp)根据cp的unicode值来确定需要多少个char来表示
			cp = Character.codePointAt(ach, i); //获取 每个代码点 的值
			acp[j++] = cp;
		}
		return acp;
	}

	/**
	 * 转换字符串，将Emoji表情编码转换成"[表情]",即对表情不给予显示
	 * @param input
	 * @return
	 */
	public String convertEmoji(String input) {
		if (input == null || input.length() <= 0) {
			return "";
		}
		StringBuilder result = new StringBuilder();
		int[] codePoints = toCodePointArray(input);
		List<Integer> key = null;
		for (int i = 0; i < codePoints.length; i++) {
			key = new ArrayList<Integer>();
			if (i + 1 < codePoints.length) {
				key.add(codePoints[i]);
				key.add(codePoints[i + 1]);
				if (convertMap.containsKey(key)) {
					String value = convertMap.get(key);
					if (value != null) {
						result.append("[表情]");
					}
					i++;
					continue;
				}
			}
			key.clear();
			key.add(codePoints[i]);
			if (convertMap.containsKey(key)) {
				String value = convertMap.get(key);
				if (value != null) {
					result.append("[表情]");
				}
				continue;
			}
			
			result.append(Character.toChars(codePoints[i])); //如果不是表情，则直接转换成字符加入
		}
		return result.toString();
	}
}
