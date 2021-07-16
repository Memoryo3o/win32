// tools.h: interface for the tools class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLS_H__08AEB5EE_46C5_4A67_A479_89B13A2A5D47__INCLUDED_)
#define AFX_TOOLS_H__08AEB5EE_46C5_4A67_A479_89B13A2A5D47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

void __cdecl OutputDebugStringF(const char *format, ...); 

#ifdef _DEBUG  
#define DbgPrintf   OutputDebugStringF  
#else  
#define DbgPrintf  
#endif 

#endif // !defined(AFX_TOOLS_H__08AEB5EE_46C5_4A67_A479_89B13A2A5D47__INCLUDED_)
