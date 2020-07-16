package com.dx.mobile.risk.supplier.vivo.internal;

import android.content.Context;
import android.database.Cursor;
import android.net.Uri;

public class VivoContentResolver {

    private Context _ctx; // a

    public VivoContentResolver(Context context) {
        _ctx = context;
    }

    public String getValue(int type, String str) {

        Uri parse;
        String str2 = null;

        switch (type) {
            case 0:
                parse = Uri.parse("content://com.vivo.vms.IdProvider/IdentifierId/OAID");
                break;
            case 1:
                parse = Uri.parse("content://com.vivo.vms.IdProvider/IdentifierId/VAID_" + str);
                break;
            case 2:
                parse = Uri.parse("content://com.vivo.vms.IdProvider/IdentifierId/AAID_" + str);
                break;
            default:
                parse = null;
                break;
        }

        Cursor query = _ctx.getContentResolver().query(parse, null, null, null, null);

        if (query != null) {
            if (query.moveToNext()) {
                str2 = query.getString(query.getColumnIndex("value"));
            }
            query.close();

        } else {
        }

        return str2;
    }
}
