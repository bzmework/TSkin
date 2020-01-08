// Skin.h : Declaration of the CSkin

#ifndef __SKIN_H_
#define __SKIN_H_

#include "stdafx.h"
#include "resource.h"       // main symbols
#include <atlwin.h>

#include "SkinWindow.h"
#include "ShadowWindow.h"

/////////////////////////////////////////////////////////////////////////////
// CSkin
class ATL_NO_VTABLE CSkin : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSkin, &CLSID_Skin>,
	public IDispatchImpl<ISkin, &IID_ISkin, &LIBID_TSkin>,
	public CWindowImpl<CSkin>
{
public:
	CSkin()
	{
		m_hWnd = NULL;
		m_Skin = new CSkinWIndow();
		m_Shadow = new CShadowWindow();
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_SKIN)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CSkin)
		COM_INTERFACE_ENTRY(ISkin)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	BEGIN_MSG_MAP(CSkin)
		MESSAGE_RANGE_HANDLER(0x0000, 0xFFFF, OnMessage)
	END_MSG_MAP()

	//
	// 卸载所有子类窗口
	//
	void FinalRelease()
	{
		if (m_hWnd)	
		{
			UnsubclassWindow();
		}
		if(m_Skin)
		{
			delete m_Skin;
		}
		if(m_Shadow)
		{
			delete m_Shadow;
		}
	}

public:

	//消息处理
	LRESULT OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bEatIt);

// ISkin
public:

	STDMETHOD(AboutMe)();
	STDMETHOD(ApplySkin)(/*[in]*/ long hWnd, /*[out, retval]*/ VARIANT_BOOL *pRetVal);
	STDMETHOD(get_TitleColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_TitleColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_GradientBeginColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_GradientBeginColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_GradientEndColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_GradientEndColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_BackColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_BackColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_TitleBarHeight)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_TitleBarHeight)(/*[in]*/ long newVal);
	STDMETHOD(get_BorderWidth)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_BorderWidth)(/*[in]*/ long newVal);
	STDMETHOD(get_Alpha)(/*[out, retval]*/ unsigned char *pVal);
	STDMETHOD(put_Alpha)(/*[in]*/ unsigned char newVal);
	STDMETHOD(get_ShadowColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_ShadowColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_ShadowDepth)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_ShadowDepth)(/*[in]*/ long newVal);
	STDMETHOD(get_ShadowTransparency)(/*[out, retval]*/ unsigned char *pVal);
	STDMETHOD(put_ShadowTransparency)(/*[in]*/ unsigned char newVal);
	STDMETHOD(get_ShadowType)(/*[out, retval]*/ ShadowTypeConstants *pVal);
	STDMETHOD(put_ShadowType)(/*[in]*/ ShadowTypeConstants newVal);

	STDMETHOD(get_ShowMinButton)(/*[out, retval]*/ VARIANT_BOOL  *pVal);
	STDMETHOD(put_ShowMinButton)(/*[in, defaultvalue(TRUE)]*/ VARIANT_BOOL  newVal);
	STDMETHOD(get_ShowMaxButton)(/*[out, retval]*/ VARIANT_BOOL  *pVal);
	STDMETHOD(put_ShowMaxButton)(/*[in, defaultvalue(TRUE)]*/ VARIANT_BOOL  newVal);
	STDMETHOD(get_ShowHelpButton)(/*[out, retval]*/ VARIANT_BOOL  *pVal);
	STDMETHOD(put_ShowHelpButton)(/*[in, defaultvalue(TRUE)]*/ VARIANT_BOOL  newVal);
	STDMETHOD(get_CloseButtonColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_CloseButtonColor)(/*[in, defaultvalue(0xff0000)]*/ OLE_COLOR newVal);
	//STDMETHOD(get_FontSmoothing)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	//STDMETHOD(put_FontSmoothing)(/*[in, defaultvalue(TRUE)]*/ VARIANT_BOOL newVal);

	STDMETHOD(Transparent)(/*[in, defaultvalue(225)]*/ unsigned char nAlpha, /*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(Refresh)();

private:
	CSkinWIndow *m_Skin;
	CShadowWindow *m_Shadow;
};

#endif //__SKIN_H_
