package com.dx.mobile.risk.supplier;

public interface ISupplier extends IdSupplier {

    // void a(SupplierListener supplierListener);
    void setListener(SupplierListener supplierListener);

    boolean b();
}