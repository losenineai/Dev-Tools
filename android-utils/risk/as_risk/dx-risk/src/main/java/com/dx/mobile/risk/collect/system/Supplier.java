package com.dx.mobile.risk.collect.system;

import android.content.Context;
import android.text.TextUtils;

import com.dx.mobile.risk.data.JNIConst;
import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.JSONUtils;
import com.dx.mobile.risk.supplier.IdSupplier;
import com.dx.mobile.risk.supplier.huawei.HuaweiSupplier;
import com.dx.mobile.risk.supplier.vivo.VivoSupplier;
import com.dx.mobile.risk.supplier.xiaomi.XiaomiSupplier;

import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

/**
 * Time: 2019-11-01
 * Author: blue
 * Description:
 */
public class Supplier {

    /**
     * 获取厂商特有的ids
     * @return
     */
    public static String getSupplierIds() {
        Context ctx = ContextFinder.getApplication();

        try {
            JSONObject action_info = new JSONObject();

            List<IdSupplier> list = new ArrayList<>();
            // 全版本
            list.add(new HuaweiSupplier(ctx));
            // FuntouchOS 9
            list.add(new VivoSupplier(ctx));
            // MIUI10.2及以上
            list.add(new XiaomiSupplier(ctx));
            // Android Q 才可获取
//            list.add(new AsusSupplier(ctx, listener));
            // ZUI 11.4及以上才可获取
//            list.add(new LenovoSupplier(ctx, listener));
            // Color OS 7.0 及以上才可获取
//            list.add(new OppoSupplier(ctx));

            String udid = null;
            String oaid = null;
            String vaid = null;
            String aaid = null;

            for (IdSupplier supplier : list) {
//                Log.d(TAG, "supplier:"+supplier.getType().name() + " aaid:"+supplier.getAAID()+" oaid:"+supplier.getOAID() + " udid:"+supplier.getUDID()+" vaid:"+supplier.getVAID());

                if (!TextUtils.isEmpty(supplier.getAAID())) {
                    aaid = supplier.getAAID();
                }
                if (!TextUtils.isEmpty(supplier.getOAID())) {
                    oaid = supplier.getOAID();
                }
                if (!TextUtils.isEmpty(supplier.getUDID())) {
                    udid = supplier.getUDID();
                }
                if (!TextUtils.isEmpty(supplier.getVAID())) {
                    vaid = supplier.getVAID();
                }
            }

            if (!TextUtils.isEmpty(udid)) {
                JSONUtils.jsonPut(action_info, "K2100", udid);
            }
            if (!TextUtils.isEmpty(oaid)) {
                JSONUtils.jsonPut(action_info, "K2101", oaid);
            }
            if (!TextUtils.isEmpty(vaid)) {
                JSONUtils.jsonPut(action_info, "K2102", vaid);
            }
            if (!TextUtils.isEmpty(aaid)) {
                JSONUtils.jsonPut(action_info, "K2103", aaid);
            }
            JSONUtils.jsonPut(action_info, JNIConst.KEY_APP_KOCK_INDEX15, JNIConst.VALUE_APP_KOCK);
//            Log.d(TAG, "final aaid:"+aaid+" oaid:"+oaid+" udid:"+udid+" vaid:"+vaid);

            return action_info.toString();

        } catch (Throwable t) {
        }

        return "";
    }
}
