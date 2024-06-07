#if !defined(AFX_EVENTVIEW_H__1EEF4D1C_B50C_11D2_8147_444553540000__INCLUDED_)
#define AFX_EVENTVIEW_H__1EEF4D1C_B50C_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventView.h : header file
//

#include "MidiDefs.h"

#include "MWObjects.h"
#include "MWFormView.h"

/////////////////////////////////////////////////////////////////////////////
// CEventView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "MidiList.h"

class CEventView : public CMWFormView
{
	friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	friend	BOOL	FriendEraseBkgndScrollView(CScrollView* pWnd, CDC* pDC);
	friend	void	FriendActivate(BOOL bActivate, CView* pActivateView, CView* pDeactiveView, bool bForce);

protected:
	CEventView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEventView)

// Form Data
public:
	//{{AFX_DATA(CEventView)
	enum { IDD = IDD_TRACK_EVENTS };
	CSpinButtonCtrl	m_Spin;
	CMWEdit			m_TrackName;
	CMWListCtrl		m_TrackEvents;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void IncrementCurrentIndice();
	void SearchEvents();
	void UnSelectAll();
	void SetSearchInfo ( 
		const CString strEvent, 
		const CString strProgram, 
		const CString strController,
		int V1_From, int V1_To, 
		int V2_From, int V2_To );

	void FillEventTableAddress();
	void AskAndDeleteEvents( bool bRemove );

	void ShowEventDetail();
	void InsertEventDetail( bool bBefore );
	void ModifyEventDetail();

	void DeleteEvents( bool bRemove );
	void InsertEvents( bool bBefore );
	void ModifyEvents();

	unsigned GetIDD();
	CMidiList * GetCurrentMidiEvent();
	CMidiList * GetSelectedItem();
	void DisplayValues();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	int					m_V1_From;
	int					m_V1_To;
	int					m_V2_From;
	int					m_V2_To;
	CString				m_strControllerSearched;
	CString				m_strProgramSearched;
	CString				m_strEventSearched;
	bool				m_bFirstTime;
	CMidiList			* m_MidiList [ MAX_LINES ]; 
	CMidiList			* m_pCurrentEvent;
	int					m_iCurrentIndice;
	CListCtrl			* m_pTrackEvents;
	virtual ~CEventView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CEventView)
	afx_msg void OnDblclkTrackEventList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownTrackEventList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnEventsDetail();
	afx_msg void OnEventsDelete();
	afx_msg void OnEventsRemove();
	afx_msg void OnEventsInsertBefore();
	afx_msg void OnEventsInsertAfter();
	afx_msg void OnEventsModify();

	afx_msg void OnEventsHelp();
	afx_msg void OnGetdispinfoTrackEventList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeTrackname();
	afx_msg void OnClickTrackEventList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTVIEW_H__1EEF4D1C_B50C_11D2_8147_444553540000__INCLUDED_)
