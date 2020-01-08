// Skin.cpp : Implementation of CSkin
#include "stdafx.h"
#include "TSkin.h"
#include "Skin.h"

/////////////////////////////////////////////////////////////////////////////
// CSkin

//����
STDMETHODIMP CSkin::AboutMe()
{
	MessageBox(ABOUTME, COPYRIGHT, MB_ICONWARNING | MB_OK);
	return S_OK;
}

STDMETHODIMP CSkin::ApplySkin(long hWnd, VARIANT_BOOL *pRetVal)
{
	HWND hwnd = (HWND)hWnd;

	//��֤�����Ƿ���Ҫ����
	if (::IsWindow(hwnd))
	{
		if(!m_Skin->HasCaption(hwnd))
		{
			return S_FALSE;
		}
	}
	else
	{
		return S_FALSE;
	}
	
	//���໯����
	if (m_hWnd)
	{
		UnsubclassWindow();
	}
	if (::IsWindow(hwnd))
	{
		SubclassWindow(hwnd);
	}
	
	//Ϊ���ڴ�����Ӱ
	if (m_hWnd)
	{
		m_Shadow->Shadow(m_hWnd);
	}
	
	return S_OK;
}

STDMETHODIMP CSkin::get_TitleColor(OLE_COLOR *pVal)
{
	*pVal = m_Skin->TitleColor;
	return S_OK;
}
STDMETHODIMP CSkin::put_TitleColor(OLE_COLOR newVal)
{
	m_Skin->TitleColor = newVal;
	return S_OK;
}

STDMETHODIMP CSkin::get_GradientBeginColor(OLE_COLOR *pVal)
{
	*pVal = m_Skin->GradientBeginColor;
	return S_OK;
}
STDMETHODIMP CSkin::put_GradientBeginColor(OLE_COLOR newVal)
{
	m_Skin->GradientBeginColor = newVal;
	return S_OK;
}

STDMETHODIMP CSkin::get_GradientEndColor(OLE_COLOR *pVal)
{
	*pVal = m_Skin->GradientEndColor;
	return S_OK;
}
STDMETHODIMP CSkin::put_GradientEndColor(OLE_COLOR newVal)
{
	m_Skin->GradientEndColor = newVal;
	return S_OK;
}

STDMETHODIMP CSkin::get_BackColor(OLE_COLOR *pVal)
{
	*pVal = m_Skin->BackColor;
	return S_OK;
}
STDMETHODIMP CSkin::put_BackColor(OLE_COLOR newVal)
{
	m_Skin->BackColor = newVal;
	return S_OK;
}

STDMETHODIMP CSkin::get_TitleBarHeight(long *pVal)
{
	*pVal = m_Skin->TitleBarHeight;
	return S_OK;
}
STDMETHODIMP CSkin::put_TitleBarHeight(long newVal)
{
	if(newVal < 16) newVal = 16;
	if(newVal > 64) newVal = 64;
	m_Skin->TitleBarHeight = newVal;
	return S_OK;
}

STDMETHODIMP CSkin::get_BorderWidth(long *pVal)
{
	*pVal = m_Skin->BorderWidth;
	return S_OK;
}
STDMETHODIMP CSkin::put_BorderWidth(long newVal)
{
	if(newVal <= 0) newVal = 1;
	if(newVal > 8) newVal = 8;
	m_Skin->BorderWidth = newVal;
	return S_OK;
}

STDMETHODIMP CSkin::get_Alpha(unsigned char *pVal)
{
	*pVal = m_Skin->Alpha;
	return S_OK;
}
STDMETHODIMP CSkin::put_Alpha(unsigned char newVal)
{
	m_Skin->Alpha = newVal;
	return S_OK;
}

STDMETHODIMP CSkin::get_ShadowColor(OLE_COLOR *pVal)
{
	*pVal = m_Shadow->ShadowColor;
	return S_OK;
}
STDMETHODIMP CSkin::put_ShadowColor(OLE_COLOR newVal)
{
	m_Shadow->ShadowColor = newVal;
	return S_OK;
}

STDMETHODIMP CSkin::get_ShadowDepth(long *pVal)
{
	*pVal = m_Shadow->ShadowDepth;
	return S_OK;
}
STDMETHODIMP CSkin::put_ShadowDepth(long newVal)
{
	if(newVal < 0) newVal = 0;
	if(newVal > 32) newVal = 32;
	m_Shadow->ShadowDepth = newVal;
	return S_OK;
}

STDMETHODIMP CSkin::get_ShadowTransparency(unsigned char *pVal)
{
	*pVal = m_Shadow->Transparency;
	return S_OK;
}
STDMETHODIMP CSkin::put_ShadowTransparency(unsigned char newVal)
{
	m_Shadow->Transparency = newVal;
	return S_OK;
}

