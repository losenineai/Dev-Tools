## so保护FAQ
### Q1. so保护失败，并出现日志"stdc++/stl, can not protect".
答：确认保护的so是否名字为："libstdc++.so"、"gabi++_shared.so"、"stlport_shared.so"、"gnustl_shared.so"、"c++_shared.so"，若是则属于正常现象，我们不做保护，并且保护的必要性不大。

原因是：这些so为android官方所提供的stl相关的库，这些库会涉及到部分数据会在so解壳之前做初始化，如果对它们做保护，则会出现初始化失败，而导致so运行崩溃。


### Q2. so保护失败，并出现日志"CantPackException:DT_TEXTREL found; re-compile with -fPIC"
答：我们的so保护工具只能对使用PIC加载方式的so做保护，不然可能会存在一些被保护的数据需要在so解壳之前做初始化，从而导致so加载失败，因此我们不对该so做保护。

解决办法：对so做重新编译，并在编译选项中添加-fPIC参数。

### Q3. so保护失败，并出现日志"NotCompressibleException".
答：由于so过小，做压缩时，压缩率过低，而无法对其添加压缩壳保护。

解决办法：对so做重新编译，并在项目的一个源文件中添加代码：`extern \"C\" void stub(){asm (\"\t.space 8192\n\");}`。

