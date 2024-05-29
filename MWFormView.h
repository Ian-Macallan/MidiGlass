#pragma once



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
};


