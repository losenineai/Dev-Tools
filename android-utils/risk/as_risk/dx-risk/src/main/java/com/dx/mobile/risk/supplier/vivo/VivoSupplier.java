package com.dx.mobile.risk.supplier.vivo;

import android.content.Context;

import com.dx.mobile.risk.supplier.ISupplier;
import com.dx.mobile.risk.supplier.MobileType;
import com.dx.mobile.risk.supplier.SupplierListener;
import com.dx.mobile.risk.supplier.vivo.internal.VivoId;


public class VivoSupplier implements ISupplier {

    private String _appId = "";
    private Context _ctx;

    public VivoSupplier(Context context) {
        this._ctx = context;
    }

    public void setAppId(String appId) {
        this._appId = appId;
    }

    public String getAAID() {
        String aaid = VivoId.getAAID(_ctx, _appId);
        return aaid;
    }

    public String getOAID() {
        String str = VivoId.getOAID(_ctx);
        return str == null ? "" : str;
    }

    public String getUDID() {
        return "";
    }

    public String getVAID() {
        String str = VivoId.getVAID(_ctx, _appId);
        return str == null ? "" : str;
    }

    @Override
    public MobileType.MobileEnum getType() {
        return MobileType.MobileEnum.VIVO;
    }

    public boolean isSupported() {
        return VivoId.isSupport(_ctx);
    }

    public void shutDown() {
    }


    @Override
    public void setListener(SupplierListener supplierListener) {

    }

    @Override
    public boolean b() {
        return true;
    }
}
