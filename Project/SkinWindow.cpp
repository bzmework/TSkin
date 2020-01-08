#include "stdafx.h" 
#include "win32gdi.h"
#include "SkinWIndow.h"

/*
注意：
在Windows XP中，主题设置成“Windows XP”时会出现Region资源泄漏，
这应该是XP操作系统的bug，在“性能选项”中去掉“在窗口和按钮上使用视觉样式”后Region资源泄漏消失，
可能是CreateRoundRectRgn函数造成的。
*/

CSkinWIndow::CSkinWIndow()
{
	m_hWnd = NULL;
	m_hRgn = NULL;

	m_oWindow = new TheObj(otWindow);
	m_oIcon = new TheObj(otIcon);
	m_oTitle = new TheObj(otText);
	m_oClose = new TheObj(otClose);
	m_oMax = NULL;
	m_oMin = NULL;
	m_oHelp = NULL;

	m_oCustomBtn1 = NULL;
	m_oCustomBtn2 = NULL;
	m_oCustomBtn3 = NULL;
	m_oCustomBtn4 = NULL;
	m_oCustomBtn5 = NULL;
	//m_oPrevButton = NULL;
	
	m_bMaxButton = TRUE;
	m_bMinButton = TRUE;
	m_bHelpButton = TRUE;

	m_bMaximized = FALSE;
	m_bMinimized = FALSE;
	m_bFirstShow = FALSE;
	m_bActive = FALSE;
	m_bAntiAliased = FALSE;

    Alpha = 100;	
    TitleColor = RGB(255, 255, 255);
    GradientBeginColor = RGB(0, 0, 0);
    GradientEndColor = RGB(102, 102, 102);
    BackColor = RGB(207, 221, 232);
	CloseButtonColor = RGB(255, 0, 0);
	TitleBarHeight = TITLE_HEIGHT;
	BorderWidth = BORDER_WIDTH;
	Icon = NULL;
	Title = NULL;
	FontSmoothing = FALSE;

}

CSkinWIndow::~CSkinWIndow()
{
	if(m_bAntiAliased)
	{
		this->CloseAntiAliasedFont(m_hWnd);
	}

	if(m_oWindow)
	{
		delete m_oWindow;
	}

	if(m_oIcon)
	{
		delete m_oIcon;
	}
	if(m_oTitle)
	{
		delete m_oTitle;
	}

	if(m_oClose)
	{
		delete m_oClose;
	}
	if(m_oMax)
	{
		delete m_oMax;
	}
	if(m_oMin)
	{
		delete m_oMin;	 
	}
	if(m_oHelp)
	{
		delete m_oHelp;	 
	}

	if(m_oCustomBtn1)
	{
		delete m_oCustomBtn1;
	}
	if(m_oCustomBtn2)
	{
		delete m_oCustomBtn2;
	}
	if(m_oCustomBtn3)
	{
		delete m_oCustomBtn3;
	}
	if(m_oCustomBtn4)
	{
		delete m_oCustomBtn4;
	}
	if(m_oCustomBtn5)
	{
		delete m_oCustomBtn5;
	}
	//if(m_oPrevButton)
	//{
	//	delete m_oPrevButton;
	//}

	Title = NULL;
	Icon = NULL;

	//销毁内存DC
	m_MemDC.Destroy();

}

/*
窗口是否有标题栏
*/
BOOL CSkinWIndow::HasCaption(HWND hWnd) const
{
	LONG dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	return (dwStyle & WS_CAPTION) == WS_CAPTION;
}

/*
是否有最大化按钮
*/
BOOL CSkinWIndow::MaxButton() const
{
	return m_bMaxButton;
}		
VOID CSkinWIndow::MaxButton(BOOL bValue)
{
	m_bMaxButton = bValue;
}

/*
是否有最小化按钮
*/	
BOOL CSkinWIndow::MinButton() const
{
	return m_bMinButton;
}		
VOID CSkinWIndow::MinButton(BOOL bValue)
{
	m_bMinButton = bValue;
}

/*
是否有帮助按钮
*/		
BOOL CSkinWIndow::HelpButton() const
{
	return m_bHelpButton;
}	
VOID CSkinWIndow::HelpButton(BOOL bValue)
{
	m_bHelpButton = bValue;
}

/*让窗口透明*/
BOOL CSkinWIndow::Transparent(BYTE nAlpha)
{
	BOOL bOk = FALSE;
	if(m_hWnd)
	{
		HDC hDC = GetDC(NULL);//获得屏幕DC 
		BOOL bIsLayered = (GetDeviceCaps(hDC, BITSPIXEL) >= 16); //是否支持分层
		ReleaseDC(NULL, hDC);//必须释放，否则会造成DC泄漏
		if(bIsLayered)
		{
			INT dwExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
			if ((dwExStyle & WS_EX_LAYERED) == WS_EX_LAYERED)
			{
				bOk = SetLayeredWindowAttributes(m_hWnd, 0, nAlpha, LWA_ALPHA); //更改分层窗口的透明度
			}
			else
			{
				SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle | WS_EX_LAYERED);//设置分层窗口
				bOk = SetLayeredWindowAttributes(m_hWnd, 0, nAlpha, LWA_ALPHA); //让分层窗口生效，并更改分层窗口的透明度
			}
		}
	}
	return bOk;
}

//刷新
VOID CSkinWIndow::Refresh()
{
	this->OnNcPaint();
}

/*
显示窗口

MSDN: The WM_SHOWWINDOW message is sent to a window when the window is about to be hidden or shown. 
当窗口即将被隐藏或显示时，WM_SHOWWINDOW消息被发送到窗口。

 WM_SHOWWINDOW 
 fShow = (BOOL) wParam;      // show/hide flag 
 fnStatus = (int) lParam;    // status flag 

*/
VOID CSkinWIndow::OnShowWindow(HWND hWnd)
{
	if(!m_bFirstShow)
	{
		m_hWnd = hWnd;
		
		//初始化窗口框架
		this->InitFrame(hWnd);

		//设置窗口框架
		//this->ResetFrame(m_bMaximized);

		//第一次显示
		m_bFirstShow = TRUE;
	}

}

/*
设置窗口框架区域，包括：顶边框（标题栏高度）、左边框、底边框、右边框。
*/
/*
VOID CSkinWIndow::OnNcCalcSize(RECT* lpRect)
{
	lpRect->left += BorderWidth;	//左边框位置
	lpRect->top += TitleBarHeight;	//调整标题栏高度
	lpRect->right -= BorderWidth + 1;	//右边框位置
	lpRect->bottom -= BorderWidth + 1;	//底边框位置
}
*/

