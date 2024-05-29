// MidiDoc.h : interface of the CMidiDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDIDOC_H__F6E8EC4E_B230_11D2_8147_444553540000__INCLUDED_)
#define AFX_MIDIDOC_H__F6E8EC4E_B230_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MidiFile.h"

class CMidiDoc : public CDocument
{
protected: // create from serialization only
	CMidiDoc();
	DECLARE_DYNCREATE(CMidiDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMidiDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetCurrentTrack ( unsigned short iTrack );
	unsigned short GetCurrentTrack();
	unsigned short GetEventMask();
	void SetEventMask ( unsigned short iMask );
	CMidiFile * GetDocumentMidiFile();
	virtual ~CMidiDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	unsigned short m_iMidiTrack;
	unsigned short m_iEventMask;
	CMidiFile * m_pMidiFile;
	//{{AFX_MSG(CMidiDoc)
	afx_msg void OnFileSendMail();
	afx_msg void OnUpdateFileSendMail(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDIDOC_H__F6E8EC4E_B230_11D2_8147_444553540000__INCLUDED_)
