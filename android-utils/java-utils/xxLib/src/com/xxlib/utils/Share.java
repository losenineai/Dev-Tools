package com.xxlib.utils;

import android.content.Context;
import android.content.Intent;

import com.xxlib.R;


/**
 * 分享工具类
 * 目前是客户端分享
 * @author linbin
 * 2014
 */
public class Share {

	public static void client (Context context)
	{
		
		Intent shareIntent = new Intent();
		shareIntent.setAction(Intent.ACTION_SEND);
		shareIntent.setType("text/plain");
		// 需要指定意图的数据类型
		shareIntent.putExtra(Intent.EXTRA_SUBJECT, context.getResources()
				.getString(R.string.share));
		shareIntent.putExtra(Intent.EXTRA_TEXT, context.getResources()
				.getString(R.string.announce));
		shareIntent = Intent.createChooser(shareIntent, context.getResources()
				.getString(R.string.share));
		context.startActivity(shareIntent);
		
//		Intent it = new Intent(Intent.ACTION_SEND);
//        it.setType("text/plain");
//        List<ResolveInfo> resInfo = context.getPackageManager().queryIntentActivities(it, 0);
//        if (!resInfo.isEmpty()) {
//            List<Intent> targetedShareIntents = new ArrayList<Intent>();
//            for (ResolveInfo info : resInfo) {
//                Intent targeted = new Intent(Intent.ACTION_SEND);
//                targeted.setType("text/plain");
//                ActivityInfo activityInfo = info.activityInfo;
//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
//
//                if (activityInfo.packageName.contains("qihoo") || activityInfo.name.contains("手机助手")) {
//                    continue;
//                }
//                if (activityInfo.packageName.contains("gm") || activityInfo.name.contains("mail")) {
//                    targeted.putExtra(Intent.EXTRA_TEXT, context.getResources()
//            				.getString(R.string.announce));
//                } else if (activityInfo.packageName.contains("zxing")) {
//                    targeted.putExtra(Intent.EXTRA_TEXT, context.getResources()
//            				.getString(R.string.announce));
//                } else {
//                    targeted.putExtra(Intent.EXTRA_TEXT, context.getResources()
//            				.getString(R.string.announce));
//                }
//                
//                targeted.putExtra(Intent.EXTRA_SUBJECT, context.getResources()
//        				.getString(R.string.share));
//                targeted.putExtra(Intent.EXTRA_TEXT, context.getResources()
//        				.getString(R.string.announce));
//                		
//                targeted.setPackage(activityInfo.packageName);
//                targetedShareIntents.add(targeted);
//            }
//            Intent chooserIntent = Intent.createChooser(targetedShareIntents.remove(0), "Select app to share");
//            if (chooserIntent == null) {
//                return;
//            }
//
//            chooserIntent.putExtra(Intent.EXTRA_INITIAL_INTENTS, targetedShareIntents.toArray(new Parcelable[] {}));
//            try {
//                context.startActivity(chooserIntent);
//            } catch (android.content.ActivityNotFoundException ex) {
//
//            }
//	}
	}
}
