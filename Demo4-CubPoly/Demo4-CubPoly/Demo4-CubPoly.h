
// Demo4-CubPoly.h : Demo4-CubPoly 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CDemo4CubPolyApp:
// 有关此类的实现，请参阅 Demo4-CubPoly.cpp
//

class CDemo4CubPolyApp : public CWinAppEx
{
public:
	CDemo4CubPolyApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDemo4CubPolyApp theApp;
