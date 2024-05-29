// ToolBarSoundFont.cpp�: fichier d'impl�mentation
//

#include "stdafx.h"

#include <io.h>

#include "ToolBarSoundFont.h"

#include "resource.h"

#include "MidiGlassApp.h"

#include "strstr.h"

//
extern CMidiWorksApp	theApp;

//
/////////////////////////////////////////////////////////////////////////////////////////
// CToolBarSoundFont
//
/////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CToolBarSoundFont, CMWToolBar)

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CToolBarSoundFont::CToolBarSoundFont()
{

}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CToolBarSoundFont::~CToolBarSoundFont()
{
}


//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CToolBarSoundFont, CToolBar)
ON_MESSAGE(WM_MW_EDIT_SELECT,OnEditSelect)
END_MESSAGE_MAP()



//
/////////////////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CToolBarSoundFont
//
/////////////////////////////////////////////////////////////////////////////////////////
BOOL CToolBarSoundFont::CreateComboBox(CMWSF2FilesComboBox & comboBox, 
                                       UINT nIndex, 
                                       UINT nID, 
                                       int nWidth, 
                                       int nDropHeight)
{
    // Create the combo box
    SetButtonInfo(nIndex, nID, TBBS_SEPARATOR, nWidth);

    CRect rect;
    GetItemRect(nIndex, &rect);
    rect.top = 1;
    rect.bottom = rect.top + nDropHeight;
    if ( ! comboBox.Create(CBS_DROPDOWN|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL, rect, this, nID))
    {
        TRACE("Failed to create combo-box\n");
        return FALSE;
    }

	ReLoadComboBox();

	//	Set Selection
	for ( int i = 0; i < comboBox.GetCount(); i++ )
	{
		CString current = "";
		comboBox.GetLBText( i, current);
		if ( strcmp ( FindFileName ( theApp.m_SoundFontFile ), current ) == 0 )
		{
			comboBox.SetCurSel ( i );
			break;
		}
	}

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
BOOL CToolBarSoundFont::ReLoadComboBox()
{
	//
	m_comboBox.ResetContent();

	return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
BOOL CToolBarSoundFont::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: ajoutez ici votre code sp�cialis� et/ou l'appel de la classe de base
	return CMWToolBar::OnCommand(wParam, lParam);
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
LRESULT CToolBarSoundFont::OnEditSelect(WPARAM wParam, LPARAM lParam)
{
	char pathname [ MAX_PATH ];
	strcpy_s ( pathname, sizeof(pathname) , theApp.m_SoundFontFile );
	RemoveFileName ( pathname );
	int i = m_comboBox.GetCurSel();
	if ( i >= 0 )
	{
		CString current = "";
		m_comboBox.GetLBText( i, current);
		strcat_s ( pathname, sizeof(pathname) , "\\" );
		strcat_s ( pathname, sizeof(pathname) , current );
		theApp.m_SoundFontFile = pathname;
		return 1;
	}

	return 0;
}