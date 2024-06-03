// PlayDirThreadProc.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "PlayDirThreadProc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMidiWorksApp	theApp;

/////////////////////////////////////////////////////////////////////////////
// CPlayDirThreadProc

IMPLEMENT_DYNCREATE(CPlayDirThreadProc, CWinThread)

CPlayDirThreadProc::CPlayDirThreadProc()
{
}

CPlayDirThreadProc::~CPlayDirThreadProc()
{
}

BOOL CPlayDirThreadProc::InitInstance()
{
	// TODO
	m_pMainWnd = &theApp.m_dlgPlayerDialogDirectory;

	theApp.m_dlgPlayerDialogDirectory.DoModal ();

	return TRUE;
}

int CPlayDirThreadProc::ExitInstance()
{
	// TODO
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPlayDirThreadProc, CWinThread)
	//{{AFX_MSG_MAP(CPlayDirThreadProc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayDirThreadProc message handlers


