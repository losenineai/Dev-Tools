package com.xxlib.utils;

import android.content.Context;
import android.graphics.Bitmap;
import android.text.Spannable;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.SpannedString;
import android.text.TextUtils;
import android.text.style.ImageSpan;

import com.xxlib.config.DanMuChattConfig;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ExpressionUtil {

    private static final String TAG = "ExpressionUtil";
    private static HashMap<String, Integer> mResourcesMap;
    public static final Pattern sinaPatten = Pattern.compile(DanMuChattConfig.EXPRESSION_PATTERN,
            Pattern.CASE_INSENSITIVE);

    /**
     * 处理表情，符合正则匹配
     *
     * @param context         上下文
     * @param spannableString 含有编码的字符串
     * @param patten          正则匹配模式
     * @throws SecurityException
     * @throws NoSuchFieldException
     * @throws NumberFormatException
     * @throws IllegalArgumentException
     * @throws IllegalAccessException
     */
    private static void dealExpression(Context context, SpannableString spannableString, Pattern patten, int lineHeight)
            throws SecurityException, NoSuchFieldException, NumberFormatException, IllegalArgumentException,
            IllegalAccessException {
        Matcher matcher = patten.matcher(spannableString);

        while (matcher.find()) {
            String key = matcher.group();
            int trueStartPosition = key.lastIndexOf("[e");
            key = key.substring(key.lastIndexOf("[e"), key.length());

            String name = key.substring(2, key.length() - DanMuChattConfig.EXPRESSION_CODE_SUFFIX.length());
            int resId = getImageResId(context, name);
            if (resId != 0) {
                ImageSpan imageSpan = PictureTool.getEmojiImageSpan(context, resId, lineHeight);
                int end = matcher.start() + trueStartPosition + key.length();
                spannableString.setSpan(imageSpan, matcher.start() + trueStartPosition, end, Spannable.SPAN_INCLUSIVE_EXCLUSIVE);

            }
        }
    }

    /**
     * 解析获得含有emoji表情的字符串，返回富文本的字符串
     *
     * @param context 上下文
     * @param str     含有emoji编码的源字符串
     * @return 返回富文本的字符串
     */
    public static SpannableString getExpressionString(Context context, String str) {
        return getExpressionString(context, str, 0);
    }

    /**
     * 解析获得含有emoji表情的字符串，返回富文本的字符串
     *
     * @param context 上下文
     * @param str     含有emoji编码的源字符串
     * @return 返回富文本的字符串
     */
    public static SpannableString getExpressionString(Context context, String str, int lineHeight) {
        SpannableString spannableString = new SpannableString(str);
        getExpressionString(context, spannableString, lineHeight);
        return spannableString;
    }

    /**
     * 解析获得含有emoji表情的字符串，返回富文本的字符串
     *
     * @param context 上下文
     * @param spannableString     含有emoji编码的源字符串
     * @return 返回富文本的字符串
     */
    public static SpannableString getExpressionString(Context context, SpannableString spannableString) {
        return getExpressionString(context, spannableString, 0);
    }

    /**
     * 解析获得含有emoji表情的字符串，返回富文本的字符串
     *
     * @param context 上下文
     * @param spannableString     含有emoji编码的源字符串
     * @param lineHeight 用于指定表情图片的大小,一般为EditText或者TextView的行高
     * @return 返回富文本的字符串
     */
    public static SpannableString getExpressionString(Context context, SpannableString spannableString, int lineHeight) {
        Pattern sinaPatten = Pattern.compile(DanMuChattConfig.EXPRESSION_PATTERN, Pattern.CASE_INSENSITIVE);
        try {
            dealExpression(context, spannableString, sinaPatten, lineHeight);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return spannableString;
    }

    /**
     * 解析获得含有emoji表情的字符串，返回富文本的字符串
     *
     * @param context 上下文
     * @param str     含有emoji编码的源字符串
     * @return 返回富文本的字符串
     */
    public static SpannableString getExpressionString(Context context, Spanned str) {
        return getExpressionString(context, str, 0);
    }

    /**
     * 解析获得含有emoji表情的字符串，返回富文本的字符串
     *
     * @param context 上下文
     * @param str     含有emoji编码的源字符串
     * @param lineHeight 用于指定表情图片的大小,一般为EditText或者TextView的行高
     * @return 返回富文本的字符串
     */
    public static SpannableString getExpressionString(Context context, Spanned str, int lineHeight) {
        SpannableString spannableString = new SpannableString(str);
        Pattern sinaPatten = Pattern.compile(DanMuChattConfig.EXPRESSION_PATTERN, Pattern.CASE_INSENSITIVE);
        try {
            dealExpression(context, spannableString, sinaPatten, lineHeight);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return spannableString;
    }

    /**
     * 通过图片名称，获取显示emoji的字符串
     *
     * @param context context对象
     * @param imageName 图片的文件名不含扩展名
     * @return 含有图片ImageSpan的表情文字
     */
    public static SpannableString getExpStrByImageName(Context context, String imageName) {
        return getExpStrByImageName(context, imageName, 0);
    }

    /**
     * 通过图片名称，获取显示emoji的字符串并自定义表情图片大小
     *
     * @param context context对象
     * @param imageName 图片的文件名不含扩展名
     * @param lineHeight 用于指定表情图片的大小,一般为EditText或者TextView的行高
     * @return 含有图片ImageSpan的表情文字
     */
    public static SpannableString getExpStrByImageName(Context context, String imageName, int lineHeight) {
        if (context == null || TextUtils.isEmpty(imageName)) {
            return new SpannableString("");
        }

        ImageSpan imageSpan = PictureTool.getEmojiImageSpan(context, getImageResId(context, imageName), lineHeight);
        String string = DanMuChattConfig.EXPRESSION_CODE_PREFIX + imageName + DanMuChattConfig.EXPRESSION_CODE_SUFFIX;

        SpannableString spannableString = new SpannableString(string);
        spannableString.setSpan(imageSpan, 0, string.length(), SpannedString.SPAN_EXCLUSIVE_EXCLUSIVE);
        return spannableString;
    }

    /**
     * 获得删除长度，普通字符默认删除一个，emoji编码默认删除一个表情编码
     *
     * @param str 字符串
     * @return 长度
     */
    public static int getDeleteLength(String str) {
        if (TextUtils.isEmpty(str)) {
            return 0;
        }

        int emojiLength = DanMuChattConfig.EXPRESSION_CODE_PREFIX.length()
                + DanMuChattConfig.EXPRESSION_CODE_SUFFIX.length() + 3;

        if (str.length() < emojiLength) {
            return 1;
        }

        String imageStr = str.substring(str.length() - emojiLength);

        if (imageStr.matches(DanMuChattConfig.EXPRESSION_PATTERN)) {
            return emojiLength;
        }

        return 1;
    }

    /**
     * 获取emoji表情文件名的列表
     *
     * @return 表情列表
     */
    public static List<String> getExpressNameList(Context context) {
        List<String> list = new ArrayList<>();
        String fieldName;

        for (int i = 1; i <= DanMuChattConfig.MAX_EXPRESSION_COUNT; i++) {
            if (i < 10) {
                fieldName = "e00" + i;
            } else if (i < 100) {
                fieldName = "e0" + i;
            } else {
                fieldName = "e" + i;
            }
            list.add(fieldName);
        }
        HashMap<String, ArrayList<String>> mEmojiMap;
        mEmojiMap = EmojiParser.getInstance(context).getEmoMap();

        List<String> emojiGroup = new ArrayList<>();
        for (String emoji : mEmojiMap.get("people")) {
            emojiGroup.add(emoji);
        }
        return emojiGroup;
    }

    /**
     * 通过图片文件名，获得资源id
     *
     * @param name 图片资源文件名
     * @return 资源的id
     */
    public static int getImageResId(Context context, String name) {
        name = "emoji_" + name;
        if (mResourcesMap == null)
            mResourcesMap = new HashMap<>();
        int resId;
        if (mResourcesMap.get(name) != null && mResourcesMap.get(name) != 0)
            resId = mResourcesMap.get(name);
        else {
            resId = context.getResources().getIdentifier(name, "drawable", context.getPackageName());
            if (resId == 0) { // 这里获取不到的要么就是sdk的,要么是真的没有的
                resId = context.getResources().getIdentifier(name, "drawable", "com.flamingo.sdk");
            }
            mResourcesMap.put(name, resId);
        }
        return resId;
    }

    public static Bitmap getExpBitmap(Context context, int resId) {
        return PictureTool.getEmojiBitmap(context, resId, 0);
    }

    /**============================start xmod 迁移过来的方法================================**/
    public static boolean containEmoji(CharSequence cs) {
        Matcher matcher = sinaPatten.matcher(cs);
        return matcher.find();
    }

    public static int getCustomExpressionCode(int tab, int index) {
        return ((tab & 0xffff) << 16) | ((index + 1) & 0xffff);
    }
    /**============================end xmod 迁移过来的方法================================**/

}
