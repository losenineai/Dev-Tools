# make apk debuggable

## environment
- root 手机
- mprop 工具


## 常用命令
- 查看系统各属性内容：`getprop`
- 查看当前`package`是否具备`debuggable`属性：`run-as packagename`
- 查看当前所有`package`列表：`pm list packages -f`
-

## 令系统具备debuggable属性
流程：
解压mprop.zip文件，将里面的mprop工具push到手机，并运行，具体命令如下：

- adb push mprop/armeabi-v7a/mprop /data/local/tmp/
- adb shell
- cd /data/local/tmp
- chmod 755 mprop
- ./mprop ro.debuggable 1

注：该设置在手机重启后将会失效！！！