

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for SystemGapCOM.idl:
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

#ifndef __SystemGapCOM_i_h__
#define __SystemGapCOM_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISystemGapObject_FWD_DEFINED__
#define __ISystemGapObject_FWD_DEFINED__
typedef interface ISystemGapObject ISystemGapObject;

#endif 	/* __ISystemGapObject_FWD_DEFINED__ */


#ifndef __SystemGapObject_FWD_DEFINED__
#define __SystemGapObject_FWD_DEFINED__

#ifdef __cplusplus
typedef class SystemGapObject SystemGapObject;
#else
typedef struct SystemGapObject SystemGapObject;
#endif /* __cplusplus */

#endif 	/* __SystemGapObject_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ISystemGapObject_INTERFACE_DEFINED__
#define __ISystemGapObject_INTERFACE_DEFINED__

/* interface ISystemGapObject */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISystemGapObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("793586dd-ce44-432f-a5e8-358791e99e1d")
    ISystemGapObject : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Create( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISystemGapObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISystemGapObject * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISystemGapObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISystemGapObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISystemGapObject * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISystemGapObject * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISystemGapObject * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISystemGapObject * This,
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
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Create )( 
            ISystemGapObject * This);
        
        END_INTERFACE
    } ISystemGapObjectVtbl;

    interface ISystemGapObject
    {
        CONST_VTBL struct ISystemGapObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISystemGapObject_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISystemGapObject_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISystemGapObject_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISystemGapObject_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISystemGapObject_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISystemGapObject_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISystemGapObject_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISystemGapObject_Create(This)	\
    ( (This)->lpVtbl -> Create(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISystemGapObject_INTERFACE_DEFINED__ */



#ifndef __SystemGapCOMLib_LIBRARY_DEFINED__
#define __SystemGapCOMLib_LIBRARY_DEFINED__

/* library SystemGapCOMLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_SystemGapCOMLib;

EXTERN_C const CLSID CLSID_SystemGapObject;

#ifdef __cplusplus

class DECLSPEC_UUID("b8209762-5da8-44b8-905e-d01375b074d2")
SystemGapObject;
#endif
#endif /* __SystemGapCOMLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


