package com.dx.mobile.risk.supplier.xiaomi;

import android.content.Context;

import com.dx.mobile.risk.supplier.ISupplier;
import com.dx.mobile.risk.supplier.MobileType;
import com.dx.mobile.risk.supplier.SupplierListener;

public class XiaomiSupplier implements ISupplier {

    private Context _ctx;

    public XiaomiSupplier(Context context) {
        this._ctx = context;
    }

    public String getAAID() {
        String aaid = XiaomiHelper.getAAID(this._ctx);
        return aaid;
    }

    public String getOAID() {
        String str = XiaomiHelper.getOAID(this._ctx);
        return str == null ? "" : str;
    }

    public String getUDID() {
        return "";
    }

    public String getVAID() {
        String str = XiaomiHelper.getVAID(this._ctx);
        return str == null ? "" : str;
    }

    public boolean isSupported() {
        return XiaomiHelper.isSupported();
    }

    public void shutDown() {
    }


    @Override
    public MobileType.MobileEnum getType() {
        return MobileType.MobileEnum.XIAOMI;
    }


    @Override
    public void setListener(SupplierListener supplierListener) {

    }

    @Override
    public boolean b() {
        return true;
    }
}
