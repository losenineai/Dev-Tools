# <center>app-release.apk</center>
# <center>so保护报告</center>
### <center>2020-04-23 顶像技术</center>
## 任务信息
* 防静态分析

## 基本信息
* 文件名：app-release.apk
* 大小：0.4418MB
* MD5：eb9ba352e54eb2ae794292f7365e5bf3

## 加固说明
### 保护记录
* `test.so (758533-->439140)`  
  - 防静态分析

### 检查工具
[010editor](https://www.sweetscape.com/download/010editor/)
[7z](https://www.7-zip.org/)
[ida](https://www.hex-rays.com/products/ida/support/download_freeware.shtml)

### 防静态分析
#### 检查过程
按下列步骤进行验证：
1. 使用zip压缩工具对压缩包做解压。
2. 提取解压包中so文件。
3. 把步骤2中提取的so文件拖入[IDA试用版](https://www.hex-rays.com/products/ida/support/download_freeware.shtml)工具中。
4. 如[IDA试用版](https://www.hex-rays.com/products/ida/support/download_freeware.shtml)不断弹出如下图的错误提示框，则功能生效。

![ida](/Users/white/Desktop/其他/avatar.png/anti-ida.png)
<center>[SO壳保护后在IDA下无法分析的效果图]</center>

