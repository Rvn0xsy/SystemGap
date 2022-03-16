// SystemGapCOMCall.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <combaseapi.h>
#include <Windows.h>
#include <assert.h>
#include <atlbase.h>

int main()
{
    HRESULT hr = CoInitialize(NULL);

    // ProgId值存放
    CLSID clsid;

    // 通过 ProgID 取得组件的 CLSID
    hr = ::CLSIDFromProgID(L"SystemGapCOM.SystemGapObject", &clsid);
    assert(hr != S_OK);

    CComPtr<IUnknown>UnkPtr;

    hr = ::CoCreateInstance(clsid, NULL, CLSCTX_ALL, IID_IUnknown, (LPVOID*)&UnkPtr);
    CComDispatchDriver DispatchInterface(UnkPtr);

    hr = DispatchInterface.InvokeN((LPCOLESTR)L"Create", NULL, NULL, NULL);

    assert(hr != S_OK);

    CoUninitialize();
}
