// PlayListProperty.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "PlayListProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MainFrm.h"

extern CMidiWorksApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPlayListProperty dialog


CPlayListProperty::CPlayListProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayListProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayListProperty)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPlayListProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayListProperty)
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDC_FILENAME, m_Filename);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayListProperty, CDialog)
	//{{AFX_MSG_MAP(CPlayListProperty)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayListProperty message handlers

HBRUSH CPlayListProperty::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	HBRUSH hBrush = FriendCtlColor(pDC, pWnd, nCtlColor);
	if ( hBrush != NULL )
	{
		return hBrush;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CPlayListProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CMainFrame			*hMainFrame;
	char				*pFilename;

	hMainFrame = dynamic_cast< CMainFrame * >( GetParent() );
	hMainFrame = dynamic_cast< CMainFrame * >( AfxGetMainWnd( ) );

	//		Fills the combo text.
	pFilename = hMainFrame->GetCurrentPlaylistFilename ();
	if ( pFilename != NULL )
	{
		m_Filename.SetWindowText ( pFilename );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPlayListProperty::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	return CDialog::OnEraseBkgnd ( pDC );
}
