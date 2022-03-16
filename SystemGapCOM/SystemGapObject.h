// SystemGapObject.h: CSystemGapObject 的声明

#pragma once
#include "resource.h"       // 主符号
#include <Windows.h>
#include <stdio.h>
#include <aclapi.h>
#define BUFF_SIZE 1024


#include "SystemGapCOM_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CSystemGapObject

class ATL_NO_VTABLE CSystemGapObject :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSystemGapObject, &CLSID_SystemGapObject>,
	public IDispatchImpl<ISystemGapObject, &IID_ISystemGapObject, &LIBID_SystemGapCOMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CSystemGapObject()
	{
	}

DECLARE_REGISTRY_RESOURCEID(106)


BEGIN_COM_MAP(CSystemGapObject)
	COM_INTERFACE_ENTRY(ISystemGapObject)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

private:
	BOOL GenerateEveryoneSecAttr(PSECURITY_ATTRIBUTES sa);
	VOID TransmissionData(HANDLE hRead, HANDLE hGap);
	BOOL ExecCommand(HANDLE hGap, char* szBuffer);
	HANDLE CreateSystemGap(char* gap_name);
	VOID HandleGapMsg(HANDLE hGap);

public:
	STDMETHODIMP Create();
	

};

OBJECT_ENTRY_AUTO(__uuidof(SystemGapObject), CSystemGapObject)
