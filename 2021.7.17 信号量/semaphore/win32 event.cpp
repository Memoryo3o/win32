// win32 event.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

HANDLE hSemaphore;				
				
HWND hEdit1,hEdit2,hEdit3,hEdit4;				
HANDLE hThread1,hThread2,hThread3,hThread4;

HANDLE hThreadArr[4];				
				

DWORD WINAPI ThreadProc2(LPVOID lpParameter)				
{				
	TCHAR szBuffer[10];			
	DWORD dwTimmer=0;			
	WaitForSingleObject(hSemaphore, INFINITE);	
	
	//DWORD dwIndex = (DWORD)lpParameter;
	while(dwTimmer<50)			
	{			
		Sleep(100);		
		memset(szBuffer,0,10);		
		GetWindowText((HWND)lpParameter,szBuffer,10);
		//GetWindowText(hThreadArr[dwIndex],szBuffer,10);
		sscanf( szBuffer, "%d", &dwTimmer );		
		dwTimmer++;		
		memset(szBuffer,0,10);		
		sprintf(szBuffer,"%d",dwTimmer);		
		SetWindowText((HWND)lpParameter,szBuffer);	
		//SetWindowText(hThreadArr[dwIndex],szBuffer);
	}			
	ReleaseSemaphore(hSemaphore, 1, NULL);			
	return 0;			
	
}			
		
DWORD WINAPI ThreadMain(LPVOID lpParameter)				
{			
	TCHAR szBuffer[10];	
	DWORD dwMoney=0;	


	//创建信号量			
	hSemaphore = CreateSemaphore(NULL,0,3,NULL);			
			
	HANDLE hThread[3];			
	//创建3个线程			
	hThread[0] = ::CreateThread(NULL, 0, ThreadProc2, hEdit2, 0, NULL);	
	//hThread[0] = ::CreateThread(NULL, 0, ThreadProc2, (void*)0, 0, NULL);	
	hThread[1] = ::CreateThread(NULL, 0, ThreadProc2, hEdit3, 0, NULL);			
	hThread[2] = ::CreateThread(NULL, 0, ThreadProc2, hEdit4, 0, NULL);			
				
	//设置文本框的值			
	
	while(dwMoney<1000)			
	{			
		memset(szBuffer,0,10);		
		GetWindowText(hEdit1,szBuffer,10);		
		sscanf( szBuffer, "%d", &dwMoney );		
		dwMoney++;		
		memset(szBuffer,0,10);		
		sprintf(szBuffer,"%d",dwMoney);		
		SetWindowText(hEdit1,szBuffer);		
	}			

				
	//设置事件为已通知			
	ReleaseSemaphore(hSemaphore, 2, NULL);	
	
	Sleep(500);
		
				
	//等待线程结束 销毁内核对象			
	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);  			
	CloseHandle(hThread[0]);  			
	CloseHandle(hThread[1]);			
	CloseHandle(hThread[2]);			
	CloseHandle(hSemaphore);  			
				
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
			hEdit1 = GetDlgItem(hDlg,IDC_EDIT1);
			hEdit2 = GetDlgItem(hDlg,IDC_EDIT2);
			hEdit3 = GetDlgItem(hDlg,IDC_EDIT3);
			hEdit4 = GetDlgItem(hDlg,IDC_EDIT4);

			
			SetWindowText(hEdit1, "0");
			SetWindowText(hEdit2, "0");
			SetWindowText(hEdit3, "0");
			SetWindowText(hEdit4, "0");

			//hThreadArr[0] = hEdit2;
			//hThreadArr[1] = hEdit3;
			//hThreadArr[2] = hEdit4;					
			break;		
		}			
	case WM_COMMAND:				
					
		switch (LOWORD (wParam))			
		{			
		case IDC_BUTTON_BEGIN:			
			{		
				::CreateThread(NULL, 0, ThreadMain, NULL, 0, NULL);			

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
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, MainDlgProc);

	return 0;
}



