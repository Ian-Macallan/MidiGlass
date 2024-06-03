// ModifyThread.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "ModifyThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMidiWorksApp	theApp;

/////////////////////////////////////////////////////////////////////////////
// CModifyThread

IMPLEMENT_DYNCREATE(CModifyThread, CWinThread)

CModifyThread::CModifyThread()
{
}

CModifyThread::~CModifyThread()
{
}

BOOL CModifyThread::InitInstance()
{
	// TODO
	m_pMainWnd = &theApp.m_ModifyThreadDialog;

	theApp.m_ModifyThreadDialog.DoModal ();

	return TRUE;
}

int CModifyThread::ExitInstance()
{
	// TODO
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CModifyThread, CWinThread)
	//{{AFX_MSG_MAP(CModifyThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifyThread message handlers
