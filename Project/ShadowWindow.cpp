#include "stdafx.h" 
#include "win32gdi.h"
#include "ShadowWIndow.h"

/*
	����������
*/

CShadowWindow::CShadowWindow()
{
	m_hWnd = NULL;
	m_hWndLt = NULL;
	m_hWndTt = NULL;
	m_hWndBt = NULL;
	m_hWndRt = NULL;
	m_bIsLayered = FALSE;
	m_bLastShow = FALSE;
	m_cx = 0;
	m_cy = 0;

	ShadowColor = 0x000000;
	ShadowDepth = DEF_DEPTH;
	Transparency = DEF_TRANSPARENCY;
	ShadowType = stDefaultShadow;
	m_bIsDwmCompLayered = FALSE;
	m_bIsOpend = TRUE;

}

CShadowWindow::~CShadowWindow()
{
	this->DestroyShadows();
}

/*
	����ӿ�
*/

//Ӧ����Ӱ
BOOL CShadowWindow::Shadow(HWND hWnd)
{
	m_hWnd = hWnd;

	this->CheckEnvironment();

	if(m_bIsLayered)
	{
		this->DestroyShadows();
		this->CreateShadows();
	}
	return (m_hWndRt != NULL);
}

//����Ӱ
VOID CShadowWindow::OpenShadows()
{
	m_bIsOpend = TRUE;
	if(m_bIsLayered)
	{
		this->ShowShadows(TRUE, TRUE);
	}
}		
	
//�ر���Ӱ
VOID CShadowWindow::CloseShadows()
{
	m_bIsOpend = FALSE;
	if(m_bIsLayered)
	{
		this->ShowShadows(FALSE, TRUE);
	}
}
	
//Vista������ϵͳ�����ˡ�����������ϡ�+������󻯺���С��ʱ��̬��ʾ���ڡ�����ѡ��
BOOL CShadowWindow::IsDwmCompLayered()
{
	return m_bIsDwmCompLayered;
}
	
/*
	�¼�����
*/

//�����ڴ�С��λ�á�Z��˳��ı�ʱ��������Ӱλ�ã���������ʾ��Ӱ
VOID CShadowWindow::OnWindowPosChanged(WINDOWPOS *lpwp)
{
	if(m_bIsLayered)
	{
		if(!IsWindowVisible(m_hWnd))
		{
			this->ShowShadows(FALSE);
		}
		else
		{
			CopyMemory(&m_wp, lpwp, sizeof(WINDOWPOS));
			this->AdjustShadowPos();
			if(m_bIsOpend)
			{
				this->ShowShadows(TRUE);
			}
		}
	}
}

//�����ڴ�С�ı�ʱ, ������ʾ��Ӱ
VOID CShadowWindow::OnSize()
{
	if(m_bIsLayered && m_bIsOpend && IsWindowVisible(m_hWnd))
	{
		this->ShowShadows(TRUE);
	}
}

//ϵͳ���øı�
VOID CShadowWindow::OnSettingChange()
{
	this->CheckEnvironment();

	if(!m_bIsLayered)
	{
		this->ShowShadows(FALSE, TRUE);
	}
	else
	{
		if(IsWindowVisible(m_hWnd))
		{
			//�ؽ���Ӱ����
			this->DestroyShadows();
			this->CreateShadows();
			
			//������Ӱ����λ��
			RECT tR;
			GetWindowRect(m_hWnd, &tR);
			m_wp.x = tR.left;
			m_wp.y = tR.top;
			m_wp.cx = tR.right - tR.left;
			m_wp.cy = tR.bottom - tR.top;

			//������ʾ����
			this->AdjustShadowPos();
			this->ShowShadows(TRUE, TRUE);
		}
	}
}

/*
	���ܺ���
*/

