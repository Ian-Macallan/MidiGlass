// MidiGlassApp.h : main header file for the MIDI application
//

#if !defined(AFX_MIDI_H__F6E8EC48_B230_11D2_8147_444553540000__INCLUDED_)
#define AFX_MIDI_H__F6E8EC48_B230_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "MidiDefs.h"
#include "MWObjects.h"
#include "MidiFile.h"

#include "MidiAbout.h"

#include "RecordDialog.h"	
#include "PlayerDialog.h"	// Added by ClassView

#include "ModifyThreadDialog.h"	// Added by ClassView
#include "MainFrm.h"

//
///////////////////////////////////////////////////////////////////////////////////////////
// CMidiWorksApp:
// See Midi Works.cpp for the implementation of this class
//
///////////////////////////////////////////////////////////////////////////////////////////
class CMidiWorksApp : public CWinApp
{
	public:
		CSingleDocTemplate* m_pSplitterDocTemplate;

		CImageList			m_imgExplorerIcons16x14;

		CImageList			m_imgImageEvents16x32;
		CImageList			m_imgImageEvents32x32;

		CImageList			m_imgImageList16x2;
		CImageList			m_imgImageList32x2;
		CImageList			m_imgImageList48x2;

		CImageList			m_imgImageListChannel16x14;

		CBitmap				m_bmBackGround;
		CBitmap				m_bmBackEmpty;

		CBrush				m_brBackGround;
		CBrush				m_brBackEmpty;

		CBrush				m_brWhitexff;
        CBrush				m_brWhitexee;
        CBrush				m_brWhitexdd;

        CBrush				m_brBlackx00;
        CBrush				m_brBlackx10;
        CBrush				m_brBlackx20;

		CRecordDialog		m_dlgRecordDialog;
		CWinThread			*m_RecordThread;

		CPlayerDialog		m_dlgPlayerDialogList;
		CWinThread			*m_PlayListThread;

		CPlayerDialog		m_dlgPlayerDialogDirectory;
		CWinThread			*m_PlayDirThread;

		CModifyThreadDialog	m_ModifyThreadDialog;
		CWinThread			*m_ModifyThread;

		CString				m_MidiFileName;
		CString				m_WaveDirectory;
		CString				m_SoundFontFile;

		int					m_iMidiDevice;
		int					m_iMidiInDevice;

		char				m_Explorer_Pathname [ MAX_PATHNAME_STRING ];
		int					m_bPlay_Tune_In_AboutBox;
		int					m_bSkip_To_Note;
		int					m_cParameter_Settings [ MAX_CONTROLERS ];
		bool				m_bParameter_Settings [ MAX_CONTROLERS ];
		int					m_iStart_Playing;

		unsigned			m_iSysEx;
		int					m_bAddStartOfTrack;
		int					m_bAddEndOfTrack;
		int					m_bBankSelectLow;
		int					m_bBankSelectHigh;
		int					m_bFilter;
		int					m_bCorrectStartOfTrack;

		int					m_bMainToolBar;
		int					m_bMciToolBar;
		int					m_bMidiToolBar;
		int					m_bIeToolBar;
		int					m_bSf2ToolBar;
		int					m_bStatusBar;

		int					m_bLargeBar;
		int					m_bReBar;

		int					m_bOwnBackground;
		int					m_bOwnControlColor;
		int					m_iSplitterWindow;
		bool				m_bSplitterWindow;
		int					m_bUse_Sound_Font;

		char				ModuleFileName [ MAX_PATH ];
		char				InitFileName [ MAX_PATH ];

		//
		//	FluidStnth
		CString				m_synth_midi_bank_select;
		CString				m_synth_gain;

	public :
		void GetModule ();
		bool SearchInitFile ( const char *pInitFileName, size_t iInitFileName );

		BOOL WriteProfileIniFile ( const char *section,  const char *keyName, const char *pValue );
		BOOL ReadProfileIniFile ( const char *section,  const char *keyName, char *pValue, size_t iValue, const char *pDefault );

		UINT GetProfileInt ( const char *section,  const char *keyName, int defaultValue );
		double GetProfileDouble ( const char *section,  const char *keyName, double defaultValue );
		CString GetProfileString( const char *section,  const char *keyName, const char *pDefaultValue = "" );

		BOOL WriteProfileInt ( const char *section,  const char *keyName, int value );
		BOOL WriteProfileDouble ( const char *section,  const char *keyName, double value );
		BOOL WriteProfileString ( const char *section,  const char *keyName, const char *pValue );
	public:
		CMidiWorksApp();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMidiWorksApp)
	public:
		virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	// Implementation
	//{{AFX_MSG(CMidiWorksApp)
		afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDI_H__F6E8EC48_B230_11D2_8147_444553540000__INCLUDED_)
