package com.dx.mobile.risk.supplier;

public interface IdSupplier {

    MobileType.MobileEnum getType();

    String getAAID();

    String getOAID();

    String getUDID();

    String getVAID();

    boolean isSupported();

    void shutDown();
}
