#include "stdafx.h" 
#include "win32gdi.h"
#include "ShadowWIndow.h"

/*
	构造与析构
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
	对外接口
*/

//应用阴影
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

//打开阴影
VOID CShadowWindow::OpenShadows()
{
	m_bIsOpend = TRUE;
	if(m_bIsLayered)
	{
		this->ShowShadows(TRUE, TRUE);
	}
}		
	
//关闭阴影
VOID CShadowWindow::CloseShadows()
{
	m_bIsOpend = FALSE;
	if(m_bIsLayered)
	{
		this->ShowShadows(FALSE, TRUE);
	}
}
	
//Vista及以上系统启用了“启用桌面组合”+“在最大化和最小化时动态显示窗口”性能选项
BOOL CShadowWindow::IsDwmCompLayered()
{
	return m_bIsDwmCompLayered;
}
	
/*
	事件处理
*/

//父窗口大小、位置、Z轴顺序改变时，调整阴影位置，并重新显示阴影
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

//父窗口大小改变时, 重新显示阴影
VOID CShadowWindow::OnSize()
{
	if(m_bIsLayered && m_bIsOpend && IsWindowVisible(m_hWnd))
	{
		this->ShowShadows(TRUE);
	}
}

//系统设置改变
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
			//重建阴影窗口
			this->DestroyShadows();
			this->CreateShadows();
			
			//重设阴影窗口位置
			RECT tR;
			GetWindowRect(m_hWnd, &tR);
			m_wp.x = tR.left;
			m_wp.y = tR.top;
			m_wp.cx = tR.right - tR.left;
			m_wp.cy = tR.bottom - tR.top;

			//重新显示窗口
			this->AdjustShadowPos();
			this->ShowShadows(TRUE, TRUE);
		}
	}
}

/*
	功能函数
*/

//检查环境以确定是否支持阴影
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
			
			HDC hDC = GetDC(NULL);//获得屏幕DC 
			m_bIsLayered = (GetDeviceCaps(hDC, BITSPIXEL) >= 16);//Ensure we have enough screen colors
			ReleaseDC(NULL, hDC);//必须释放，否则会造成DC泄漏
		}
	}
	
	if(m_bIsLayered)
	{
		//获得“在最大化和最小化时动态显示窗口”性能选项
		//SPI_GETANIMATION系统参数获取一个ANIMATIONINFO结构。如果结构的iMinAnimate成员为非零，则启用最小化和还原动画动画效果。
		BOOL bMinAnimate = FALSE;
		ANIMATIONINFO ai = {sizeof(ANIMATIONINFO), 0};
		SystemParametersInfo(SPI_GETANIMATION, sizeof(ANIMATIONINFO), &ai, NULL);
		bMinAnimate = ai.iMinAnimate;
		
		//关闭窗口最大化最小化动画
		//ai.iMinAnimate = 0;
		//SystemParametersInfo(SPI_SETANIMATION, sizeof(ANIMATIONINFO), &ai, NULL); 

		//获得“启用桌面组合”性能选项
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
		
		//Vista及以上系统启用了“启用桌面组合”和“在最大化和最小化时动态显示窗口”性能选项时，
		//阴影显示有问题，因此设置m_bIsLayered=FALSE，表示不支持阴影。
		m_bIsDwmCompLayered = bAero && bMinAnimate;
		if (m_bIsDwmCompLayered)
		{
			m_bIsLayered = FALSE; 
		}
	}
	
}

//销毁阴影窗口
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

//创建阴影窗口
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

//显示或隐藏阴影窗口
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

