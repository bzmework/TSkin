#include "stdafx.h" 
#include "win32gdi.h"
#include "SkinWIndow.h"

/*
ע�⣺
��Windows XP�У��������óɡ�Windows XP��ʱ�����Region��Դй©��
��Ӧ����XP����ϵͳ��bug���ڡ�����ѡ���ȥ�����ڴ��ںͰ�ť��ʹ���Ӿ���ʽ����Region��Դй©��ʧ��
������CreateRoundRectRgn������ɵġ�
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

	//�����ڴ�DC
	m_MemDC.Destroy();

}

/*
�����Ƿ��б�����
*/
BOOL CSkinWIndow::HasCaption(HWND hWnd) const
{
	LONG dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	return (dwStyle & WS_CAPTION) == WS_CAPTION;
}

/*
�Ƿ�����󻯰�ť
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
�Ƿ�����С����ť
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
�Ƿ��а�����ť
*/		
BOOL CSkinWIndow::HelpButton() const
{
	return m_bHelpButton;
}	
VOID CSkinWIndow::HelpButton(BOOL bValue)
{
	m_bHelpButton = bValue;
}

/*�ô���͸��*/
BOOL CSkinWIndow::Transparent(BYTE nAlpha)
{
	BOOL bOk = FALSE;
	if(m_hWnd)
	{
		HDC hDC = GetDC(NULL);//�����ĻDC 
		BOOL bIsLayered = (GetDeviceCaps(hDC, BITSPIXEL) >= 16); //�Ƿ�֧�ֲַ�
		ReleaseDC(NULL, hDC);//�����ͷţ���������DCй©
		if(bIsLayered)
		{
			INT dwExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
			if ((dwExStyle & WS_EX_LAYERED) == WS_EX_LAYERED)
			{
				bOk = SetLayeredWindowAttributes(m_hWnd, 0, nAlpha, LWA_ALPHA); //���ķֲ㴰�ڵ�͸����
			}
			else
			{
				SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle | WS_EX_LAYERED);//���÷ֲ㴰��
				bOk = SetLayeredWindowAttributes(m_hWnd, 0, nAlpha, LWA_ALPHA); //�÷ֲ㴰����Ч�������ķֲ㴰�ڵ�͸����
			}
		}
	}
	return bOk;
}

//ˢ��
VOID CSkinWIndow::Refresh()
{
	this->OnNcPaint();
}

/*
��ʾ����

MSDN: The WM_SHOWWINDOW message is sent to a window when the window is about to be hidden or shown. 
�����ڼ��������ػ���ʾʱ��WM_SHOWWINDOW��Ϣ�����͵����ڡ�

 WM_SHOWWINDOW 
 fShow = (BOOL) wParam;      // show/hide flag 
 fnStatus = (int) lParam;    // status flag 

*/
VOID CSkinWIndow::OnShowWindow(HWND hWnd)
{
	if(!m_bFirstShow)
	{
		m_hWnd = hWnd;
		
		//��ʼ�����ڿ��
		this->InitFrame(hWnd);

		//���ô��ڿ��
		//this->ResetFrame(m_bMaximized);

		//��һ����ʾ
		m_bFirstShow = TRUE;
	}

}

