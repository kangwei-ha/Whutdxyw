# 大学语文刷时长脚本
武汉理工大学大学语文刷时长脚本，老东西要毕业了，代刷也赚够了，索性开源了  
基于MFC和C++实现的大学语文挂时长GUI脚本  
## 文件描述
/src文件夹下是实现的一些源码，外面是MFC生成的对话框文件  
/src/common/HttpClient文件夹是实现的能发送HTTP请求的类  
/src/common/Tool文件夹是一些工具函数  
/src/Dxyw文件夹是主要实现功能的类，获取SessionId，登录学号密码之类的  
## 如何使用
1.编译/直接下载Release的Whutdxyw.exe，软件界面长这样  
![111](https://github.com/user-attachments/assets/d2ed9ebb-0778-403f-a8e8-de1f60f929c4)  
2.输入学号和密码(默认一般都是学号)输入完成点添加就行  
![222](https://github.com/user-attachments/assets/efa74e2a-828f-4af5-93c0-d1b4c337125f)  
3.支持添加多个，但别挂太多，多了请求解析很慢  
![222](https://github.com/user-attachments/assets/2c6a272f-8318-4017-ad99-0bc3c324ada4)  
4.默认一分钟自动刷新一次，可以手动点强制刷新，选中某行后可以删除  
