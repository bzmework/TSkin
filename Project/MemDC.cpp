#include "MemDC.h"

CMemDC::CMemDC()
{
	m_hDC = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_hBmp = NULL;
	m_hBmpOld = NULL;
}

CMemDC::~CMemDC()
{
	Destroy();
}

HDC CMemDC::GetDC()
{
	return m_hDC;
}

INT CMemDC::GetWidth()
{
	return m_nWidth;
}

VOID CMemDC::SetWidth(INT nValue)
{
	m_nWidth = nValue;
}

INT CMemDC::GetHeight()
{
	return m_nHeight;
}

VOID CMemDC::SetHeight(INT nValue)
{
	m_nHeight = nValue;
}

HDC CMemDC::Create(INT nWidth, INT nHeight)
{
	Destroy();
	if(nWidth <= 0 || nHeight <= 0)
	{
		nWidth = GetSystemMetrics(SM_CXSCREEN);
		nHeight = GetSystemMetrics(SM_CYSCREEN);
	}
	HDC hDC = CreateDC(_T("DISPLAY"), 0, 0, 0);
	m_hDC = CreateCompatibleDC(hDC);
	m_hBmp = CreateCompatibleBitmap(hDC, nWidth, nHeight);
	m_hBmpOld = (HBITMAP)SelectObject(m_hDC, m_hBmp);
	if (m_hBmpOld == NULL)
	{
		Destroy();
	}
	else
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
	}
	DeleteDC(hDC);

	return m_hDC;
}

VOID CMemDC::Destroy()
{
	m_nWidth = 0;
	m_nHeight = 0;
	if (m_hBmpOld)
	{
		SelectObject(m_hDC, m_hBmpOld);
		m_hBmpOld = NULL;
	}
	if (m_hBmp)
	{
		DeleteObject(m_hBmp);
		m_hBmp = NULL;
	}
	if (m_hDC)
	{
		DeleteDC(m_hDC);
		m_hDC = NULL;
	}
}