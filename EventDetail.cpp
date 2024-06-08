//
////////////////////////////////////////////////////////////////////////
// EventDetail.cpp : implementation file
//
////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MidiGlassApp.h"
#include "EventDetail.h"
#include "MainFrm.h"
#include "MidiClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMidiWorksApp theApp;

IMPLEMENT_DYNAMIC(CEventDetail,CMWDialog)

//
////////////////////////////////////////////////////////////////////////
// CEventDetail dialog
//
////////////////////////////////////////////////////////////////////////
CEventDetail::CEventDetail(CWnd* pParent /*=NULL*/) : CMWDialog(CEventDetail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEventDetail)
	//}}AFX_DATA_INIT

	m_bShow					= true;
	m_bInsert				= false;
	m_bModify				= false;
	m_pOldMidiList			= NULL;
	m_pNewMidiList			= NULL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CEventDetail:: ~CEventDetail()
 {
 }

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CEventDetail::setShow( CMidiList *pOldMidiList )
{
	m_bShow			= true;
	m_bInsert		= false;
	m_bModify		= false;
	m_pOldMidiList	= pOldMidiList;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CEventDetail::setInsert( CMidiList *pOldMidiList )
{
	m_bShow			= false;
	m_bInsert		= true;
	m_bModify		= false;
	m_pOldMidiList	= pOldMidiList;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CEventDetail::setModify(CMidiList *pOldMidiList)
{
	m_bShow			= false;
	m_bInsert		= false;
	m_bModify		= true;
	m_pOldMidiList	= pOldMidiList;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CEventDetail::Free()
{
	if ( m_bInsert || m_bModify )
	{
		if ( m_pNewMidiList != NULL )
		{
			delete m_pNewMidiList;
			m_pNewMidiList = NULL;
		}
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiList *CEventDetail::getNewMidiList()
{
	return m_pNewMidiList;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CEventDetail::DoDataExchange(CDataExchange* pDX)
{
	CMWDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventDetail)
	DDX_Control(pDX, IDC_MESURE, m_Mesure);
	DDX_Control(pDX, IDC_DELTA_TIME, m_DeltaTime);
	DDX_Control(pDX, IDC_EVENT_CHANNEL, m_Channel);
	DDX_Control(pDX, IDC_EVENT_COMBO, m_Event_Combo);

	DDX_Control(pDX, IDC_EVENT_COMBO_1, m_Value_1_Combo);
	DDX_Control(pDX, IDC_EVENT_VALUE_1, m_Value_1_Text);
	DDX_Control(pDX, IDC_EVENT_VALUE_2, m_Value_2_Text);
	DDX_Control(pDX, IDC_EVENT_VALUE_3, m_Value_3_Text);
	DDX_Control(pDX, IDC_EVENT_VALUE_4, m_Value_4_Text);
	DDX_Control(pDX, IDC_EVENT_VALUE_5, m_Value_5_Text);

	DDX_Control(pDX, IDC_EVENT_LABEL_1, m_Label_1_Combo);
	DDX_Control(pDX, IDC_EVENT_TEXT_1, m_Label_1_Text);
	DDX_Control(pDX, IDC_EVENT_TEXT_2, m_Label_2_Text);
	DDX_Control(pDX, IDC_EVENT_TEXT_3, m_Label_3_Text);
	DDX_Control(pDX, IDC_EVENT_TEXT_4, m_Label_4_Text);
	DDX_Control(pDX, IDC_EVENT_TEXT_5, m_Label_5_Text);

	DDX_Control(pDX, IDC_NUM_LABEL_1, m_Num_1_Combo);
	DDX_Control(pDX, IDC_NUM_TEXT_1, m_Num_1_Text);
	DDX_Control(pDX, IDC_NUM_TEXT_2, m_Num_2_Text);
	DDX_Control(pDX, IDC_NUM_TEXT_3, m_Num_3_Text);
	DDX_Control(pDX, IDC_NUM_TEXT_4, m_Num_4_Text);
	DDX_Control(pDX, IDC_NUM_TEXT_5, m_Num_5_Text);

	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	//}}AFX_DATA_MAP
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CEventDetail, CMWDialog)
	//{{AFX_MSG_MAP(CEventDetail)
	ON_MESSAGE(WM_MW_EDIT_SELECT,OnEditSelect)
	ON_CBN_SELCHANGE(IDC_EVENT_COMBO, &CEventDetail::OnCbnSelchangeEventCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
////////////////////////////////////////////////////////////////////////
// CEventDetail message handlers
//
////////////////////////////////////////////////////////////////////////
BOOL CEventDetail::OnInitDialog() 
{
	CMWDialog::OnInitDialog();
	
	//
	CMainFrame			*pMainFrame;
	char				szString [ 64 ];

	pMainFrame = dynamic_cast< CMainFrame * >( GetParent() );
	pMainFrame = dynamic_cast< CMainFrame * >( AfxGetMainWnd( ) );


	//		Fills the combo text.
	m_Event_Combo.ResetContent ();

	//
	if ( m_pOldMidiList != NULL )
	{
		//
		//	Dupplicate Current Event
		if ( m_bInsert || m_bModify )
		{
			m_pNewMidiList	= new CMidiList ( m_pOldMidiList );
		}
		else
		{
			m_pNewMidiList	= m_pOldMidiList;
		}

		if ( m_pNewMidiList != NULL )
		{
			m_Mesure.SetWindowText ( ( char * ) m_pNewMidiList->GetMesureText () );

			sprintf_s ( szString, sizeof ( szString ), "%ld", m_pNewMidiList->GetDeltaTime () );
			m_DeltaTime.SetWindowText ( szString );

			m_Event_Combo.SetCurSel ( m_pNewMidiList->GetEventIndex () );


			//
			if ( m_pNewMidiList->useChannel() )
			{
				int iChannel = m_pNewMidiList->GetChannel();
				sprintf_s ( szString, sizeof ( szString ), "%d", m_pNewMidiList->GetChannel() );
			}
			else
			{
				sprintf_s ( szString, sizeof ( szString ), "NA" );
			}
			m_Channel.SetWindowText ( szString );

			//
			ChangeValue1Combo ( m_pNewMidiList );
		}
	
		//
		m_Mesure.SetReadOnly ( FALSE );

		//
		if ( m_bModify || m_bInsert )
		{
			m_DeltaTime.SetReadOnly ( FALSE );
			m_Event_Combo.EnableWindow ( TRUE );
			if ( m_pNewMidiList != NULL && m_pNewMidiList->useChannel() )
			{
				m_Channel.SetReadOnly ( FALSE );
			}
			m_Value_1_Combo.EnableWindow ( TRUE );
			m_Value_1_Text.SetReadOnly ( FALSE );
			m_Value_2_Text.SetReadOnly ( FALSE );
			m_Value_3_Text.SetReadOnly ( FALSE );
			m_Value_4_Text.SetReadOnly ( FALSE );
			m_Value_5_Text.SetReadOnly ( FALSE );
		}
		else
		{
			m_DeltaTime.SetReadOnly ( TRUE );
			m_Event_Combo.EnableWindow ( FALSE );
			m_Channel.SetReadOnly ( TRUE );
			m_Value_1_Combo.EnableWindow ( FALSE );
			m_Value_1_Text.SetReadOnly ( TRUE );
			m_Value_2_Text.SetReadOnly ( TRUE );
			m_Value_3_Text.SetReadOnly ( TRUE );
			m_Value_4_Text.SetReadOnly ( TRUE );
			m_Value_5_Text.SetReadOnly ( TRUE );
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CEventDetail::OnOK() 
{
	//
	if ( m_pNewMidiList != NULL )
	{
		//
		//	Alter Values
		int iEvent = m_pNewMidiList->GetEventIndex();

		//
		switch ( iEvent )
		{
			case	MIDI_EVENT_TEXT :
			case	MIDI_EVENT_COPYRIGHT :
			case	MIDI_EVENT_TRACKNAME :
			case	MIDI_EVENT_INSTRNAME :
			case	MIDI_EVENT_LYRICS :
			case	MIDI_EVENT_MARKERS :
			case	MIDI_EVENT_CUEPOINT :
			{
				//
				char	szValueString [ MAX_PATH ];
				m_Value_1_Text.GetWindowText ( szValueString, sizeof(szValueString) );

				CMidiEvent *pEvent = dynamic_cast <CMidiText *>(m_pNewMidiList->GetMidiEvent());
				pEvent->SetText ( (unsigned char *)szValueString );
				break;
			}
		
		
			case	MIDI_EVENT_SEQUENCE :
			case	MIDI_EVENT_SEQUENCER :
			{
				//
				char	szValueString [ MAX_PATH ];
				m_Value_1_Text.GetWindowText ( szValueString, sizeof(szValueString) );

				CMidiEvent *pEvent = dynamic_cast <CMidiBinary *>(m_pNewMidiList->GetMidiEvent());
				pEvent->SetText ( (unsigned char *) szValueString );
				break;
			}

			case	MIDI_EVENT_SYSEX :
			{
				//
				char	szValueString [ MAX_PATH ];
				m_Value_1_Text.GetWindowText ( szValueString, sizeof(szValueString) );

				CMidiEvent *pEvent = dynamic_cast <CMidiSystemExclusive *>(m_pNewMidiList->GetMidiEvent());
				pEvent->SetText ( (unsigned char *)szValueString );
				break;
			}

			case	MIDI_EVENT_STARTTRACK :
			{
				//
				char	szValueString [ 32 ];
				m_Value_1_Text.GetWindowText ( szValueString, sizeof(szValueString) );

				m_pNewMidiList->GetMidiEvent()->SetEventValue1 ( atoi(szValueString) );
				break;
			}

			case	MIDI_EVENT_STARTTRACK20 :
			{
				//
				char	szValueString [ 32 ];
				m_Value_1_Text.GetWindowText ( szValueString, sizeof(szValueString) );

				m_pNewMidiList->GetMidiEvent()->SetEventValue1 ( atoi(szValueString) );
				break;
			}
		
			case	MIDI_EVENT_ENDOFTRACK :
			{
				// No Parameters 
				break;
			}
		
			case	MIDI_EVENT_SETTEMPO :
			{
				char	szValueString [ 32 ];

				m_Value_1_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue1 ( atoi(szValueString) );

				m_Value_2_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue2 ( atoi(szValueString) );

				m_Value_3_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue3 ( atoi(szValueString) );

				break;
			}
		
			case	MIDI_EVENT_SETSMPTE :
			{
				char	szValueString [ 32 ];

				m_Value_1_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue1 ( atoi(szValueString) );

				m_Value_2_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue2 ( atoi(szValueString) );

				m_Value_3_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue3 ( atoi(szValueString) );

				m_Value_4_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue4 ( atoi(szValueString) );

				m_Value_5_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue5 ( atoi(szValueString) );

				break;
			}
		
			case	MIDI_EVENT_TIMESIG :
			{
				char	szValueString [ 32 ];

				m_Value_1_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue1 ( atoi(szValueString) );

				m_Value_2_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue2 ( atoi(szValueString) );

				m_Value_3_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue3 ( atoi(szValueString) );

				m_Value_4_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue4 ( atoi(szValueString) );

				m_Value_5_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue5 ( atoi(szValueString) );

				break;
			}
		

			case	MIDI_EVENT_KEYSIG :
			{
				char	szValueString [ 32 ];

				m_Value_1_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue1 ( atoi(szValueString) );

				m_Value_2_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue2 ( atoi(szValueString) );
				break;
			}
		
			case	MIDI_EVENT_NOTEOFF :
			{
				char	szValueString [ 32 ];

				m_Channel.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->SetChannelAndCode ( atoi ( szValueString ) );

				m_pNewMidiList->GetMidiEvent()->SetEventValue1 ( m_Value_1_Combo.GetCurSel() );

				m_Value_2_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue2 ( atoi(szValueString) );

				break;
			}
		
			case	MIDI_EVENT_NOTEON :
			{
				char	szValueString [ 32 ];

				m_Channel.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->SetChannelAndCode ( atoi ( szValueString ) );

				m_pNewMidiList->GetMidiEvent()->SetEventValue1 ( m_Value_1_Combo.GetCurSel() );

				m_Value_2_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue2 ( atoi(szValueString) );
				break;
			}
		
			case	MIDI_EVENT_KEYPRESSURE :
			{
				char	szValueString [ 32 ];

				m_Channel.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->SetChannelAndCode ( atoi ( szValueString ) );

				m_Value_1_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue1 ( atoi(szValueString) );

				m_Value_2_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue2 ( atoi(szValueString) );
				break;
			}
		
			case	MIDI_EVENT_PARAMETER :
			{
				char	szValueString [ 32 ];

				m_Channel.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->SetChannelAndCode ( atoi ( szValueString ) );

				m_pNewMidiList->GetMidiEvent()->SetEventValue1 ( m_Value_1_Combo.GetCurSel() );

				m_Value_2_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue2 ( atoi(szValueString) );
				break;
			}
		
			case	MIDI_EVENT_PROGRAM :
			{
				char	szValueString [ 32 ];

				m_Channel.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->SetChannelAndCode ( atoi ( szValueString ) );

				m_pNewMidiList->GetMidiEvent()->SetEventValue1 ( m_Value_1_Combo.GetCurSel() );

				break;
			}
		
			case	MIDI_EVENT_CHANNELPRES :
			{
				char	szValueString [ 32 ];

				m_Channel.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->SetChannelAndCode ( atoi ( szValueString ) );

				m_Value_1_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue1 ( atoi(szValueString) );

				break;
			}
		
			case	MIDI_EVENT_WHEEL :
			{
				char	szValueString [ 32 ];

				m_Channel.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->SetChannelAndCode ( atoi ( szValueString ) );

				m_Value_1_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue1 ( atoi(szValueString) );

				m_Value_2_Text.GetWindowText ( szValueString, sizeof(szValueString) );
				m_pNewMidiList->GetMidiEvent()->SetEventValue2 ( atoi(szValueString) );

				break;
			}
		}

		{
			char	szValueString [ 32 ];
			m_DeltaTime.GetWindowText ( szValueString, sizeof(szValueString) );
			m_pNewMidiList->SetDeltaTime ( atoi ( szValueString ) );
		}
	}

	CMWDialog::OnOK();
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CEventDetail::OnCancel()
{
	//
	Free();
	CMWDialog::OnCancel();
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CEventDetail::showHide(CMidiList *pMidiList)
{
	if ( pMidiList != NULL )
	{
		if ( pMidiList->valueCount() >= 2 )
		{
			m_Label_2_Text.ShowWindow( TRUE );
			m_Value_2_Text.ShowWindow( TRUE );
			m_Num_2_Text.ShowWindow( TRUE );
		}
		else
		{
			m_Label_2_Text.ShowWindow( FALSE );
			m_Value_2_Text.ShowWindow( FALSE );
			m_Num_2_Text.ShowWindow( FALSE );
		}

		if ( pMidiList->valueCount() >= 3 )
		{
			m_Label_3_Text.ShowWindow( TRUE );
			m_Value_3_Text.ShowWindow( TRUE );
			m_Num_3_Text.ShowWindow( TRUE );
		}
		else
		{
			m_Label_3_Text.ShowWindow( FALSE );
			m_Value_3_Text.ShowWindow( FALSE );
			m_Num_3_Text.ShowWindow( FALSE );
		}

		if ( pMidiList->valueCount() >= 4 )
		{
			m_Label_4_Text.ShowWindow( TRUE );
			m_Value_4_Text.ShowWindow( TRUE );
			m_Num_4_Text.ShowWindow( TRUE );
		}
		else
		{
			m_Label_4_Text.ShowWindow( FALSE );
			m_Value_4_Text.ShowWindow( FALSE );
			m_Num_4_Text.ShowWindow( FALSE );
		}

		if ( pMidiList->valueCount() >= 5 )
		{
			m_Label_5_Text.ShowWindow( TRUE );
			m_Value_5_Text.ShowWindow( TRUE );
			m_Num_5_Text.ShowWindow( TRUE );
		}
		else
		{
			m_Label_5_Text.ShowWindow( FALSE );
			m_Value_5_Text.ShowWindow( FALSE );
			m_Num_5_Text.ShowWindow( FALSE );
		}
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CEventDetail::ChangeValue1Combo(CMidiList *pMidiList)
{
	//
	char szValue [ 16 ];

	//
	m_Value_1_Combo.ResetContent();

	//
	int iEvent = m_Event_Combo.GetCurSel();
	if ( iEvent >= 0 )
	{
		switch ( iEvent )
		{
			case MIDI_EVENT_UNKNOWN :
			{
				break;
			}

			case MIDI_EVENT_TEXT : 
			case MIDI_EVENT_COPYRIGHT :
			case MIDI_EVENT_TRACKNAME :
			case MIDI_EVENT_INSTRNAME :
			case MIDI_EVENT_LYRICS :
			case MIDI_EVENT_MARKERS :
			case MIDI_EVENT_CUEPOINT :
			{
				m_Value_1_Combo.ShowWindow ( FALSE );
				m_Label_1_Combo.ShowWindow ( FALSE );
				m_Num_1_Combo.ShowWindow ( FALSE );
				m_Value_1_Text.ShowWindow ( TRUE );
				m_Label_1_Text.ShowWindow ( TRUE );
				m_Num_1_Text.ShowWindow ( TRUE );
				showHide ( pMidiList );

				m_Label_1_Text.SetWindowText (  (LPCSTR) CMidiClass::GetEventSmallText ( iEvent ) );
				const char *pText = (LPCSTR) pMidiList->GetTextAddress ();
				if ( pText != NULL )
				{
					m_Value_1_Text.SetWindowText ( pText );
				}

				m_Label_2_Text.SetWindowText ( pMidiList->GetLabel2() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue2() );
				m_Value_2_Text.SetWindowText ( szValue );

				m_Label_3_Text.SetWindowText ( pMidiList->GetLabel3() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue3() );
				m_Value_3_Text.SetWindowText ( szValue );

				m_Label_4_Text.SetWindowText ( pMidiList->GetLabel4() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue4() );
				m_Value_4_Text.SetWindowText ( szValue );

				m_Label_5_Text.SetWindowText ( pMidiList->GetLabel5() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue5() );
				m_Value_5_Text.SetWindowText ( szValue );

				break;
			}

			case MIDI_EVENT_SEQUENCE :
			case MIDI_EVENT_SEQUENCER :
			{
				m_Value_1_Combo.ShowWindow ( FALSE );
				m_Label_1_Combo.ShowWindow ( FALSE );
				m_Num_1_Combo.ShowWindow ( FALSE );
				m_Value_1_Text.ShowWindow ( TRUE );
				m_Label_1_Text.ShowWindow ( TRUE );
				m_Num_1_Text.ShowWindow ( TRUE );
				showHide ( pMidiList );

				m_Label_2_Text.SetWindowText ( pMidiList->GetLabel1() );
				const char *pText = (LPCSTR) pMidiList->GetExtraText();
				if ( pText != NULL )
				{
					m_Value_1_Text.SetWindowText ( pText );
				}
				break;
			}

			case MIDI_EVENT_SYSEX :
			{
				m_Value_1_Combo.ShowWindow ( FALSE );
				m_Label_1_Combo.ShowWindow ( FALSE );
				m_Num_1_Combo.ShowWindow ( FALSE );
				m_Value_1_Text.ShowWindow ( TRUE );
				m_Label_1_Text.ShowWindow ( TRUE );
				m_Num_1_Text.ShowWindow ( TRUE );
				showHide ( pMidiList );

				m_Label_2_Text.SetWindowText ( pMidiList->GetLabel1() );
				const char *pText = (LPCSTR) pMidiList->GetExtraText();
				if ( pText != NULL )
				{
					m_Value_1_Text.SetWindowText ( pText );
				}
				break;
			}

			case MIDI_EVENT_STARTTRACK :
			case MIDI_EVENT_STARTTRACK20 :
			case MIDI_EVENT_ENDOFTRACK :
			{
				m_Value_1_Combo.ShowWindow ( FALSE );
				m_Label_1_Combo.ShowWindow ( FALSE );
				m_Num_1_Combo.ShowWindow ( FALSE );
				m_Value_1_Text.ShowWindow ( TRUE );
				m_Label_1_Text.ShowWindow ( TRUE );
				m_Num_1_Text.ShowWindow ( TRUE );
				showHide ( pMidiList );

				m_Label_1_Text.SetWindowText ( pMidiList->GetLabel1() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue1() );
				m_Value_1_Text.SetWindowText ( szValue );

				m_Label_2_Text.SetWindowText ( pMidiList->GetLabel2() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue2() );
				m_Value_2_Text.SetWindowText ( szValue );

				m_Label_3_Text.SetWindowText ( pMidiList->GetLabel3() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue3() );
				m_Value_3_Text.SetWindowText ( szValue );

				m_Label_4_Text.SetWindowText ( pMidiList->GetLabel4() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue4() );
				m_Value_4_Text.SetWindowText ( szValue );

				m_Label_5_Text.SetWindowText ( pMidiList->GetLabel5() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue5() );
				m_Value_5_Text.SetWindowText ( szValue );

				break;
			}

			case MIDI_EVENT_SETTEMPO :
			{
				m_Value_1_Combo.ShowWindow ( FALSE );
				m_Label_1_Combo.ShowWindow ( FALSE );
				m_Num_1_Combo.ShowWindow ( FALSE );
				m_Value_1_Text.ShowWindow ( TRUE );
				m_Label_1_Text.ShowWindow ( TRUE );
				m_Num_1_Text.ShowWindow ( TRUE );
				showHide ( pMidiList );

				m_Label_1_Text.SetWindowText ( pMidiList->GetLabel1() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue1() );
				m_Value_1_Text.SetWindowText ( szValue );

				m_Label_2_Text.SetWindowText ( pMidiList->GetLabel2() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue2() );
				m_Value_2_Text.SetWindowText ( szValue );

				m_Label_3_Text.SetWindowText ( pMidiList->GetLabel3() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue3() );
				m_Value_3_Text.SetWindowText ( szValue );

				m_Label_4_Text.SetWindowText ( pMidiList->GetLabel4() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue4() );
				m_Value_4_Text.SetWindowText ( szValue );

				m_Label_5_Text.SetWindowText ( pMidiList->GetLabel5() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue5() );
				m_Value_5_Text.SetWindowText ( szValue );

				break;
			}

			case MIDI_EVENT_SETSMPTE :
			case MIDI_EVENT_TIMESIG :
			case MIDI_EVENT_KEYSIG :
			{
				m_Value_1_Combo.ShowWindow ( FALSE );
				m_Label_1_Combo.ShowWindow ( FALSE );
				m_Num_1_Combo.ShowWindow ( FALSE );
				m_Value_1_Text.ShowWindow ( TRUE );
				m_Label_1_Text.ShowWindow ( TRUE );
				m_Num_1_Text.ShowWindow ( TRUE );
				showHide ( pMidiList );

				m_Label_1_Text.SetWindowText ( pMidiList->GetLabel1() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue1() );
				m_Value_1_Text.SetWindowText ( szValue );

				m_Label_2_Text.SetWindowText (pMidiList->GetLabel2() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue2() );
				m_Value_2_Text.SetWindowText ( szValue );

				m_Label_3_Text.SetWindowText ( pMidiList->GetLabel3() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue3() );
				m_Value_3_Text.SetWindowText ( szValue );

				m_Label_4_Text.SetWindowText ( pMidiList->GetLabel4() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue4() );
				m_Value_4_Text.SetWindowText ( szValue );

				m_Label_5_Text.SetWindowText ( pMidiList->GetLabel5() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue5() );
				m_Value_5_Text.SetWindowText ( szValue );

				break;
			}

			case MIDI_EVENT_NOTEOFF :
			case MIDI_EVENT_NOTEON :
			{
				m_Value_1_Combo.ShowWindow ( TRUE );
				m_Label_1_Combo.ShowWindow ( TRUE );
				m_Num_1_Combo.ShowWindow ( TRUE );
				m_Value_1_Text.ShowWindow ( FALSE );
				m_Label_1_Text.ShowWindow ( FALSE );
				m_Num_1_Text.ShowWindow ( FALSE );
				showHide ( pMidiList );

				m_Label_1_Combo.SetWindowText ( pMidiList->GetLabel1() );
				for ( int i = 0; i < 128; i++ )
				{
					if ( pMidiList->GetChannel() != PERCUSSION_CHANNEL )
					{
						m_Value_1_Combo.AddString ( CMidiClass::NoteFullText ( i ) );
					}
					else
					{
						m_Value_1_Combo.AddString ( CMidiClass::DrumText ( i ) );
					}
				}

				if ( pMidiList ) 
				{
					int value1 = pMidiList->GetValue1();
					m_Value_1_Combo.SetCurSel(value1);
				}

				m_Label_2_Text.SetWindowText ( pMidiList->GetLabel2() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue2() );
				m_Value_2_Text.SetWindowText ( szValue );

				m_Label_3_Text.SetWindowText (pMidiList->GetLabel3() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue3() );
				m_Value_3_Text.SetWindowText ( szValue );

				m_Label_4_Text.SetWindowText (pMidiList->GetLabel4() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue4() );
				m_Value_4_Text.SetWindowText ( szValue );

				m_Label_5_Text.SetWindowText ( pMidiList->GetLabel5() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue5() );
				m_Value_5_Text.SetWindowText ( szValue );

				break;
			}

			case MIDI_EVENT_PARAMETER :
			{
				m_Value_1_Combo.ShowWindow ( TRUE );
				m_Label_1_Combo.ShowWindow ( TRUE );
				m_Num_1_Combo.ShowWindow ( TRUE );
				m_Value_1_Text.ShowWindow ( FALSE );
				m_Label_1_Text.ShowWindow ( FALSE );
				m_Num_1_Text.ShowWindow ( FALSE );
				showHide ( pMidiList );

				m_Label_1_Combo.SetWindowText ( pMidiList->GetLabel1() );
				for ( int i = 0; i < 128; i++ )
				{
					m_Value_1_Combo.AddString ( CMidiClass::ControlerText ( i ) );
				}

				if ( pMidiList ) 
				{
					int value1 = pMidiList->GetValue1();
					m_Value_1_Combo.SetCurSel(value1);
				}

				m_Label_2_Text.SetWindowText (pMidiList->GetLabel2() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue2() );
				m_Value_2_Text.SetWindowText ( szValue );

				m_Label_3_Text.SetWindowText ( pMidiList->GetLabel3() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue3() );
				m_Value_3_Text.SetWindowText ( szValue );

				m_Label_4_Text.SetWindowText ( pMidiList->GetLabel4() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue4() );
				m_Value_4_Text.SetWindowText ( szValue );

				m_Label_5_Text.SetWindowText ( pMidiList->GetLabel5() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue5() );
				m_Value_5_Text.SetWindowText ( szValue );

				break;
			}

			case MIDI_EVENT_PROGRAM :
			{
				m_Value_1_Combo.ShowWindow ( TRUE );
				m_Label_1_Combo.ShowWindow ( TRUE );
				m_Num_1_Combo.ShowWindow ( TRUE );
				m_Value_1_Text.ShowWindow ( FALSE );
				m_Label_1_Text.ShowWindow ( FALSE );
				m_Num_1_Text.ShowWindow ( FALSE );
				showHide ( pMidiList );

				m_Label_1_Combo.SetWindowText ( pMidiList->GetLabel1() );
				for ( int i = 0; i < 128; i++ )
				{
					m_Value_1_Combo.AddString ( CMidiClass::InstrumentText ( i ) );
				}

				if ( pMidiList ) 
				{
					int value1 = pMidiList->GetValue1();
					m_Value_1_Combo.SetCurSel(value1);
				}

				m_Label_2_Text.SetWindowText ( pMidiList->GetLabel2() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue2() );
				m_Value_2_Text.SetWindowText ( szValue );

				m_Label_3_Text.SetWindowText ( pMidiList->GetLabel3() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue3() );
				m_Value_3_Text.SetWindowText ( szValue );

				m_Label_4_Text.SetWindowText ( pMidiList->GetLabel4() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue4() );
				m_Value_4_Text.SetWindowText ( szValue );

				m_Label_5_Text.SetWindowText ( pMidiList->GetLabel5() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue5() );
				m_Value_5_Text.SetWindowText ( szValue );

				break;
			}

			case MIDI_EVENT_KEYPRESSURE :
			case MIDI_EVENT_CHANNELPRES :
			case MIDI_EVENT_WHEEL :
			case MIDI_EVENT_UNKNOWNTEXT :
			default :
			{
				m_Value_1_Combo.ShowWindow ( FALSE );
				m_Label_1_Combo.ShowWindow ( FALSE );
				m_Num_1_Combo.ShowWindow ( FALSE );
				m_Value_1_Text.ShowWindow ( TRUE );
				m_Label_1_Text.ShowWindow ( TRUE );
				m_Num_1_Text.ShowWindow ( TRUE );
				showHide ( pMidiList );

				m_Label_1_Text.SetWindowText ( pMidiList->GetLabel1()  );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue2() );
				m_Value_1_Text.SetWindowText ( szValue );

				m_Label_2_Text.SetWindowText ( pMidiList->GetLabel2() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue2() );
				m_Value_2_Text.SetWindowText ( szValue );

				m_Label_3_Text.SetWindowText ( pMidiList->GetLabel3() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue3() );
				m_Value_3_Text.SetWindowText ( szValue );

				m_Label_4_Text.SetWindowText ( pMidiList->GetLabel4() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue4() );
				m_Value_4_Text.SetWindowText ( szValue );

				m_Label_5_Text.SetWindowText ( pMidiList->GetLabel5() );
				sprintf_s ( szValue, sizeof(szValue), "%d", pMidiList->GetValue5() );
				m_Value_5_Text.SetWindowText ( szValue );

				break;
			}
		}
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
LRESULT CEventDetail::OnEditSelect(WPARAM wParam, LPARAM lParam)
{
	if ( lParam == (LPARAM)(m_Event_Combo.m_hWnd) )
	{
		int iSel = m_Event_Combo.GetCurSel();
		if ( iSel != m_pNewMidiList->GetEventIndex() )
		{
			m_pNewMidiList->NewEmptyEvent( iSel );
		}

		ChangeValue1Combo ( m_pNewMidiList );
		return 1;
	}

	return 0;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CEventDetail::OnCbnSelchangeEventCombo()
{
	int i = 0;
}