/*
���ô��ڿ�����򣬰��������߿򣨱������߶ȣ�����߿򡢵ױ߿��ұ߿�
*/
/*
VOID CSkinWIndow::OnNcCalcSize(RECT* lpRect)
{
	lpRect->left += BorderWidth;	//��߿�λ��
	lpRect->top += TitleBarHeight;	//�����������߶�
	lpRect->right -= BorderWidth + 1;	//�ұ߿�λ��
	lpRect->bottom -= BorderWidth + 1;	//�ױ߿�λ��
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
����ϵͳ����

MSDN: A window receives this message when the user chooses a command from the window menu (formerly known as the system or control menu) or when the user chooses the maximize button, minimize button, restore button, or close button.
���û��Ӵ��ڲ˵�(��ǰ��Ϊϵͳ����Ʋ˵�)��ѡ������ʱ�����ߵ��û�������󻯰�ť����С����ť����ԭ��ť��رհ�ťʱ�����ڽ��մ���Ϣ��

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
    case SC_RESTORE: //�ָ�����
        this->ResetFrame(FALSE);
		break;

	case SC_MAXIMIZE: //��󻯴���
		this->ResetFrame(TRUE);
		break;

	case 61441: //���ڴ�С�������
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
��ô�����С��󻯻�λ�øı�ʱ��������������

MSDN��The WM_GETMINMAXINFO message is sent to a window when the size or position of the window is about to change. 
An application can use this message to override the window's default maximized size and position, 
or its default minimum or maximum tracking size. 
�����ڵĴ�С��λ�ü�������ʱ��WM_GETMINMAXINFO��Ϣ�����͵����ڡ�
Ӧ�ó������ʹ�ô���Ϣ���Ǵ��ڵ�Ĭ������С��λ�ã�
����Ĭ����С�������ٴ�С��

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
	��������ʾ�������Ϲ�������Ĵ�С��
	����������Ļ��û�б�ϵͳ��������Ӧ�ó������湤�����ڵ��ĵĲ��֡�
	pvParam��������ָ��һ��RECT�ṹ���ýṹ������������Ļ�����ʾ�Ĺ��������ꡣ
	��Ҫ��ȡ����ʾ���������ʾ���Ĺ������������getmonitorinfo������
	*/
    SystemParametersInfo(SPI_GETWORKAREA, 0, &tR, 0);
	OffsetRect(&tR, -tR.left, -tR.top);
	lpMMI->ptMaxPosition.x = tR.left;
	lpMMI->ptMaxPosition.y = tR.top;
	lpMMI->ptMaxSize.x = tR.right - tR.left;
	lpMMI->ptMaxSize.y = tR.bottom - tR.top;

}

/*
���ڵ������ڴ�С
MSDN: The WM_SIZE message is sent to a window after its size has changed. 
WM_SIZE��Ϣ�����С���ĺ󱻷��͵����ڡ�

WM_SIZE 
fwSizeType = wParam;      // resizing flag 
nWidth = LOWORD(lParam);  // width of client area 
nHeight = HIWORD(lParam); // height of client area 

fwSizeType:

SIZE_MAXHIDE 
Message is sent to all pop-up windows when some other window is maximized. 
�������������ʱ����Ϣ�����͵����е������ڡ�

SIZE_MAXIMIZED 
The window has been maximized. 
����������ˡ�

SIZE_MAXSHOW
Message is sent to all pop-up windows when some other window has been restored to its former size. 
���������ڻָ�����ǰ�Ĵ�Сʱ���������е������ڷ�����Ϣ��

SIZE_MINIMIZED 
The window has been minimized. 
��������С���ˡ�

SIZE_RESTORED 
The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies. 
�����Ѿ������˴�С������SIZE_MINIMIZED��SIZE_MAXIMIZEDֵ�������á�

*/
VOID CSkinWIndow::OnSize(INT nWidth, INT nHeight)
{
	if(m_hWnd)
	{
		this->ResetFrame(m_bMaximized);
	}
}

