// win32 create process.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>				
					
				
int main(int argc, char* argv[])
{
	char szBuffer[256] = {0};
	char szHandle[8] = {0};		


	SECURITY_ATTRIBUTES sa;			
	sa.nLength = sizeof(sa);			
	sa.lpSecurityDescriptor = NULL;			
	sa.bInheritHandle = TRUE; 			

	//创建一个可以被继承的内核对象						
	HANDLE g_hEvent = CreateEvent(&sa, TRUE, FALSE, NULL);						
							
	//组织命令行参数						
	sprintf(szHandle,"%x",g_hEvent);						
	sprintf(szBuffer,"C:/project/win32_create_process2/Debug/win32_create_process2.exe %s",szHandle);						
							
	//定义创建进程需要用的结构体						
	STARTUPINFO si = {0};   						
	PROCESS_INFORMATION pi;						
	si.cb = sizeof(si);	
	
						
	//创建子进程						
	BOOL res = CreateProcess(						
		NULL, 					
		szBuffer, 					
		NULL, 					
		NULL, 					
		TRUE, 				
		CREATE_NEW_CONSOLE, 					
		NULL, 					
		NULL, &si, &pi); 	
	
	//设置事件为已通知			
	SetEvent(g_hEvent);			
				
	//关闭句柄 内核对象不会被销毁		
	CloseHandle(g_hEvent);  			

	getchar();

	return 0;
}

