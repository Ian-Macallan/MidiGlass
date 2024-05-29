// SysExDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "SysExDialog.h"
#include "MidiSysEx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	CMidiWorksApp		theApp;

extern	CMidiSysEx			clSysExGSReset [ MAX_SYSEX_LINES ];
extern	CMidiSysEx			clSysExGMReset [ MAX_SYSEX_LINES ];
extern	CMidiSysEx			clSysExXGReset [ MAX_SYSEX_LINES ];

/*
 *      Some Exclusive messages.
 */
unsigned char SysExGSReset [ 10 ] =
   { 0x41, 0x10, 0x42, 0x12, 0x40, 0x00, 0x7F, 0x0, 0x41, 0xF7 };

unsigned char SysExGSExit [ 10 ] =
   { 0x41, 0x10, 0x42, 0x12, 0x40, 0x00, 0x7F, 0x7F, 0x42, 0xF7 };

unsigned char SysExMidiOn [ 5 ] =
    { 0x7E, 0x7F, 0x9, 0x1, 0xF7 };

unsigned char SysExMidiOff [ 5 ] =
    { 0x7E, 0x7F, 0x9, 0x2, 0xF7 };

/////////////////////////////////////////////////////////////////////////////
// CSysExDialog dialog


CSysExDialog::CSysExDialog(CWnd* pParent /*=NULL*/) : CDialog(CSysExDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSysExDialog)
	//}}AFX_DATA_INIT

	m_bFirstTime = true;
}


void CSysExDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysExDialog)
	DDX_Control(pDX, IDC_STOP_RECEIVE, m_Stop_Receiving);
	DDX_Control(pDX, IDC_START_RECEIVE, m_Start_Receiving);
	DDX_Control(pDX, IDC_SIZE_RECEIVE, m_Size_Received);
	DDX_Control(pDX, IDC_DEFAULT, m_Default_Value);
	DDX_Control(pDX, IDC_SYSEX_VALUES, m_SysEx_Values);
	DDX_Control(pDX, IDC_SYSEX, m_SysEx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysExDialog, CDialog)
	//{{AFX_MSG_MAP(CSysExDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_SYSEX_VALUES, OnEndlabeleditSysexValues)
	ON_CBN_SELENDOK(IDC_SYSEX, OnSelendokSysex)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefaultValues)
	ON_BN_CLICKED(IDC_START_RECEIVE, OnStartReceive)
	ON_BN_CLICKED(IDC_STOP_RECEIVE, OnStopReceive)
	ON_BN_CLICKED(IDC_CHECK_RECEIVE, OnCheckReceive)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysExDialog message handlers

