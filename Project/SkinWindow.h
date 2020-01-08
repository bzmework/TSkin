#ifndef __SKINWINDOW_H_
#define __SKINWINDOW_H_

#include <windows.h>
#include <tchar.h>
#include "MemDC.h"

#define BORDER_WIDTH			3//GetSystemMetrics(SM_CXSIZEFRAME)
#define BUTTON_SIZE				22//GetSystemMetrics(SM_CXSIZE)
#define TITLE_HEIGHT			22//GetSystemMetrics(SM_CYCAPTION)
#define ICON_WIDTH				GetSystemMetrics(SM_CXSMICON)
#define ICON_HEIGHT				GetSystemMetrics(SM_CYSMICON)

// 光标位置
enum HitTestConstants
{
	//In the border of a window that does not have a sizing border 
	//在没有大小边框的窗口的边框中
	htBorder = HTBORDER,		
	//In the lower horizontal border of a resizable window (the user can click the mouse to resize the window vertically) 
	//在可调整大小窗口的较低水平边框中(用户可以单击鼠标垂直调整窗口的大小)
	htBottom = HTBOTTOM,
	//In the lower-left corner of a border of a resizable window (the user can click the mouse to resize the window diagonally)
	//在可调整大小窗口边框的左下角(用户可以单击鼠标对角线调整窗口的大小)
	htBottomLeft = HTBOTTOMLEFT,
	//In the lower-right corner of a border of a resizable window (the user can click the mouse to resize the window diagonally) 
	//在可调整大小窗口边框的右下角(用户可以单击鼠标对角线调整窗口的大小)
	htBottomRight = HTBOTTOMRIGHT,
	//In a title bar 
	//在标题栏中
	htCaption = HTCAPTION,
	//In a client area
	//在客户区域
	htClient = HTCLIENT,
	//In a close button
	//在关闭按钮中
	htClose = HTCLOSE,
	//On the screen background or on a dividing line between windows (same as HTNOWHERE, except that the DefWindowProc function produces a system beep to indicate an error) 
	//在屏幕背景上或窗口之间的分界线上(与HTNOWHERE相同，除了DefWindowProc函数产生系统哔哔声以指示错误之外)
    htError = HTERROR,
	//In a size box (same as HTSIZE) 
	//在大小框中(与HTSIZE相同)
	htGrowBox = HTSIZE,
	//In a Help button
	//在帮助按钮中
	htHelp = HTHELP,
	//In a horizontal scroll bar 
	//在水平滚动条中
	htHScroll = HTHSCROLL,
	//In the left border of a resizable window (the user can click the mouse to resize the window horizontally) 
	//在可调整大小窗口的左边框中(用户可以单击鼠标水平调整窗口的大小)
	htLeft = HTLEFT,
	//In a menu 
	//在菜单位置
	htMenu = HTMENU,
	//In Maximize button 
	//在最大化按钮中
	htMaxButton = HTMAXBUTTON,
	//In Minimize button 
	//在最小化按钮中
	htMinButton = HTMINBUTTON,
	//On the screen background or on a dividing line between windows 
	//在屏幕背景上或窗口之间的分界线上
    htNoWhere =	HTNOWHERE,
	//In a Minimize button 
	//在最小化按钮中
	htReduce = HTREDUCE,
	//In the right border of a resizable window (the user can click the mouse to resize the window horizontally) 
	//在可调整大小窗口的右边框中(用户可以单击鼠标水平调整窗口的大小)
	htRight = HTRIGHT,
	//In a size box (same as HTGROWBOX) 
	//在一个大小框中(与HTGROWBOX相同)
	htSize = HTSIZE,
	//In a System menu or in a Close button in a child window 
	//在系统菜单中或在子窗口中的关闭按钮中
	htSysMenu = HTSYSMENU,
	//In the upper horizontal border of a window
	//在窗口的上水平边界
	htTop = HTTOP,
	//In the upper-left corner of a window border 
	//在窗口边框的左上角
	htTopLeft = HTTOPLEFT,
	//In the upper right corner of a window border 
	//在窗口边框的右上角
	htTopRight = HTTOPRIGHT,
	//In a window currently covered by another window in the same thread (the message will be sent to underlying windows in the same thread until one of them returns a code that is not HTTRANSPARENT) 
	//在当前由同一线程中的另一个窗口覆盖的窗口中(消息将被发送到同一线程中的底层窗口，直到其中一个窗口返回非HTTRANSPARENT代码)
    htTransparent =	HTTRANSPARENT,
	//In the vertical scroll bar 
	//在垂直滚动条中
	htVScroll =	HTVSCROLL,
	//In a Maximize button 
	//在最大化按钮中
	htZoom = HTZOOM,
	//在自定义位置
	htCustom = HTCLIENT
};

// 对象状态
enum ObjStateConstants
{
    osNormal = 0x0,						//正常状态				
    osHot = 0x1,						//激活状态
    osPressed = 0x2,					//按下状态
    osDisabled = 0x3,					//不可用状态
	osVisible = 0x4						//是否可见状态
};

// 对象类型
enum ObjTypeConstants
{
    otWindow = 0x1,						//窗口				
	otIcon = 0x2,						//图标
	otText = 0x3,						//文本
	otHelp = 0x4,						//帮助按钮
	otMin = 0x5,						//最小化按钮
	otMax = 0x6,						//最大化按钮
	otClose = 0x7,						//关闭按钮
	otCustomButton = 0x8				//自定义按钮
};