//��黷����ȷ���Ƿ�֧����Ӱ
VOID CShadowWindow::CheckEnvironment()
{
	m_bIsLayered = FALSE;
	
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(osv);//Set the length element
	GetVersionEx(&osv);//Fill the type with OS version info
	
	if(osv.dwPlatformId == 2)	//If it's an NT based OS
	{
		if(osv.dwMajorVersion >= 5) //If the major version is 5 or greater, OS supports transparency
        {
			if(osv.dwMinorVersion > 0)
			{
				//bIsXP = True;
				//bIsLuna = IsLuna;   //Determine if the Luna theme is active
			}
			
			HDC hDC = GetDC(NULL);//�����ĻDC 
			m_bIsLayered = (GetDeviceCaps(hDC, BITSPIXEL) >= 16);//Ensure we have enough screen colors
			ReleaseDC(NULL, hDC);//�����ͷţ���������DCй©
		}
	}
	
	if(m_bIsLayered)
	{
		//��á�����󻯺���С��ʱ��̬��ʾ���ڡ�����ѡ��
		//SPI_GETANIMATIONϵͳ������ȡһ��ANIMATIONINFO�ṹ������ṹ��iMinAnimate��ԱΪ���㣬��������С���ͻ�ԭ��������Ч����
		BOOL bMinAnimate = FALSE;
		ANIMATIONINFO ai = {sizeof(ANIMATIONINFO), 0};
		SystemParametersInfo(SPI_GETANIMATION, sizeof(ANIMATIONINFO), &ai, NULL);
		bMinAnimate = ai.iMinAnimate;
		
		//�رմ��������С������
		//ai.iMinAnimate = 0;
		//SystemParametersInfo(SPI_SETANIMATION, sizeof(ANIMATIONINFO), &ai, NULL); 

		//��á�����������ϡ�����ѡ��
		BOOL bAero = FALSE;
		if(s_DwmIsCompositionEnabled)
		{
			s_DwmIsCompositionEnabled(&bAero);
		}
		else
		{
			HMODULE hSysDll = LoadLibrary(_T("dwmapi.dll"));
			if(hSysDll)	// Loaded dwmapi.dll succefull, must on Vista or above
			{
				s_DwmIsCompositionEnabled = (pfnDwmIsCompositionEnabled)GetProcAddress(hSysDll, "DwmIsCompositionEnabled");
				if(s_DwmIsCompositionEnabled)
				{
					s_DwmIsCompositionEnabled(&bAero);
				}
				FreeLibrary(hSysDll);
			}
		}
		
		//Vista������ϵͳ�����ˡ�����������ϡ��͡�����󻯺���С��ʱ��̬��ʾ���ڡ�����ѡ��ʱ��
		//��Ӱ��ʾ�����⣬�������m_bIsLayered=FALSE����ʾ��֧����Ӱ��
		m_bIsDwmCompLayered = bAero && bMinAnimate;
		if (m_bIsDwmCompLayered)
		{
			m_bIsLayered = FALSE; 
		}
	}
	
}

//������Ӱ����
VOID CShadowWindow::DestroyShadows()
{
	if(m_hWndLt)
	{
		DestroyWindow(m_hWndLt);
	}
	if(m_hWndTt)
	{
		DestroyWindow(m_hWndTt);
	}
	if(m_hWndBt)
	{
		DestroyWindow(m_hWndBt);
	}
	if(m_hWndRt)
	{
		DestroyWindow(m_hWndRt);
	}
	m_hWndLt = NULL;
	m_hWndTt = NULL;
	m_hWndBt = NULL;
	m_hWndRt = NULL;
	m_bLastShow = FALSE;
	m_cx = 0;
	m_cy = 0;
}

//������Ӱ����
VOID CShadowWindow::CreateShadows()
{
	if(this->ShadowType != stNoneShadow)
	{
		//WS_EX_LAYERED = 0x80000
		//WS_EX_NOPARENTNOTIFY = 0x4
		//WS_EX_TRANSPARENT = 0x20

		LONG dwExStyle = WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOPARENTNOTIFY;
  		HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);

		m_hWndRt = CreateWindowEx(dwExStyle, _T("#32770"), _T(""), WS_POPUP, 0, 0, 0, 0, m_hWnd, 0, hInstance, 0);
		m_hWndBt = CreateWindowEx(dwExStyle, _T("#32770"), _T(""), WS_POPUP, 0, 0, 0, 0, m_hWnd, 0, hInstance, 0);

		if(this->ShadowType == stFullShadow)
		{
			m_hWndLt = CreateWindowEx(dwExStyle, _T("#32770"), _T(""), WS_POPUP, 0, 0, 0, 0, m_hWnd, 0, hInstance, 0);
			m_hWndTt = CreateWindowEx(dwExStyle, _T("#32770"), _T(""), WS_POPUP, 0, 0, 0, 0, m_hWnd, 0, hInstance, 0);
		}
	}
}

