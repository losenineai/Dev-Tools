package com.auric.intell.commonlib.utils;


import android.text.TextUtils;

/**
 * @author white
 * @description：版本比较
 * @date 2016/10/14
 */

public class VersionCompareUtil {

    /**
     * 版本比较
     * @param oldVersion
     * @param newVersion
     * @return
     * <p/>1，表示oldVersion大于newVersion
     * <p/>0，表示相等
     * <p/>-1，表示oldVersion小于newVersion
     */
	public static int compareTo(String oldVersion, String newVersion) {
        String str1 = TextUtils.isEmpty(oldVersion) ? "0.0.0" : oldVersion;
        String str2 = TextUtils.isEmpty(newVersion) ? "0.0.0" : newVersion;

		str1 = str1.replaceAll("[^-+.\\d]", ".0.");
		str2 = str2.replaceAll("[^-+.\\d]", ".0.");

		if (str1.equals("")) {
			str1 = "0.0.0";
		}
		if (str2.equals("")) {
			str2 = "0.0.0";
		}

		String[] s1 = str1.split("\\.");
		String[] s2 = str2.split("\\.");

		int n1 = s1.length;
		int n2 = s2.length;

		int result = 0;
		try {
			if (n1 == n2) {
				for (int i = 0; i < n1; i++) {
					if (s1[i].length() != 0)
						if (Integer.parseInt(s1[i]) - Integer.parseInt(s2[i]) > 0) {
							result = 1;
							break;
						}
					;
					if (s1[i].length() != 0)
						if (Integer.parseInt(s1[i]) - Integer.parseInt(s2[i]) < 0) {
							result = -1;
							break;
						}
					;
				}
			}
			if (n1 < n2) {
				for (int i = 0; i < n1; i++) {
					if (s1[i].length() != 0)
						if (Integer.parseInt(s1[i]) - Integer.parseInt(s2[i]) > 0) {
							result = 1;
							break;
						}
					;
					if (s1[i].length() != 0)
						if (Integer.parseInt(s1[i]) - Integer.parseInt(s2[i]) < 0) {
							result = -1;
							break;
						}
					;
				}
			}
			if (n1 > n2) {
				for (int i = 0; i < n2; i++) {
					if (s1[i].length() != 0)
						if (Integer.parseInt(s1[i]) - Integer.parseInt(s2[i]) > 0) {
							result = 1;
							break;
						}
					;
					if (s1[i].length() != 0)
						if (Integer.parseInt(s1[i]) - Integer.parseInt(s2[i]) < 0) {
							result = -1;
							break;
						}
					;
				}
			}
		} catch (Exception e) {
			return 0;
		}


		return result;
	}
}
