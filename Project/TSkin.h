

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for TSkin.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __TSkin_h__
#define __TSkin_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISkin_FWD_DEFINED__
#define __ISkin_FWD_DEFINED__
typedef interface ISkin ISkin;

#endif 	/* __ISkin_FWD_DEFINED__ */


#ifndef __Skin_FWD_DEFINED__
#define __Skin_FWD_DEFINED__

#ifdef __cplusplus
typedef class Skin Skin;
#else
typedef struct Skin Skin;
#endif /* __cplusplus */

#endif 	/* __Skin_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ISkin_INTERFACE_DEFINED__
#define __ISkin_INTERFACE_DEFINED__

/* interface ISkin */
/* [unique][helpstring][dual][uuid][object] */ 

typedef /* [helpstring][v1_enum] */ 
enum ShadowTypeConstants
    {
        stNoneShadow	= 0,
        stDefaultShadow	= 1,
        stFullShadow	= 2
    } 	ShadowTypeConstants;


EXTERN_C const IID IID_ISkin;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BEEFEC8A-6B26-4444-9B9E-6D7B94BC1158")
    ISkin : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AboutMe( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ApplySkin( 
            /* [in] */ long hWnd,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TitleColor( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TitleColor( 
            /* [defaultvalue][in] */ OLE_COLOR newVal = 0xffffff) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GradientBeginColor( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_GradientBeginColor( 
            /* [defaultvalue][in] */ OLE_COLOR newVal = 0) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GradientEndColor( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_GradientEndColor( 
            /* [defaultvalue][in] */ OLE_COLOR newVal = 0x666666) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BackColor( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BackColor( 
            /* [defaultvalue][in] */ OLE_COLOR newVal = 0xe8ddcf) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TitleBarHeight( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TitleBarHeight( 
            /* [defaultvalue][in] */ long newVal = 22) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BorderWidth( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BorderWidth( 
            /* [defaultvalue][in] */ long newVal = 3) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Alpha( 
            /* [retval][out] */ unsigned char *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Alpha( 
            /* [defaultvalue][in] */ unsigned char newVal = 100) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ShadowColor( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ShadowColor( 
            /* [defaultvalue][in] */ OLE_COLOR newVal = 0) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ShadowDepth( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ShadowDepth( 
            /* [defaultvalue][in] */ long newVal = 5) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ShadowTransparency( 
            /* [retval][out] */ unsigned char *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ShadowTransparency( 
            /* [defaultvalue][in] */ unsigned char newVal = 200) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ShadowType( 
            /* [retval][out] */ ShadowTypeConstants *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ShadowType( 
            /* [defaultvalue][in] */ ShadowTypeConstants newVal = stDefaultShadow) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ShowMinButton( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ShowMinButton( 
            /* [defaultvalue][in] */ VARIANT_BOOL newVal = TRUE) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ShowMaxButton( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ShowMaxButton( 
            /* [defaultvalue][in] */ VARIANT_BOOL newVal = TRUE) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ShowHelpButton( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ShowHelpButton( 
            /* [defaultvalue][in] */ VARIANT_BOOL newVal = TRUE) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CloseButtonColor( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CloseButtonColor( 
            /* [defaultvalue][in] */ OLE_COLOR newVal = 0xff) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Transparent( 
            /* [defaultvalue][in] */ unsigned char nAlpha,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Refresh( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISkinVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISkin * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISkin * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISkin * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISkin * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISkin * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISkin * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISkin * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AboutMe )( 
            ISkin * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ApplySkin )( 
            ISkin * This,
            /* [in] */ long hWnd,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TitleColor )( 
            ISkin * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TitleColor )( 
            ISkin * This,
            /* [defaultvalue][in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GradientBeginColor )( 
            ISkin * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_GradientBeginColor )( 
            ISkin * This,
            /* [defaultvalue][in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GradientEndColor )( 
            ISkin * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_GradientEndColor )( 
            ISkin * This,
            /* [defaultvalue][in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BackColor )( 
            ISkin * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BackColor )( 
            ISkin * This,
            /* [defaultvalue][in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TitleBarHeight )( 
            ISkin * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TitleBarHeight )( 
            ISkin * This,
            /* [defaultvalue][in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BorderWidth )( 
            ISkin * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BorderWidth )( 
            ISkin * This,
            /* [defaultvalue][in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Alpha )( 
            ISkin * This,
            /* [retval][out] */ unsigned char *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Alpha )( 
            ISkin * This,
            /* [defaultvalue][in] */ unsigned char newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShadowColor )( 
            ISkin * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShadowColor )( 
            ISkin * This,
            /* [defaultvalue][in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShadowDepth )( 
            ISkin * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShadowDepth )( 
            ISkin * This,
            /* [defaultvalue][in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShadowTransparency )( 
            ISkin * This,
            /* [retval][out] */ unsigned char *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShadowTransparency )( 
            ISkin * This,
            /* [defaultvalue][in] */ unsigned char newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShadowType )( 
            ISkin * This,
            /* [retval][out] */ ShadowTypeConstants *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShadowType )( 
            ISkin * This,
            /* [defaultvalue][in] */ ShadowTypeConstants newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShowMinButton )( 
            ISkin * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShowMinButton )( 
            ISkin * This,
            /* [defaultvalue][in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShowMaxButton )( 
            ISkin * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShowMaxButton )( 
            ISkin * This,
            /* [defaultvalue][in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShowHelpButton )( 
            ISkin * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShowHelpButton )( 
            ISkin * This,
            /* [defaultvalue][in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CloseButtonColor )( 
            ISkin * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CloseButtonColor )( 
            ISkin * This,
            /* [defaultvalue][in] */ OLE_COLOR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Transparent )( 
            ISkin * This,
            /* [defaultvalue][in] */ unsigned char nAlpha,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Refresh )( 
            ISkin * This);
        
        END_INTERFACE
    } ISkinVtbl;

    interface ISkin
    {
        CONST_VTBL struct ISkinVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISkin_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISkin_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISkin_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISkin_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISkin_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISkin_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISkin_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISkin_AboutMe(This)	\
    ( (This)->lpVtbl -> AboutMe(This) ) 

#define ISkin_ApplySkin(This,hWnd,pRetVal)	\
    ( (This)->lpVtbl -> ApplySkin(This,hWnd,pRetVal) ) 

#define ISkin_get_TitleColor(This,pVal)	\
    ( (This)->lpVtbl -> get_TitleColor(This,pVal) ) 

#define ISkin_put_TitleColor(This,newVal)	\
    ( (This)->lpVtbl -> put_TitleColor(This,newVal) ) 

#define ISkin_get_GradientBeginColor(This,pVal)	\
    ( (This)->lpVtbl -> get_GradientBeginColor(This,pVal) ) 

#define ISkin_put_GradientBeginColor(This,newVal)	\
    ( (This)->lpVtbl -> put_GradientBeginColor(This,newVal) ) 

#define ISkin_get_GradientEndColor(This,pVal)	\
    ( (This)->lpVtbl -> get_GradientEndColor(This,pVal) ) 

#define ISkin_put_GradientEndColor(This,newVal)	\
    ( (This)->lpVtbl -> put_GradientEndColor(This,newVal) ) 

#define ISkin_get_BackColor(This,pVal)	\
    ( (This)->lpVtbl -> get_BackColor(This,pVal) ) 

#define ISkin_put_BackColor(This,newVal)	\
    ( (This)->lpVtbl -> put_BackColor(This,newVal) ) 

#define ISkin_get_TitleBarHeight(This,pVal)	\
    ( (This)->lpVtbl -> get_TitleBarHeight(This,pVal) ) 

#define ISkin_put_TitleBarHeight(This,newVal)	\
    ( (This)->lpVtbl -> put_TitleBarHeight(This,newVal) ) 

#define ISkin_get_BorderWidth(This,pVal)	\
    ( (This)->lpVtbl -> get_BorderWidth(This,pVal) ) 

#define ISkin_put_BorderWidth(This,newVal)	\
    ( (This)->lpVtbl -> put_BorderWidth(This,newVal) ) 

#define ISkin_get_Alpha(This,pVal)	\
    ( (This)->lpVtbl -> get_Alpha(This,pVal) ) 

#define ISkin_put_Alpha(This,newVal)	\
    ( (This)->lpVtbl -> put_Alpha(This,newVal) ) 

#define ISkin_get_ShadowColor(This,pVal)	\
    ( (This)->lpVtbl -> get_ShadowColor(This,pVal) ) 

#define ISkin_put_ShadowColor(This,newVal)	\
    ( (This)->lpVtbl -> put_ShadowColor(This,newVal) ) 

#define ISkin_get_ShadowDepth(This,pVal)	\
    ( (This)->lpVtbl -> get_ShadowDepth(This,pVal) ) 

#define ISkin_put_ShadowDepth(This,newVal)	\
    ( (This)->lpVtbl -> put_ShadowDepth(This,newVal) ) 

#define ISkin_get_ShadowTransparency(This,pVal)	\
    ( (This)->lpVtbl -> get_ShadowTransparency(This,pVal) ) 

#define ISkin_put_ShadowTransparency(This,newVal)	\
    ( (This)->lpVtbl -> put_ShadowTransparency(This,newVal) ) 

#define ISkin_get_ShadowType(This,pVal)	\
    ( (This)->lpVtbl -> get_ShadowType(This,pVal) ) 

#define ISkin_put_ShadowType(This,newVal)	\
    ( (This)->lpVtbl -> put_ShadowType(This,newVal) ) 

#define ISkin_get_ShowMinButton(This,pVal)	\
    ( (This)->lpVtbl -> get_ShowMinButton(This,pVal) ) 

#define ISkin_put_ShowMinButton(This,newVal)	\
    ( (This)->lpVtbl -> put_ShowMinButton(This,newVal) ) 

#define ISkin_get_ShowMaxButton(This,pVal)	\
    ( (This)->lpVtbl -> get_ShowMaxButton(This,pVal) ) 

#define ISkin_put_ShowMaxButton(This,newVal)	\
    ( (This)->lpVtbl -> put_ShowMaxButton(This,newVal) ) 

#define ISkin_get_ShowHelpButton(This,pVal)	\
    ( (This)->lpVtbl -> get_ShowHelpButton(This,pVal) ) 

#define ISkin_put_ShowHelpButton(This,newVal)	\
    ( (This)->lpVtbl -> put_ShowHelpButton(This,newVal) ) 

#define ISkin_get_CloseButtonColor(This,pVal)	\
    ( (This)->lpVtbl -> get_CloseButtonColor(This,pVal) ) 

#define ISkin_put_CloseButtonColor(This,newVal)	\
    ( (This)->lpVtbl -> put_CloseButtonColor(This,newVal) ) 

#define ISkin_Transparent(This,nAlpha,pVal)	\
    ( (This)->lpVtbl -> Transparent(This,nAlpha,pVal) ) 

#define ISkin_Refresh(This)	\
    ( (This)->lpVtbl -> Refresh(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISkin_INTERFACE_DEFINED__ */



#ifndef __TSkin_LIBRARY_DEFINED__
#define __TSkin_LIBRARY_DEFINED__

/* library TSkin */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_TSkin;

EXTERN_C const CLSID CLSID_Skin;

#ifdef __cplusplus

class DECLSPEC_UUID("434DACEF-505B-4F50-B728-7AD9EAE83EBD")
Skin;
#endif
#endif /* __TSkin_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


