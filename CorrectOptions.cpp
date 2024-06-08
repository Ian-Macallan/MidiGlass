// CorrectOptions.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "CorrectOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	CMidiWorksApp	theApp;


//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
// CCorrectOptions dialog
IMPLEMENT_DYNAMIC(CCorrectOptions, CMWDialog)

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
CCorrectOptions::CCorrectOptions(CWnd* pParent /*=NULL*/) : CMWDialog(CCorrectOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCorrectOptions)
	//}}AFX_DATA_INIT
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CCorrectOptions::DoDataExchange(CDataExchange* pDX)
{
	CMWDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCorrectOptions)
	DDX_Control(pDX, IDC_SYSEX, m_SysEx);
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_FILTER, m_Filter);
	DDX_Control(pDX, IDC_BANKSELECTLOW, m_BankSelectLow);
	DDX_Control(pDX, IDC_BANKSELECTHIGH, m_BankSelectHigh);
	DDX_Control(pDX, IDC_ADDSTARTTRACK, m_AddStartOfTrack);
	DDX_Control(pDX, IDC_ADDENDTRACK, m_AddEndOfTrack);
	DDX_Control(pDX, IDC_CORRECT_STARTOFTRACK, m_CorrectStartOfTrack);
	//}}AFX_DATA_MAP
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCorrectOptions, CMWDialog)
	//{{AFX_MSG_MAP(CCorrectOptions)
	ON_BN_CLICKED(IDOK, &CCorrectOptions::OnBnClickedOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
////////////////////////////////////////////////////////////////////////////////////
// CCorrectOptions message handlers
//
////////////////////////////////////////////////////////////////////////////////////
BOOL CCorrectOptions::OnInitDialog() 
{
	CMWDialog::OnInitDialog();
	
	//
	m_SysEx.SetCurSel ( theApp.m_iSysEx	);
	m_AddStartOfTrack.SetCheck ( theApp.m_bAddStartOfTrack );
	m_AddEndOfTrack.SetCheck ( theApp.m_bAddEndOfTrack );
	m_BankSelectLow.SetCheck ( theApp.m_bBankSelectLow );
	m_BankSelectHigh.SetCheck ( theApp.m_bBankSelectHigh );
	m_Filter.SetCheck ( theApp.m_bFilter );
	m_CorrectStartOfTrack.SetCheck ( theApp.m_bCorrectStartOfTrack );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CCorrectOptions::OnOK() 
{
	//
	theApp.m_iSysEx					= m_SysEx.GetCurSel ();
	theApp.m_bAddStartOfTrack		= m_AddStartOfTrack.GetCheck ( );
	theApp.m_bAddEndOfTrack			= m_AddEndOfTrack.GetCheck ( );
	theApp.m_bBankSelectLow			= m_BankSelectLow.GetCheck ( );
	theApp.m_bBankSelectHigh		= m_BankSelectHigh.GetCheck ( );
	theApp.m_bFilter				= m_Filter.GetCheck ( );
	theApp.m_bCorrectStartOfTrack	= m_CorrectStartOfTrack.GetCheck ( );

	theApp.WriteProfileInt ( "Correction", "System Exclusive", theApp.m_iSysEx );
	theApp.WriteProfileInt ( "Correction", "Add Start Of Track", theApp.m_bAddStartOfTrack );
	theApp.WriteProfileInt ( "Correction", "Add End Of Track", theApp.m_bAddEndOfTrack );
	theApp.WriteProfileInt ( "Correction", "Remove Bank Select Low", theApp.m_bBankSelectLow );
	theApp.WriteProfileInt ( "Correction", "Remove Bank Select High", theApp.m_bBankSelectHigh );
	theApp.WriteProfileInt ( "Correction", "Filter Tracks By Channel", theApp.m_bFilter );
	theApp.WriteProfileInt ( "Correction", "Correct Start Of Track", theApp.m_bCorrectStartOfTrack );

	CMWDialog::OnOK();
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CCorrectOptions::OnBnClickedOk()
{
	//
	CCorrectOptions::OnOK();
}
