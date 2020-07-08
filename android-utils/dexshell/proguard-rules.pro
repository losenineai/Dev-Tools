# Add project specific ProGuard rules here.
# You can control the set of applied configuration files using the
# proguardFiles setting in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}

# Uncomment this to preserve the line number information for
# debugging stack traces.
#-keepattributes SourceFile,LineNumberTable

# If you keep the line number information, uncomment this to
# hide the original source file name.
#-renamesourcefileattribute SourceFile

-ignorewarnings
-dontshrink
# this should different from j2c_dxvmp to advoid rename
-repackageclasses '_se_'
-flattenpackagehierarchy 'com.security.mobile.appen'
-allowaccessmodification
-printseeds


-keep class com.security.mobile.annotation.* {*;}
-keep class com.security.shell.AppStub* {
    public <methods>;
    protected <methods>;
    private void _attachBaseContext(android.content.Context);
}

-keep class com.keep.DebugUtils {
    public <methods>;
    native <methods>;
}

-keep class com.security.shell.JNITools {
*;
}

-keep class com.security.shell.ShellSupporter {
*;
}

