package com.dx.mobile.risk.supplier.vivo.internal;

import android.database.ContentObserver;

public class VivoContentObserver extends ContentObserver {

    private String _appId;
    private int _type;
    private VivoHelper _vvHelper;

    public VivoContentObserver(VivoHelper vvHelper, int type, String appId) {

        super(null);
        this._vvHelper = vvHelper;
        this._type = type;
        this._appId = appId;
    }

    public void onChange(boolean selfChange) {

        if (_vvHelper != null) {
            _vvHelper.getId(_type, _appId);
        } else {
        }

    }

}
