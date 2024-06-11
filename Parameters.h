#if !defined(AFX_PARAMETERS_H__A5531C61_D31F_11D2_8147_444553540000__INCLUDED_)
#define AFX_PARAMETERS_H__A5531C61_D31F_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Parameters.h : header file
//
#include "MWObjects.h"
#include "MWDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CParameters dialog

class CParameters : public CMWDialog
{
    DECLARE_DYNAMIC(CParameters)

	friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

    // Construction
    public:
	    void DisplayValues();
	    CParameters(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
	    //{{AFX_DATA(CParameters)
	    enum { IDD = IDD_CONTROLER };
	    CMWButton	m_Check_Settings;
	    CMWButton	m_Ok;
	    CMWButton	m_Cancel;
	    CMWEdit	m_Settings;
	    CMWComboBox	m_Controler;
	    //}}AFX_DATA


    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CParameters)
	    protected:
	    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	    //}}AFX_VIRTUAL

    // Implementation
    protected:

	    // Generated message map functions
	    //{{AFX_MSG(CParameters)
	    virtual BOOL OnInitDialog();
	    afx_msg void OnSelchangeControler();
	    afx_msg void OnChangeSettings();
	    afx_msg void OnCheckSettings();
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMETERS_H__A5531C61_D31F_11D2_8147_444553540000__INCLUDED_)