VOID CSkinWIndow::OnNcCalcSize(NCCALCSIZE_PARAMS *lpncsp)
{
	WINDOWPOS *tWP = lpncsp->lppos;
	RECT rc;

	//get new size
	rc.left = tWP->x;
	rc.top = tWP->y;
	rc.right = tWP->x + tWP->cx;
	rc.bottom = tWP->y + tWP->cy;
	
	//set size of new rectangle
	rc.left = rc.left + BorderWidth;
	rc.top = rc.top + TitleBarHeight;
	rc.right = rc.right - BorderWidth;
	rc.bottom = rc.bottom - BorderWidth;

    lpncsp->rgrc[0].left = rc.left;
    lpncsp->rgrc[0].top = rc.top;
    lpncsp->rgrc[0].right = rc.right;
    lpncsp->rgrc[0].bottom = rc.bottom;
	
}


/*
处理系统命令

MSDN: A window receives this message when the user chooses a command from the window menu (formerly known as the system or control menu) or when the user chooses the maximize button, minimize button, restore button, or close button.
当用户从窗口菜单(以前称为系统或控制菜单)中选择命令时，或者当用户单击最大化按钮、最小化按钮、还原按钮或关闭按钮时，窗口接收此消息。

WM_SYSCOMMAND 
uCmdType = wParam;        // type of system command requested 
xPos = LOWORD(lParam);    // horizontal position, in screen coordinates 
yPos = HIWORD(lParam);    // vertical position, in screen coordinates 

*/
VOID CSkinWIndow::OnSystemCommand(UINT nID, LPARAM lParam)
{
	LRESULT lresult = 0;

	switch(nID)
	{
    case SC_RESTORE: //恢复窗口
        this->ResetFrame(FALSE);
		break;

	case SC_MAXIMIZE: //最大化窗口
		this->ResetFrame(TRUE);
		break;

	case 61441: //窗口大小调整完毕
	case 61442:
	case 61443:
	case 61444:
	case 61445:
	case 61446:
	case 61447:
	case 61448:
		this->ResetFrame(FALSE);
		break;
    }
}

/*
获得窗口最小最大化或位置改变时，窗口所在区域

MSDN：The WM_GETMINMAXINFO message is sent to a window when the size or position of the window is about to change. 
An application can use this message to override the window's default maximized size and position, 
or its default minimum or maximum tracking size. 
当窗口的大小或位置即将更改时，WM_GETMINMAXINFO消息被发送到窗口。
应用程序可以使用此消息覆盖窗口的默认最大大小和位置，
或其默认最小或最大跟踪大小。

WM_GETMINMAXINFO 
lpmmi = (LPMINMAXINFO) lParam; // address of structure 

*/
VOID CSkinWIndow::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	RECT tR;
	
	/*
	SPI_GETWORKAREA
	Retrieves the size of the work area on the primary display monitor. 
	The work area is the portion of the screen not obscured by the system taskbar or by application desktop toolbars. 
	The pvParam parameter must point to a RECT structure that receives the coordinates of the work area, expressed in virtual screen coordinates.
	To get the work area of a monitor other than the primary display monitor, call theGetMonitorInfo function.
	检索主显示监视器上工作区域的大小。
	工作区是屏幕上没有被系统任务栏或应用程序桌面工具栏遮挡的的部分。
	pvParam参数必须指向一个RECT结构，该结构接收以虚拟屏幕坐标表示的工作区坐标。
	若要获取主显示器以外的显示器的工作区，请调用getmonitorinfo函数。
	*/
    SystemParametersInfo(SPI_GETWORKAREA, 0, &tR, 0);
	OffsetRect(&tR, -tR.left, -tR.top);
	lpMMI->ptMaxPosition.x = tR.left;
	lpMMI->ptMaxPosition.y = tR.top;
	lpMMI->ptMaxSize.x = tR.right - tR.left;
	lpMMI->ptMaxSize.y = tR.bottom - tR.top;

}

/*
正在调整窗口大小
MSDN: The WM_SIZE message is sent to a window after its size has changed. 
WM_SIZE消息在其大小更改后被发送到窗口。

WM_SIZE 
fwSizeType = wParam;      // resizing flag 
nWidth = LOWORD(lParam);  // width of client area 
nHeight = HIWORD(lParam); // height of client area 

fwSizeType:

SIZE_MAXHIDE 
Message is sent to all pop-up windows when some other window is maximized. 
当其他窗口最大化时，消息被发送到所有弹出窗口。

SIZE_MAXIMIZED 
The window has been maximized. 
窗户被最大化了。

SIZE_MAXSHOW
Message is sent to all pop-up windows when some other window has been restored to its former size. 
当其他窗口恢复到以前的大小时，将向所有弹出窗口发送消息。

SIZE_MINIMIZED 
The window has been minimized. 
窗户被最小化了。

SIZE_RESTORED 
The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies. 
窗口已经调整了大小，但是SIZE_MINIMIZED和SIZE_MAXIMIZED值都不适用。

*/
VOID CSkinWIndow::OnSize(INT nWidth, INT nHeight)
{
	if(m_hWnd)
	{
		this->ResetFrame(m_bMaximized);
	}
}

/*
非活动区域鼠标“命中测试”,即鼠标单击了窗口的哪个部位。Windows会根据鼠标键点击的部位给窗口发送相应的消息。例如：
如果WM_NCHITTEST的消息响应函数的返回值是HTCLIENT，表示鼠标点击的是客户区，则Windows会向窗口发送WM_LBUTTONDOWN消息；
如果WM_NCHITTEST的消息响应函数的返回值不是HTCLIENT（可能是HTCAPTION、HTCLOSE、HTMAXBUTTON等），即鼠标点击的是非客户区，Windows就会向窗口发送WM_NCLBUTTONDOWN消息。

无标题窗口拖动，我们一般是采用：响应WM_NCHITTEST消息，返回HTCAPTION来实现。
但是，如果在这同时还要处理鼠标的消息，如WM_LBUTTONDOWN、WM_LBUTTONUP、WM_RBUTTONUP、WM_MOUSEMOVE，你会发现，这些消息都收不到了。
原因是你在WM_NCHITTEST中处理了鼠标消息，把他定位成HTCAPTION，也就是鼠标在标题栏上，而标题栏属于非客户区（NC）;
非客户区的事件消息都是以WM_NC开头的，也就是说，当你的WM_NCHITTEST返回HTCAPTION时，原来可以用WM_LBUTTONUP处理的消息，你只能用WM_NCLBUTTONUP来处理。
因此如果要处理鼠标消息自然是同时处理WM_NCHITTEST和WM_NCRBUTTONUP，而不处理WM_RBUTTONUP等。


MSDN：The WM_NCHITTEST message is sent to a window when the cursor moves, or when a mouse button is pressed or released. 
If the mouse is not captured, the message is sent to the window beneath the cursor. 
Otherwise, the message is sent to the window that has captured the mouse. 
当光标移动时，或者当鼠标按钮被按下或释放时，WM_NCHITTEST消息被发送到窗口。
如果没有捕获鼠标，则消息将发送到光标下方的窗口。
否则，消息将发送到捕获鼠标的窗口。

WM_NCHITTEST 
xPos = LOWORD(lParam);  // horizontal position of cursor 
yPos = HIWORD(lParam);  // vertical position of cursor 

*/
UINT CSkinWIndow::OnNcHitTest(UINT nHitTest, INT X, INT Y)
{
	switch (nHitTest)
	{
	//在关闭、最大化、最小化、帮助、其它区域时，将鼠标“命中测试”设置成标题区域,
	//以屏蔽Windows重绘关闭、最大化、最小化、帮助按钮。
	case HTCLOSE:
	case HTMAXBUTTON:
	case HTMINBUTTON:
    case HTHELP:
	case HTMENU:
		return HTCAPTION;

	default:
		return nHitTest;
	}
	
}