//��ʾ��������Ӱ����
VOID CShadowWindow::ShowShadows(BOOL bShow, BOOL bForce)
{
	if(this->ShadowType != stNoneShadow)
	{
		if(!bForce)
		{
			if(m_bLastShow == bShow)
			{
				return;
			}
		}
		m_bLastShow = bShow;
		
		if(bShow) 
		{
			ShowWindow(m_hWndRt, SW_SHOWNOACTIVATE);
			ShowWindow(m_hWndBt, SW_SHOWNOACTIVATE);
			if(this->ShadowType == stFullShadow)
			{
				ShowWindow(m_hWndLt, SW_SHOWNOACTIVATE);
				ShowWindow(m_hWndTt, SW_SHOWNOACTIVATE);
			}
		}
		else
		{
			ShowWindow(m_hWndRt, SW_HIDE);
			ShowWindow(m_hWndBt, SW_HIDE);
			if(this->ShadowType == stFullShadow)
			{
				ShowWindow(m_hWndLt, SW_HIDE);
				ShowWindow(m_hWndTt, SW_HIDE);
			}
		}
	}
	
}

//������Ӱ���ڳߴ��λ��
VOID CShadowWindow::AdjustShadowPos()
{
	if(this->ShadowType != stNoneShadow)
	{
		if(this->ShadowType == stDefaultShadow)
		{
			if(m_wp.cy != m_cy)//�����ڸ߶ȸı�
			{
				m_cy = m_wp.cy;//�洢�µĸ߶�
				this->UpdateShadowEdge(m_wp.x + m_wp.cx, m_wp.y + ShadowDepth, ShadowDepth, m_wp.cy, 3);//�����ұ���Ӱ����
			}
			if(m_wp.cx != m_cx)//�����ڿ�ȸı�
			{
				m_cx = m_wp.cx;//�洢�µĿ��
				this->UpdateShadowEdge(m_wp.x + ShadowDepth, m_wp.y + m_wp.cy, m_wp.cx - ShadowDepth, ShadowDepth, 4); //����ױ���Ӱ����
			}
			
			//��ɺ���Ӱ�����ƶ���ָ��λ��
			MoveWindow(m_hWndRt, m_wp.x + m_wp.cx, m_wp.y + ShadowDepth, ShadowDepth, m_wp.cy, FALSE);
			MoveWindow(m_hWndBt, m_wp.x + ShadowDepth, m_wp.y + m_wp.cy, m_wp.cx - ShadowDepth, ShadowDepth, FALSE);
		}
		else
		{
			if(m_wp.cy != m_cy)//�����ڸ߶ȸı�
			{
				m_cy = m_wp.cy;//�洢�µĸ߶�
				this->UpdateShadowEdge(m_wp.x - ShadowDepth, m_wp.y - ShadowDepth, ShadowDepth, m_wp.cy + ShadowDepth * 2, 1);//���������Ӱ����
				this->UpdateShadowEdge(m_wp.x + m_wp.cx, m_wp.y - ShadowDepth, ShadowDepth, m_wp.cy + ShadowDepth * 2, 3);//�����ұ���Ӱ����
			}
			if(m_wp.cx != m_cx)//�����ڿ�ȸı�
			{
				m_cx = m_wp.cx;//�洢�µĿ��
				this->UpdateShadowEdge(m_wp.x, m_wp.y - ShadowDepth, m_wp.cx, ShadowDepth, 2); //���趥����Ӱ����
				this->UpdateShadowEdge(m_wp.x, m_wp.y + m_wp.cy, m_wp.cx , ShadowDepth, 4); //����ױ���Ӱ����
			}
			
			//��ɺ���Ӱ�����ƶ���ָ��λ��
			MoveWindow(m_hWndLt, m_wp.x - ShadowDepth, m_wp.y - ShadowDepth, ShadowDepth, m_wp.cy + ShadowDepth*2, TRUE);//�ƶ������Ӱ����
			MoveWindow(m_hWndTt, m_wp.x, m_wp.y - ShadowDepth, m_wp.cx, ShadowDepth, TRUE);//�ƶ�������Ӱ����
			MoveWindow(m_hWndRt, m_wp.x + m_wp.cx, m_wp.y - ShadowDepth, ShadowDepth, m_wp.cy + ShadowDepth*2, TRUE);//�ƶ��ұ���Ӱ����
			MoveWindow(m_hWndBt, m_wp.x, m_wp.y + m_wp.cy, m_wp.cx, ShadowDepth, TRUE);//�ƶ��ױ���Ӱ����
		}
	}
}

