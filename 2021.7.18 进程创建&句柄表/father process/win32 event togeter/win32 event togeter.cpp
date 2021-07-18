// win32 event togeter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

//事件和临界区		
HANDLE g_hSet, g_hClear;		
int g_Max = 10;		
int g_Number = 0;		
                   		
//生产者线程函数  		
DWORD WINAPI ThreadProduct(LPVOID pM)  		
{  		
    for (int i = 0; i < g_Max; i++)		
    {  		
        WaitForSingleObject(g_hSet, INFINITE);  		
		g_Number = 1; 
		DWORD id = GetCurrentThreadId();
		printf("生产者%d将数据%d放入缓冲区\n",id, g_Number); 
        SetEvent(g_hClear);   		
    }  		
    return 0;  		
}  		
//消费者线程函数		
DWORD WINAPI ThreadConsumer(LPVOID pM)  		
{  		
    for (int i = 0; i < g_Max; i++)		
    {  		
        WaitForSingleObject(g_hClear, INFINITE);  		
		g_Number = 0; 
		DWORD id = GetCurrentThreadId();
		printf("****消费者%d将数据%d放入缓冲区****\n",id, g_Number); 
        SetEvent(g_hSet);   		
    }  		
    return 0;  		
}  		
		
int main(int argc, char* argv[])		
{		
		
    HANDLE hThread[2]; 		
		
	g_hSet = CreateEvent(NULL, FALSE, TRUE, NULL);  	
    g_hClear = CreateEvent(NULL, FALSE, FALSE, NULL); 		
		
    hThread[0] = ::CreateThread(NULL, 0, ThreadProduct, NULL, 0, NULL); 		
	hThread[1] = ::CreateThread(NULL, 0, ThreadConsumer, NULL, 0, NULL);	
		
    WaitForMultipleObjects(2, hThread, TRUE, INFINITE);  		
    CloseHandle(hThread[0]);  		
    CloseHandle(hThread[1]);		
		
	//销毁 	
	CloseHandle(g_hSet);  	
    CloseHandle(g_hClear);  		
		
	getchar();

	return 0;	
}		

