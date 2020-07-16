// IDeviceidInterface.aidl
package com.zui.deviceidservice;

// Declare any non-default types here with import statements
// lenovo

interface IDeviceidInterface {

    boolean isSupported();

    String getUDID();

    String getOAID();

    String getVAID(String packageName);

    String getAAID(String packageName);
}
