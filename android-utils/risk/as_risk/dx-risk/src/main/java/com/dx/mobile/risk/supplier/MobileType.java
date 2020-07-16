package com.dx.mobile.risk.supplier;

import android.text.TextUtils;

public class MobileType {

    public enum MobileEnum {

        UNSUPPORT(-1, "unsupport"),
        HUA_WEI(0, "HUAWEI"),
        XIAOMI(1, "Xiaomi"),
        VIVO(2, "vivo"),
        OPPO(3, "oppo"),
        MOTO(4, "motorola"),
        LENOVO(5, "lenovo"),
        ASUS(6, "asus");

        private int index;
        private String value;

        private MobileEnum(int index, String value) {
            this.index = index;
            this.value = value;
        }

        public static MobileEnum getCurMobileType(String manufacturer) {

            if (TextUtils.isEmpty(manufacturer)) {
                return UNSUPPORT;
            }

            for (MobileEnum var : values()) {
                if (var.value.equalsIgnoreCase(manufacturer)) {
                    return var;
                }
            }
            return UNSUPPORT;
        }
    }
}
