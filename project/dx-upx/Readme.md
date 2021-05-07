# 说明

dx-protector是基于upx改进的加固工具，各个平台二进制文件可以直接从gitlab上下载。

# 编译

### Linux
> 初始化环境：ucl-dev zlib-dev
> 编译upx：./build-upx.sh
> 编译stub：./build-stub.sh

### WIN32

> make -C src target=cross-mingw32

### MAC OSX

> 编译upx：./build-upx.sh
> 编译stub，需要在linux的docker中进行编译。

### Support Platform

- armeabi
- armeabi-v7a
- arm64-v8a
- x86
- 其他平台


### Update 2019-01-23
===============

新增命令行参数 `--licfile`

新增命令行参数 `--fix-sdk-build`

新增命令行参数 `--replace-identify`

新增命令行参数 `--encrypt-shellcode`

新增命令行参数 `--anti-ida`

新增命令行参数 `--android-anti-debug`

新增命令行参数 `--android-anti-dump`

新增命令行参数 `--android-check-sign`

新增命令行参数 `--android-check-integrity`

新增命令行参数 `--expire-time`

#### 使用方法：

> ./upx.out libnative-lib.so --android-shlib --android-check-sign=0x5F2E081B --anti-ida --licfile=/work/license.key --android-check-integrity --android-anti-dump --replace-identify=company_name

- android-shlib: 在加固android平台so时**必须**具备的参数。
- licfile：指定授权文件路径，默认读取当前目录下的license.key，如果指定本参数则从参数指定路径获取授权文件。
- replace-identify= : 指定客户公司名称，用于控制log等标识。
- fix-sdk-build: 修复elf header 用于解决ndk链接外部动态库报错的问题。
- encrypt-shellcode: 用于对patch上去的shellcode做加密处理。
- anti-ida: 用于防ida静态分析，包括隐藏符号表，IDA错误弹窗、IDA耗尽系统内存等功能。
- android-anti-debug: so防调试功能。
- android-check-sign: 为附加so签名校验功能，该hash值通过CheckSignApk/SignApk.jar工具计算得到，使用命令`java -jar SignApk.jar CERT.RSA`，`CERT.RSA`为apk包中签名文件，只支持v1签名。
- android-check-integrity: so完整性校验功能（防篡改）。
- android-anti-dump: 用于在so加载后清除内存中phdrs信息。
- expire-time: 加固有效期，以秒为单位。
- expand-space: 扩充so文件，以解决部分so体积过小无法做保护的问题。

### 生成报告
#### 使用方法：

> ./upx.out --orig="" --pic="" --report="" --android-shlib so-files ...

例子：
> ./upx.out --orig="testapks/app-debug.apk" --pic="../report-pic/" --report="report.md" --android-shlib lib/armeabi-v7a/libnative.so .so --android-shlib

#### 参数说明：
- orig: 输入的原始zip型压缩或二进制文件。
- pic: 生成md文件所需的图片路径。
- report: 生成的报告路径。

注：需要将vendor.json放在upx.out同级目录下。


### 返回值-报告
> 0：正常 ----> （有报告生成）提示：so保护完成，详情查看报告。

> 1：保护失败。（由于不可抗力，而导致添加保护失败，此时报告没有生成） ----> （无报告生成）提示：so保护失败，请联系相关客服。

> 2：部分so无法加上保护。（由于已知原因，而不去添加保护，此时有生成报告）----> （有报告生成）提示：存在部分so无法保护，详情请查看报告。

> 3：so无法加上保护。（由于已知原因，而不去添加保护，此时没有生成报告——全部so都无法加上保护的情况。）----> （无报告生成）提示：由于当前so无法保护。

start.sh返回值
> 0 ---> 提示：so保护完成，详情请查看报告。

> 3 ---> 提示：由于兼容性问题，当前加固工具无法对so做保护。

> 1或其他 ---> 提示：so保护失败，请联系客服。