//�޸���Ӱ����	
VOID CShadowWindow::UpdateShadowEdge(INT X, INT Y, INT nWidth, INT nHeight, INT nType)
{
	HDC hMemDC;
	HWND hWin;
	HBITMAP hBmp, hBmpOrg;
	POINT ptSrc, ptWndPos;
	SIZE szWndSize;
	BLENDFUNCTION blendFunc;
	BITMAPINFO bmi;
	BYTE R, G, B, Alpha;
	BYTE r, g, b, alpha;
	INT clrColor = 0;
	INT row, col;
	INT x, y;
	double grayscale;

	/* ����������Ļ�ļ����ڴ�DC*/
	hMemDC = CreateCompatibleDC(0);

	/* ��ʼ��λͼ�ṹ��Ϊ����λͼ��׼�� */
	memset(&bmi, 0, sizeof(bmi));//��ʼ��
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);	
    bmi.bmiHeader.biWidth = nWidth;						//λͼ���
    bmi.bmiHeader.biHeight = nHeight;					//λͼ�߶ȣ����biHeightΪ��������ô���DIBͼ����Ǵ������µģ���ԭ�������Ͻ�
    bmi.bmiHeader.biPlanes = 1;							//ʼ��Ϊ1
    bmi.bmiHeader.biBitCount = 32;						//ÿ������32λ(4���ֽ�)��������Blue(��1���ֽ�), Green(��2���ֽ�), Red(��3���ֽ�), Alpha(��4���ֽ�)
	bmi.bmiHeader.biCompression = BI_RGB;				//biBitCount=24��32ʱ��biCompressionʼ�����ó�BI_RGB

	INT nPerRowBytes = ((((bmi.bmiHeader.biWidth * bmi.bmiHeader.biBitCount) + 31) / 32) * 4); //ÿ���ֽ���
	nPerRowBytes = ((nPerRowBytes + 3) / 4) * 4; //4�ֽڶ���
    bmi.bmiHeader.biSizeImage = nPerRowBytes * abs(bmi.bmiHeader.biHeight);	//����������ռ���ֽ���: width * height * 32bit

	/* �������õ�λͼ�ṹ, ����DC����һ��λͼ */
	BYTE *pPixBits = NULL;//��������λͼ����
	hBmp = CreateDIBSection(hMemDC, &bmi, DIB_RGB_COLORS, (VOID**) &pPixBits, NULL, 0);	//����λͼ
	
	/* �޸�λͼ�������ݣ������ɽ�����Ӱ */
	B = GetBValue(ShadowColor);
	G = GetGValue(ShadowColor);
	R = GetRValue(ShadowColor);

	/* ��ô��ھ�� */
	switch(nType)
	{
	case 1://���
		hWin = m_hWndLt;
		
		for(y=0; y<nHeight; y++)
		{
			row = y * nPerRowBytes; //��: n�еĿ�ʼλ��
			
			//���ñ�Ե��͸����
			if(y < nWidth)//�ױ�Ե
				Alpha = (255 * y) / nWidth;
			else if(y >= (nHeight - nWidth))//�ϱ�Ե
				Alpha = ((nHeight - y) * 255) / nWidth;
			else//�м�
				Alpha = 255;
			
			for(x=0; x<nWidth; x++)
			{
				col = x * 4; //��: ÿ������ռ4���ֽڣ�����Ҫ����4
				
				//����r,g,b,alpha
				//alpha =(Alpha * (nWidth - x)) / nWidth;
				alpha = (Alpha * x) / nWidth;
				grayscale = ((double)alpha) / 255;
				b = (BYTE)(B * grayscale);
				g = (BYTE)(G * grayscale);
				r = (BYTE)(R * grayscale);
				
				//�޸�λͼ
				pPixBits[row + col + 0] = b;
				pPixBits[row + col + 1] = g;
				pPixBits[row + col + 2] = r;
				pPixBits[row + col + 3] = alpha;
				
				//dc.SetPixel(x, nHeight-y, RGB(r,g,b)); //������ʾ
			}
		}
		break;
		
	case 2://����
		hWin = m_hWndTt;
		for(y=0; y<nHeight; y++)
		{
			row = y * nPerRowBytes; //��: n�еĿ�ʼλ��
			
			for(x=0; x<nWidth; x++)
			{
				col = x * 4; //��: ÿ������ռ4���ֽڣ�����Ҫ����4
				
				//���ñ�Ե��͸����
				//if(x < nHeight)//���Ե
				//	Alpha = (255 * x) / (nHeight);
				//else//�м�
				Alpha = 255;
				
				//����r,g,b,alpha
				alpha =(Alpha * (nHeight - y)) / nHeight;
				grayscale = ((double)alpha) / 255;
				b = (BYTE)(B * grayscale);
				g = (BYTE)(G * grayscale);
				r = (BYTE)(R * grayscale);
				
				//�޸�λͼ
				pPixBits[row + col + 0] = b;
				pPixBits[row + col + 1] = g;
				pPixBits[row + col + 2] = r;
				pPixBits[row + col + 3] = alpha;
				
				//dc.SetPixel(x, nHeight-y, RGB(r,g,b)); //������ʾ
			}
		}
		break;

	case 3: //�Ҷ�
		hWin = m_hWndRt;
		for(y=0; y<nHeight; y++)
		{
			row = y * nPerRowBytes; //��: n�еĿ�ʼλ��
			
			//���ñ�Ե��͸����
			if(y < nWidth)//�ϱ�Ե
				Alpha = (255 * y) / nWidth;
			else if(y >= (nHeight - nWidth))//�ױ�Ե
				Alpha = ((nHeight - y) * 255) / nWidth;
			else//�м�
				Alpha = 255;

			for(x=0; x<nWidth; x++)
			{
				col = x * 4; //��: ÿ������ռ4���ֽڣ�����Ҫ����4
				
				//����r,g,b,alpha
				alpha =(Alpha * (nWidth - x)) / nWidth;
				grayscale = ((double)alpha) / 255;
				b = (BYTE)(B * grayscale);
				g = (BYTE)(G * grayscale);
				r = (BYTE)(R * grayscale);
				
				//�޸�λͼ
				pPixBits[row + col + 0] = b;
				pPixBits[row + col + 1] = g;
				pPixBits[row + col + 2] = r;
				pPixBits[row + col + 3] = alpha;
				
				//dc.SetPixel(x, nHeight-y, RGB(r,g,b)); //������ʾ
			}
		}
		break;

	case 4: //�׶�	
		hWin = m_hWndBt;
		for(y=0; y<nHeight; y++)
		{
			row = y * nPerRowBytes; //��: n�еĿ�ʼλ��
			
			for(x=0; x<nWidth; x++)
			{
				col = x * 4; //��: ÿ������ռ4���ֽڣ�����Ҫ����4
				
				//���ñ�Ե��͸����
				if(ShadowType == stDefaultShadow)
				{
					if(x < nHeight)//���Ե
						Alpha = (255 * x) / nHeight;//����ˮƽ͸����
					else//�м�
						Alpha = 255;
				}
				else
				{
					Alpha = 255;
				}
				
				//����r,g,b,alpha
				alpha = (Alpha * y) / nHeight; //���ô�ֱ͸����
				grayscale = ((double)alpha); //�Ҷ�
				b = (BYTE)(B * grayscale);
				g = (BYTE)(G * grayscale);
				r = (BYTE)(R * grayscale);
				
				//�޸�λͼ
				pPixBits[row + col + 0] = b;
				pPixBits[row + col + 1] = g;
				pPixBits[row + col + 2] = r;
				pPixBits[row + col + 3] = alpha;

				//dc.SetPixel(x, nHeight-y, RGB(r,g,b)); //������ʾ
			}
		}
		break;
	}
	
	/* ���޸ĺõ�λͼ�����ڴ�DC */
	hBmpOrg = (HBITMAP)SelectObject(hMemDC, hBmp);	

	/* ����ͼ�㴰�� */

	//����͸����
	blendFunc.AlphaFormat = AC_SRC_ALPHA;
    blendFunc.BlendFlags = 0;
    blendFunc.BlendOp = AC_SRC_OVER;
    blendFunc.SourceConstantAlpha = Transparency;
	//���ô���λ�úʹ�С
	ptSrc.x = 0;
	ptSrc.y = 0;
	ptWndPos.x = X;                                                                  
	ptWndPos.y = Y;
	szWndSize.cx = nWidth;
	szWndSize.cy = nHeight;
	//Ӧ��ͼ��
	BOOL bRet = UpdateLayeredWindow(hWin, hMemDC, &ptWndPos, &szWndSize, hMemDC, &ptSrc, 0, &blendFunc, ULW_ALPHA);

	/* ɾ��λͼ���ڴ�DC */
	SelectObject(hMemDC, hBmpOrg);
	DeleteObject(hBmp);
	DeleteDC(hMemDC);

}
