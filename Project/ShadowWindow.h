#ifndef __SHADOWWINDOW_H_
#define __SHADOWWINDOW_H_

#include <windows.h>
#include <tchar.h>

#define DEF_DEPTH			5
#define DEF_TRANSPARENCY	200

// Vista compatibility APIs
//�Ƿ�����������ϡ���
//����Vista������ϵͳ���ڡ�����ѡ������ˡ�����������ϡ��͡�����󻯺���С��ʱ��̬��ʾ���ڡ����ܺ���Ӱ��ʾ�����⡣
//����ڡ�����������ϡ�ʱ�ر���Ӱ��
typedef HRESULT (WINAPI *pfnDwmIsCompositionEnabled)(BOOL *pfEnabled);
static pfnDwmIsCompositionEnabled s_DwmIsCompositionEnabled;

class CShadowWindow
{
// ����������
public:
	CShadowWindow();
	virtual ~CShadowWindow();

// ����
public:

	//��Ӱ����
	enum ShadowTypeConstants
	{		
		stNoneShadow = 0,		//û����Ӱ
		stDefaultShadow = 1,	//ȱʡ��Ӱ(��Ӱ���ұߺ͵ױ�)
		stFullShadow = 2		//��ȫ(��Ӱ�����������ı�)
	};

	COLORREF	ShadowColor;	//��Ӱ��ɫ
	INT			ShadowDepth;	//��Ӱ���
	BYTE		Transparency;	//͸����
	ShadowTypeConstants ShadowType; //��Ӱ����

// ����
public:
	
	BOOL Shadow(HWND hWnd);		//Ӧ����Ӱ
	VOID OpenShadows();			//����Ӱ
	VOID CloseShadows();		//�ر���Ӱ
	BOOL IsDwmCompLayered();	//Vista������ϵͳ�����ˡ�����������ϡ�+������󻯺���С��ʱ��̬��ʾ���ڡ�����ѡ��

//�¼�
public:
	VOID OnWindowPosChanged(WINDOWPOS *lpwp);
	VOID OnSize();
	VOID OnSettingChange(); 

private:

	HWND m_hWnd;				//�����ھ��
	HWND m_hWndLt;				//�����Ӱ���ھ��
	HWND m_hWndTt;				//������Ӱ���ھ��
	HWND m_hWndBt;				//�׶���Ӱ���ھ��
	HWND m_hWndRt;				//�ұ���Ӱ���ھ��
	BOOL m_bIsLayered;		    //�ֲ㴰��֧��
	BOOL m_bLastShow;			//�ϴ���ʾ״̬
	WINDOWPOS m_wp;				//������λ��
	INT	 m_cx;					//��һ���
	INT  m_cy;					//��һ�߶�
	BOOL m_bIsDwmCompLayered;	//Vista������ϵͳ�����ˡ�����������ϡ�+������󻯺���С��ʱ��̬��ʾ���ڡ�����ѡ��
	BOOL m_bIsOpend;			//��Ӱ�Ƿ񱻴�

	VOID CheckEnvironment();	//��黷����ȷ���Ƿ�֧����Ӱ
	VOID DestroyShadows();		//������Ӱ����
	VOID CreateShadows();		//������Ӱ����
	VOID ShowShadows(BOOL bShow, BOOL bForce = FALSE);//��ʾ��������Ӱ����
	VOID AdjustShadowPos();     //������Ӱ���ڳߴ��λ��
	VOID UpdateShadowEdge(INT X, INT Y, INT nWidth, INT nHeight, INT nType);//������Ӱ����

};

#endif //__SHADOWWINDOW_H_