/*
在非活动区域移动鼠标

MSDN：The WM_NCMOUSEMOVE message is posted to a window when the cursor is moved within the nonclient area of the window. This message is posted to the window that contains the cursor. If a window has captured the mouse, this message is not posted. 
当光标移动到窗口的非客户端区域时，WM_NCMOUSEMOVE消息被发送到窗口。此消息被发布到包含光标的窗口。如果窗口捕获了鼠标，则不会发布此消息。

WM_NCMOUSEMOVE 
nHittest = (INT) wParam;    // hit-test value 
pts = MAKEPOINTS(lParam);   // position of cursor 

*/
/*
VOID CSkinWIndow::OnNcMouseMove(INT X, INT Y)
{
	POINT pt;
	RECT rc;
	TheObj *oCurrButton = NULL; //当前按钮

	GetWindowRect(m_hWnd, &rc);
	pt.x = X - rc.left - 1;
	pt.y = Y - rc.top;
	
	//设置鼠标位置
	if (PtInRect(&m_oIcon->Area, pt) && m_eMousePos != htSysMenu)//在图标或系统菜单区域
	{
		m_eMousePos = htSysMenu;
	}
	else if (PtInRect(&m_oTitle->Area, pt) && m_eMousePos != htCaption)//在标题栏区域
	{
		m_eMousePos = htCaption;
	}
	else if (PtInRect(&m_oClose->Area, pt) && m_eMousePos != htClose) //在关闭按钮区域
	{
		m_eMousePos = htClose;
		m_oClose->Type = otClose;
		oCurrButton = m_oClose;
	}
	else if (m_bMaxButton && PtInRect(&m_oMax->Area, pt) && m_eMousePos != htMaxButton) //在最大化按钮区域
	{
		m_eMousePos = htMaxButton;
		m_oMax->Type = otMax;
		oCurrButton = m_oMax;
	}
	else if (m_bMinButton && PtInRect(&m_oMin->Area, pt) && m_eMousePos != htMinButton) //在最小化按钮区域
	{
		m_eMousePos = htMinButton;
		m_oMin->Type = otMin;
		oCurrButton = m_oMin;
	}
	else if(m_oCustomBtn1)
	{
		if (PtInRect(&m_oCustomBtn1->Area, pt) && m_eMousePos != htCustom) //在自定义按钮区域
		{
			m_eMousePos = htCustom;
			m_oCustomBtn1->Type = otCustomButton;
			oCurrButton = m_oCustomBtn1;
		}
	}
	else if(m_oCustomBtn2) 
	{
		if (PtInRect(&m_oCustomBtn2->Area, pt) && m_eMousePos != htCustom) //在自定义按钮区域
		{
			m_eMousePos = htCustom;
			m_oMax->Type = otCustomButton;
			oCurrButton = m_oCustomBtn2;
		}
	}
	else if(m_oCustomBtn3)
	{
		if (PtInRect(&m_oCustomBtn3->Area, pt) && m_eMousePos != htCustom) //在自定义按钮区域
		{
			m_eMousePos = htCustom;
			m_oMax->Type = otCustomButton;
			oCurrButton = m_oCustomBtn3;
		}
	}
	else if(m_oCustomBtn4)
	{
		if (PtInRect(&m_oCustomBtn4->Area, pt) && m_eMousePos != htCustom) //在自定义按钮区域
		{
			m_eMousePos = htCustom;
			m_oMax->Type = otCustomButton;
			oCurrButton = m_oCustomBtn4;
		}
	}
	else if(m_oCustomBtn5)
	{
		if (PtInRect(&m_oCustomBtn5->Area, pt) && m_eMousePos != htCustom) //在自定义按钮区域
		{
			m_eMousePos = htCustom;
			m_oMax->Type = otCustomButton;
			oCurrButton = m_oCustomBtn5;
		}
	}
	
	//绘制上一个按钮
	if(m_oPrevButton != NULL && oCurrButton != NULL && oCurrButton != m_oPrevButton)
	{
		m_oPrevButton->State = osNormal;
		if (m_oPrevButton->Type == otClose)
		{
			DrawCloseButton(m_oPrevButton);
		}
		else if (m_oPrevButton->Type == otMax)
		{
			DrawMaxButton(m_oPrevButton);
		}
		else if (m_oPrevButton->Type == otMin)
		{
			DrawMinButton(m_oPrevButton);
		}
		else if (m_oPrevButton->Type == otHelp)
		{
			DrawHelpButton(m_oPrevButton);
		}
		else
		{
			DrawCustomButton(m_oPrevButton);
		}
	}
	
	//绘制当前按钮
	if(oCurrButton != NULL && oCurrButton != m_oPrevButton)
	{
		oCurrButton->State = osHot;
		if (m_eMousePos == htClose)
		{
			DrawCloseButton(oCurrButton);
		}
		else if (m_eMousePos == htMaxButton)
		{
			DrawMaxButton(oCurrButton);
		}
		else if (m_eMousePos == htMinButton)
		{
			DrawMinButton(oCurrButton);
		}
		else if (m_eMousePos == htHelp)
		{
			DrawHelpButton(oCurrButton);
		}
		else
		{
			DrawCustomButton(oCurrButton);
		}

		m_oPrevButton = oCurrButton;
	}
	
	this->OnNcPaint();
}
*/
VOID CSkinWIndow::OnNcMouseMove(INT X, INT Y)
{
	POINT pt;
	RECT rc;

	GetWindowRect(m_hWnd, &rc);
	pt.x = X - rc.left - 1;
	pt.y = Y - rc.top;
	
	//设置鼠标位置
	if (PtInRect(&m_oIcon->Area, pt) && m_eMousePos != htSysMenu)//在图标或系统菜单区域
	{
		m_eMousePos = htSysMenu;
	}
	else if (PtInRect(&m_oTitle->Area, pt) && m_eMousePos != htCaption)//在标题栏区域
	{
		m_eMousePos = htCaption;
	}
	else if (PtInRect(&m_oClose->Area, pt) && m_eMousePos != htClose) //在关闭按钮区域
	{
		m_eMousePos = htClose;
	}
	else if (m_bMaxButton && PtInRect(&m_oMax->Area, pt) && m_eMousePos != htMaxButton) //在最大化按钮区域
	{
		m_eMousePos = htMaxButton;
	}
	else if (m_bMinButton && PtInRect(&m_oMin->Area, pt) && m_eMousePos != htMinButton) //在最小化按钮区域
	{
		m_eMousePos = htMinButton;
	}
	else if(m_oCustomBtn1)
	{
		if (PtInRect(&m_oCustomBtn1->Area, pt) && m_eMousePos != htCustom) //在自定义按钮区域
		{
			m_eMousePos = htCustom;
		}
	}
	else if(m_oCustomBtn2) 
	{
		if (PtInRect(&m_oCustomBtn2->Area, pt) && m_eMousePos != htCustom) //在自定义按钮区域
		{
			m_eMousePos = htCustom;
		}
	}
	else if(m_oCustomBtn3)
	{
		if (PtInRect(&m_oCustomBtn3->Area, pt) && m_eMousePos != htCustom) //在自定义按钮区域
		{
			m_eMousePos = htCustom;
		}
	}
	else if(m_oCustomBtn4)
	{
		if (PtInRect(&m_oCustomBtn4->Area, pt) && m_eMousePos != htCustom) //在自定义按钮区域
		{
			m_eMousePos = htCustom;
		}
	}
	else if(m_oCustomBtn5)
	{
		if (PtInRect(&m_oCustomBtn5->Area, pt) && m_eMousePos != htCustom) //在自定义按钮区域
		{
			m_eMousePos = htCustom;
		}
	}

}

