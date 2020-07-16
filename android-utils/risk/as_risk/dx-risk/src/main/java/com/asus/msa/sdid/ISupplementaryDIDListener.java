package com.asus.msa.sdid;

import android.os.IInterface;

import com.asus.msa.SupplementaryDID.IDidAidlInterface;

public interface ISupplementaryDIDListener extends IInterface {

    // a
    void handle() ;

    // a
    void handle(IDidAidlInterface service) throws android.os.RemoteException;
}
