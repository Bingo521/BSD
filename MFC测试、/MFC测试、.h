
// MFC���ԡ�.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFC���ԡ�App:
// �йش����ʵ�֣������ MFC���ԡ�.cpp
//

class CMFC���ԡ�App : public CWinApp
{
public:
	CMFC���ԡ�App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFC���ԡ�App theApp;