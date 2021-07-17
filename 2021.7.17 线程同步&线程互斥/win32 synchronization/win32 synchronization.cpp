// win32 synchronization.cpp : Defines the entry point for the application.
//


#include "stdafx.h"

HWND hEdit_RC,hEdit_B1,hEdit_B2,hEdit_C1,hEdit_C2,hEdit_C3,hEdit_C4;	//文本框句柄

HANDLE hThread[5];				//线程数组

CRITICAL_SECTION g_Buffer_CS;	//互斥体：缓冲区

HANDLE hSemaphoreEmpty;			//信号量：缓冲区有空闲

HANDLE hSemaphoreFull;			//信号量：缓冲区有资源

HWND hEditBuffer[3];			//缓冲区文本框数组

HWND hEditCustomer[4];			//消费者文本框数组

DWORD WINAPI ThreadProduct(LPVOID lpParameter)
{
	TCHAR szBuffer[256];
	TCHAR szTemp[2];
	TCHAR szTemp2[2] = { 0 };
	DWORD dwLength;
	memset(szBuffer, 0, 256);

	GetWindowText(hEdit_RC,szBuffer,256);
	dwLength = strlen(szBuffer);

	if(dwLength == 0)
		return -1;
	for(DWORD i = 0;i< dwLength ; i++)
	{
		//等待缓冲区可写资源的信息
		WaitForSingleObject(hSemaphoreEmpty, INFINITE);

		//寻找可写缓冲区
		for(DWORD k = 0; k< 2; k++)
		{
			memset(szTemp, 0, 2);
			EnterCriticalSection(&g_Buffer_CS);			//进入临界区
			 
			GetWindowText(hEditBuffer[k], szTemp, 2);

			if(!strcmp(szTemp, "0"))
			{
				//TCHAR szT[2] = {0};
				memcpy(szTemp2, &szBuffer[i], 1);
				SetWindowText(hEditBuffer[k], szTemp2);
				LeaveCriticalSection(&g_Buffer_CS);		//离开临界区
				
				break;
			}
			
			LeaveCriticalSection(&g_Buffer_CS);			//当szTemp = 0时,离开缓冲区
		}

		//发出缓冲区有资源的信息
		Sleep(1000);
		ReleaseSemaphore(hSemaphoreFull, 1, NULL);
	}

	return 0;
}
DWORD WINAPI ThreadCustomer(LPVOID lpParameter)
{
	TCHAR szTemp[2];
	TCHAR szBuffer[256];
	TCHAR szNewBuffer[256];
	DWORD dwExitCode;
	
	while(1)
	{
		//等待缓冲区有资源
		dwExitCode = WaitForSingleObject(hSemaphoreFull, 10000);

		if(dwExitCode == 0x102)
			return -1;

		//寻找可写资源的缓冲区
		EnterCriticalSection(&g_Buffer_CS);		//进入临界区

		for(DWORD k = 0; k < 2; k++)
		{
			memset(szTemp, 0, 2);
			GetWindowText(hEditBuffer[k], szTemp, 2);

			if(strcmp(szTemp, "0"))
			{
				//存储到文本框
				memset(szBuffer, 0, 256);	

				GetWindowText((HWND)lpParameter, szBuffer, 256);
				sprintf(szNewBuffer,"%s->%s", szBuffer, szTemp);
				SetWindowText((HWND)lpParameter, szNewBuffer);

				Sleep(500);
				SetWindowText(hEditBuffer[k], "0");
				break;
			}
		}
		LeaveCriticalSection(&g_Buffer_CS);		//离开临界区

		//发出缓冲区已空的信息
		Sleep(1000);
		ReleaseSemaphore(hSemaphoreEmpty, 1, NULL);
	}
	return 0;
}

DWORD WINAPI ThreadMain(LPVOID lpParameter)
{
	//创建信号量
	hSemaphoreEmpty = CreateSemaphore(NULL, 2, 2,NULL);		//缓冲区为空时允许两个线程运行
	hSemaphoreFull = CreateSemaphore(NULL, 0, 2,NULL);		//缓冲区为空时不允许线程运行
	
	//创建互斥体
	InitializeCriticalSection(&g_Buffer_CS);

	//创建线程

	hThread[0] = ::CreateThread(NULL, 0, ThreadProduct,NULL, 0, NULL);
	hThread[1] = ::CreateThread(NULL, 0, ThreadCustomer,hEdit_C1, 0, NULL);
	hThread[2] = ::CreateThread(NULL, 0, ThreadCustomer,hEdit_C2, 0, NULL);
	hThread[3] = ::CreateThread(NULL, 0, ThreadCustomer,hEdit_C3, 0, NULL);
	hThread[4] = ::CreateThread(NULL, 0, ThreadCustomer,hEdit_C4, 0, NULL);
	
	//等待线程结束

	::WaitForMultipleObjects(5, hThread, TRUE, INFINITE);	//等待所有线程结束	
	::CloseHandle(hSemaphoreEmpty);							//关闭对象句柄
	::CloseHandle(hSemaphoreFull);							//关闭对象句柄
	::DeleteCriticalSection(&g_Buffer_CS);					//删除临界区

	return 0;
}
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	BOOL bRet = FALSE;
	
	switch(uMsg)
	{
	case WM_CLOSE:
		{
			EndDialog(hDlg,0);
			break;
		}
	case WM_INITDIALOG:
		{
			hEdit_RC = GetDlgItem(hDlg,IDC_EDIT1);
			hEdit_B1 = GetDlgItem(hDlg,IDC_EDIT2);
			hEdit_B2 = GetDlgItem(hDlg,IDC_EDIT3);

			hEditBuffer[0] = hEdit_B1;
			hEditBuffer[1] = hEdit_B2;

			hEdit_C1 = GetDlgItem(hDlg,IDC_EDIT4);
			hEdit_C2 = GetDlgItem(hDlg,IDC_EDIT5);
			hEdit_C3 = GetDlgItem(hDlg,IDC_EDIT6);
			hEdit_C4 = GetDlgItem(hDlg,IDC_EDIT7);

			hEditCustomer[0] = hEdit_C1;
			hEditCustomer[1] = hEdit_C2;
			hEditCustomer[2] = hEdit_C3;
			hEditCustomer[3] = hEdit_C4;

			SetWindowText(hEdit_RC,"0");
			SetWindowText(hEdit_B1,"0");
			SetWindowText(hEdit_B2,"0");
			SetWindowText(hEdit_C1,"");
			SetWindowText(hEdit_C2,"");
			SetWindowText(hEdit_C3,"");
			SetWindowText(hEdit_C4,"");
			
			break;
		}
	case WM_COMMAND:
		
		switch (LOWORD (wParam))
		{
		case IDC_BUTTON_BEGIN:
			{

				CreateThread(NULL, 0, ThreadMain,NULL, 0, NULL);
				
				return TRUE;
			}
		}
		
		break ;
	}
	
	return bRet;
}
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.

	DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG_MAIN),NULL,MainDlgProc);

	return 0;
}

