# 目录结构说明

##  目录说明
1. CheckSignApk，“签名校验”功能所需用到工具，用于提取Apk中的签名信息，生产哈希值。

2. doc，文档说明。

3. libelf，elf文件修改所使用到的工具库，主要为插入空section header时，需要用到。该代码会编译成静态库，提供给upx编译时链接。

4. report-img，生成报告所需用到的图片。

5. src，整个项目核心源码。
    - src/shellcode：包含签名校验，防调试，防dump，防篡改等功能所需的shellcode。
    - src/stub：包含upx解壳代码。


6. test，测试。

7. ucl-1.03，加解密库，编译成静态库文件，再将静态库文件与upx进行编译链接。

8. bin-upx-20160918，生成upx解壳的所用到的loader的机器码，所需用到的工具。