STDMETHODIMP CSkin::get_ShadowType(ShadowTypeConstants *pVal)
{
	*pVal = (ShadowTypeConstants)m_Shadow->ShadowType;
	return S_OK;
}
STDMETHODIMP CSkin::put_ShadowType(ShadowTypeConstants newVal)
{
	m_Shadow->ShadowType = (CShadowWindow::ShadowTypeConstants)newVal;
	return S_OK;
}

STDMETHODIMP CSkin::get_ShowMinButton(VARIANT_BOOL *pVal)
{
	*pVal = m_Skin->MinButton();
	return S_OK;
}
STDMETHODIMP CSkin::put_ShowMinButton(VARIANT_BOOL newVal)
{
	m_Skin->MinButton(newVal);
	return S_OK;
}

STDMETHODIMP CSkin::get_ShowMaxButton(VARIANT_BOOL *pVal)
{
	*pVal = m_Skin->MaxButton();
	return S_OK;
}
STDMETHODIMP CSkin::put_ShowMaxButton(VARIANT_BOOL newVal)
{
	m_Skin->MaxButton(newVal);
	return S_OK;
}

STDMETHODIMP CSkin::get_ShowHelpButton(VARIANT_BOOL *pVal)
{
	*pVal = m_Skin->HelpButton();
	return S_OK;
}
STDMETHODIMP CSkin::put_ShowHelpButton(VARIANT_BOOL newVal)
{
	m_Skin->HelpButton(newVal);
	return S_OK;
}

STDMETHODIMP CSkin::get_CloseButtonColor(OLE_COLOR *pVal)
{
	*pVal = m_Skin->CloseButtonColor;
	return S_OK;
}
STDMETHODIMP CSkin::put_CloseButtonColor(OLE_COLOR newVal)
{
	m_Skin->CloseButtonColor = newVal;
	return S_OK;
}

// STDMETHODIMP CSkin::get_FontSmoothing(VARIANT_BOOL *pVal)
// {
// 	*pVal = m_Skin->FontSmoothing;
// 	return S_OK;
// }
// STDMETHODIMP CSkin::put_FontSmoothing(VARIANT_BOOL newVal)
// {
// 	m_Skin->FontSmoothing = newVal;
// 	return S_OK;
// }

STDMETHODIMP CSkin::Transparent(unsigned char nAlpha, VARIANT_BOOL *pVal)
{
	*pVal = m_Skin->Transparent(nAlpha);
	return S_OK;
}

//ˢ��
STDMETHODIMP CSkin::Refresh()
{
	m_Skin->Refresh();
	return S_OK;
}