/*
双击标题栏区域

The WM_NCLBUTTONDBLCLK message is posted when the user double-clicks the left mouse button while the cursor is within the nonclient area of a window. This message is posted to the window that contains the cursor. If a window has captured the mouse, this message is not posted. 
当用户双击鼠标左键时，WM_NCLBUTTONDBLCLK消息被发布，而光标位于窗口的非客户机区域内。此消息被发布到包含游标的窗口。如果窗口捕获了鼠标，则不会发布此消息。

WM_NCLBUTTONDBLCLK 
nHittest = (INT) wParam;    // hit-test value 
pts = MAKEPOINTS(lParam);   // position of cursor 
	
*/
VOID CSkinWIndow::OnNcLButtonDblClk(INT X, INT Y)
{
	if (m_eMousePos == htCaption && m_bMaxButton) //双击标题栏，最大化或恢复窗口
	{
		(m_bMaximized)?
		SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0):
		SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}
	else if(m_eMousePos == htSysMenu) //双击系统菜单，关闭窗口
	{
		SendMessage(m_hWnd, WM_CLOSE, 0, 0);
	}
}

/*
鼠标左键单击标题栏
MSDN: The WM_NCLBUTTONDOWN message is posted when the user presses the left mouse button while the cursor is within the nonclient area of a window. 
This message is posted to the window that contains the cursor. If a window has captured the mouse, this message is not posted. 
当用户在窗口的非客户端区域内按下鼠标左键时，将发出WM_NCLBUTTONDOWN消息。
此消息被发布到包含游标的窗口。如果窗口捕获了鼠标，则不会发布此消息。

WM_NCLBUTTONDOWN 
nHittest = (INT) wParam;    // hit-test value 
pts = MAKEPOINTS(lParam);   // position of cursor 


*/
VOID CSkinWIndow::OnNcLButtonDown(INT X, INT Y)
{
	if (m_eMousePos == htClose) //单击关闭按钮
	{
		SendMessage(m_hWnd, WM_CLOSE, 0, 0);
	}
	else if (m_eMousePos == htMaxButton) //单击最大化按钮
	{
		if (m_bMaximized) //已经最大化则恢复
		{
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		else
		{
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
	}
	else if (m_eMousePos == htMinButton) //单击最小化按钮
	{
		SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
}

/*
绘制窗口框架，包括：标题栏、左边框、右边框、底边框

MSDN：An application sends the WM_NCPAINT message to a window when its frame must be painted. 
应用程序在必须绘制框架时向窗口发送WM_NCPAINT消息。

*/
VOID CSkinWIndow::OnNcPaint()
{
	if(m_hWnd)
	{
		//HDC hDC = GetDCEx(m_hWnd, hRgn, DCX_WINDOW | DCX_INTERSECTRGN);
		HDC hDC = GetWindowDC(m_hWnd);
		this->DrawFrame(hDC);//画窗口框架
		ReleaseDC(m_hWnd, hDC);
	}
}

/*
绘制客户区
*/
VOID CSkinWIndow::OnPaint()
{
	if(m_hWnd)
	{
		RECT tR;
		PAINTSTRUCT ps;
 
		GetClientRect(m_hWnd, &tR); 
		HDC hDC = BeginPaint(m_hWnd, &ps);
		Win32GDI::DrawArea(hDC, tR.left, tR.top, tR.right - tR.left, tR.bottom - tR.top, BackColor);
		EndPaint(m_hWnd, &ps);
	}
}

/*
系统设置改变
*/
VOID CSkinWIndow::OnSettingChange()
{
	if(m_hWnd)
	{
		InvalidateRect(m_hWnd, NULL, TRUE);//重绘所有区域
	}
}

/*
窗口激活
*/
VOID CSkinWIndow::OnNcActivate(BOOL bActive)
{
	if(m_hWnd && m_bActive != bActive) //禁止反复绘制
	{
		m_bActive = bActive;
		this->OnNcPaint();
	}
}


/*
初始化窗口框架
*/
VOID CSkinWIndow::InitFrame(HWND hWnd)
{
	//获得窗口属性
	LONG dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	LONG dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
	
	m_bSysMenu = (dwStyle & WS_SYSMENU);
	//m_bToolWindow = (dwExStyle & WS_EX_TOOLWINDOW) == WS_EX_TOOLWINDOW;
	//m_bDialogFrame = (dwStyle & WS_DLGFRAME || dwExStyle & WS_EX_DLGMODALFRAME);
	
	m_bMaxButton = m_bMaxButton && ((dwStyle & WS_MAXIMIZEBOX) == WS_MAXIMIZEBOX);
	m_bMinButton = m_bMinButton && ((dwStyle & WS_MINIMIZEBOX) == WS_MINIMIZEBOX);
	m_bHelpButton = m_bHelpButton && ((dwExStyle & WS_EX_CONTEXTHELP) == WS_EX_CONTEXTHELP);
	//m_bShowMinMaxButtons = !m_bToolWindow && m_bSysMenu && (m_bMinButton || m_bMaxButton);
	
	m_bMaximized = (dwStyle & WS_MAXIMIZE) == WS_MAXIMIZE;
	m_bMinimized = (dwStyle & WS_MINIMIZE) == WS_MINIMIZE;
	
	//获得窗口图标
	if(Icon == NULL)
	{
		//Icon = (HICON)GetClassLong(hWnd, GCL_HICON);
		Icon = (HICON)SendMessage(hWnd, 0x7F, 0x2, 0);
	}
	m_oIcon->Icon = Icon;
	
	//获得窗口标题
	if(Title == NULL)
	{
		INT nLen = GetWindowTextLength(hWnd) + 1;
		Title = new TCHAR[nLen];
		TCHAR* lpszTitle = new TCHAR[nLen];
		GetWindowText(m_hWnd, lpszTitle, nLen);
		strncpy(Title, lpszTitle, nLen);
		delete[] lpszTitle;
	}
	m_oTitle->Title = Title;
	
	//去掉系统菜单。如果不去除菜单，单击标题栏时系统会重画最小化最大化关闭按钮，不好处理。
	SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_SYSMENU);

	//为窗口设置阴影
	//SPI_SETDROPSHADOW = 0x1025
	//SystemParametersInfo(0x1025, 0, (PVOID)TRUE, 0);// - 打开阴影效果
	//CS_DROPSHADOW = 0x20000
	//SetClassLong(hWnd, GCL_STYLE, GetClassLong(hWnd, GCL_STYLE) | 0x20000);
	
	//是否启用ClearType。Windows XP在控制面板“ClearType Tuning”中设置
	//SPI_GETCLEARTYPE = 0x1048
	//SPI_SETCLEARTYPE = 0x1049
	//BOOL bClearType = FALSE;
	//SystemParametersInfo(0x1048, 0, &bClearType, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
	//if(!bClearType)
	//{
	//	SystemParametersInfo(0x1048, TRUE, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE); //开启ClearType
	//}
	
	//启用字体平滑功能, 即开启“平滑屏幕字体边缘”性能选项
	//SPI_GETFONTSMOOTHING = 0x004A
	//SPI_SETFONTSMOOTHING = 0x004B
	BOOL bFontSmoothing = FALSE;
	SystemParametersInfo(SPI_GETFONTSMOOTHING, 0, &bFontSmoothing, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
	if(!bFontSmoothing && this->FontSmoothing)
	{
		//SystemParametersInfo(SPI_SETFONTSMOOTHING, TRUE, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE); //开启字体平滑功能
		this->OpenAntiAliasedFont(hWnd); //让所有窗口的字体支持平滑功能
	}

	//关闭窗口最大化最小化动画
	//ai.iMinAnimate = 0;
	//SystemParametersInfo(SPI_SETANIMATION, sizeof(ANIMATIONINFO), &ai, NULL); 

}

// 重设窗口框架
VOID CSkinWIndow::ResetFrame(BOOL bMaximized)
{
	RECT tR;
	
	m_bMaximized = bMaximized;
	GetWindowRect(m_hWnd, &tR);

	// 窗口区域
    m_oWindow->Area.left = 0;
	m_oWindow->Area.top = 0;
	m_oWindow->Area.right = tR.right - tR.left;
	m_oWindow->Area.bottom = tR.bottom - tR.top;

	// 关闭按钮区域
	m_oClose->Area.left = (m_oWindow->Area.right - m_oWindow->Area.left) - BUTTON_SIZE - 5;
	m_oClose->Area.top = 1;
	m_oClose->Area.right = m_oWindow->Area.right - m_oWindow->Area.left;
	m_oClose->Area.bottom = TitleBarHeight - 1;

	// 最大化按钮区域
	if (m_bMaxButton)
	{
		if(!m_oMax)
		{
			m_oMax = new TheObj(otMax);
		}

		m_oMax->Area.left = m_oClose->Area.left - BUTTON_SIZE + 4;
		m_oMax->Area.top = 1;
		m_oMax->Area.right = m_oClose->Area.left;
		m_oMax->Area.bottom = m_oClose->Area.bottom;
	}

	// 最小化按钮区域
	if (m_bMinButton)
	{
		if(!m_oMin)
		{
			m_oMin = new TheObj(otMin);
		}

		if (m_oMax) //有最大化按钮
		{
			m_oMin->Area.left = m_oMax->Area.left - BUTTON_SIZE + 4;
			m_oMin->Area.top = 1;
			m_oMin->Area.right = m_oMax->Area.left;
			m_oMin->Area.bottom = m_oMax->Area.bottom;
		}
		else
		{
			m_oMin->Area.left = m_oClose->Area.left - BUTTON_SIZE + 4;
			m_oMin->Area.top = 1;
			m_oMin->Area.right = m_oClose->Area.left;
			m_oMin->Area.bottom = m_oClose->Area.bottom;
		}
	}
	
	//帮助按钮区域
	if (m_bHelpButton)
	{
		if(!m_oHelp)
		{
			m_oHelp = new TheObj(otHelp);
		}
		
		if (m_oMin) //有最小化按钮
		{
			m_oHelp->Area.left = m_oMin->Area.left - BUTTON_SIZE + 4;
			m_oHelp->Area.top = 1;
			m_oHelp->Area.right = m_oMin->Area.left;
			m_oHelp->Area.bottom = m_oMin->Area.bottom;
		}
		else
		{
			if (m_oMax) //有最大化按钮
			{
				m_oHelp->Area.left = m_oMax->Area.left - BUTTON_SIZE + 4;
				m_oHelp->Area.top = 1;
				m_oHelp->Area.right = m_oMax->Area.left;
				m_oHelp->Area.bottom = m_oMax->Area.bottom;
			}
			else
			{
				m_oHelp->Area.left = m_oClose->Area.left - BUTTON_SIZE + 4;
				m_oHelp->Area.top = 1;
				m_oHelp->Area.right = m_oClose->Area.left;
				m_oHelp->Area.bottom = m_oClose->Area.bottom;
			}
		}
	}

	// 图标区域
	m_oIcon->Area.left = 5;
	m_oIcon->Area.top = 1;
	m_oIcon->Area.right = m_oIcon->Area.left + ICON_WIDTH;
	m_oIcon->Area.bottom = TitleBarHeight - 1;

	// 标题区域
	if (m_bMinButton)
	{
		m_oTitle->Area.left = m_oIcon->Area.right + 4;
		m_oTitle->Area.top = 1;
		m_oTitle->Area.right = m_oMin->Area.left - 1;
		m_oTitle->Area.bottom = TitleBarHeight - 1;
	}
	else if (m_bMaxButton)
	{
		m_oTitle->Area.left = m_oIcon->Area.right + 4;
		m_oTitle->Area.top = 1;
		m_oTitle->Area.right = m_oMax->Area.left - 1;
		m_oTitle->Area.bottom = TitleBarHeight - 1;
	}
	else
	{
		m_oTitle->Area.left = m_oIcon->Area.right + 4;
		m_oTitle->Area.top = 1;
		m_oTitle->Area.right = m_oClose->Area.left - 1;
		m_oTitle->Area.bottom = TitleBarHeight - 1;
	}

	//基于屏幕大小创建内存DC
	m_MemDC.Create(tR.right - tR.left + BorderWidth * 2, tR.bottom - tR.top + BorderWidth * 2);

	// 创建矩形窗口
	// CreateRoundRectRgn函数会导致区域泄露，不知道为什么
	//HRGN hRgn = CreateRoundRectRgn(0, 0, tR.right - tR.left, tR.bottom - tR.top, 0, 0);
	HRGN hRgn = CreateRectRgn(0, 0, tR.right - tR.left, tR.bottom - tR.top);
	//If the bRedraw parameter is TRUE, the system sends theWM_WINDOWPOSCHANGING and WM_WINDOWPOSCHANGED messages to the window. 
	//After a successful call to SetWindowRgn, the system owns the region specified by the region handle hRgn. 
	//The system does not make a copy of the region. Thus, you should not make any further function calls with this region handle. 
	//In particular, do not close this region handle. 
	SetWindowRgn(m_hWnd, hRgn, TRUE);
	DeleteObject(hRgn);

}

// 画窗口框架
VOID CSkinWIndow::DrawFrame(HDC hDC)
{
	INT X, Y, nWidth, nHeight;
	COLORREF clrBeginColor;
	COLORREF clrEndColor;
	
	HDC hMemDC = m_MemDC.GetDC();

	if(m_bActive)
	{
		clrBeginColor = GradientBeginColor;
		clrEndColor = GradientEndColor;
	}
	else
	{
		clrBeginColor = Win32GDI::AlphaColor(GradientBeginColor, Alpha);
		clrEndColor = Win32GDI::AlphaColor(GradientEndColor, Alpha);
	}

	//填充背景
	X = m_oWindow->Area.left;
	Y = m_oWindow->Area.top;
	nWidth = m_oWindow->Area.right - m_oWindow->Area.left;
 	nHeight = m_oWindow->Area.bottom - m_oWindow->Area.top;
	Win32GDI::DrawArea(hMemDC, X, Y, nWidth, nWidth, BackColor);

	//画标题栏背景
	X = 0;
	Y = 0;
	nWidth = m_oWindow->Area.right - m_oWindow->Area.left;
	nHeight = TitleBarHeight;
	Win32GDI::DrawGradient(hMemDC, X, Y, nWidth, nHeight, clrBeginColor, clrEndColor, Win32GDI::dLeftToRight);

	//画关闭按钮
	if(m_oClose)
	{
		this->DrawCloseButton(hMemDC);
	}

	//画最大化按钮
	if(m_oMax)
	{
		this->DrawMaxButton(hMemDC);
	}

	//画最小化按钮
	if(m_oMin)
	{
		this->DrawMinButton(hMemDC);
	}

	//画帮助按钮
	if(m_oHelp)
	{
		this->DrawHelpButton(hMemDC);
	}

	//画自定义按钮
	if(m_oCustomBtn1)
	{
		this->DrawCustomButton(hMemDC, m_oCustomBtn1);
	}

	if(m_oCustomBtn2)
	{
		this->DrawCustomButton(hMemDC, m_oCustomBtn2);
	}

	if(m_oCustomBtn3)
	{
		this->DrawCustomButton(hMemDC, m_oCustomBtn3);
	}

	if(m_oCustomBtn4)
	{
		this->DrawCustomButton(hMemDC, m_oCustomBtn4);
	}

	if(m_oCustomBtn5)
	{
		this->DrawCustomButton(hMemDC, m_oCustomBtn5);
	}

	//画标题栏图标
	if (m_oIcon)
	{
		if(m_oIcon->Icon)
		{
			X = m_oIcon->Area.left;
			Y = (TitleBarHeight - ICON_HEIGHT) / 2;
			nWidth = ICON_WIDTH;
			nHeight = ICON_HEIGHT;
			DrawIconEx(hMemDC, X, Y, m_oIcon->Icon, nWidth, nHeight, 0, NULL, DI_NORMAL);
		}
	}
	
	//画标题栏文本
	if (m_oTitle)
	{
		if(m_oTitle->Title)
		{
			COLORREF clrTitleColor = 0;
			if(m_bActive)
			{
				clrTitleColor = TitleColor;
			}
			else
			{
				clrTitleColor = Win32GDI::BlendColor(TitleColor, clrBeginColor, Alpha);
			}
			X = m_oTitle->Area.left;
			Y = m_oTitle->Area.top;
			nWidth = m_oTitle->Area.right - m_oTitle->Area.left;
			nHeight = m_oTitle->Area.bottom - m_oTitle->Area.top;
			Win32GDI::DrawAText(hMemDC, m_oTitle->Title, X, Y, nWidth, nHeight, clrTitleColor, Win32GDI::dtDefault1, TRUE, TRUE);
		}
	}
	
	//拷贝标题栏
	X = 0;
	Y = 0;
	nWidth = m_oWindow->Area.right - m_oWindow->Area.left;
	nHeight = TitleBarHeight;
	BitBlt(hDC, X, Y, nWidth, nHeight, hMemDC, X, Y, SRCCOPY);

	//画左边框
	X = 0;
	Y = TitleBarHeight;
	nWidth = BorderWidth;
	nHeight = m_oWindow->Area.bottom - m_oWindow->Area.top - TitleBarHeight;
	Win32GDI::DrawGradient(hMemDC, X, Y, nWidth, nHeight, clrBeginColor, clrEndColor, Win32GDI::dTopToBottom);
	BitBlt(hDC, X, Y, nWidth, nHeight, hMemDC, X, Y, SRCCOPY);

	//画底边框
	X = BorderWidth;
	Y = m_oWindow->Area.bottom - BorderWidth;
	nWidth = m_oWindow->Area.right - m_oWindow->Area.left - 2 * BorderWidth;
	nHeight = BorderWidth;
	Win32GDI::DrawGradient(hMemDC, X, Y, nWidth, nHeight, clrEndColor, clrBeginColor, Win32GDI::dLeftToRight);
	BitBlt(hDC, X, Y, nWidth, nHeight, hMemDC, X, Y, SRCCOPY);

	//画右边框
	X = m_oWindow->Area.right - BorderWidth;
	Y = TitleBarHeight;
	nWidth = BorderWidth;
	nHeight = m_oWindow->Area.bottom - m_oWindow->Area.top - TitleBarHeight;
	Win32GDI::DrawGradient(hMemDC, X, Y, nWidth, nHeight, clrEndColor, clrBeginColor, Win32GDI::dTopToBottom);
	BitBlt(hDC, X, Y, nWidth, nHeight, hMemDC, X, Y, SRCCOPY);
 
}

// 画关闭按钮
/*
VOID CSkinWIndow::DrawCloseButton(TheObj *oButton, HDC hSrcDC, HDC hDstDC)
{
	INT X, Y, nWidth, nHeight;
	COLORREF clrBeginColor;
	COLORREF clrEndColor;

	if (hSrcDC == 0) hSrcDC = m_MemDC->GetDC();
	ObjStateConstants eButtonState = oButton->State;

	switch(eButtonState)
	{
	case osNormal:
	case osPressed:
		clrBeginColor = Win32GDI::AlphaColor(0xFF, 60);
		clrEndColor = Win32GDI::BlendColor(0xFF, 0x0, 100);
		break;

	case osHot:
		clrBeginColor = Win32GDI::AlphaColor(0xFF, 100);
		clrEndColor = Win32GDI::BlendColor(0xFF, 0x40, 100);
		break;
	}

	X = m_oClose->Area.left;
	Y = m_oClose->Area.top;
	nWidth = m_oClose->Area.right;
	nHeight = TitleBarHeight / 2 + 2;
	Win32GDI::DrawGradient(hSrcDC, X, Y, nWidth, nHeight, clrBeginColor, clrEndColor, Win32GDI::dTopToBottom);

    X = m_oClose->Area.left;
	Y = TitleBarHeight / 2;
	nWidth = m_oClose->Area.right;
	nHeight = TitleBarHeight - 1;
	Win32GDI::DrawGradient(hSrcDC, X, Y, nWidth, nHeight, clrBeginColor, clrEndColor, Win32GDI::dTopToBottom);

	X = m_oClose->Area.left;
	Y = m_oClose->Area.top;
	nWidth = m_oClose->Area.right - m_oClose->Area.left;
	nHeight = m_oClose->Area.bottom - m_oClose->Area.top;
    switch(eButtonState)
	{
    case osNormal:
        Win32GDI::DrawClose(hSrcDC, X + 11, (nHeight - 8) / 2, 8, 0xFFFFFF);
		break;

    case osHot:
        Win32GDI::DrawClose(hSrcDC, X + 11, (nHeight - 8) / 2, 8, 0xFFFFFF);
		break;

	case osPressed:
        Win32GDI::DrawClose(hSrcDC, X + 12, (nHeight - 8) / 2 + 1, 8, 0xFFFFFF);
		break;
	}
	
	X = m_oClose->Area.left;
	Y = m_oClose->Area.top;
	nWidth = m_oClose->Area.right - m_oClose->Area.left;
	nHeight = m_oClose->Area.bottom - m_oClose->Area.top;
	if (hDstDC == 0)
	{
		hDstDC = GetWindowDC(m_hWnd);
		Win32GDI::TransparentPaintAlpha(hDstDC, X, Y, nWidth, nHeight, hSrcDC, X, Y, nWidth, nHeight, 0xFF);
		ReleaseDC(m_hWnd, hDstDC);
	}
	else
	{
		Win32GDI::TransparentPaintAlpha(hDstDC, X, Y, nWidth, nHeight, hSrcDC, X, Y, nWidth, nHeight, 0xFF);
	}

}
*/
VOID CSkinWIndow::DrawCloseButton(HDC hDC)
{
	INT X, Y, nWidth, nHeight;
	COLORREF clrColor;
	
	clrColor = Win32GDI::BlendColor(CloseButtonColor, GradientEndColor, 120);
	if(!m_bActive)
	{
		clrColor = Win32GDI::AlphaColor(clrColor, Alpha);
	}

	X = m_oClose->Area.left;
	Y = m_oClose->Area.top;
	nWidth = m_oClose->Area.right - m_oClose->Area.left;
	nHeight = m_oClose->Area.bottom - m_oClose->Area.top;
	if(CloseButtonColor != -1)
	{
		Win32GDI::DrawArea(hDC, X, Y-1, nWidth+1, nHeight+2, clrColor);
	}
    Win32GDI::DrawClose(hDC, X + 9, (nHeight - 8) / 2, 8, 0xFFFFFF);

}

// 画最大化按钮
/*
VOID CSkinWIndow::DrawMaxButton(TheObj *oButton, HDC hSrcDC, HDC hDstDC)
{
	INT X, Y, nWidth, nHeight;
	COLORREF clrBeginColor;
	COLORREF clrEndColor;

	if (hSrcDC == 0) hSrcDC = m_MemDC->GetDC();
	ObjStateConstants eButtonState = oButton->State;
	
	switch(eButtonState)
	{
	case osNormal:
		clrBeginColor = Win32GDI::AlphaColor(HighlightColor, Alpha);
		clrEndColor = Win32GDI::BlendColor(HighlightColor, 0x0, Alpha);
		break;
	case osHot:
		clrBeginColor = Win32GDI::AlphaColor(HighlightColor, Alpha);
		clrEndColor = Win32GDI::BlendColor(HighlightColor, 0x60, Alpha);
		break;
	}

	X = m_oMax->Area.left;
	Y = m_oMax->Area.top;
	nWidth = m_oMax->Area.right;
	nHeight = TitleBarHeight / 2 + 2;
	Win32GDI::DrawGradient(hSrcDC, X, Y, nWidth, nHeight, clrBeginColor, clrEndColor, Win32GDI::dTopToBottom);
	
	X = m_oMax->Area.left;
	Y = TitleBarHeight / 2;
	nWidth = m_oMax->Area.right;
	nHeight = TitleBarHeight - 1;
	Win32GDI::DrawGradient(hSrcDC, X, Y, nWidth, nHeight, clrEndColor, clrEndColor, Win32GDI::dTopToBottom);

	if (m_bMaximized)
	{
        Win32GDI::DrawRestore(hSrcDC, X + 4, (nHeight - 8) / 2, 8, 0xFFFFFF);
	} 
	else
	{
		Win32GDI::DrawMax(hSrcDC, X + 4, (nHeight - 8) / 2, 8, 0xFFFFFF);
	}

	X = m_oMax->Area.left;
	Y = m_oMax->Area.top;
	nWidth = m_oMax->Area.right - m_oMax->Area.left;
	nHeight = m_oMax->Area.bottom - m_oMax->Area.top;
	if (hDstDC == 0)
	{
		hDstDC = GetWindowDC(m_hWnd);
		Win32GDI::TransparentPaintAlpha(hDstDC, X, Y, nWidth, nHeight, hSrcDC, X, Y, nWidth, nHeight, 0xFF);
		ReleaseDC(m_hWnd, hDstDC);
	}
	else
	{
		Win32GDI::TransparentPaintAlpha(hDstDC, X, Y, nWidth, nHeight, hSrcDC, X, Y, nWidth, nHeight, 0xFF);
	}

}
*/

VOID CSkinWIndow::DrawMaxButton(HDC hDC)
{
	INT X, Y, nWidth, nHeight;

	X = m_oMax->Area.left;
	Y = TitleBarHeight / 2;
	nWidth = m_oMax->Area.right - m_oMax->Area.left;
	nHeight = TitleBarHeight - 1;
	if (m_bMaximized)
	{
        Win32GDI::DrawRestore(hDC, X + 4, (nHeight - 8) / 2, 8, 0xFFFFFF);
	} 
	else
	{
		Win32GDI::DrawMax(hDC, X + 4, (nHeight - 8) / 2, 8, 0xFFFFFF);
	}
}


// 画最小化按钮
/*
VOID CSkinWIndow::DrawMinButton(TheObj *oButton, HDC hSrcDC, HDC hDstDC)
{
	INT X, Y, nWidth, nHeight;
	COLORREF clrBeginColor;
	COLORREF clrEndColor;

	if (hSrcDC == 0) hSrcDC = m_MemDC->GetDC();
	ObjStateConstants eButtonState = oButton->State;
	
	COLORREF c = HighlightColor;
	switch(eButtonState)
	{
	case osNormal:
		clrBeginColor = Win32GDI::AlphaColor(HighlightColor, Alpha);
		clrEndColor = Win32GDI::BlendColor(HighlightColor, 0x0, Alpha);
		break;
	case osHot:
		clrBeginColor = Win32GDI::AlphaColor(HighlightColor, Alpha);
		clrEndColor = Win32GDI::BlendColor(HighlightColor, 0x60, Alpha);
		break;
	}

	X = m_oMin->Area.left;
	Y = m_oMin->Area.top;
	nWidth = m_oMin->Area.right;
	nHeight = TitleBarHeight / 2 + 2;
	Win32GDI::DrawGradient(hSrcDC, X, Y, nWidth, nHeight, clrBeginColor, clrEndColor, Win32GDI::dTopToBottom);

	X = m_oMin->Area.left;
	Y = TitleBarHeight / 2;
	nWidth = m_oMin->Area.right;
	nHeight = TitleBarHeight - 1;
	Win32GDI::DrawGradient(hSrcDC, X, Y, nWidth, nHeight, clrEndColor, clrEndColor, Win32GDI::dTopToBottom);
    Win32GDI::DrawMin(hSrcDC, X + 4, nHeight / 2, 8, 0xFFFFFF);

	X = m_oMin->Area.left;
	Y = m_oMin->Area.top;
	nWidth = m_oMin->Area.right - m_oMin->Area.left;
	nHeight = m_oMin->Area.bottom - m_oMin->Area.top;
	if (hDstDC == 0)
	{
		hDstDC = GetWindowDC(m_hWnd);
		Win32GDI::TransparentPaintAlpha(hDstDC, X, Y, nWidth, nHeight, hSrcDC, X, Y, nWidth, nHeight, 0xFF);
		ReleaseDC(m_hWnd, hDstDC);
	}
	else
	{
		Win32GDI::TransparentPaintAlpha(hDstDC, X, Y, nWidth, nHeight, hSrcDC, X, Y, nWidth, nHeight, 0xFF);
	}
}
*/

VOID CSkinWIndow::DrawMinButton(HDC hDC)
{
	INT X, Y, nWidth, nHeight;
  
	X = m_oMin->Area.left;
	Y = TitleBarHeight / 2;
	nWidth = m_oMin->Area.right - m_oMin->Area.left;
	nHeight = TitleBarHeight - 1;
    Win32GDI::DrawMin(hDC, X + 4, nHeight / 2, 8, 0xFFFFFF);
}

// 画帮助按钮
VOID CSkinWIndow::DrawHelpButton(HDC hDC)
{
	INT X, Y, nWidth, nHeight;
	
	X = m_oHelp->Area.left;
	Y = m_oHelp->Area.top;
	nWidth = m_oHelp->Area.right - m_oHelp->Area.left;
	nHeight = TitleBarHeight - 1;
	Win32GDI::DrawAText(hDC, _T("?"), X, Y, nWidth, nHeight, 0xFFFFFF, Win32GDI::dtDefault);
}

// 画自定义按钮
VOID CSkinWIndow::DrawCustomButton(HDC hDC, TheObj *oButton)
{

}

// 打开抗锯齿
VOID CSkinWIndow::OpenAntiAliasedFont(HWND hWnd)
{
	if(hWnd)
	{
		LOGFONT lf;
		HFONT hFont = NULL;
		HFONT hNewFont = NULL;
		HWND hChildWnd = NULL;
		
		//主窗口
		hFont = (HFONT)SendMessage(hWnd, WM_GETFONT, 0, 0);
		if(hFont)
		{
			GetObject(hFont, sizeof(lf), &lf);
			lf.lfQuality = CLEARTYPE_QUALITY;//ANTIALIASED_QUALITIY=5抗锯齿 CLEARTYPE_QUALITIY=5清晰字体 CLEARTYPE_NATURAL_QUALITY=6
			hNewFont = CreateFontIndirect(&lf);
			SendMessage(hWnd, WM_SETFONT, (WPARAM)hNewFont, TRUE);
		}
		
		//子窗口
		hChildWnd = GetWindow(hWnd, GW_CHILD);
		while(hChildWnd != NULL)
		{
			hFont = (HFONT)SendMessage(hChildWnd, WM_GETFONT, 0, 0);
			if(hFont)
			{
				GetObject(hFont, sizeof(lf), &lf);
				lf.lfQuality = CLEARTYPE_QUALITY;
				hNewFont = CreateFontIndirect(&lf);
				SendMessage(hChildWnd, WM_SETFONT, (WPARAM)hNewFont, TRUE);
			}
			hChildWnd = GetNextWindow(hChildWnd, GW_HWNDNEXT);
		}
		
		//开启了抗锯齿功能
		m_bAntiAliased = TRUE;
	}
}

// 关闭抗锯齿
VOID CSkinWIndow::CloseAntiAliasedFont(HWND hWnd)
{
	if(hWnd)
	{
		HFONT hFont = NULL;
		HWND hChildWnd = NULL;
	
		//主窗口
		hFont = (HFONT)SendMessage(hWnd, WM_GETFONT, 0, 0);
		if(hFont) DeleteObject(hFont);
		
		//子窗口
		hChildWnd = GetWindow(hWnd, GW_CHILD);
		while(hChildWnd != NULL)
		{
			hFont = (HFONT)SendMessage(hChildWnd, WM_GETFONT, 0, 0);
			if(hFont) DeleteObject(hFont);

			hChildWnd = GetNextWindow(hChildWnd, GW_HWNDNEXT);
		}
	}
}

