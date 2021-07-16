// thread CONTEXT.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

HWND hEdit;
HANDLE hThread;

DWORD WINAPI ThreadProc1(LPVOID lpParameter)					
{					
	TCHAR szBuffer[10];				
	DWORD dwIndex = 0;				
	DWORD dwCount;				
					
	while(dwIndex<1000)				
	{		
		GetWindowText(hEdit,szBuffer,10);			
		sscanf( szBuffer, "%d", &dwCount );			
		dwCount++;		
		
		Sleep(200);

		memset(szBuffer,0,10);			
		sprintf(szBuffer,"%d",dwCount);			
		SetWindowText(hEdit,szBuffer);			
		dwIndex++;			
	}				
					
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
			hEdit = GetDlgItem(hDlg,IDC_EDIT);		
			SetWindowText(hEdit,"0");		
					
			break;		
		}			
	case WM_COMMAND:				
					
		switch (LOWORD (wParam))			
		{			
		case IDC_BUTTON1:			
			{		
				//创建线程
				hThread = ::CreateThread(NULL, 0, ThreadProc1, 	NULL, 0, NULL);
						
				return TRUE;	
			}		
		case IDC_BUTTON2:			
			{		
				//挂起线程
				::SuspendThread(hThread);	

				/*CONTEXT context;
			
				//设置要获取的类型					
					
				context.ContextFlags = CONTEXT_CONTROL;					
									
				//获取					
									
				BOOL ok = ::GetThreadContext(hThread,&context);					

				//设置					
									
				context.Eip = 0x401000;					
									
				SetThreadContext(hThread,&context);	
				
				::ResumeThread(hThread);*/

				return TRUE;	
			}
		case IDC_BUTTON3:			
			{		
				//恢复线程
				::ResumeThread(hThread);		
						
				return TRUE;	
			}
		case IDC_BUTTON4:			
			{			
				::TerminateThread(hThread,2);				
				::WaitForSingleObject(hThread,INFINITE);				

						
				return TRUE;	
			}
		case IDC_BUTTON5:
			{
				DWORD dwExitCode = 0;
				::GetExitCodeThread(hThread, &dwExitCode);

				OutputDebugStringF("The process ID is:%d",dwExitCode);


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



