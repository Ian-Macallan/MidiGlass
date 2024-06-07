#if !defined(AFX_EVENTDETAIL_H__67690921_B6AD_11D2_8147_444553540000__INCLUDED_)
#define AFX_EVENTDETAIL_H__67690921_B6AD_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventDetail.h : header file
//
#include "MWObjects.h"
#include "afxwin.h"
#include "MWDialog.h"

//
////////////////////////////////////////////////////////////////////////
// CEventDetail dialog
//
////////////////////////////////////////////////////////////////////////
class CEventDetail : public CMWDialog
{
	DECLARE_DYNAMIC(CEventDetail)

	friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	// Construction
	public:
		CEventDetail(CWnd* pParent = NULL);   // standard constructor
		virtual ~CEventDetail();

		void			setShow ( CMidiList *pOldMidiList );
		void			setInsert ( CMidiList *pOldMidiList );
		void			setModify( CMidiList *pOldMidiList );
		void			Free();
		void			showHide(CMidiList *pMidiList);
		CMidiList		*getNewMidiList();

		void ChangeValue1Combo ( CMidiList *pMidiList );

		CMidiList			*m_pNewMidiList;
		CMidiList			*m_pOldMidiList;

		// Dialog Data
		//{{AFX_DATA(CEventDetail)
		enum { IDD = IDD_EVENT_DETAIL };

		CMWEdit				m_Mesure;
		CMWEdit				m_DeltaTime;
		CMWEdit				m_Channel;
		CMWEventsComboBox	m_Event_Combo;

		CMWComboBox			m_Value_1_Combo;
		CMWEdit				m_Value_1_Text;
		CMWEdit				m_Value_2_Text;
		CMWEdit				m_Value_3_Text;
		CMWEdit				m_Value_4_Text;
		CMWEdit				m_Value_5_Text;

		CMWStatic			m_Label_1_Combo;
		CMWStatic			m_Label_1_Text;
		CMWStatic			m_Label_2_Text;
		CMWStatic			m_Label_3_Text;
		CMWStatic			m_Label_4_Text;
		CMWStatic			m_Label_5_Text;

		CMWStatic			m_Num_1_Combo;
		CMWStatic			m_Num_1_Text;
		CMWStatic			m_Num_2_Text;
		CMWStatic			m_Num_3_Text;
		CMWStatic			m_Num_4_Text;
		CMWStatic			m_Num_5_Text;

		CMWButton			m_Ok;
		CMWButton			m_Cancel;

		boolean				m_bShow;
		boolean				m_bInsert;
		boolean				m_bModify;

		//}}AFX_DATA


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CEventDetail)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL

	// Implementation
	protected:

		// Generated message map functions
		//{{AFX_MSG(CEventDetail)
		virtual BOOL OnInitDialog();
		virtual void OnOK();
		afx_msg void OnCbnSelchangeEventCombo();
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

		//
		virtual void OnCancel();
		LRESULT OnEditSelect(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTDETAIL_H__67690921_B6AD_11D2_8147_444553540000__INCLUDED_)
