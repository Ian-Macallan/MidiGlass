#pragma once

#define ID_MNU_MASK         0xff
#define ID_SYS_MENU         0xff
#define ID_POP_MENU         0xfe


//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
class CMWMenu : public CMenu
{
    DECLARE_DYNAMIC(CMWMenu)

    public:
        CMWMenu(void);
        virtual ~CMWMenu(void);

        //
    public:
        static int SetOwnDraw ( HMENU hMenu, bool bOwnDrawn = true, int level = 0 );

        int         m_iBitmapWidth;
        int         m_iBitmapHeight;

        //
        virtual BOOL CreateMenu();
        virtual BOOL CreatePopupMenu();

        BOOL Attach(HMENU hMenu);
	    HMENU Detach();

        //  Entry Point for Popup Menu
        virtual BOOL LoadMenu(LPCTSTR lpszResourceName);
        //  Entry Point for Popup Menu
        virtual BOOL LoadMenu(UINT nIDResource);

        //  Entry Point for System Menu
        CMWMenu *SetSystemMenu(CWnd* pWnd, CMenu* pSysMenu);
        CMWMenu *SetApplicationMenu(CWnd* pWnd, CMenu* pMenu);
        void UnSetSystemMenu(CMenu* pSysMenu);

        CMWMenu *GetSubMenu ( int nPos );
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct );
        virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct );
        virtual BOOL TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect = NULL);

        virtual UINT GetMenuItemCount ( );
        virtual BOOL GetMenuItemInfo ( UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos = FALSE );
        virtual BOOL SetMenuItemInfo ( UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos = FALSE );

        BOOL AppendMenu(UINT nFlags, UINT_PTR nIDNewItem = 0, LPCTSTR lpszNewItem = NULL);
	    BOOL AppendMenu(UINT nFlags, UINT_PTR nIDNewItem, const CBitmap* pBmp);

	    BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem = 0, LPCTSTR lpszNewItem = NULL);
	    BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const CBitmap* pBmp);
	    BOOL InsertMenuItem(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos = FALSE);
	    BOOL ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem = 0, LPCTSTR lpszNewItem = NULL);
	    BOOL ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const CBitmap* pBmp);

        virtual BOOL SetMenuInfo(LPCMENUINFO lpcmi);
	    virtual BOOL GetMenuInfo(LPMENUINFO lpcmi) const;

        virtual UINT EnableMenuItem ( UINT nIDEnableItem, UINT nEnable );

        void        MeasureMenuItem ( CDC *pDC, const char *pText, CSize *pSize, MENUITEMINFO *menuInfo );
        void        MeasureMenuItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
        void        DrawMenuItem (  LPDRAWITEMSTRUCT lpDrawItemStruct, CDC *pDC, CRect *pRect, const char *pText, MENUITEMINFO *menuInfo );
        void        DrawMenuItem ( LPDRAWITEMSTRUCT lpDrawItemStruct );

        void        ComputeBitmapMaxSize ( );
        void        ComputeBitmapMaxSize ( HBITMAP hBitmap );
        void        DrawBitmap ( CDC *pDC, CBitmap *pBitmap, CRect *pRect );
        int         DrawBitmap(LPDRAWITEMSTRUCT lpDrawItemStruct );
        BOOL        SetMenuItemBitmaps ( UINT nPosition, UINT nFlags, const CBitmap *pBmpUnchecked, const CBitmap *pBmpChecked );
        CMenu       *GetSubMenu ();
    protected :
        CWnd        *m_pWnd;
        CMenu       *m_pSubMenu;
        HICON       m_hCheckWhiteIcon;
};

