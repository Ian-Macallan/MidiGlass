// GeneralView.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "GeneralView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MidiDoc.h"
#include "MidiFile.h"
#include "MainFrm.h"

extern CMidiWorksApp theApp;

//
/////////////////////////////////////////////////////////////////
// CGeneralView
//
/////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CGeneralView, CMWFormView)

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
CGeneralView::CGeneralView() : CMWFormView(CGeneralView::IDD)
{
	//{{AFX_DATA_INIT(CGeneralView)
	//}}AFX_DATA_INIT
	m_bFirstTime = true;
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
CGeneralView::~CGeneralView()
{
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CGeneralView::DoDataExchange(CDataExchange* pDX)
{
	CMWFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralView)
	DDX_Control(pDX, IDC_WEB, m_Web_Address);
	DDX_Control(pDX, IDC_MAIL, m_Mail_Address);
	DDX_Control(pDX, IDC_TEXT, m_Text);
	DDX_Control(pDX, IDC_NBTRACKS, m_NbTracks);
	DDX_Control(pDX, IDC_FORMAT, m_Format);
	DDX_Control(pDX, IDC_DIVISION_LO, m_Division_Lo);
	DDX_Control(pDX, IDC_DIVISION_HI, m_Division_Hi);
	//}}AFX_DATA_MAP
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGeneralView, CMWFormView)
	//{{AFX_MSG_MAP(CGeneralView)
	ON_EN_CHANGE(IDC_DIVISION_HI, OnChangeDivisionHi)
	ON_EN_CHANGE(IDC_DIVISION_LO, OnChangeDivisionLo)
	ON_BN_CLICKED(IDC_MAIL, OnMail)
	ON_BN_CLICKED(IDC_WEB, OnWeb)
	//}}AFX_MSG_MAP
	ON_WM_MOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////
// CGeneralView diagnostics
//
/////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CGeneralView::AssertValid() const
{
	CMWFormView::AssertValid();
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CGeneralView::Dump(CDumpContext& dc) const
{
	CMWFormView::Dump(dc);
}
#endif //_DEBUG

//
/////////////////////////////////////////////////////////////////
// CGeneralView message handlers
//
/////////////////////////////////////////////////////////////////
void CGeneralView::DisplayValues()
{
	char				szString [ MAX_INT_STRING ];
	unsigned char		*pString;
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		sprintf_s ( szString, sizeof ( szString ), "%d",
					pMidifile->GetMidiFileFormat () );
		m_Format.SetWindowText ( szString );

		sprintf_s ( szString, sizeof ( szString ), "%d",
					pMidifile->GetMidiFileTracksCount () );
		m_NbTracks.SetWindowText ( szString );

		unsigned lo = ( unsigned ) pMidifile->GetMidiFileDivisionLo ();
		sprintf_s ( szString, sizeof ( szString ), "%u", lo );
		m_Division_Lo.SetWindowText ( szString );

		unsigned hi = ( unsigned ) pMidifile->GetMidiFileDivisionHi ();
		sprintf_s ( szString, sizeof ( szString ), "%u", hi );
		m_Division_Hi.SetWindowText ( szString );

		if ( pMidifile->GetWebAddress ( ) != "" )
		{
			m_Web_Address.SetWindowText ( pMidifile->GetWebAddress ( ) );
		}
		else
		{
			m_Web_Address.SetWindowText ( "http://localhost/" );
		}

		if ( pMidifile->GetEMail ( ) != "" )
		{
			m_Mail_Address.SetWindowText ( pMidifile->GetEMail ( ) );
		}
		else
		{
			m_Mail_Address.SetWindowText ( "mailto:admin@localhost" );
		}

		pMidifile->ResetContext ();

		m_Text.SetWindowText ( "" );

		strcpy_s ( szString, sizeof ( szString ), "\r\n" );

		int iCount = 0;

		//		Limit Copyright Notice to 1 line
		do
		{
			//		Get Copyright Text
			pString = 
				pMidifile->GetText ( MASK_COPYRIGHT );

			if ( pString != NULL )
			{
				m_Text.SetSel ( -1, -1, true );
				m_Text.ReplaceSel ( ( char * ) pString );

				m_Text.SetSel ( -1, -1, true );
				m_Text.ReplaceSel ( szString );
			}
			iCount++;
		}
		while ( pString != NULL && iCount < 1 );

	}
	else
	{
		m_Format.SetWindowText ( "" );
		m_NbTracks.SetWindowText ( "" );
		m_Division_Lo.SetWindowText ( "" );
		m_Division_Hi.SetWindowText ( "" );
		m_Text.SetWindowText ( "" );
	}
	

}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CGeneralView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO
	if ( m_bFirstTime || ( lHint & RIGHT_MASK_HEADER ) || lHint == 0 )
	{
		m_bFirstTime = false;
		DisplayValues ();
	}
	
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
unsigned CGeneralView::GetIDD()
{
	return ( IDD );
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CGeneralView::OnChangeDivisionHi() 
{
	// TODO
	// send this notification unless you override the CMWFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO
	char				szString [ MAX_INT_STRING ];
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		m_Division_Hi.GetWindowText ( szString, sizeof ( szString ) );
		if ( pMidifile->GetMidiFileDivisionHi () != atoi ( szString ) )
		{
			pMidifile->SetMidiFileDivisionHi ( atoi ( szString ) );

			CMWFormView::SetModified ( TRUE );
		}

	}
	
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CGeneralView::OnChangeDivisionLo() 
{
	// TODO
	// send this notification unless you override the CMWFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO
	char				szString [ MAX_INT_STRING ];
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{

		m_Division_Lo.GetWindowText ( szString, sizeof ( szString ) );
		if ( pMidifile->GetMidiFileDivisionLo () != atoi ( szString ) )
		{
			pMidifile->SetMidiFileDivisionLo ( atoi ( szString ) );

			CMWFormView::SetModified ( TRUE );
		}
	}
	
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CGeneralView::OnInitialUpdate() 
{
	CMWFormView::OnInitialUpdate();
	
	// TODO
	if ( ! theApp.m_bSplitterWindow )
	{
		GetParentFrame ( )->RecalcLayout ( );
		if ( GetParentFrame ( ) != dynamic_cast< CMainFrame * >( AfxGetMainWnd() ) )
		{
			ResizeParentToFit ( FALSE );
			ResizeParentToFit ( TRUE );
		}
	}
	
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CGeneralView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO
	FriendActivate( bActivate, pActivateView, pDeactiveView, false);
	
	CMWFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CGeneralView::OnMail() 
{
	// TODO
	
	HINSTANCE			hInst;
	CString				rString;

	m_Mail_Address.GetWindowText ( rString );	

	hInst = 
		ShellExecute ( NULL, "open", 
						rString, "", "", SW_SHOWDEFAULT );	
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CGeneralView::OnWeb() 
{
	// TODO
	
	HINSTANCE			hInst;
	CString				rString;

	m_Web_Address.GetWindowText ( rString );	

	hInst = 
		ShellExecute ( NULL, "open", 
						rString, "", "", SW_SHOWDEFAULT );	
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CGeneralView::OnMove(int x, int y)
{
	CMWFormView::OnMove(x, y);

	RECT	clientRect;
	GetClientRect ( &clientRect );

#if 0
	EnableScrollBarCtrl(SB_BOTH, FALSE);

	if ( m_Rich_Edit.m_hWnd != NULL )
	{
		m_Rich_Edit.MoveWindow ( &clientRect, true );
	}
#endif
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CGeneralView::OnSize(UINT nType, int cx, int cy)
{
	CMWFormView::OnSize(nType, cx, cy);

#if 0
	EnableScrollBarCtrl(SB_BOTH, FALSE);

	RECT	clientRect;
	GetClientRect ( &clientRect );

	if ( m_Rich_Edit.m_hWnd != NULL )
	{
		m_Rich_Edit.MoveWindow ( &clientRect, true );
	}
#endif
}
