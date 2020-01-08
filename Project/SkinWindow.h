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

// ���λ��
enum HitTestConstants
{
	//In the border of a window that does not have a sizing border 
	//��û�д�С�߿�Ĵ��ڵı߿���
	htBorder = HTBORDER,		
	//In the lower horizontal border of a resizable window (the user can click the mouse to resize the window vertically) 
	//�ڿɵ�����С���ڵĽϵ�ˮƽ�߿���(�û����Ե�����괹ֱ�������ڵĴ�С)
	htBottom = HTBOTTOM,
	//In the lower-left corner of a border of a resizable window (the user can click the mouse to resize the window diagonally)
	//�ڿɵ�����С���ڱ߿�����½�(�û����Ե������Խ��ߵ������ڵĴ�С)
	htBottomLeft = HTBOTTOMLEFT,
	//In the lower-right corner of a border of a resizable window (the user can click the mouse to resize the window diagonally) 
	//�ڿɵ�����С���ڱ߿�����½�(�û����Ե������Խ��ߵ������ڵĴ�С)
	htBottomRight = HTBOTTOMRIGHT,
	//In a title bar 
	//�ڱ�������
	htCaption = HTCAPTION,
	//In a client area
	//�ڿͻ�����
	htClient = HTCLIENT,
	//In a close button
	//�ڹرհ�ť��
	htClose = HTCLOSE,
	//On the screen background or on a dividing line between windows (same as HTNOWHERE, except that the DefWindowProc function produces a system beep to indicate an error) 
	//����Ļ�����ϻ򴰿�֮��ķֽ�����(��HTNOWHERE��ͬ������DefWindowProc��������ϵͳ��������ָʾ����֮��)
    htError = HTERROR,
	//In a size box (same as HTSIZE) 
	//�ڴ�С����(��HTSIZE��ͬ)
	htGrowBox = HTSIZE,
	//In a Help button
	//�ڰ�����ť��
	htHelp = HTHELP,
	//In a horizontal scroll bar 
	//��ˮƽ��������
	htHScroll = HTHSCROLL,
	//In the left border of a resizable window (the user can click the mouse to resize the window horizontally) 
	//�ڿɵ�����С���ڵ���߿���(�û����Ե������ˮƽ�������ڵĴ�С)
	htLeft = HTLEFT,
	//In a menu 
	//�ڲ˵�λ��
	htMenu = HTMENU,
	//In Maximize button 
	//����󻯰�ť��
	htMaxButton = HTMAXBUTTON,
	//In Minimize button 
	//����С����ť��
	htMinButton = HTMINBUTTON,
	//On the screen background or on a dividing line between windows 
	//����Ļ�����ϻ򴰿�֮��ķֽ�����
    htNoWhere =	HTNOWHERE,
	//In a Minimize button 
	//����С����ť��
	htReduce = HTREDUCE,
	//In the right border of a resizable window (the user can click the mouse to resize the window horizontally) 
	//�ڿɵ�����С���ڵ��ұ߿���(�û����Ե������ˮƽ�������ڵĴ�С)
	htRight = HTRIGHT,
	//In a size box (same as HTGROWBOX) 
	//��һ����С����(��HTGROWBOX��ͬ)
	htSize = HTSIZE,
	//In a System menu or in a Close button in a child window 
	//��ϵͳ�˵��л����Ӵ����еĹرհ�ť��
	htSysMenu = HTSYSMENU,
	//In the upper horizontal border of a window
	//�ڴ��ڵ���ˮƽ�߽�
	htTop = HTTOP,
	//In the upper-left corner of a window border 
	//�ڴ��ڱ߿�����Ͻ�
	htTopLeft = HTTOPLEFT,
	//In the upper right corner of a window border 
	//�ڴ��ڱ߿�����Ͻ�
	htTopRight = HTTOPRIGHT,
	//In a window currently covered by another window in the same thread (the message will be sent to underlying windows in the same thread until one of them returns a code that is not HTTRANSPARENT) 
	//�ڵ�ǰ��ͬһ�߳��е���һ�����ڸ��ǵĴ�����(��Ϣ�������͵�ͬһ�߳��еĵײ㴰�ڣ�ֱ������һ�����ڷ��ط�HTTRANSPARENT����)
    htTransparent =	HTTRANSPARENT,
	//In the vertical scroll bar 
	//�ڴ�ֱ��������
	htVScroll =	HTVSCROLL,
	//In a Maximize button 
	//����󻯰�ť��
	htZoom = HTZOOM,
	//���Զ���λ��
	htCustom = HTCLIENT
};

