
// Demo4-CubPoly.h : Demo4-CubPoly Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CDemo4CubPolyApp:
// �йش����ʵ�֣������ Demo4-CubPoly.cpp
//

class CDemo4CubPolyApp : public CWinAppEx
{
public:
	CDemo4CubPolyApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDemo4CubPolyApp theApp;