//��Ϣ����
LRESULT CSkin::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bEatIt)
{
	LRESULT lresult = 0;
	INT nHittest = 0;
	POINTS pts;

	switch(uMsg)
	{
	case WM_SHOWWINDOW:
		//1.��ʾ����
		m_Skin->OnShowWindow(m_hWnd);
		break;

	case WM_NCCALCSIZE:
		//2.�ı䴰�ڵĿ�����򣬰��������߿򣨱������߶ȣ�����߿򡢵ױ߿��ұ߿�
		{
			NCCALCSIZE_PARAMS *lpncsp = (LPNCCALCSIZE_PARAMS) lParam;
			m_Skin->OnNcCalcSize(lpncsp);
		}
		break;

	case WM_NCACTIVATE:
		//3.���ڱ�����
		{
			BOOL bActive = (BOOL)wParam;
			m_Skin->OnNcActivate(bActive);
			/*
			MSDN:When the fActive parameter is FALSE, an application should return TRUE to indicate that the system should proceed with the default processing,
			or it should return FALSE to prevent the title bar or icon from being deactivated.
			When fActive is TRUE, the return value is ignored. 
			��fActive����ΪFALSEʱ��Ӧ�ó���Ӧ�÷���TRUE����ʾϵͳӦ�ü���Ĭ�ϴ���
			������Ӧ�÷���FALSE���Է�ֹ��������ͼ�걻ͣ�á�
			��fActiveΪ��ʱ������ֵ�����ԡ�
			*/
			if(!bActive) //�Ǽ���״̬ʱ���뷵��TRUE, ������VB�в����л���IDE�н�����ͣ���ԡ�
			{
				bEatIt = TRUE;
				return TRUE;
			}
		}
		break;

	case WM_SIZE:
		//4.���ڳߴ�ı�
		{
			INT fwSizeType = (INT)wParam;
			INT nWidth = LOWORD(lParam);  // width of client area 
			INT nHeight = HIWORD(lParam); // height of client area 
			m_Skin->OnSize(nWidth, nHeight);

			//��Ӱ����
			if(fwSizeType == SIZE_RESTORED)
			{
				m_Shadow->OnSize();
			}
		}
		break;
	
	//���ù��ʱ
	case WM_SETCURSOR:
		{
			lresult = DefWindowProc(uMsg, wParam, lParam);
			nHittest = LOWORD(lParam);  // hit-test code 
			INT wMouseMsg = HIWORD(lParam); // mouse-message identifier 
			if((nHittest == HTCAPTION || nHittest == HTCLIENT) && wMouseMsg== WM_LBUTTONDOWN)
			{
				m_Skin->OnNcActivate(TRUE);
			}
		}
		break;

	//����ϵͳ����
	case WM_SYSCOMMAND:
		//m_Shadow->CloseShadows(); //Ϊ�˲������õ����ܣ��������������Ե��������ڳߴ�ʱ�ر���Ӱ
		lresult = DefWindowProc(uMsg, wParam, lParam);
		m_Skin->OnSystemCommand(wParam, lParam);
		//m_Shadow->OpenShadows();
		break;

	//�������ڳߴ�ʱ�޶����ڴ�С
	case WM_GETMINMAXINFO:
		{
			MINMAXINFO *lpmmi = (LPMINMAXINFO) lParam;
			m_Skin->OnGetMinMaxInfo(lpmmi);
		}
		break;
	
	//����ڷǿͻ����ƶ�������ǰ���������в���
	case WM_NCHITTEST:
		{
			INT nHittest = (INT) DefWindowProc(uMsg, wParam, lParam); //ȡ�����в���
			INT x = LOWORD(lParam);  // horizontal position of cursor 
			INT y = HIWORD(lParam);  // vertical position of cursor 
			return m_Skin->OnNcHitTest(nHittest, x, y);//�����������в���
		}
		break;
	
	//�ڷǿͻ����ƶ����
	case WM_NCMOUSEMOVE:
		//nHittest = (INT) wParam;    // hit-test value 
		pts = MAKEPOINTS(lParam);   // position of cursor 
		m_Skin->OnNcMouseMove(pts.x, pts.y);
		break;
	
	//�ڷǿͻ���˫�����
	case WM_NCLBUTTONDBLCLK:
		//nHittest = (INT) wParam;    // hit-test value 
		pts = MAKEPOINTS(lParam);   // position of cursor 
		m_Skin->OnNcLButtonDblClk(pts.x, pts.y);
		break;
	
	//�ڷǿͻ����������
	case WM_NCLBUTTONDOWN:
		nHittest = (INT) wParam;    // hit-test value 
		switch(nHittest)
		{
		case HTLEFT:
		case HTTOP:
		case HTBOTTOM:
		case HTRIGHT:
		case HTBOTTOMLEFT:
		case HTBOTTOMRIGHT:
		case HTTOPLEFT:
		case HTTOPRIGHT:
		case HTSYSMENU:
			lresult = DefWindowProc(uMsg, wParam, lParam);
			break;

		case HTCAPTION:
			lresult = DefWindowProc(uMsg, wParam, lParam);
			pts = MAKEPOINTS(lParam);   // position of cursor 
			m_Skin->OnNcLButtonDown(pts.x, pts.y);
			break;

		default:
			break;
		}
		break;

	//����ͼ��ͱ���
	case WM_SETICON:
	case WM_SETTEXT:
		lresult = DefWindowProc(uMsg, wParam, lParam);
		m_Skin->OnNcPaint();
		break;
		
	//���Ʒǿͻ���
	case WM_NCPAINT:
		{
			m_Skin->OnNcPaint();
		}
		break;
	
	//���ƿͻ���
	//���VB��������Label�ؼ�,�û�ˢ��䱳��ʱLabel�����ݱ�����,
	//����ԭ��������Label���޴��ڿؼ���VB����ӦWM_PAINTʱ��Label���ݻ�����DC��,
	//��ʱ������û�ˢ���DC������DC�ϵ�Label���ݽ������ǣ�
	//��˴��ڱ���ɫ���ڴ�����, Ӧ���ɸ���Ӧ�ó����Լ����á�
	//case WM_PAINT:
	//	m_Skin->OnPaint();
	//	break;
	
	//��Ӱ����
	case WM_WINDOWPOSCHANGED:
		{
			lresult = DefWindowProc(uMsg, wParam, lParam);
			WINDOWPOS *lpwp = (LPWINDOWPOS) lParam;
			m_Shadow->OnWindowPosChanged(lpwp);
		}
		break;

	//ϵͳ���øı�
	case 0x031A://WM_THEMECHANGED Windwos����ı�
	case 0x031E://WM_DWMCOMPOSITIONCHANGED Vista������ϵͳ������������ϡ�ѡ��ı�
	case WM_SETTINGCHANGE: //ϵͳ���øı�
		m_Skin->OnSettingChange();
		m_Shadow->OnSettingChange();
		break;
	
	default:
		return DefWindowProc(uMsg, wParam, lParam);

	}

	bEatIt = TRUE;
	return 0;

}