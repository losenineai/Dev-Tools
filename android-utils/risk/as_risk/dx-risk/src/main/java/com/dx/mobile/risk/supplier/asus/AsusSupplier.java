package com.dx.mobile.risk.supplier.asus;

import android.content.Context;
import android.os.IBinder;

import com.asus.msa.SupplementaryDID.IDidAidlInterface;
import com.asus.msa.sdid.ISupplementaryDIDListener;
import com.dx.mobile.risk.supplier.ISupplier;
import com.dx.mobile.risk.supplier.MobileType;
import com.dx.mobile.risk.supplier.SupplierListener;

/**
 * @author white
 * @description：
 * @date 2019/10/12
 */
public class AsusSupplier implements ISupplementaryDIDListener, ISupplier {

    private SupplierListener _supplierListener;     // a
    private String _udid = "";  // b
    private String _oaid = "";  // c
    private String _vaid = "";  // d
    private String _aaid = "";  // e
    private SupplementaryDIDManager _didMannager;   // f
    private boolean g = false;
    private boolean _bSupported = false;    // h

    public AsusSupplier(Context context, SupplierListener supplierListener) {
        this._supplierListener = supplierListener;
        this._didMannager = new SupplementaryDIDManager(context);
    }

    @Override
    public MobileType.MobileEnum getType() {
        return MobileType.MobileEnum.ASUS;
    }

    public void handle() {
        if (_supplierListener != null) {
            _supplierListener.OnSupport(false, this);
        }
    }

    // public void a(com.asus.msa.a.a aVar)
    @Override
    public void handle(IDidAidlInterface service) {

        try {
            _udid = service.getUDID();
            if (_udid == null) {
                _udid = "";
            }
        } catch (Exception e) {
        }

        try {
            _oaid = service.getOAID();
            if (_oaid == null) {
                _oaid = "";
            }
        } catch (Exception e) {
        }

        try {
            _vaid = service.getVAID();
            if (_vaid == null) {
                _vaid = "";
            }
        } catch (Exception e) {
        }

        try {
            _aaid = service.getAAID();
            if (_aaid == null) {
                _aaid = "";
            }
        } catch (Exception e4) {
        }

        try {
            _bSupported = service.isSupported();
        } catch (Exception e5) {
        }

        this.g = true;
        if (_supplierListener != null) {
            _supplierListener.OnSupport(_bSupported, this);
        }
    }

    // public void a(SupplierListener supplierListener)
    @Override
    public void setListener(SupplierListener supplierListener) {
        _didMannager.init(this);
    }

    public IBinder asBinder() {
        return null;
    }

    public boolean b() {
        return false;
    }

    public String getAAID() {
        return _aaid;
    }

    public String getOAID() {
        return _oaid;
    }

    public String getUDID() {
        return _udid;
    }

    public String getVAID() {
        return _vaid;
    }

    public boolean isSupported() {
        return _bSupported;
    }

    public void shutDown() {
        if (this.g && _didMannager != null) {
            _didMannager.deInit();
        }
    }

}
