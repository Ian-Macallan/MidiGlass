#pragma once
#if !defined(AFX_RECORDDIALOG_H__09A065AA_3050_11D3_8149_444553540000__INCLUDED_)

#define AFX_RECORDDIALOG_H__09A065AA_3050_11D3_8149_444553540000__INCLUDED_

// #include "MidiWave.h"	// Added by ClassView
#include "AudioLoopbackThread.h"
#include "MWDialog.h"

//

//
////////////////////////////////////////////////////////////////////////////////////////
// CRecordDialog dialog
//
////////////////////////////////////////////////////////////////////////////////////////
class CRecordDialog : public CMWDialog
{
	// Construction
	public:
		CRecordDialog(CWnd* pParent = NULL);   // standard constructor
		LRESULT OnMciNotify(WPARAM wParam, LPARAM lParam);
	// Dialog Data
		//{{AFX_DATA(CRecordDialog)
		enum { IDD = IDD_RECORD_DIALOG };
		CMWEdit	m_Text;
		CMWProgressCtrl	m_Progress_Bar;
		CMWEdit	m_WaveDirectory;
		CMWEdit	m_dlgMidiFile;
		CMWButton	m_Ok;
		CMWButton	m_Cancel;
		//}}AFX_DATA


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CRecordDialog)
		public:
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL

	// Implementation
	protected:
		CWinThread				*m_pAudioloopbackThread;
		bool					m_bSaved;
		bool					m_bClosing;

		CMidiFile				m_MidiFile;

		// Generated message map functions
		//{{AFX_MSG(CRecordDialog)
		afx_msg void OnDestroy();
		virtual BOOL OnInitDialog();
		virtual void OnOK();
		virtual void OnCancel();
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDDIALOG_H__09A065AA_3050_11D3_8149_444553540000__INCLUDED_)
