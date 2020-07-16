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
-repackageclasses 'com.dx.mobile.risk.dx'
#-flattenpackagehierarchy 'com.dx.mobile.risk'
-allowaccessmodification

-keep class com.dx.mobile.risk.dx.* {*;}
-keep class com.dx.mobile.risk.* {*;}
-keep class com.dx.mobile.risk.exp.* {*;}

-keep class com.dx.mobile.annotation.JXC_RISK {*;}

# keep native方法及其所属类
-keepclasseswithmembernames class * {
    native <methods>;
}

# 解决和 okhttp 冲突问题
-keepclassmembers class * extends javax.net.ssl.SSLSocketFactory{
    private javax.net.ssl.SSLSocketFactory delegate;
}