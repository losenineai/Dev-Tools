package com.dx.mobile.risk.supplier.lenovo;

import android.content.Context;

import com.dx.mobile.risk.supplier.ISupplier;
import com.dx.mobile.risk.supplier.MobileType;
import com.dx.mobile.risk.supplier.SupplierListener;


public class LenovoSupplier implements OpenDeviceId.ILSupplier, ISupplier {

    private OpenDeviceId _deviceId; // a
    private SupplierListener _listener; // b

    public LenovoSupplier(Context context, SupplierListener supplierListener) {
        _listener = supplierListener;
        _deviceId = new OpenDeviceId(context, this);
    }

    public void handle(OpenDeviceId deviceId) {

        if (_listener != null) {
            _listener.OnSupport(isSupported(), this);
        }

    }


    @Override
    public MobileType.MobileEnum getType() {
        return MobileType.MobileEnum.LENOVO;
    }

    @Override
    public void setListener(SupplierListener supplierListener) {
    }

    public boolean b() {
        return false;
    }

    public String getAAID() {

        if (!isSupported()) {
            return "";
        }

        String e = _deviceId.getAAID();
        return e == null ? "" : e;
    }

    public String getOAID() {

        if (!isSupported()) {
            return "";
        }

        String a = _deviceId.getOAID();
        return a == null ? "" : a;
    }

    public String getUDID() {

        if (!isSupported()) {
            return "";
        }

        String b = _deviceId.getUDID();
        return b == null ? "" : b;
    }

    public String getVAID() {

        if (!isSupported()) {
            return "";
        }

        String d = _deviceId.getVAID();
        return d == null ? "" : d;
    }

    public boolean isSupported() {
        return _deviceId != null ? _deviceId.isSupported() : false;
    }

    public void shutDown() {
        if (_deviceId != null) {
            _deviceId.unbind();
        }
    }
}
