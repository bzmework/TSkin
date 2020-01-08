# TSkin
 采用ATL实现的窗体换肤的COM DLL，扁平化效果，简单易用。   
The com DLL of window skin changing realized by ATL is flat and easy to use.   
   
支持的功能：   
设置标题栏文本颜色(TitleColor)   
设置标题栏渐变开始色(GradientBeginColor)   
设置标题栏渐变结束色(GradientEndColor)   
设置窗口背景色(BackColor)   
设置标题栏高度,取值范围(TitleBarHeight)   
设置边框宽度(BorderWidth）   
设置窗口透明度(Alpha)   
设置阴影颜色(ShadowColor)   
设置阴影深度,(ShadowDepth)   
设置阴影透明度(ShadowTransparency)   
设置阴影类型(ShadowType)   
设置是否显示最小化按钮(ShowMinButton)   
设置是否显示最大化按钮(ShowMaxButton)   
设置是否显示帮助按钮(ShowHelpButton)   
设置关闭按钮颜色(CloseButtonColor)   
设置是否启用字体平滑,(FontSmoothing) 只有少数字体支持平滑, 例如: Arial,Tahoma,微软雅黑.   
   
编译环境：   
Windows 10   
Visual Studio 2019   
说明：支持vs6-vs2019所有版本编译，如果你要支持在xp上使用，请采用TSkin.dsw，若有必要则进行升级，   
升级路线一般是：vs6升级到vs2005进行过渡然后升级到更高版本的vs编译环境，   
如果你嫌繁琐则自己手工构造工程文件。   
   
测试效果:   
获得焦点   
![image](https://github.com/bzmework/TSkin/blob/master/test1.jpg)      
失去焦点    
![image](https://github.com/bzmework/TSkin/blob/master/test2.jpg)    
窗口透明   
![image](https://github.com/bzmework/TSkin/blob/master/test3.jpg)    
自定义   
![image](https://github.com/bzmework/TSkin/blob/master/test4.jpg)    
   
DLL发布在：Project\ReleaseMinDependency   