//调整阴影窗口尺寸和位置
VOID CShadowWindow::AdjustShadowPos()
{
	if(this->ShadowType != stNoneShadow)
	{
		if(this->ShadowType == stDefaultShadow)
		{
			if(m_wp.cy != m_cy)//父窗口高度改变
			{
				m_cy = m_wp.cy;//存储新的高度
				this->UpdateShadowEdge(m_wp.x + m_wp.cx, m_wp.y + ShadowDepth, ShadowDepth, m_wp.cy, 3);//重设右边阴影内容
			}
			if(m_wp.cx != m_cx)//父窗口宽度改变
			{
				m_cx = m_wp.cx;//存储新的宽度
				this->UpdateShadowEdge(m_wp.x + ShadowDepth, m_wp.y + m_wp.cy, m_wp.cx - ShadowDepth, ShadowDepth, 4); //重设底边阴影内容
			}
			
			//完成后将阴影窗口移动到指定位置
			MoveWindow(m_hWndRt, m_wp.x + m_wp.cx, m_wp.y + ShadowDepth, ShadowDepth, m_wp.cy, FALSE);
			MoveWindow(m_hWndBt, m_wp.x + ShadowDepth, m_wp.y + m_wp.cy, m_wp.cx - ShadowDepth, ShadowDepth, FALSE);
		}
		else
		{
			if(m_wp.cy != m_cy)//父窗口高度改变
			{
				m_cy = m_wp.cy;//存储新的高度
				this->UpdateShadowEdge(m_wp.x - ShadowDepth, m_wp.y - ShadowDepth, ShadowDepth, m_wp.cy + ShadowDepth * 2, 1);//重设左边阴影内容
				this->UpdateShadowEdge(m_wp.x + m_wp.cx, m_wp.y - ShadowDepth, ShadowDepth, m_wp.cy + ShadowDepth * 2, 3);//重设右边阴影内容
			}
			if(m_wp.cx != m_cx)//父窗口宽度改变
			{
				m_cx = m_wp.cx;//存储新的宽度
				this->UpdateShadowEdge(m_wp.x, m_wp.y - ShadowDepth, m_wp.cx, ShadowDepth, 2); //重设顶边阴影内容
				this->UpdateShadowEdge(m_wp.x, m_wp.y + m_wp.cy, m_wp.cx , ShadowDepth, 4); //重设底边阴影内容
			}
			
			//完成后将阴影窗口移动到指定位置
			MoveWindow(m_hWndLt, m_wp.x - ShadowDepth, m_wp.y - ShadowDepth, ShadowDepth, m_wp.cy + ShadowDepth*2, TRUE);//移动左边阴影窗口
			MoveWindow(m_hWndTt, m_wp.x, m_wp.y - ShadowDepth, m_wp.cx, ShadowDepth, TRUE);//移动顶边阴影窗口
			MoveWindow(m_hWndRt, m_wp.x + m_wp.cx, m_wp.y - ShadowDepth, ShadowDepth, m_wp.cy + ShadowDepth*2, TRUE);//移动右边阴影窗口
			MoveWindow(m_hWndBt, m_wp.x, m_wp.y + m_wp.cy, m_wp.cx, ShadowDepth, TRUE);//移动底边阴影窗口
		}
	}
}

