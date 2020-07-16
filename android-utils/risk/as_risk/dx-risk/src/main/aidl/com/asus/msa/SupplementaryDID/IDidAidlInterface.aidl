// IDidAidlInterface.aidl
package com.asus.msa.SupplementaryDID;

// Declare any non-default types here with import statements

interface IDidAidlInterface {

    boolean isSupported();

    String getUDID();

    String getOAID();

    String getVAID();

    String getAAID();
}
