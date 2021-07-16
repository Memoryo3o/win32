// Tools.h: interface for the Tools class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLS_H__1F806FB5_01E3_4467_8252_B95D458D6684__INCLUDED_)
#define AFX_TOOLS_H__1F806FB5_01E3_4467_8252_B95D458D6684__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

void __cdecl OutputDebugStringF(const char *format, ...); 

#ifdef _DEBUG  
#define DbgPrintf   OutputDebugStringF  
#else  
#define DbgPrintf  
#endif 


#endif // !defined(AFX_TOOLS_H__1F806FB5_01E3_4467_8252_B95D458D6684__INCLUDED_)