// 对象
class TheObj
{
public:
	TheObj(ObjTypeConstants eType)
	{
		Type = eType;
		State = osNormal;
		Icon = NULL;
		Title = NULL;
		Tip = NULL;
	}
	~TheObj()
	{
		Icon = NULL;
		Title = NULL; 
		Tip = NULL;
	}

	ObjTypeConstants Type;				//类型
	ObjStateConstants State;			//状态
	RECT Area;							//区域
	HICON Icon;							//图标
	TCHAR* Title;						//标题
	TCHAR* Tip;							//提示文本
};

class CSkinWIndow
{
// 构造与析构
public:
	CSkinWIndow();
	virtual ~CSkinWIndow();

// 属性
public:
	BYTE		Alpha;					//透明度
	COLORREF	TitleColor;				//标题栏颜色
	COLORREF	GradientBeginColor;		//标题栏渐变开始色
	COLORREF	GradientEndColor;		//标题栏渐变结束色
	COLORREF	BackColor;				//窗口背景色
	COLORREF	CloseButtonColor;		//关闭按钮颜色
	INT			TitleBarHeight;			//标题栏高度
	INT			BorderWidth;			//边框宽度
	HICON		Icon;					//窗口图标
	TCHAR*		Title;					//窗口标题
	BOOL		FontSmoothing;			//启用字体平滑

// 方法
public:
	
	//是否有标题栏
	BOOL HasCaption(HWND hWnd) const;

	//是否有最大化按钮
	BOOL MaxButton() const;		
	VOID MaxButton(BOOL bValue);
	
	//是否有最小化按钮	
	BOOL MinButton() const;		
	VOID MinButton(BOOL bValue);

	//是否有帮助按钮		
	BOOL HelpButton() const;		
	VOID HelpButton(BOOL bValue);
	
	//让窗口透明
	BOOL Transparent(BYTE nAlpha);
	
	//刷新
	VOID Refresh();

// 事件
public:
	VOID OnShowWindow(HWND hWnd);
	//VOID OnNcCalcSize(RECT* lpRect);
	VOID OnNcCalcSize(NCCALCSIZE_PARAMS *lpncsp);
	VOID OnNcActivate(BOOL bActive);
	VOID OnSystemCommand(UINT nID, LPARAM lParam);
	VOID OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	VOID OnSize(INT nWidth, INT nHeight);
	UINT OnNcHitTest(UINT nHitTest, INT X, INT Y);
	VOID OnNcMouseMove(INT X, INT Y);
    VOID OnNcLButtonDblClk(INT X, INT Y);
	VOID OnNcLButtonDown(INT X, INT Y);
	VOID OnNcPaint();
	VOID OnPaint();
	VOID OnSettingChange();

private:
	HWND	m_hWnd;				//窗口句柄
	HRGN	m_hRgn;				//区域句柄
	CMemDC  m_MemDC;			//内存DC

	TheObj	*m_oWindow;			//窗口
	TheObj	*m_oIcon;			//标题图标
	TheObj	*m_oTitle;			//标题文本
	TheObj	*m_oClose;			//关闭按钮
	TheObj	*m_oMax;			//最大化按钮
	TheObj	*m_oMin;			//最小化按钮
	TheObj	*m_oHelp;			//帮助按钮

	TheObj	*m_oCustomBtn1;		//自定义按钮1
	TheObj	*m_oCustomBtn2;		//自定义按钮2
	TheObj	*m_oCustomBtn3;		//自定义按钮3
	TheObj	*m_oCustomBtn4;		//自定义按钮4
	TheObj	*m_oCustomBtn5;		//自定义按钮5
	//TheObj	*m_oPrevButton;		//上一个按钮
	
	HitTestConstants m_eMousePos;	//当前鼠标位置

	BOOL	m_bSysMenu;			//是否有系统菜单
	//BOOL	m_bToolWindow;		//是否是工具窗口
	//BOOL	m_bDialogFrame;		//是否对话框窗口
	
	BOOL	m_bMaxButton;		//是否有最大化按钮
	BOOL	m_bMinButton;		//是否有最小化按钮
	BOOL	m_bHelpButton;		//是否有帮助按钮
	//BOOL	m_bShowMinMaxButtons;//是否有最小化最大化按钮

	BOOL	m_bMaximized;		//窗口是否被最大化
	BOOL	m_bMinimized;		//窗口是否被最小化
	
	BOOL	m_bFirstShow;		//是否第一次显示窗口
	BOOL	m_bActive;			//窗口是否被激活		
	
	BOOL	m_bAntiAliased;		//开启了抗锯齿功能
		
private:
	
	VOID InitFrame(HWND hWnd);
	VOID ResetFrame(BOOL bMaximized);
	VOID DrawFrame(HDC hDC);

	VOID DrawCloseButton(HDC hDC);
	VOID DrawMaxButton(HDC hDC);
	VOID DrawMinButton(HDC hDC);
	VOID DrawHelpButton(HDC hDC);
	VOID DrawCustomButton(HDC hDC, TheObj *oButton);
	
	VOID OpenAntiAliasedFont(HWND hWnd);
	VOID CloseAntiAliasedFont(HWND hWnd);

};

#endif //__SKINWINDOW_H_
