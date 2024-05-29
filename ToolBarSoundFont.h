#pragma once

#include "MWToolBar.h"
#include "MWSF2FilesComboBox.h"

//
/////////////////////////////////////////////////////////////////////////////////////////
// CToolBarSoundFont
//
/////////////////////////////////////////////////////////////////////////////////////////
class CToolBarSoundFont : public CMWToolBar
{
	DECLARE_DYNAMIC(CToolBarSoundFont)

	public :
	    CMWSF2FilesComboBox m_comboBox;

	public:
		CToolBarSoundFont();
		virtual ~CToolBarSoundFont();

		BOOL CreateComboBox(class CMWSF2FilesComboBox& comboBox, 
							UINT nIndex, 
							UINT nID, 
							int nWidth, 
							int nDropHeight);

		BOOL ReLoadComboBox();

		LRESULT OnEditSelect(WPARAM wParam, LPARAM lParam);

	protected:
		DECLARE_MESSAGE_MAP()
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};


