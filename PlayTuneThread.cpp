// PlayTuneThead.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "PlayTuneThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMidiWorksApp	theApp;

/////////////////////////////////////////////////////////////////////////////
// CPlayTuneThread

IMPLEMENT_DYNCREATE(CPlayTuneThread, CWinThread)

CPlayTuneThread::CPlayTuneThread()
{
}

CPlayTuneThread::~CPlayTuneThread()
{
}

BOOL CPlayTuneThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	//		Start the associated dialog box
	m_pMainWnd = &theApp.m_dlgPlayerDialogList;

	theApp.m_dlgPlayerDialogList.DoModal ();

	return TRUE;
}

int CPlayTuneThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPlayTuneThread, CWinThread)
	//{{AFX_MSG_MAP(CPlayTuneThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayTuneThread message handlers
