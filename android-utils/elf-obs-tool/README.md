# ELF 符号混淆工具使用说明-v1.0.0
## 一、功能说明
1. 支持静态库.a混淆
2. 支持符号混淆
3. 不支持动态库.so，因未测试
4. 不支持.o文件名混淆


## 二、开始执行
### 2.1 添加ELF文件
把需要符号混淆的ELF文件，放置对应目录下`src/{abi}/`

### 2.2 配置不混淆符号
编辑文件`sym_black.cfg`。
每行代表不参与混淆的符号关键字，包含此关键字的符号均不会混淆处理。

### 2.3 执行脚本 start_obscure.sh

### 2.4 执行结果
* 输出目录`out/{abi}/`
* `out/{abi}/sym_mapping.txt`文件为混淆后的mapping表

