
// GenVelocity.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGenVelocityApp: 
// �йش����ʵ�֣������ GenVelocity.cpp
//

class CGenVelocityApp : public CWinApp
{
public:
	CGenVelocityApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGenVelocityApp theApp;