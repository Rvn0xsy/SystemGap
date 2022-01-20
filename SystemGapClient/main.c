#include <Windows.h>
#include <stdio.h>

BOOL ReciveSystemGapMsg(HANDLE hGap) {
	if (hGap == NULL)
		return FALSE;
	DWORD dwBytesRead = 0;
	char buffer[100];
	while (ReadFile(hGap, buffer, 100, &dwBytesRead, NULL) != 0)
	{
		printf("%s", buffer);
		memset(buffer, 0, 100);
		Sleep(100);
	}
	return TRUE;
}

BOOL SendSystemGapMsg(char * gap_name,char * msg,int msg_len) {
	DWORD dwWritten = 0;
	WaitNamedPipe(gap_name, NMPWAIT_WAIT_FOREVER);
	HANDLE hGap = CreateFile(gap_name, GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hGap == INVALID_HANDLE_VALUE) {
		printf("[+]Can't Open Gap Error %d \n", GetLastError());
		return FALSE;
	}
	WriteFile(hGap, msg, msg_len, &dwWritten, NULL);
	if (dwWritten == dwWritten) {
		printf("[+]Send %d Bytes.\n", dwWritten);
		ReciveSystemGapMsg(hGap);
		CloseHandle(hGap);
		return TRUE;
	}
	CloseHandle(hGap);
	return FALSE;
}


int main(int argc, char* argv[]) {
	if (argc < 2)
	{
		printf("[+]Usage : SystemGapClient.exe <Command>\n");
		return 0;
	}
	// SystemGap
	SendSystemGapMsg("\\\\.\\pipe\\SystemGap", argv[1], strlen(argv[1]));
	return 0;
}