/*
�ǻ������ꡰ���в��ԡ�,����굥���˴��ڵ��ĸ���λ��Windows�������������Ĳ�λ�����ڷ�����Ӧ����Ϣ�����磺
���WM_NCHITTEST����Ϣ��Ӧ�����ķ���ֵ��HTCLIENT����ʾ��������ǿͻ�������Windows���򴰿ڷ���WM_LBUTTONDOWN��Ϣ��
���WM_NCHITTEST����Ϣ��Ӧ�����ķ���ֵ����HTCLIENT��������HTCAPTION��HTCLOSE��HTMAXBUTTON�ȣ�������������Ƿǿͻ�����Windows�ͻ��򴰿ڷ���WM_NCLBUTTONDOWN��Ϣ��

�ޱ��ⴰ���϶�������һ���ǲ��ã���ӦWM_NCHITTEST��Ϣ������HTCAPTION��ʵ�֡�
���ǣ��������ͬʱ��Ҫ����������Ϣ����WM_LBUTTONDOWN��WM_LBUTTONUP��WM_RBUTTONUP��WM_MOUSEMOVE����ᷢ�֣���Щ��Ϣ���ղ����ˡ�
ԭ��������WM_NCHITTEST�д����������Ϣ��������λ��HTCAPTION��Ҳ��������ڱ������ϣ������������ڷǿͻ�����NC��;
�ǿͻ������¼���Ϣ������WM_NC��ͷ�ģ�Ҳ����˵�������WM_NCHITTEST����HTCAPTIONʱ��ԭ��������WM_LBUTTONUP�������Ϣ����ֻ����WM_NCLBUTTONUP������
������Ҫ���������Ϣ��Ȼ��ͬʱ����WM_NCHITTEST��WM_NCRBUTTONUP����������WM_RBUTTONUP�ȡ�


MSDN��The WM_NCHITTEST message is sent to a window when the cursor moves, or when a mouse button is pressed or released. 
If the mouse is not captured, the message is sent to the window beneath the cursor. 
Otherwise, the message is sent to the window that has captured the mouse. 
������ƶ�ʱ�����ߵ���갴ť�����»��ͷ�ʱ��WM_NCHITTEST��Ϣ�����͵����ڡ�
���û�в�����꣬����Ϣ�����͵�����·��Ĵ��ڡ�
������Ϣ�����͵��������Ĵ��ڡ�

WM_NCHITTEST 
xPos = LOWORD(lParam);  // horizontal position of cursor 
yPos = HIWORD(lParam);  // vertical position of cursor 

*/
UINT CSkinWIndow::OnNcHitTest(UINT nHitTest, INT X, INT Y)
{
	switch (nHitTest)
	{
	//�ڹرա���󻯡���С������������������ʱ������ꡰ���в��ԡ����óɱ�������,
	//������Windows�ػ�رա���󻯡���С����������ť��
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
�ڷǻ�����ƶ����

MSDN��The WM_NCMOUSEMOVE message is posted to a window when the cursor is moved within the nonclient area of the window. This message is posted to the window that contains the cursor. If a window has captured the mouse, this message is not posted. 
������ƶ������ڵķǿͻ�������ʱ��WM_NCMOUSEMOVE��Ϣ�����͵����ڡ�����Ϣ���������������Ĵ��ڡ�������ڲ�������꣬�򲻻ᷢ������Ϣ��

WM_NCMOUSEMOVE 
nHittest = (INT) wParam;    // hit-test value 
pts = MAKEPOINTS(lParam);   // position of cursor 

*/
/*
VOID CSkinWIndow::OnNcMouseMove(INT X, INT Y)
{
	POINT pt;
	RECT rc;
	TheObj *oCurrButton = NULL; //��ǰ��ť

	GetWindowRect(m_hWnd, &rc);
	pt.x = X - rc.left - 1;
	pt.y = Y - rc.top;
	
	//�������λ��
	if (PtInRect(&m_oIcon->Area, pt) && m_eMousePos != htSysMenu)//��ͼ���ϵͳ�˵�����
	{
		m_eMousePos = htSysMenu;
	}
	else if (PtInRect(&m_oTitle->Area, pt) && m_eMousePos != htCaption)//�ڱ���������
	{
		m_eMousePos = htCaption;
	}
	else if (PtInRect(&m_oClose->Area, pt) && m_eMousePos != htClose) //�ڹرհ�ť����
	{
		m_eMousePos = htClose;
		m_oClose->Type = otClose;
		oCurrButton = m_oClose;
	}
	else if (m_bMaxButton && PtInRect(&m_oMax->Area, pt) && m_eMousePos != htMaxButton) //����󻯰�ť����
	{
		m_eMousePos = htMaxButton;
		m_oMax->Type = otMax;
		oCurrButton = m_oMax;
	}
	else if (m_bMinButton && PtInRect(&m_oMin->Area, pt) && m_eMousePos != htMinButton) //����С����ť����
	{
		m_eMousePos = htMinButton;
		m_oMin->Type = otMin;
		oCurrButton = m_oMin;
	}
	else if(m_oCustomBtn1)
	{
		if (PtInRect(&m_oCustomBtn1->Area, pt) && m_eMousePos != htCustom) //���Զ��尴ť����
		{
			m_eMousePos = htCustom;
			m_oCustomBtn1->Type = otCustomButton;
			oCurrButton = m_oCustomBtn1;
		}
	}
	else if(m_oCustomBtn2) 
	{
		if (PtInRect(&m_oCustomBtn2->Area, pt) && m_eMousePos != htCustom) //���Զ��尴ť����
		{
			m_eMousePos = htCustom;
			m_oMax->Type = otCustomButton;
			oCurrButton = m_oCustomBtn2;
		}
	}
	else if(m_oCustomBtn3)
	{
		if (PtInRect(&m_oCustomBtn3->Area, pt) && m_eMousePos != htCustom) //���Զ��尴ť����
		{
			m_eMousePos = htCustom;
			m_oMax->Type = otCustomButton;
			oCurrButton = m_oCustomBtn3;
		}
	}
	else if(m_oCustomBtn4)
	{
		if (PtInRect(&m_oCustomBtn4->Area, pt) && m_eMousePos != htCustom) //���Զ��尴ť����
		{
			m_eMousePos = htCustom;
			m_oMax->Type = otCustomButton;
			oCurrButton = m_oCustomBtn4;
		}
	}
	else if(m_oCustomBtn5)
	{
		if (PtInRect(&m_oCustomBtn5->Area, pt) && m_eMousePos != htCustom) //���Զ��尴ť����
		{
			m_eMousePos = htCustom;
			m_oMax->Type = otCustomButton;
			oCurrButton = m_oCustomBtn5;
		}
	}
	
	//������һ����ť
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
	
	//���Ƶ�ǰ��ť
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
	
	//�������λ��
	if (PtInRect(&m_oIcon->Area, pt) && m_eMousePos != htSysMenu)//��ͼ���ϵͳ�˵�����
	{
		m_eMousePos = htSysMenu;
	}
	else if (PtInRect(&m_oTitle->Area, pt) && m_eMousePos != htCaption)//�ڱ���������
	{
		m_eMousePos = htCaption;
	}
	else if (PtInRect(&m_oClose->Area, pt) && m_eMousePos != htClose) //�ڹرհ�ť����
	{
		m_eMousePos = htClose;
	}
	else if (m_bMaxButton && PtInRect(&m_oMax->Area, pt) && m_eMousePos != htMaxButton) //����󻯰�ť����
	{
		m_eMousePos = htMaxButton;
	}
	else if (m_bMinButton && PtInRect(&m_oMin->Area, pt) && m_eMousePos != htMinButton) //����С����ť����
	{
		m_eMousePos = htMinButton;
	}
	else if(m_oCustomBtn1)
	{
		if (PtInRect(&m_oCustomBtn1->Area, pt) && m_eMousePos != htCustom) //���Զ��尴ť����
		{
			m_eMousePos = htCustom;
		}
	}
	else if(m_oCustomBtn2) 
	{
		if (PtInRect(&m_oCustomBtn2->Area, pt) && m_eMousePos != htCustom) //���Զ��尴ť����
		{
			m_eMousePos = htCustom;
		}
	}
	else if(m_oCustomBtn3)
	{
		if (PtInRect(&m_oCustomBtn3->Area, pt) && m_eMousePos != htCustom) //���Զ��尴ť����
		{
			m_eMousePos = htCustom;
		}
	}
	else if(m_oCustomBtn4)
	{
		if (PtInRect(&m_oCustomBtn4->Area, pt) && m_eMousePos != htCustom) //���Զ��尴ť����
		{
			m_eMousePos = htCustom;
		}
	}
	else if(m_oCustomBtn5)
	{
		if (PtInRect(&m_oCustomBtn5->Area, pt) && m_eMousePos != htCustom) //���Զ��尴ť����
		{
			m_eMousePos = htCustom;
		}
	}

}

/*
˫������������

The WM_NCLBUTTONDBLCLK message is posted when the user double-clicks the left mouse button while the cursor is within the nonclient area of a window. This message is posted to the window that contains the cursor. If a window has captured the mouse, this message is not posted. 
���û�˫��������ʱ��WM_NCLBUTTONDBLCLK��Ϣ�������������λ�ڴ��ڵķǿͻ��������ڡ�����Ϣ�������������α�Ĵ��ڡ�������ڲ�������꣬�򲻻ᷢ������Ϣ��

WM_NCLBUTTONDBLCLK 
nHittest = (INT) wParam;    // hit-test value 
pts = MAKEPOINTS(lParam);   // position of cursor 
	
*/
VOID CSkinWIndow::OnNcLButtonDblClk(INT X, INT Y)
{
	if (m_eMousePos == htCaption && m_bMaxButton) //˫������������󻯻�ָ�����
	{
		(m_bMaximized)?
		SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0):
		SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}
	else if(m_eMousePos == htSysMenu) //˫��ϵͳ�˵����رմ���
	{
		SendMessage(m_hWnd, WM_CLOSE, 0, 0);
	}
}

/*
����������������
MSDN: The WM_NCLBUTTONDOWN message is posted when the user presses the left mouse button while the cursor is within the nonclient area of a window. 
This message is posted to the window that contains the cursor. If a window has captured the mouse, this message is not posted. 
���û��ڴ��ڵķǿͻ��������ڰ���������ʱ��������WM_NCLBUTTONDOWN��Ϣ��
����Ϣ�������������α�Ĵ��ڡ�������ڲ�������꣬�򲻻ᷢ������Ϣ��

WM_NCLBUTTONDOWN 
nHittest = (INT) wParam;    // hit-test value 
pts = MAKEPOINTS(lParam);   // position of cursor 


*/
VOID CSkinWIndow::OnNcLButtonDown(INT X, INT Y)
{
	if (m_eMousePos == htClose) //�����رհ�ť
	{
		SendMessage(m_hWnd, WM_CLOSE, 0, 0);
	}
	else if (m_eMousePos == htMaxButton) //������󻯰�ť
	{
		if (m_bMaximized) //�Ѿ������ָ�
		{
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		else
		{
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
	}
	else if (m_eMousePos == htMinButton) //������С����ť
	{
		SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
}

/*
���ƴ��ڿ�ܣ�����������������߿��ұ߿򡢵ױ߿�

MSDN��An application sends the WM_NCPAINT message to a window when its frame must be painted. 
Ӧ�ó����ڱ�����ƿ��ʱ�򴰿ڷ���WM_NCPAINT��Ϣ��

*/
VOID CSkinWIndow::OnNcPaint()
{
	if(m_hWnd)
	{
		//HDC hDC = GetDCEx(m_hWnd, hRgn, DCX_WINDOW | DCX_INTERSECTRGN);
		HDC hDC = GetWindowDC(m_hWnd);
		this->DrawFrame(hDC);//�����ڿ��
		ReleaseDC(m_hWnd, hDC);
	}
}

/*
���ƿͻ���
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
ϵͳ���øı�
*/
VOID CSkinWIndow::OnSettingChange()
{
	if(m_hWnd)
	{
		InvalidateRect(m_hWnd, NULL, TRUE);//�ػ���������
	}
}

/*
���ڼ���
*/
VOID CSkinWIndow::OnNcActivate(BOOL bActive)
{
	if(m_hWnd && m_bActive != bActive) //��ֹ��������
	{
		m_bActive = bActive;
		this->OnNcPaint();
	}
}


/*
��ʼ�����ڿ��
*/
VOID CSkinWIndow::InitFrame(HWND hWnd)
{
	//��ô�������
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
	
	//��ô���ͼ��
	if(Icon == NULL)
	{
		//Icon = (HICON)GetClassLong(hWnd, GCL_HICON);
		Icon = (HICON)SendMessage(hWnd, 0x7F, 0x2, 0);
	}
	m_oIcon->Icon = Icon;
	
	//��ô��ڱ���
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
	
	//ȥ��ϵͳ�˵��������ȥ���˵�������������ʱϵͳ���ػ���С����󻯹رհ�ť�����ô���
	SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_SYSMENU);

	//Ϊ����������Ӱ
	//SPI_SETDROPSHADOW = 0x1025
	//SystemParametersInfo(0x1025, 0, (PVOID)TRUE, 0);// - ����ӰЧ��
	//CS_DROPSHADOW = 0x20000
	//SetClassLong(hWnd, GCL_STYLE, GetClassLong(hWnd, GCL_STYLE) | 0x20000);
	
	//�Ƿ�����ClearType��Windows XP�ڿ�����塰ClearType Tuning��������
	//SPI_GETCLEARTYPE = 0x1048
	//SPI_SETCLEARTYPE = 0x1049
	//BOOL bClearType = FALSE;
	//SystemParametersInfo(0x1048, 0, &bClearType, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
	//if(!bClearType)
	//{
	//	SystemParametersInfo(0x1048, TRUE, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE); //����ClearType
	//}
	
	//��������ƽ������, ��������ƽ����Ļ�����Ե������ѡ��
	//SPI_GETFONTSMOOTHING = 0x004A
	//SPI_SETFONTSMOOTHING = 0x004B
	BOOL bFontSmoothing = FALSE;
	SystemParametersInfo(SPI_GETFONTSMOOTHING, 0, &bFontSmoothing, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
	if(!bFontSmoothing && this->FontSmoothing)
	{
		//SystemParametersInfo(SPI_SETFONTSMOOTHING, TRUE, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE); //��������ƽ������
		this->OpenAntiAliasedFont(hWnd); //�����д��ڵ�����֧��ƽ������
	}

	//�رմ��������С������
	//ai.iMinAnimate = 0;
	//SystemParametersInfo(SPI_SETANIMATION, sizeof(ANIMATIONINFO), &ai, NULL); 

}

// ���贰�ڿ��
VOID CSkinWIndow::ResetFrame(BOOL bMaximized)
{
	RECT tR;
	
	m_bMaximized = bMaximized;
	GetWindowRect(m_hWnd, &tR);

	// ��������
    m_oWindow->Area.left = 0;
	m_oWindow->Area.top = 0;
	m_oWindow->Area.right = tR.right - tR.left;
	m_oWindow->Area.bottom = tR.bottom - tR.top;

	// �رհ�ť����
	m_oClose->Area.left = (m_oWindow->Area.right - m_oWindow->Area.left) - BUTTON_SIZE - 5;
	m_oClose->Area.top = 1;
	m_oClose->Area.right = m_oWindow->Area.right - m_oWindow->Area.left;
	m_oClose->Area.bottom = TitleBarHeight - 1;

	// ��󻯰�ť����
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

	// ��С����ť����
	if (m_bMinButton)
	{
		if(!m_oMin)
		{
			m_oMin = new TheObj(otMin);
		}

		if (m_oMax) //����󻯰�ť
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
	
	//������ť����
	if (m_bHelpButton)
	{
		if(!m_oHelp)
		{
			m_oHelp = new TheObj(otHelp);
		}
		
		if (m_oMin) //����С����ť
		{
			m_oHelp->Area.left = m_oMin->Area.left - BUTTON_SIZE + 4;
			m_oHelp->Area.top = 1;
			m_oHelp->Area.right = m_oMin->Area.left;
			m_oHelp->Area.bottom = m_oMin->Area.bottom;
		}
		else
		{
			if (m_oMax) //����󻯰�ť
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

	// ͼ������
	m_oIcon->Area.left = 5;
	m_oIcon->Area.top = 1;
	m_oIcon->Area.right = m_oIcon->Area.left + ICON_WIDTH;
	m_oIcon->Area.bottom = TitleBarHeight - 1;

	// ��������
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

	//������Ļ��С�����ڴ�DC
	m_MemDC.Create(tR.right - tR.left + BorderWidth * 2, tR.bottom - tR.top + BorderWidth * 2);

	// �������δ���
	// CreateRoundRectRgn�����ᵼ������й¶����֪��Ϊʲô
	//HRGN hRgn = CreateRoundRectRgn(0, 0, tR.right - tR.left, tR.bottom - tR.top, 0, 0);
	HRGN hRgn = CreateRectRgn(0, 0, tR.right - tR.left, tR.bottom - tR.top);
	//If the bRedraw parameter is TRUE, the system sends theWM_WINDOWPOSCHANGING and WM_WINDOWPOSCHANGED messages to the window. 
	//After a successful call to SetWindowRgn, the system owns the region specified by the region handle hRgn. 
	//The system does not make a copy of the region. Thus, you should not make any further function calls with this region handle. 
	//In particular, do not close this region handle. 
	SetWindowRgn(m_hWnd, hRgn, TRUE);
	DeleteObject(hRgn);

}

// �����ڿ��
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

	//��䱳��
	X = m_oWindow->Area.left;
	Y = m_oWindow->Area.top;
	nWidth = m_oWindow->Area.right - m_oWindow->Area.left;
 	nHeight = m_oWindow->Area.bottom - m_oWindow->Area.top;
	Win32GDI::DrawArea(hMemDC, X, Y, nWidth, nWidth, BackColor);

	//������������
	X = 0;
	Y = 0;
	nWidth = m_oWindow->Area.right - m_oWindow->Area.left;
	nHeight = TitleBarHeight;
	Win32GDI::DrawGradient(hMemDC, X, Y, nWidth, nHeight, clrBeginColor, clrEndColor, Win32GDI::dLeftToRight);

	//���رհ�ť
	if(m_oClose)
	{
		this->DrawCloseButton(hMemDC);
	}

	//����󻯰�ť
	if(m_oMax)
	{
		this->DrawMaxButton(hMemDC);
	}

	//����С����ť
	if(m_oMin)
	{
		this->DrawMinButton(hMemDC);
	}

	//��������ť
	if(m_oHelp)
	{
		this->DrawHelpButton(hMemDC);
	}

	//���Զ��尴ť
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

	//��������ͼ��
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
	
	//���������ı�
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
	
	//����������
	X = 0;
	Y = 0;
	nWidth = m_oWindow->Area.right - m_oWindow->Area.left;
	nHeight = TitleBarHeight;
	BitBlt(hDC, X, Y, nWidth, nHeight, hMemDC, X, Y, SRCCOPY);

	//����߿�
	X = 0;
	Y = TitleBarHeight;
	nWidth = BorderWidth;
	nHeight = m_oWindow->Area.bottom - m_oWindow->Area.top - TitleBarHeight;
	Win32GDI::DrawGradient(hMemDC, X, Y, nWidth, nHeight, clrBeginColor, clrEndColor, Win32GDI::dTopToBottom);
	BitBlt(hDC, X, Y, nWidth, nHeight, hMemDC, X, Y, SRCCOPY);

	//���ױ߿�
	X = BorderWidth;
	Y = m_oWindow->Area.bottom - BorderWidth;
	nWidth = m_oWindow->Area.right - m_oWindow->Area.left - 2 * BorderWidth;
	nHeight = BorderWidth;
	Win32GDI::DrawGradient(hMemDC, X, Y, nWidth, nHeight, clrEndColor, clrBeginColor, Win32GDI::dLeftToRight);
	BitBlt(hDC, X, Y, nWidth, nHeight, hMemDC, X, Y, SRCCOPY);

	//���ұ߿�
	X = m_oWindow->Area.right - BorderWidth;
	Y = TitleBarHeight;
	nWidth = BorderWidth;
	nHeight = m_oWindow->Area.bottom - m_oWindow->Area.top - TitleBarHeight;
	Win32GDI::DrawGradient(hMemDC, X, Y, nWidth, nHeight, clrEndColor, clrBeginColor, Win32GDI::dTopToBottom);
	BitBlt(hDC, X, Y, nWidth, nHeight, hMemDC, X, Y, SRCCOPY);
 
}

// ���رհ�ť
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

// ����󻯰�ť
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


// ����С����ť
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

// ��������ť
VOID CSkinWIndow::DrawHelpButton(HDC hDC)
{
	INT X, Y, nWidth, nHeight;
	
	X = m_oHelp->Area.left;
	Y = m_oHelp->Area.top;
	nWidth = m_oHelp->Area.right - m_oHelp->Area.left;
	nHeight = TitleBarHeight - 1;
	Win32GDI::DrawAText(hDC, _T("?"), X, Y, nWidth, nHeight, 0xFFFFFF, Win32GDI::dtDefault);
}

// ���Զ��尴ť
VOID CSkinWIndow::DrawCustomButton(HDC hDC, TheObj *oButton)
{

}

// �򿪿����
VOID CSkinWIndow::OpenAntiAliasedFont(HWND hWnd)
{
	if(hWnd)
	{
		LOGFONT lf;
		HFONT hFont = NULL;
		HFONT hNewFont = NULL;
		HWND hChildWnd = NULL;
		
		//������
		hFont = (HFONT)SendMessage(hWnd, WM_GETFONT, 0, 0);
		if(hFont)
		{
			GetObject(hFont, sizeof(lf), &lf);
			lf.lfQuality = CLEARTYPE_QUALITY;//ANTIALIASED_QUALITIY=5����� CLEARTYPE_QUALITIY=5�������� CLEARTYPE_NATURAL_QUALITY=6
			hNewFont = CreateFontIndirect(&lf);
			SendMessage(hWnd, WM_SETFONT, (WPARAM)hNewFont, TRUE);
		}
		
		//�Ӵ���
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
		
		//�����˿���ݹ���
		m_bAntiAliased = TRUE;
	}
}

// �رտ����
VOID CSkinWIndow::CloseAntiAliasedFont(HWND hWnd)
{
	if(hWnd)
	{
		HFONT hFont = NULL;
		HWND hChildWnd = NULL;
	
		//������
		hFont = (HFONT)SendMessage(hWnd, WM_GETFONT, 0, 0);
		if(hFont) DeleteObject(hFont);
		
		//�Ӵ���
		hChildWnd = GetWindow(hWnd, GW_CHILD);
		while(hChildWnd != NULL)
		{
			hFont = (HFONT)SendMessage(hChildWnd, WM_GETFONT, 0, 0);
			if(hFont) DeleteObject(hFont);

			hChildWnd = GetNextWindow(hChildWnd, GW_HWNDNEXT);
		}
	}
}

