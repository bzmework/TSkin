#ifndef __MEMDC_H_
#define __MEMDC_H_

#include <windows.h>
#include <tchar.h>

class CMemDC
{
private:
	HDC m_hDC;
	INT m_nWidth;
	INT m_nHeight;
	HBITMAP m_hBmp;
	HBITMAP m_hBmpOld;

public:
	
	CMemDC();
	~CMemDC();
	HDC GetDC();
	INT GetWidth();
	INT GetHeight();
	VOID SetWidth(INT nValue);
	VOID SetHeight(INT nValue);
	HDC Create(INT nWidth = 0, INT nHeight = 0);
	VOID Destroy();
};

#endif //__MEMDC_H_
