package com.dx.mobile.risk.supplier.oppo;

import android.content.Context;

import com.dx.mobile.risk.supplier.ISupplier;
import com.dx.mobile.risk.supplier.MobileType;
import com.dx.mobile.risk.supplier.SupplierListener;

public class OppoSupplier implements ISupplier {

    private Context _ctx;

    public OppoSupplier(Context context) {
        OppoId.init(context);
        _ctx = context;
    }

    @Override
    public void setListener(final SupplierListener supplierListener) {

        new Thread(new Runnable() {
            public void run() {
                try {
                    Thread.sleep(1000);
                    if (supplierListener != null) {
                        supplierListener.OnSupport(isSupported(), OppoSupplier.this);
                    }
                } catch (Exception e) {
                }
            }
        }).start();
    }

    @Override
    public boolean b() {
        return false;
    }


    @Override
    public MobileType.MobileEnum getType() {
        return MobileType.MobileEnum.OPPO;
    }

    @Override
    public String getAAID() {

        String id;

        try {

            id = OppoId.getAUID(_ctx);

            if (id != null) {
                return id;
            }
        } catch (Exception e) {
            id = "";
        }

        return id;
    }

    @Override
    public String getOAID() {

        String id;

        try {

            id = OppoId.getOUID(_ctx);

            if (id != null) {
                return id;
            }
        } catch (Exception e) {
            id = "";
        }

        return id;

    }

    @Override
    public String getUDID() {
        return "";
    }

    @Override
    public String getVAID() {

        String id;

        try {

            id = OppoId.getDUID(_ctx);

            if (id != null) {
                return id;
            }
        } catch (Exception e) {
            id = "";
        }

        return id;
    }

    @Override
    public boolean isSupported() {
        return OppoId.isSupported();
    }

    @Override
    public void shutDown() {

    }
}
