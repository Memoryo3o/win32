// win32_create_process2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	char szBuffer[256] = {0};					
					
	memcpy(szBuffer,argv[1],8);					
						
	DWORD dwHandle = 0;					
						
	sscanf(szBuffer,"%x",&dwHandle);					
						
	printf("%s\n",argv[0]);					
						
	printf("%x\n",dwHandle);					
						
	HANDLE g_hEvent = (HANDLE)dwHandle;					
						
						
	printf("开始等待.....\n");					
	//当事件变成已通知时 					
	WaitForSingleObject(g_hEvent, INFINITE);					
						
	DWORD dwCode = GetLastError();					
						
	printf("等到消息.....%x\n",dwCode);					
						
	getchar();					

	return 0;
}

