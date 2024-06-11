#if !defined(AFX_MODIFYTHREADDIALOG_H__BD1F84E1_F431_11D2_8148_444553540000__INCLUDED_)
#define AFX_MODIFYTHREADDIALOG_H__BD1F84E1_F431_11D2_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModifyThreadDialog.h : header file
//

#include "ModifyThread.h"
#include "MWDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CModifyThreadDialog dialog

class CModifyThreadDialog : public CMWDialog
{
    DECLARE_DYNAMIC(CModifyThreadDialog)
    // Construction
    public:
	    CModifyThreadDialog(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
	    //{{AFX_DATA(CModifyThreadDialog)
	    enum { IDD = IDD_MODIFY_THREAD };
	    CMWEdit	m_Number;
	    CMWEdit	m_Tune;
		    // NOTE: the ClassWizard will add data members here
	    //}}AFX_DATA


    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CModifyThreadDialog)
	    protected:
	    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	    //}}AFX_VIRTUAL

    // Implementation
    protected:
	    char			m_Directory [ MAX_PATHNAME_STRING ];
	    char			* m_pFilenames [ MAX_PLAYFILES ];
	    unsigned		m_iFilenames;

	    CMidiFile		* m_pMidiFile;
	    unsigned		m_iFileNumber;

	    // Generated message map functions
	    //{{AFX_MSG(CModifyThreadDialog)
	    virtual BOOL OnInitDialog();
	    afx_msg void OnDestroy();
	    afx_msg void OnTimer(UINT_PTR nIDEvent);
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
    public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYTHREADDIALOG_H__BD1F84E1_F431_11D2_8148_444553540000__INCLUDED_)
