#if !defined(AFX_MIDIEXPLORERVIEW_H__005D69A1_F290_11D2_8148_444553540000__INCLUDED_)
#define AFX_MIDIEXPLORERVIEW_H__005D69A1_F290_11D2_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MidiExplorerView.h : header file
//
#include "afxcview.h"
#include "afxole.h"

/////////////////////////////////////////////////////////////////////////////
// CMidiExplorerView view

class CMidiExplorerView : public CTreeView
{
	    friend	void	FriendActivate(BOOL bActivate, CView* pActivateView, CView* pDeactiveView, bool bForce);

    protected:
	    CMidiExplorerView();           // protected constructor used by dynamic creation
	    DECLARE_DYNCREATE(CMidiExplorerView)

    // Attributes
    public:

    // Operations
    public:
	    void DragClean();
	    DROPEFFECT SetDropEffect(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	    bool GetDragFilename ( COleDataObject* pDataObject );
	    void DeleteItemTree ( HTREEITEM hItem );
	    HTREEITEM GetNextItem ( HTREEITEM hItem );
	    HTREEITEM GetPrevItem ( HTREEITEM hItem );
	    void EnsureVisible ( HTREEITEM hItem, bool bTop );
	    void RefreshView ( HTREEITEM hItem );
	    BOOL IsChildNodeOf(HTREEITEM hItemChild, HTREEITEM hItemSuspectedParent);
	    BOOL TransferItem(HTREEITEM hItemDrag, HTREEITEM hItemDrop);
	    void OnButtonUp();
	    HTREEITEM ExpandDirectory ( HTREEITEM hTreeItem, char *pDirectory );
	    void ExpandDisk( HTREEITEM hTreeItem, char *pDirectory );
	    void FilterPathName ( WIN32_FIND_DATA  *tagFile );
	    void GetFullPathName ( HTREEITEM hTreeItem, char *pFilename, size_t iFilename );
	    void DisplayDirectory ( HTREEITEM hTreeItem );
	    void TreatSelectedItem ( HTREEITEM hTreeItem );
	    void DisplayDrives();

    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CMidiExplorerView)
	    public:
	    virtual void OnInitialUpdate();
	    virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	    virtual void OnDragLeave();
	    virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	    virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	    virtual BOOL PreTranslateMessage(MSG* pMsg);
	    protected:
	    virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	    virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	    virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	    virtual void PostNcDestroy();
	    virtual BOOL OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult);
	    //}}AFX_VIRTUAL

    // Implementation
    protected:
	    virtual ~CMidiExplorerView();
    #ifdef _DEBUG
	    virtual void AssertValid() const;
	    virtual void Dump(CDumpContext& dc) const;
    #endif

	    // Generated message map functions
    protected:
	    //		Tool type control.
	    CToolTipCtrl	m_ToolTip;

	    //		Old Values
	    CImageList		*m_pOldImageList;
	    CPoint			m_OldPoint;
	    CPoint			m_OldHotSpot;

	    //		Ole drag and drop
	    COleDropTarget	m_ExplorerDropTarget;
	    BOOL			m_bDraggingOle;
	    char			m_DragFilename [ MAX_PATHNAME_STRING ];
	    DROPEFFECT		m_prevDropEffectOle;

	    //		Simple drag and drop
	    bool			m_bDragging;
	    HTREEITEM		m_hItemDrag;
	    HTREEITEM		m_hItemDrop;

	    //		Tree View Initialization
	    CImageList		*m_pImageList;
	    bool			m_bFirstTime;

        CMWMenu         *m_pContextMenu;

    public :
	    //{{AFX_MSG(CMidiExplorerView)
	    afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	    afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	    afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	    afx_msg void OnExplorerHelp();
	    afx_msg void OnExplorerRename();
	    afx_msg void OnExplorerOpen();
	    afx_msg void OnExplorerDelete();
	    afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	    afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	    afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	    afx_msg void OnExplorerPlaydir();
	    afx_msg void OnExplorerModifyfiles();
	    afx_msg void OnExplorerRefresh();
	    afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	    afx_msg void OnTimer(UINT_PTR nIDEvent);
	    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	    afx_msg void OnDropFiles(HDROP hDropInfo);
	    afx_msg void OnExplorerRecord();
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
    public:
        afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
        afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
        afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDIEXPLORERVIEW_H__005D69A1_F290_11D2_8148_444553540000__INCLUDED_)
