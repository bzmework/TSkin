#ifndef __SHADOWWINDOW_H_
#define __SHADOWWINDOW_H_

#include <windows.h>
#include <tchar.h>

#define DEF_DEPTH			5
#define DEF_TRANSPARENCY	200

// Vista compatibility APIs
//是否“启用桌面组合”。
//当在Vista及以上系统，在“性能选项”开启了“启用桌面组合”和“在最大化和最小化时动态显示窗口”功能后，阴影显示有问题。
//因此在“启用桌面组合”时关闭阴影。
typedef HRESULT (WINAPI *pfnDwmIsCompositionEnabled)(BOOL *pfEnabled);
static pfnDwmIsCompositionEnabled s_DwmIsCompositionEnabled;

class CShadowWindow
{
// 构造与析构
public:
	CShadowWindow();
	virtual ~CShadowWindow();

// 属性
public:

	//阴影类型
	enum ShadowTypeConstants
	{		
		stNoneShadow = 0,		//没有阴影
		stDefaultShadow = 1,	//缺省阴影(阴影在右边和底边)
		stFullShadow = 2		//完全(阴影在上下左右四边)
	};

	COLORREF	ShadowColor;	//阴影颜色
	INT			ShadowDepth;	//阴影深度
	BYTE		Transparency;	//透明度
	ShadowTypeConstants ShadowType; //阴影类型

// 方法
public:
	
	BOOL Shadow(HWND hWnd);		//应用阴影
	VOID OpenShadows();			//打开阴影
	VOID CloseShadows();		//关闭阴影
	BOOL IsDwmCompLayered();	//Vista及以上系统启用了“启用桌面组合”+“在最大化和最小化时动态显示窗口”性能选项

//事件
public:
	VOID OnWindowPosChanged(WINDOWPOS *lpwp);
	VOID OnSize();
	VOID OnSettingChange(); 

private:

	HWND m_hWnd;				//父窗口句柄
	HWND m_hWndLt;				//左边阴影窗口句柄
	HWND m_hWndTt;				//顶端阴影窗口句柄
	HWND m_hWndBt;				//底端阴影窗口句柄
	HWND m_hWndRt;				//右边阴影窗口句柄
	BOOL m_bIsLayered;		    //分层窗口支持
	BOOL m_bLastShow;			//上次显示状态
	WINDOWPOS m_wp;				//父窗口位置
	INT	 m_cx;					//上一宽度
	INT  m_cy;					//上一高度
	BOOL m_bIsDwmCompLayered;	//Vista及以上系统启用了“启用桌面组合”+“在最大化和最小化时动态显示窗口”性能选项
	BOOL m_bIsOpend;			//阴影是否被打开

	VOID CheckEnvironment();	//检查环境以确定是否支持阴影
	VOID DestroyShadows();		//销毁阴影窗口
	VOID CreateShadows();		//创建阴影窗口
	VOID ShowShadows(BOOL bShow, BOOL bForce = FALSE);//显示或隐藏阴影窗口
	VOID AdjustShadowPos();     //调整阴影窗口尺寸和位置
	VOID UpdateShadowEdge(INT X, INT Y, INT nWidth, INT nHeight, INT nType);//更新阴影内容

};

#endif //__SHADOWWINDOW_H_
