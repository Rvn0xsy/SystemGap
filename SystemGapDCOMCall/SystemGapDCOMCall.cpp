// SystemGapDCOMCall.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <Windows.h>
#include <iostream>
#include <rpc.h>
#include <assert.h>
#include <combaseapi.h>
#include <atlbase.h>
#include "SystemGapCOM_i.h"

#import "SystemGapCOM.tlb"


int main()
{
	WCHAR w_username[] = L"Administrator";
	WCHAR w_password[] = L"***";
	WCHAR w_nodename[] = L"192.168.**.**";
	WCHAR w_domain[] = L"WORKGROUP";

	COAUTHINFO      AuthInfo;
	COAUTHIDENTITY  AuthIdentity;
	COSERVERINFO    ServerInfo;
	MULTI_QI        Results = { &IID_ISystemGapObject , NULL, NULL };
	HRESULT         hr;
	ISystemGapObject* SystemGapIntfObj;

	AuthIdentity.Domain = (unsigned short*)w_domain;
	AuthIdentity.DomainLength = wcslen(w_domain);
	AuthIdentity.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
	AuthIdentity.Password = (unsigned short*)w_password;
	AuthIdentity.PasswordLength = wcslen(w_password);
	AuthIdentity.User = (unsigned short*)w_username;
	AuthIdentity.UserLength = wcslen(w_username);


	AuthInfo.dwAuthnSvc = RPC_C_AUTHN_WINNT;
	AuthInfo.dwAuthzSvc = RPC_C_AUTHZ_NONE;
	AuthInfo.pwszServerPrincName = NULL;
	AuthInfo.dwAuthnLevel = RPC_C_AUTHN_LEVEL_CALL;
	AuthInfo.dwCapabilities = EOAC_NONE;
	AuthInfo.dwImpersonationLevel = RPC_C_IMP_LEVEL_IMPERSONATE;
	AuthInfo.pAuthIdentityData = &AuthIdentity;


	ServerInfo.dwReserved1 = 0;
	ServerInfo.dwReserved2 = 0;
	ServerInfo.pAuthInfo = &AuthInfo;
	ServerInfo.pwszName = w_nodename;

	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr)) {
		wprintf(L"CoInitialize Error : %d , %x\n", GetLastError(), hr);
		return -1;
	}

	hr = ::CoInitializeSecurity(
		NULL,                    // points to security descriptor
		-1,                     // count of entries in asAuthSvc
		NULL,                    // array of names to register
		NULL,                    // reserved for future use
		RPC_C_AUTHN_LEVEL_NONE, // the default authentication level for proxies
		RPC_C_IMP_LEVEL_IMPERSONATE,// the default impersonation level for proxies
		NULL,                    // used only on Windows 2000
		EOAC_NONE,              // additional client or server-side capabilities
		NULL                     // reserved for future use
	);
	if (FAILED(hr)) {
		wprintf(L"CoInitializeSecurity Error : %d , %x\n", GetLastError(), hr);
		return -1;
	}


	hr = CoCreateInstanceEx(CLSID_SystemGapObject, NULL, CLSCTX_SERVER, &ServerInfo, 1, &Results);
	if (FAILED(Results.hr)) {
		wprintf(L"CoCreateInstanceEx Error : %d , %x\n", GetLastError(), hr);
		return -1;
	}
	if (Results.pItf != NULL) {
		hr = CoSetProxyBlanket(
			Results.pItf, 
			RPC_C_AUTHN_WINNT, 
			RPC_C_AUTHZ_NONE, 
			NULL, 
			RPC_C_AUTHN_LEVEL_CALL, 
			RPC_C_IMP_LEVEL_IMPERSONATE,
			&AuthIdentity, 
			EOAC_NONE
		);
		if (FAILED(hr)) {
			wprintf(L"CoSetProxyBlanket Error : %d , %x\n", GetLastError(), hr);
			return -1;
		}
	}
	
	SystemGapIntfObj = (ISystemGapObject*)Results.pItf;
	SystemGapIntfObj->Create();

}