
// DriveSim.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDriveSimApp: 
// �йش����ʵ�֣������ DriveSim.cpp
//

class CDriveSimApp : public CWinApp
{
public:
	CDriveSimApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDriveSimApp theApp;