// win32 snatch.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

HWND hSum, hEditA, hEditB, hEditC;
HANDLE hThreadArr[4], hMoniter;

HANDLE hMutex;

DWORD WINAPI ThreadProc1(LPVOID lpParameter)					
{					
	::WaitForMultipleObjects(3, hThreadArr, TRUE, -1);	//使用WaitForMultipleObjects监听线程
	
	for (int i = 0; i < 3 ; i++)
	{
		::CloseHandle(hThreadArr[i]);		//调用CloseHandle关闭句柄
	}

	MessageBox(0, TEXT("红包已抢完!"), TEXT("Snatch"), 0);
					
	return 0;				
}

DWORD WINAPI ThreadProc2(LPVOID lpParameter)					
{			
	TCHAR szBuffer[10];
	DWORD SumNum;
	DWORD Num;

	memset(szBuffer, 0 , sizeof(szBuffer));

	while (true)
	{
		WaitForSingleObject(hMutex, INFINITE);	

		GetWindowText(hSum, szBuffer ,10);		//获取Sum窗口的数值
		sscanf(szBuffer, "%d", &SumNum );		//字符串转整数	


		if (SumNum < 50)
		{
			   ReleaseMutex(hMutex);		//若小于50则离开临界区
			   break;
		}

		SumNum -= 50;

		//Sleep(200);

		memset(szBuffer, 0 , sizeof(szBuffer));		//清空缓冲区
		sprintf(szBuffer, "%d", SumNum);			//整数转字符串
		SetWindowText(hSum, szBuffer);				//写回线程

		//A B C窗口中的数值+50
		memset(szBuffer, 0 ,sizeof(szBuffer));		//清空缓冲区

		GetWindowText((HWND)lpParameter, szBuffer, 10);		//获取子窗口的数值
		sscanf(szBuffer, "%d", &Num);				//字符串转整数

		Num += 50;

		memset(szBuffer, 0 ,sizeof(szBuffer));		//清空缓冲区
		sprintf(szBuffer, "%d", Num);				//整数转字符串
		SetWindowText((HWND)lpParameter, szBuffer);	//写回线程

		ReleaseMutex(hMutex);				//离开临界区

		Sleep(200);

	}

	OutputDebugStringF("%d", (DWORD)lpParameter);

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
			hSum = GetDlgItem(hDlg,IDC_EDIT1);
			hEditA = GetDlgItem(hDlg,IDC_EDIT2);
			hEditB = GetDlgItem(hDlg,IDC_EDIT3);
			hEditC = GetDlgItem(hDlg,IDC_EDIT4);

			
			SetWindowText(hSum, "1000");
			SetWindowText(hEditA, "0");
			SetWindowText(hEditB, "0");
			SetWindowText(hEditC, "0");
					
			break;		
		}			
	case WM_COMMAND:				
					
		switch (LOWORD (wParam))			
		{			
		case IDC_BUTTON1:			
			{		
				//创建线程
				hThreadArr[0] = ::CreateThread(NULL, 0, ThreadProc2, hEditA, 0, NULL);
				hThreadArr[1] = ::CreateThread(NULL, 0, ThreadProc2, hEditB, 0, NULL);
				hThreadArr[2] = ::CreateThread(NULL, 0, ThreadProc2, hEditC, 0, NULL);
				hThreadArr[3] = ::CreateThread(NULL, 0, ThreadProc1, NULL, 0, NULL);

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
	hMutex = CreateMutex(NULL, false, TEXT("my mutex"));

	DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG_MAIN),NULL,MainDlgProc);	

	return 0;
}



