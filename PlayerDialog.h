#if !defined(AFX_PLAYERDIALOG_H__B43F18C1_F73F_11D2_8148_444553540000__INCLUDED_)
#define AFX_PLAYERDIALOG_H__B43F18C1_F73F_11D2_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayerDialog.h : header file
//

#define		PLAYER_PLAY_LIST		0
#define		PLAYER_PLAY_DIR			1

/////////////////////////////////////////////////////////////////////////////
// CPlayerDialog dialog

class CPlayerDialog : public CDialog
{
	friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

// Construction
public:
	void ResizeButtons ( CBitmap *pBitmap, CMWButton *pButton );
	void CreateButtons();
	void StopAndDelete( bool bNotify );
	void DisplayInstruments();
	void InitChannelList();
	void DisplayMidiCounters();
	void OnForwardList();
	void OnStopList();
	void OnPauseList();
	void OnBackwardList();
	void OnDestroyList();
	void OnPlayList();
	LRESULT OnMciNotifyList ( WPARAM wParam, LPARAM lParam );
	void PlayTuneList();
	BOOL InitPlayerList();

	void OnForwardDirectory();
	void OnStopDirectory();
	void OnPauseDirectory();
	void OnBackwardDirectory();
	void OnDestroyDirectory();
	void OnPlayDirectory();
	LRESULT OnMciNotifyDirectory ( WPARAM wParam, LPARAM lParam );
	void PlayTuneDirectory();
	BOOL InitPlayerDirectory();

	void PlayTune();
	char * GetShort ( const char *pString );
	LRESULT OnMciNotify(WPARAM wParam, LPARAM lParam);
	CPlayerDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlayerDialog)
	enum { IDD = IDD_PLAYER_DIALOG };
	CMWButton	m_Back;
	CMWButton	m_Forw;
	CListCtrl	m_Channels;
	CMWEdit	m_Time;
	CMWEdit	m_State;
	CMWSliderCtrl	m_Slider_Speed;
	CMWSliderCtrl	m_Slider;
	CMWProgressCtrl	m_Progress;
	CMWEdit	m_Maximum;
	CComboBox	m_Tune_Combo;
	CSpinButtonCtrl	m_Spin_Number;
	CMWEdit	m_Number;
	CMWButton	m_Stop;
	CMWButton	m_Play;
	CMWButton	m_Pause;
	CMWButton	m_Forward;
	CMWButton	m_Backward;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayerDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool			m_bInitializing;
	bool			m_bFirstTime;

	CListCtrl		* m_pChannels;

	int				m_iPlayer;

	char			m_Directory [ MAX_PATHNAME_STRING ];
	char			* m_pFilenames [ MAX_PLAYFILES ];
	int				m_iFilenames;

	int				m_iAction;

	CMidiFile		* m_pMidiFile;
	int				m_iTune;

	HBITMAP			m_hBitmapPlay;
	CBitmap			* m_pBitmapPlay;

	HBITMAP			m_hBitmapStop;
	CBitmap			* m_pBitmapStop;

	HBITMAP			m_hBitmapPause;
	CBitmap			* m_pBitmapPause;

	HBITMAP			m_hBitmapResume;
	CBitmap			* m_pBitmapResume;

	HBITMAP			m_hBitmapForward;
	CBitmap			* m_pBitmapForward;

	HBITMAP			m_hBitmapForw;
	CBitmap			* m_pBitmapForw;

	HBITMAP			m_hBitmapBackward;
	CBitmap			* m_pBitmapBackward;

	HBITMAP			m_hBitmapBack;
	CBitmap			* m_pBitmapBack;

	// Generated message map functions
	//{{AFX_MSG(CPlayerDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPlay();
	afx_msg void OnBackward();
	afx_msg void OnPause();
	afx_msg void OnStop();
	afx_msg void OnForward();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnChangeNumber();
	afx_msg void OnSelchangeTuneCombo();
	afx_msg void OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBack();
	afx_msg void OnForw();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYERDIALOG_H__B43F18C1_F73F_11D2_8148_444553540000__INCLUDED_)