//修改阴影内容	
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

	/* 创建基于屏幕的兼容内存DC*/
	hMemDC = CreateCompatibleDC(0);

	/* 初始化位图结构，为创建位图做准备 */
	memset(&bmi, 0, sizeof(bmi));//初始化
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);	
    bmi.bmiHeader.biWidth = nWidth;						//位图宽度
    bmi.bmiHeader.biHeight = nHeight;					//位图高度，如果biHeight为负数，那么这个DIB图象就是从上往下的，即原点在左上角
    bmi.bmiHeader.biPlanes = 1;							//始终为1
    bmi.bmiHeader.biBitCount = 32;						//每个像素32位(4个字节)，包括：Blue(第1个字节), Green(第2个字节), Red(第3个字节), Alpha(第4个字节)
	bmi.bmiHeader.biCompression = BI_RGB;				//biBitCount=24或32时，biCompression始终设置成BI_RGB

	INT nPerRowBytes = ((((bmi.bmiHeader.biWidth * bmi.bmiHeader.biBitCount) + 31) / 32) * 4); //每行字节数
	nPerRowBytes = ((nPerRowBytes + 3) / 4) * 4; //4字节对齐
    bmi.bmiHeader.biSizeImage = nPerRowBytes * abs(bmi.bmiHeader.biHeight);	//所有像素所占的字节数: width * height * 32bit

	/* 根据设置的位图结构, 基于DC创建一幅位图 */
	BYTE *pPixBits = NULL;//用于容纳位图数据
	hBmp = CreateDIBSection(hMemDC, &bmi, DIB_RGB_COLORS, (VOID**) &pPixBits, NULL, 0);	//创建位图
	
	/* 修改位图像素数据，令其变成渐变阴影 */
	B = GetBValue(ShadowColor);
	G = GetGValue(ShadowColor);
	R = GetRValue(ShadowColor);

	/* 获得窗口句柄 */
	switch(nType)
	{
	case 1://左边
		hWin = m_hWndLt;
		
		for(y=0; y<nHeight; y++)
		{
			row = y * nPerRowBytes; //行: n列的开始位置
			
			//设置边缘的透明度
			if(y < nWidth)//底边缘
				Alpha = (255 * y) / nWidth;
			else if(y >= (nHeight - nWidth))//上边缘
				Alpha = ((nHeight - y) * 255) / nWidth;
			else//中间
				Alpha = 255;
			
			for(x=0; x<nWidth; x++)
			{
				col = x * 4; //列: 每个像素占4个字节，所以要乘以4
				
				//设置r,g,b,alpha
				//alpha =(Alpha * (nWidth - x)) / nWidth;
				alpha = (Alpha * x) / nWidth;
				grayscale = ((double)alpha) / 255;
				b = (BYTE)(B * grayscale);
				g = (BYTE)(G * grayscale);
				r = (BYTE)(R * grayscale);
				
				//修改位图
				pPixBits[row + col + 0] = b;
				pPixBits[row + col + 1] = g;
				pPixBits[row + col + 2] = r;
				pPixBits[row + col + 3] = alpha;
				
				//dc.SetPixel(x, nHeight-y, RGB(r,g,b)); //测试显示
			}
		}
		break;
		
	case 2://顶端
		hWin = m_hWndTt;
		for(y=0; y<nHeight; y++)
		{
			row = y * nPerRowBytes; //行: n列的开始位置
			
			for(x=0; x<nWidth; x++)
			{
				col = x * 4; //列: 每个像素占4个字节，所以要乘以4
				
				//设置边缘的透明度
				//if(x < nHeight)//左边缘
				//	Alpha = (255 * x) / (nHeight);
				//else//中间
				Alpha = 255;
				
				//设置r,g,b,alpha
				alpha =(Alpha * (nHeight - y)) / nHeight;
				grayscale = ((double)alpha) / 255;
				b = (BYTE)(B * grayscale);
				g = (BYTE)(G * grayscale);
				r = (BYTE)(R * grayscale);
				
				//修改位图
				pPixBits[row + col + 0] = b;
				pPixBits[row + col + 1] = g;
				pPixBits[row + col + 2] = r;
				pPixBits[row + col + 3] = alpha;
				
				//dc.SetPixel(x, nHeight-y, RGB(r,g,b)); //测试显示
			}
		}
		break;

	case 3: //右端
		hWin = m_hWndRt;
		for(y=0; y<nHeight; y++)
		{
			row = y * nPerRowBytes; //行: n列的开始位置
			
			//设置边缘的透明度
			if(y < nWidth)//上边缘
				Alpha = (255 * y) / nWidth;
			else if(y >= (nHeight - nWidth))//底边缘
				Alpha = ((nHeight - y) * 255) / nWidth;
			else//中间
				Alpha = 255;

			for(x=0; x<nWidth; x++)
			{
				col = x * 4; //列: 每个像素占4个字节，所以要乘以4
				
				//设置r,g,b,alpha
				alpha =(Alpha * (nWidth - x)) / nWidth;
				grayscale = ((double)alpha) / 255;
				b = (BYTE)(B * grayscale);
				g = (BYTE)(G * grayscale);
				r = (BYTE)(R * grayscale);
				
				//修改位图
				pPixBits[row + col + 0] = b;
				pPixBits[row + col + 1] = g;
				pPixBits[row + col + 2] = r;
				pPixBits[row + col + 3] = alpha;
				
				//dc.SetPixel(x, nHeight-y, RGB(r,g,b)); //测试显示
			}
		}
		break;

	case 4: //底端	
		hWin = m_hWndBt;
		for(y=0; y<nHeight; y++)
		{
			row = y * nPerRowBytes; //行: n列的开始位置
			
			for(x=0; x<nWidth; x++)
			{
				col = x * 4; //列: 每个像素占4个字节，所以要乘以4
				
				//设置边缘的透明度
				if(ShadowType == stDefaultShadow)
				{
					if(x < nHeight)//左边缘
						Alpha = (255 * x) / nHeight;//调低水平透明度
					else//中间
						Alpha = 255;
				}
				else
				{
					Alpha = 255;
				}
				
				//设置r,g,b,alpha
				alpha = (Alpha * y) / nHeight; //设置垂直透明度
				grayscale = ((double)alpha); //灰度
				b = (BYTE)(B * grayscale);
				g = (BYTE)(G * grayscale);
				r = (BYTE)(R * grayscale);
				
				//修改位图
				pPixBits[row + col + 0] = b;
				pPixBits[row + col + 1] = g;
				pPixBits[row + col + 2] = r;
				pPixBits[row + col + 3] = alpha;

				//dc.SetPixel(x, nHeight-y, RGB(r,g,b)); //测试显示
			}
		}
		break;
	}
	
	/* 将修改好的位图放入内存DC */
	hBmpOrg = (HBITMAP)SelectObject(hMemDC, hBmp);	

	/* 创建图层窗口 */

	//设置透明度
	blendFunc.AlphaFormat = AC_SRC_ALPHA;
    blendFunc.BlendFlags = 0;
    blendFunc.BlendOp = AC_SRC_OVER;
    blendFunc.SourceConstantAlpha = Transparency;
	//设置窗口位置和大小
	ptSrc.x = 0;
	ptSrc.y = 0;
	ptWndPos.x = X;                                                                  
	ptWndPos.y = Y;
	szWndSize.cx = nWidth;
	szWndSize.cy = nHeight;
	//应用图层
	BOOL bRet = UpdateLayeredWindow(hWin, hMemDC, &ptWndPos, &szWndSize, hMemDC, &ptSrc, 0, &blendFunc, ULW_ALPHA);

	/* 删除位图和内存DC */
	SelectObject(hMemDC, hBmpOrg);
	DeleteObject(hBmp);
	DeleteDC(hMemDC);

}
