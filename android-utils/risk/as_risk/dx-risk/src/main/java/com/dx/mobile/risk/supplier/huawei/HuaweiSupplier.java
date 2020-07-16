package com.dx.mobile.risk.supplier.huawei;

import android.content.Context;

import com.dx.mobile.risk.supplier.ISupplier;
import com.dx.mobile.risk.supplier.MobileType;
import com.dx.mobile.risk.supplier.SupplierListener;


public class HuaweiSupplier implements ISupplier {

    private Context _ctx;

    private String b = "";
    private String _oaid = "";  // c
    private String _vaid = "";  // d
    private String _aaid = "";  // e

    public HuaweiSupplier(Context context) {
        this._ctx = context;

        initData();
    }

    private void initData(){
        try {
            AdvertisingIdClient.Info advertisingIdInfo = AdvertisingIdClient.getAdvertisingIdInfo(_ctx);
            _oaid = advertisingIdInfo.getId();
            advertisingIdInfo.isLimitAdTrackingEnabled();
        } catch (Throwable e) {
        }
    }

    @Override
    public MobileType.MobileEnum getType() {
        return MobileType.MobileEnum.HUA_WEI;
    }

    public void setListener(final SupplierListener supplierListener) {

//        new Thread(new Runnable() {
//            @Override
//            public void run() {
//
//                try {
//                    AdvertisingIdClient.Info advertisingIdInfo = AdvertisingIdClient.getAdvertisingIdInfo(_ctx);
//                    _oaid = advertisingIdInfo.getId();
//                    advertisingIdInfo.isLimitAdTrackingEnabled();
//
//                    if (TextUtils.isEmpty(_oaid)) {
//                        if (supplierListener != null) {
//                            supplierListener.OnSupport(false, null);
//                        }
//                    } else if (supplierListener != null) {
//                        supplierListener.OnSupport(true, HuaweiSupplier.this);
//                    }
//
//
//                } catch (Exception e) {
//                    e.printStackTrace();
//                    if (TextUtils.isEmpty(_oaid)) {
//                        if (supplierListener != null) {
//                            supplierListener.OnSupport(false, null);
//                        }
//                    } else if (supplierListener != null) {
//                        supplierListener.OnSupport(true, HuaweiSupplier.this);
//                    }
//                } catch (Throwable th) {
//                    if (TextUtils.isEmpty(_oaid)) {
//                        if (supplierListener != null) {
//                            supplierListener.OnSupport(false, null);
//                        }
//                    } else if (supplierListener != null) {
//                        supplierListener.OnSupport(true, HuaweiSupplier.this);
//                    }
//                }
//
//            }
//        }).start();
    }

    @Override
    public boolean b() {
        return false;
    }

    @Override
    public String getAAID() {
        return _aaid;
    }

    @Override
    public String getOAID() {
        return _oaid;
    }

    @Override
    public String getUDID() {
        return "";
    }

    @Override
    public String getVAID() {
        return _vaid;
    }

    @Override
    public boolean isSupported() {
        return false;
    }

    @Override
    public void shutDown() {

    }
}
