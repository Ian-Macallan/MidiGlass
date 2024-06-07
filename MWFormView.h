#pragma once

#include "MWMenu.h"

//
/////////////////////////////////////////////////////////////////
// Mode formulaire CMWFormView
//
/////////////////////////////////////////////////////////////////
class CMWFormView : public CFormView
{
	DECLARE_DYNCREATE(CMWFormView)

	protected:
		CMWFormView();           // constructeur protégé utilisé par la création dynamique
		CMWFormView(UINT nIDTemplate);
		virtual ~CMWFormView();

        CMWMenu     *m_pContextMenu;

	public:
		enum { IDD = IDD_MWFORMVIEW };

		void SetModified ( BOOL bModified );

	#ifdef _DEBUG
		virtual void AssertValid() const;
	#ifndef _WIN32_WCE
		virtual void Dump(CDumpContext& dc) const;
	#endif
	#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

		DECLARE_MESSAGE_MAP()
    public:
        afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
        afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


