// FindEvent.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "FindEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
////////////////////////////////////////////////////////////////////////
// CFindEvent dialog
//
////////////////////////////////////////////////////////////////////////
CFindEvent::CFindEvent(CWnd* pParent /*=NULL*/) : CMWDialog(CFindEvent::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindEvent)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CFindEvent::DoDataExchange(CDataExchange* pDX)
{
	CMWDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindEvent)
	DDX_Control(pDX, IDC_V2_TO, m_V2_To);
	DDX_Control(pDX, IDC_V2_FROM, m_V2_From);
	DDX_Control(pDX, IDC_SPIN_V2_TO, m_Spin_V2_To);
	DDX_Control(pDX, IDC_SPIN_V2_FROM, m_Spin_V2_From);
	DDX_Control(pDX, IDC_V1_TO, m_V1_To);
	DDX_Control(pDX, IDC_V1_FROM, m_V1_From);
	DDX_Control(pDX, IDC_SPIN_V1_TO, m_Spin_V1_To);
	DDX_Control(pDX, IDC_SPIN_V1_FROM, m_Spin_V1_From);
	DDX_Control(pDX, IDC_CONTROLLER_COMBO, m_Controller_Combo);
	DDX_Control(pDX, IDC_PROGRAM_COMBO, m_Program_Combo);
	DDX_Control(pDX, IDC_EVENT_COMBO, m_Event_Combo);
	//}}AFX_DATA_MAP
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CFindEvent, CMWDialog)
	//{{AFX_MSG_MAP(CFindEvent)
	ON_CBN_SELCHANGE(IDC_EVENT_COMBO, OnSelchangeEventCombo)
	ON_CBN_SELENDOK(IDC_EVENT_COMBO, OnSelendokEventCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BOOL CFindEvent::OnInitDialog() 
{
	CMWDialog::OnInitDialog();
	
	//
	m_Event_Combo.ResetContent ( );

	m_Program_Combo.ResetContent ( );
	m_Program_Combo.AddString ( "" );

	m_Controller_Combo.ResetContent ( );
	m_Controller_Combo.AddString ( "" );

	m_Program_Combo.SetCurSel ( 0 );
	m_Program_Combo.EnableWindow ( FALSE );

	m_Controller_Combo.SetCurSel ( 0 );
	m_Controller_Combo.EnableWindow ( FALSE );

	m_Spin_V1_From.SetRange ( 0, 255 );
	m_Spin_V1_From.SetPos ( 0 );

	m_Spin_V1_To.SetRange ( 0, 255 );
	m_Spin_V1_To.SetPos ( 255 );

	m_Spin_V2_From.SetRange ( 0, 255 );
	m_Spin_V2_From.SetPos ( 0 );

	m_Spin_V2_To.SetRange ( 0, 255 );
	m_Spin_V2_To.SetPos ( 255 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CFindEvent::OnOK() 
{
	//

	CMainFrame			*hMainFrame;
	hMainFrame = dynamic_cast< CMainFrame * >( GetParent() );
	hMainFrame = dynamic_cast< CMainFrame * >( AfxGetMainWnd( ) );

	if ( hMainFrame == NULL )
	{
		CMWDialog::OnOK();
		return;
	}

	//		Set the string selected
	CString		strEvent;
	CString		strProgram;
	CString		strController;

	if ( m_Event_Combo.GetCurSel ( ) >= 0 )
	{
		m_Event_Combo.GetLBText ( m_Event_Combo.GetCurSel ( ), strEvent );
	}
	else
	{
		strEvent = "";
	}

	if ( m_Program_Combo.GetCurSel ( ) >= 0 )
	{
		m_Program_Combo.GetLBText ( m_Program_Combo.GetCurSel ( ), strProgram );
	}
	else
	{
		strProgram = "";
	}

	if ( m_Controller_Combo.GetCurSel ( ) >= 0 )
	{
		m_Controller_Combo.GetLBText ( m_Controller_Combo.GetCurSel ( ), strController );
	}
	else
	{
		strController = "";
	}

	int		V1_From		= m_Spin_V1_From.GetPos ( );
	int		V1_To		= m_Spin_V1_To.GetPos ( );
	int		V2_From		= m_Spin_V2_From.GetPos ( );
	int		V2_To		= m_Spin_V2_To.GetPos ( );

	hMainFrame->SetSearchInfo ( strEvent, strProgram, strController, V1_From, V1_To, V2_From, V2_To );

	CMWDialog::OnOK();
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CFindEvent::OnSelchangeEventCombo() 
{
	//
	EnableDisable ( );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CFindEvent::OnSelendokEventCombo() 
{
	//
	EnableDisable ( );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CFindEvent::EnableDisable()
{
	unsigned		iEventIndex;
	int				iCurSel;
	CMidiClass		classTemp;
	CString			strCurSel;

	iCurSel			= m_Event_Combo.GetCurSel ( );
	if ( iCurSel >= 0 )
	{
		m_Event_Combo.GetLBText ( iCurSel, strCurSel );

		iEventIndex		= classTemp.SearchClassEventText ( strCurSel );
		if ( iEventIndex == MIDI_EVENT_PARAMETER )
		{
			m_Program_Combo.SetCurSel ( 0 );
			m_Program_Combo.EnableWindow ( FALSE );

			m_Controller_Combo.EnableWindow ( TRUE );
		}
		else if ( iEventIndex == MIDI_EVENT_PROGRAM )
		{
			m_Controller_Combo.SetCurSel ( 0 );
			m_Controller_Combo.EnableWindow ( FALSE );

			m_Program_Combo.EnableWindow ( TRUE );
		}
		else
		{
			m_Program_Combo.SetCurSel ( 0 );
			m_Program_Combo.EnableWindow ( FALSE );

			m_Controller_Combo.SetCurSel ( 0 );
			m_Controller_Combo.EnableWindow ( FALSE );
		}
	}
	else
	{
		m_Program_Combo.SetCurSel ( 0 );
		m_Program_Combo.EnableWindow ( FALSE );

		m_Controller_Combo.SetCurSel ( 0 );
		m_Controller_Combo.EnableWindow ( FALSE );
	}

}