BOOL CSysExDialog::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	return CDialog::OnEraseBkgnd(pDC);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HBRUSH CSysExDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CSysExDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if ( m_bFirstTime )
	{
		m_SysEx_Values.InsertColumn ( 0, "Hexadecimal Value", LVCFMT_LEFT, 384 );

		m_SysEx.SetCurSel ( 0 );

		ReadSysEx ();

		m_bFirstTime = false;

		SetTimer ( INPUT_DIALOG_TIMER, 2000, NULL );

		m_Start_Receiving.EnableWindow ( ! m_MidiFile.LLIsRecording ( ) );
		m_Stop_Receiving.EnableWindow ( m_MidiFile.LLIsRecording ( ) );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CSysExDialog::ReadSysEx()
{

	int			iCurSel;

	iCurSel = m_SysEx.GetCurSel ( );
	if ( iCurSel >= 0 )
	{
		int			iX;
		CString		strReturned;
		CString		strSysEx;
		char		szString [ MAX_LONG_STRING ];

		for ( iX = 0; iX < MAX_SYSEX_LINES; iX++ )
		{
			switch ( iCurSel )
			{
				case 0:
				{
					clSysExGMReset [ iX ].Delete ();
					break;
				}
				case 1:
				{
					clSysExGSReset [ iX ].Delete ();
					break;
				}
				case 2:
				{
					clSysExXGReset [ iX ].Delete ();
					break;
				}
			}
		}

		m_SysEx_Values.DeleteAllItems ( );

		m_SysEx.GetLBText ( iCurSel, strSysEx );

		iX = 0;
		do
		{
			sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iX );
			strReturned =  theApp.GetProfileString( strSysEx, szString, NULL );
			if ( strReturned != "" )
			{
				m_SysEx_Values.InsertItem ( iX, strReturned );
				switch ( iCurSel )
				{
					case 0:
					{
						clSysExGMReset [ iX ].Set ( strReturned );
						break;
					}
					case 1:
					{
						clSysExGSReset [ iX ].Set ( strReturned );
						break;
					}
					case 2:
					{
						clSysExXGReset [ iX ].Set ( strReturned );
						break;
					}
				}
			}
			iX++;
		}
		while ( strReturned != "" );

		m_SysEx_Values.InsertItem ( iX, "" );

	}

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CSysExDialog::OnEndlabeleditSysexValues(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	// TODO: Add your control notification handler code here
	CString		strAscii;

    if(pDispInfo->item.mask & LVIF_TEXT )
	{
		strAscii = pDispInfo->item.pszText;
		CheckSysEx(strAscii);

		m_SysEx_Values.SetItemText ( pDispInfo->item.iItem, 0, strAscii );

		if ( pDispInfo->item.iItem == m_SysEx_Values.GetItemCount ( ) - 1 )
		{
			m_SysEx_Values.InsertItem ( m_SysEx_Values.GetItemCount ( ), "" );
		}

		WriteSysEx ( );
	}

	*pResult = 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CSysExDialog::OnSelendokSysex() 
{
	// TODO: Add your control notification handler code here
	if ( ! m_bFirstTime )
	{
		ReadSysEx ( );
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CSysExDialog::WriteSysEx()
{
	int			iCurSel;

	iCurSel = m_SysEx.GetCurSel ( );
	if ( iCurSel >= 0 )
	{
		int			iX;
		CString		strReturned;
		CString		strSysEx;
		char		szString [ MAX_PATHNAME_STRING ];

		m_SysEx.GetLBText ( iCurSel, strSysEx );

		//		First Reset all Registry
		iX = 0;
		do
		{
			sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iX );
			strReturned =  theApp.GetProfileString( strSysEx, szString, NULL );
			if ( strReturned != "" )
			{
				theApp.WriteProfileString( strSysEx, szString, NULL );
			}
			iX++;
		}
		while ( strReturned != "" );

		//		Then write SysEx
		int			iCount = 0;

		for ( iX = 0; iX < m_SysEx_Values.GetItemCount ( ); iX++ )
		{
			strReturned = m_SysEx_Values.GetItemText ( iX, 0 );
			if ( strReturned != "" )
			{
				sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iCount );
				theApp.WriteProfileString( strSysEx, szString, strReturned );
				iCount++;
			}
		}

		theApp.WriteProfileInt( strSysEx, "Count", iCount );
	}

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CSysExDialog::CheckSysEx(CString &strSysEx)
{
	char		szAscii [ MAX_LONG_STRING ];
	char		szHexa [ MAX_LONG_STRING ];
	unsigned	iLen;
	CMidiClass	clForConversion;

	iLen = clForConversion.ConvertAscii2Hexa ( strSysEx, szHexa );
	if ( iLen > 0 ) 
	{
		clForConversion.ConvertHexa2Ascii ( szHexa, iLen, szAscii, sizeof ( szAscii ) );
	}

	strSysEx = szAscii;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CSysExDialog::OnDefaultValues() 
{
	// TODO: Add your control notification handler code here
	char		szAscii [ MAX_LONG_STRING ];
	CMidiClass	clForConversion;
	int			iCurSel;
	int			iCount;

	iCurSel = m_SysEx.GetCurSel ( );
	if ( iCurSel >= 0 )
	{
		int			iX;
		CString		strReturned;
		CString		strSysEx;
		char		szString [ MAX_PATHNAME_STRING ];

		m_SysEx.GetLBText ( iCurSel, strSysEx );

		//		First Reset all Registry
		iX = 0;
		do
		{
			sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iX );
			strReturned =  theApp.GetProfileString( strSysEx, szString, NULL );
			if ( strReturned != "" )
			{
				theApp.WriteProfileString( strSysEx, szString, NULL );
			}
			iX++;
		}
		while ( strReturned != "" );

		//		Then write SysEx
		if ( iCurSel == 0 )
		{
			iCount = 0;

			clForConversion.ConvertHexa2Ascii ( 
				( char * ) SysExGSReset, sizeof ( SysExGSReset ),
				szAscii, sizeof ( szAscii ) );
			sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iCount );
			theApp.WriteProfileString( strSysEx, szString, szAscii );
			iCount++;

			clForConversion.ConvertHexa2Ascii ( 
				( char * ) SysExGSExit, sizeof ( SysExGSExit ),
				szAscii, sizeof ( szAscii ) );
			sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iCount );
			theApp.WriteProfileString( strSysEx, szString, szAscii );
			iCount++;

			clForConversion.ConvertHexa2Ascii ( 
				( char * ) SysExMidiOn, sizeof ( SysExMidiOn ),
				szAscii, sizeof ( szAscii ) );
			sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iCount );
			theApp.WriteProfileString( strSysEx, szString, szAscii );
			iCount++;

			theApp.WriteProfileInt( strSysEx, "Count", iCount );
		}

		if ( iCurSel == 1 )
		{
			iCount = 0;

			clForConversion.ConvertHexa2Ascii ( 
				( char * ) SysExGSReset, sizeof ( SysExGSReset ),
				szAscii, sizeof ( szAscii ) );
			sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iCount );
			theApp.WriteProfileString( strSysEx, szString, szAscii );
			iCount++;

			clForConversion.ConvertHexa2Ascii ( 
				( char * ) SysExGSExit, sizeof ( SysExGSExit ),
				szAscii, sizeof ( szAscii ) );
			sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iCount );
			theApp.WriteProfileString( strSysEx, szString, szAscii );
			iCount++;

			clForConversion.ConvertHexa2Ascii ( 
				( char * ) SysExMidiOn, sizeof ( SysExMidiOn ),
				szAscii, sizeof ( szAscii ) );
			sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iCount );
			theApp.WriteProfileString( strSysEx, szString, szAscii );
			iCount++;

			clForConversion.ConvertHexa2Ascii ( 
				( char * ) SysExMidiOff, sizeof ( SysExMidiOff ),
				szAscii, sizeof ( szAscii ) );
			sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iCount );
			theApp.WriteProfileString( strSysEx, szString, szAscii );
			iCount++;

			theApp.WriteProfileInt( strSysEx, "Count", iCount );

		}

		if ( iCurSel == 2 )
		{
			iCount = 0;

			clForConversion.ConvertHexa2Ascii ( 
				( char * ) SysExGSReset, sizeof ( SysExGSReset ),
				szAscii, sizeof ( szAscii ) );
			sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iCount );
			theApp.WriteProfileString( strSysEx, szString, szAscii );
			iCount++;

			clForConversion.ConvertHexa2Ascii ( 
				( char * ) SysExGSExit, sizeof ( SysExGSExit ),
				szAscii, sizeof ( szAscii ) );
			sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iCount );
			theApp.WriteProfileString( strSysEx, szString, szAscii );
			iCount++;

			clForConversion.ConvertHexa2Ascii ( 
				( char * ) SysExMidiOn, sizeof ( SysExMidiOn ),
				szAscii, sizeof ( szAscii ) );
			sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iCount );
			theApp.WriteProfileString( strSysEx, szString, szAscii );
			iCount++;

			clForConversion.ConvertHexa2Ascii ( 
				( char * ) SysExMidiOff, sizeof ( SysExMidiOff ),
				szAscii, sizeof ( szAscii ) );
			sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iCount );
			theApp.WriteProfileString( strSysEx, szString, szAscii );
			iCount++;

			theApp.WriteProfileInt( strSysEx, "Count", iCount );

		}

		ReadSysEx ( );
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CSysExDialog::OnStartReceive() 
{
	// TODO: Add your control notification handler code here
	theApp.m_iMidiInDevice = theApp.GetProfileInt ( "Settings", "Midi Input Device", 0 );

	m_Size_Received.SetWindowText ( "Undefined" );
	m_MidiFile.SetMidiInpDevice ( theApp.m_iMidiInDevice );
	m_MidiFile.LLInpRecord ( m_hWnd );

	m_Start_Receiving.EnableWindow ( ! m_MidiFile.LLIsRecording ( ) );
	m_Stop_Receiving.EnableWindow ( m_MidiFile.LLIsRecording ( ) );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CSysExDialog::OnStopReceive() 
{
	// TODO: Add your control notification handler code here
	OnCheckReceive();

	m_MidiFile.LLInpStop();

	m_Start_Receiving.EnableWindow ( ! m_MidiFile.LLIsRecording ( ) );
	m_Stop_Receiving.EnableWindow ( m_MidiFile.LLIsRecording ( ) );

	static char BASED_CODE szFilter[] = 
		"Midi File|*.mid|All Files (*.*)|*.*||";

	char				szSysexFilename [ MAX_PATHNAME_STRING ];

	CFileDialog l_FileDialog ( FALSE, NULL, "*.mid", NULL, szFilter );

	/*
	 *		Starts a dialog box.
	 */
	if ( l_FileDialog.DoModal () == IDOK)
	{
		strcpy_s (	szSysexFilename, sizeof ( szSysexFilename ),
					l_FileDialog.GetFileName () );
		m_MidiFile.SaveInputChunk ( szSysexFilename );
	}	

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CSysExDialog::OnCheckReceive() 
{
	// TODO: Add your control notification handler code here
	int iNumber = m_MidiFile.GetNumberOfInputChunk ();
	char		szText [ MAX_LONG_STRING ];
	sprintf_s ( szText, sizeof ( szText ), "%d", iNumber );
	m_Size_Received.SetWindowText ( szText );
	
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CSysExDialog::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	OnCheckReceive();
	CDialog::OnTimer(nIDEvent);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CSysExDialog::OnOK() 
{
	// TODO: Add extra validation here
	KillTimer ( INPUT_DIALOG_TIMER );
	
	CDialog::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CSysExDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer ( INPUT_DIALOG_TIMER );

	CDialog::OnClose();
}
