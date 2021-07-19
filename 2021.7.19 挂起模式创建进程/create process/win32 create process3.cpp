// win32 create process3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	STARTUPINFO ie_si = {0};   				
	PROCESS_INFORMATION ie_pi;				
	ie_si.cb = sizeof(ie_si);				
					
	TCHAR szBuffer[256] = "C:\\Documents and Settings\\Administrator\\桌面\\notepad.exe";				
	CreateProcess(				
		NULL,                  			
		szBuffer,                			
		NULL, 			
		NULL,  			
		FALSE,                   			
		CREATE_SUSPENDED,     			
		NULL,                    			
		NULL,                    			
		&ie_si,                  			
		&ie_pi                  			
		);			
					
	//获取外壳程序的入口点(OEP)

	CONTEXT contx;  				
	contx.ContextFlags = CONTEXT_FULL;  				
	GetThreadContext(ie_pi.hThread, &contx);			
	
	printf("%x\n", contx.Eax);

	//获取ImageBase						
	char* baseAddress = (CHAR *) contx.Ebx+8;						
							
	memset(szBuffer,0,256);						
							
	ReadProcessMemory(ie_pi.hProcess,baseAddress,szBuffer,4,NULL);						



	//恢复执行				
	ResumeThread(ie_pi.hThread);				

	

	return 0;
}

