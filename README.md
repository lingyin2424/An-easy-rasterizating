# 从矩阵乘法到GPU优化——从零开始搓一个光栅化器

最近学了101和202的前一部分，决定从零开始搓一个光栅化器。参考了一些项目，自己模仿着设计了框架。写着写着发现里面有很多地方设计的不合理，又重构了一次。边学边写，花了两个多月，实现了CPU版本。

随后又花了一个多月学了CUDA编程，写了个GPU版本。

除了那个读取图片的代码是直接从别的项目中复制的，实际上写下来一共也就2000多行。

没有写界面，因为不好debug。

最近非常焦虑，要去努力干一些奇奇怪怪的事了。

三个多月，从不知道啥叫 CMakeLists，到写完一整套光栅化管线，实现各种着色模型，最后学习CUDA编程，还是挺艰辛的。我计划这一阶段的图形学学习就暂告一段落了。

真有意思啊，真希望以后还有机会学习图形学或者做相关工作啊。


### Update 2024 5 10 15 48
把主函数里的obj文件地址改成了相对地址，原本是绝对地址。
压缩包里的GPU也别忘了改下。