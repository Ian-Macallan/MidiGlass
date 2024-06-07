#if !defined(AFX_MIDIOWNPLAYER_H__919B1061_C5A5_11D2_8147_444553540000__INCLUDED_)
#define AFX_MIDIOWNPLAYER_H__919B1061_C5A5_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MidiOwnPlayer.h : header file
//
#include "MWObjects.h"
#include "MWFormView.h"

/////////////////////////////////////////////////////////////////////////////
// CMidiOwnPlayer form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMidiOwnPlayer : public CMWFormView
{
	friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	friend	BOOL	FriendEraseBkgndScrollView(CScrollView* pWnd, CDC* pDC);

protected:
	CMidiOwnPlayer();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMidiOwnPlayer)

protected :
	bool				m_bFirstTime;

// Form Data
public:
	//{{AFX_DATA(CMidiOwnPlayer)
	enum { IDD = IDD_MIDI_PLAYER };
	CMWSliderCtrl	m_Speed;
	CMWSliderCtrl	m_Slider;
	CMWEdit	m_State;
	CMWEdit	m_Maximum;
	CMWProgressCtrl	m_Progress;
	CMWEdit	m_Time;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void DisplayValues();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMidiOwnPlayer)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMidiOwnPlayer();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMidiOwnPlayer)
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDIOWNPLAYER_H__919B1061_C5A5_11D2_8147_444553540000__INCLUDED_)
