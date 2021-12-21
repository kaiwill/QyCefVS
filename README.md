1. [QT一步一步集成CEF](https://blog.csdn.net/paopao_wu/category_11518677.html) 示例代码,每个章节的代码在对应的分支上。

2. 因为 cef 二进制文件和 lib 文件太大，仓库中并没有包含着两个文件夹。请自行编译CEF二进制发行包后将对应文件拷贝到目录中。

   - libs/cef/bin/debug   debug版本
   - lib/cef/bin/release   release版本
   - lib/cef/lib/debug   debug版本库文件
   - lib/cef/lib/release  release版本库文件

   > 注意本项目对应的是 Windows 32 位 环境
   >
   > 环境配置请参考 [《QT集成CEF01-准备工作》](QT集成CEF01-准备工作)
   >
   > CEF二进制发行包编译请参考 [《QT集成CEF02-编译libcef_dll_wrapper》](https://blog.csdn.net/paopao_wu/article/details/121720452)