// ����״̬
enum ObjStateConstants
{
    osNormal = 0x0,						//����״̬				
    osHot = 0x1,						//����״̬
    osPressed = 0x2,					//����״̬
    osDisabled = 0x3,					//������״̬
	osVisible = 0x4						//�Ƿ�ɼ�״̬
};

// ��������
enum ObjTypeConstants
{
    otWindow = 0x1,						//����				
	otIcon = 0x2,						//ͼ��
	otText = 0x3,						//�ı�
	otHelp = 0x4,						//������ť
	otMin = 0x5,						//��С����ť
	otMax = 0x6,						//��󻯰�ť
	otClose = 0x7,						//�رհ�ť
	otCustomButton = 0x8				//�Զ��尴ť
};

// ����
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

	ObjTypeConstants Type;				//����
	ObjStateConstants State;			//״̬
	RECT Area;							//����
	HICON Icon;							//ͼ��
	TCHAR* Title;						//����
	TCHAR* Tip;							//��ʾ�ı�
};

class CSkinWIndow
{
// ����������
public:
	CSkinWIndow();
	virtual ~CSkinWIndow();

// ����
public:
	BYTE		Alpha;					//͸����
	COLORREF	TitleColor;				//��������ɫ
	COLORREF	GradientBeginColor;		//���������俪ʼɫ
	COLORREF	GradientEndColor;		//�������������ɫ
	COLORREF	BackColor;				//���ڱ���ɫ
	COLORREF	CloseButtonColor;		//�رհ�ť��ɫ
	INT			TitleBarHeight;			//�������߶�
	INT			BorderWidth;			//�߿���
	HICON		Icon;					//����ͼ��
	TCHAR*		Title;					//���ڱ���
	BOOL		FontSmoothing;			//��������ƽ��

// ����
public:
	
	//�Ƿ��б�����
	BOOL HasCaption(HWND hWnd) const;

	//�Ƿ�����󻯰�ť
	BOOL MaxButton() const;		
	VOID MaxButton(BOOL bValue);
	
	//�Ƿ�����С����ť	
	BOOL MinButton() const;		
	VOID MinButton(BOOL bValue);

	//�Ƿ��а�����ť		
	BOOL HelpButton() const;		
	VOID HelpButton(BOOL bValue);
	
	//�ô���͸��
	BOOL Transparent(BYTE nAlpha);
	
	//ˢ��
	VOID Refresh();

// �¼�
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
	HWND	m_hWnd;				//���ھ��
	HRGN	m_hRgn;				//������
	CMemDC  m_MemDC;			//�ڴ�DC

	TheObj	*m_oWindow;			//����
	TheObj	*m_oIcon;			//����ͼ��
	TheObj	*m_oTitle;			//�����ı�
	TheObj	*m_oClose;			//�رհ�ť
	TheObj	*m_oMax;			//��󻯰�ť
	TheObj	*m_oMin;			//��С����ť
	TheObj	*m_oHelp;			//������ť

	TheObj	*m_oCustomBtn1;		//�Զ��尴ť1
	TheObj	*m_oCustomBtn2;		//�Զ��尴ť2
	TheObj	*m_oCustomBtn3;		//�Զ��尴ť3
	TheObj	*m_oCustomBtn4;		//�Զ��尴ť4
	TheObj	*m_oCustomBtn5;		//�Զ��尴ť5
	//TheObj	*m_oPrevButton;		//��һ����ť
	
	HitTestConstants m_eMousePos;	//��ǰ���λ��

	BOOL	m_bSysMenu;			//�Ƿ���ϵͳ�˵�
	//BOOL	m_bToolWindow;		//�Ƿ��ǹ��ߴ���
	//BOOL	m_bDialogFrame;		//�Ƿ�Ի��򴰿�
	
	BOOL	m_bMaxButton;		//�Ƿ�����󻯰�ť
	BOOL	m_bMinButton;		//�Ƿ�����С����ť
	BOOL	m_bHelpButton;		//�Ƿ��а�����ť
	//BOOL	m_bShowMinMaxButtons;//�Ƿ�����С����󻯰�ť

	BOOL	m_bMaximized;		//�����Ƿ����
	BOOL	m_bMinimized;		//�����Ƿ���С��
	
	BOOL	m_bFirstShow;		//�Ƿ��һ����ʾ����
	BOOL	m_bActive;			//�����Ƿ񱻼���		
	
	BOOL	m_bAntiAliased;		//�����˿���ݹ���
		
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
