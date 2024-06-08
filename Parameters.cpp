// Parameters.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "Parameters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MidiClass.h"

extern CMidiWorksApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CParameters dialog


CParameters::CParameters(CWnd* pParent /*=NULL*/) : CMWDialog(CParameters::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParameters)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CParameters::DoDataExchange(CDataExchange* pDX)
{
	CMWDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParameters)
	DDX_Control(pDX, IDC_CHECK_SETTINGS, m_Check_Settings);
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_SETTINGS, m_Settings);
	DDX_Control(pDX, IDC_CONTROLER, m_Controler);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParameters, CMWDialog)
	//{{AFX_MSG_MAP(CParameters)
	ON_CBN_SELCHANGE(IDC_CONTROLER, OnSelchangeControler)
	ON_EN_CHANGE(IDC_SETTINGS, OnChangeSettings)
	ON_BN_CLICKED(IDC_CHECK_SETTINGS, OnCheckSettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParameters message handlers

BOOL CParameters::OnInitDialog() 
{
	CMWDialog::OnInitDialog();
	
	//
	CMidiClass		clTemp;

	//		Initialise the combo box
	int			iX = 0;
	char		*pText;
	do
	{
		iX++;
		pText = clTemp.GetClassControlerText ( iX );
		if ( pText != NULL )
		{
			m_Controler.InsertString ( iX - 1, pText );
		}
	}
	while ( pText != NULL );
	m_Controler.SetCurSel ( 0 );

	//		Display Value
	DisplayValues ();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CParameters::DisplayValues()
{
	int				iC;
	CMidiClass		clTemp;
	char			szString [ MAX_CONTROLER_STRING ];

	m_Controler.GetLBText( m_Controler.GetCurSel (), szString );
	iC = clTemp.SearchControlerByText ( szString );

	sprintf_s ( szString, sizeof ( szString ), "%d", theApp.m_cParameter_Settings [ iC ] );
	m_Settings.SetWindowText ( szString );

	if ( theApp.m_bParameter_Settings [ iC ] )
	{
		m_Check_Settings.SetCheck ( 1 );
		m_Settings.EnableWindow ( TRUE );
	}
	else
	{
		m_Check_Settings.SetCheck ( 0 );
		m_Settings.EnableWindow ( FALSE );
	}
}

void CParameters::OnSelchangeControler() 
{
	//
	DisplayValues ();	
}

void CParameters::OnChangeSettings() 
{
	//
	// send this notification unless you override the CMWDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	//
	int				iC;
	CMidiClass		clTemp;
	char			szString [ MAX_CONTROLER_STRING ];

	m_Controler.GetLBText( m_Controler.GetCurSel (), szString );
	iC = clTemp.SearchControlerByText ( szString );

	m_Settings.GetWindowText ( szString, sizeof ( szString ) - 1 );

	theApp.m_cParameter_Settings [ iC ] = atoi ( szString );
	
}

void CParameters::OnCheckSettings() 
{
	//
	int				iC;
	CMidiClass		clTemp;
	char			szString [ MAX_CONTROLER_STRING ];

	m_Controler.GetLBText( m_Controler.GetCurSel (), szString );
	iC = clTemp.SearchControlerByText ( szString );

	if ( m_Check_Settings.GetCheck ( ) )
	{
		theApp.m_bParameter_Settings [ iC ] = true;
		m_Settings.EnableWindow ( TRUE );
	}
	else
	{
		theApp.m_bParameter_Settings [ iC ] = false;
		m_Settings.EnableWindow ( FALSE );
	}

}
