
#include "stdafx.h"
#include "MidiGlassApp.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	CMidiWorksApp		theApp;

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateContentStopIe(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	// pCmdUI->Enable ( m_RightBrowserView != NULL );		

}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateFileTrace(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
	DisableMenuWhenNoMidifile(pCmdUI);
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateFileSendMail(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	pCmdUI->Enable ( FALSE );
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::DisableMenuWhenNoMidifile(CCmdUI *pCmdUI)
{
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		pCmdUI->Enable ( FALSE );
		return;
	}

	pCmdUI->Enable ( pMidifile != NULL );

}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateFileAddtotheplaylist(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateCorrectAddGmreset(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateCorrectAddGsreset(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateCorrectAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateCorrectEndoftrack(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateCorrectFiltertracks(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateCorrectKeysignature(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateCorrectMoveAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateCorrectStartoftrack(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateCorrectTimesignature(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateRemoveControllerBankselecthigh(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateRemoveControllerBankselectlow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateRemoveCopyright(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateRemoveCuepoint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateRemoveInstrumentname(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateRemoveLyrics(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateRemoveMarkers(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateRemoveResetallcontrolers(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateRemoveSequence(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateRemoveSequencer(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateRemoveSystemexclusive(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateRemoveText(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateRemoveTrackname(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

void CMainFrame::OnUpdateViewStatusBar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck ( m_wndStatusBar.GetStyle () & WS_VISIBLE ? 1 : 0 );
}

void CMainFrame::OnUpdateIndicatorError(CCmdUI *pCmdUI)
{
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	const char			*pString;

    pCmdUI->Enable(); 

	//		Then look current document
	if ( pMididoc != NULL )
	{

		if ( pMidifile != NULL )
		{
			pString = pMidifile->GetLastErrorText ();
			if ( pString != NULL )
			{
				pCmdUI->SetText( pString );
			}
			else
			{
				pCmdUI->SetText( "No Error" ); 
			}
		}
		else
		{
			pCmdUI->SetText( "No Error" ); 
		}
	}
	else
	{
		pCmdUI->SetText( "No Error" ); 
	}
}



void CMainFrame::OnUpdateFileSequencerPlayll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		pCmdUI->Enable ( FALSE );
		return;
	}

	pCmdUI->Enable ( pMidifile->LLIsStopped () && ! pMidifile->MciIsPlaying () );

}

void CMainFrame::OnUpdateFileSequencerStopll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		pCmdUI->Enable ( FALSE );
		return;
	}

	pCmdUI->Enable ( pMidifile->LLIsPlaying () && ! pMidifile->MciIsPlaying () );

}


void CMainFrame::OnUpdateFileSequencerForwardll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		pCmdUI->Enable ( FALSE );
		return;
	}

	pCmdUI->Enable ( pMidifile->LLIsPlaying () && ! pMidifile->MciIsPlaying () );

}

void CMainFrame::OnUpdateFileSequencerBackwardll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		pCmdUI->Enable ( FALSE );
		return;
	}

	pCmdUI->Enable ( pMidifile->LLIsPlaying () && ! pMidifile->MciIsPlaying () );

}

void CMainFrame::OnUpdateFileSequencerPausell(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		pCmdUI->Enable ( FALSE );
		return;
	}

	pCmdUI->Enable ( pMidifile->LLIsPaused () || pMidifile->LLIsPlaying () && ! pMidifile->MciIsPlaying () );

}

void CMainFrame::OnUpdateFileSequencerPlayandloopll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		pCmdUI->Enable ( FALSE );
		return;
	}

	pCmdUI->Enable ( pMidifile->LLIsStopped () && ! pMidifile->MciIsPlaying () );

}

void CMainFrame::OnUpdateFileSequencerPlay(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		pCmdUI->Enable ( FALSE );
		return;
	}

	pCmdUI->Enable ( ! pMidifile->MciIsPlaying () && ! pMidifile->LLIsPlaying () );

}

void CMainFrame::OnUpdateFileSequencerPlayandloop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		pCmdUI->Enable ( FALSE );
		return;
	}

	pCmdUI->Enable ( ! pMidifile->MciIsPlaying () && ! pMidifile->LLIsPlaying () );

}

void CMainFrame::OnUpdateFileSequencerStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		pCmdUI->Enable ( FALSE );
		return;
	}

	pCmdUI->Enable ( pMidifile->MciIsPlaying () && ! pMidifile->LLIsPlaying () );

}

void CMainFrame::OnUpdateFileSequencerResumell(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		pCmdUI->Enable ( FALSE );
		return;
	}

	pCmdUI->Enable ( pMidifile->LLIsPaused () && ! pMidifile->MciIsPlaying () );

}


void CMainFrame::OnUpdateViewMciToolBar(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck ( IsMciBarVisible ( ) );
}

void CMainFrame::OnUpdateViewMidiToolBar(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck ( IsMidiBarVisible ( )  );
}

void CMainFrame::OnUpdateViewMaintoolbar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck ( IsMainBarVisible ( )  );
}

void CMainFrame::OnUpdateViewIetoolbar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck ( IsIeBarVisible ( ) );
}

void CMainFrame::OnUpdateCorrectTruncateNote(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
}

void CMainFrame::OnUpdateContentTracksList(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck ( IsViewPresentAndVisible ( pCmdUI, m_LeftTrackView) );
}

void CMainFrame::OnUpdateContentMidiexplorer(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck ( IsViewPresentAndVisible ( pCmdUI, m_LeftExplorerView ) );
}

void CMainFrame::OnUpdateContentChannels(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck ( IsViewPresentAndVisible ( pCmdUI, m_RightChannelView ) );
}

void CMainFrame::OnUpdateContentHeader(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck ( IsViewPresentAndVisible ( pCmdUI, m_RightGeneralView ) );
	
}

void CMainFrame::OnUpdateContentNotes(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck ( IsViewPresentAndVisible ( pCmdUI, m_RightNotesView ) );
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateContentPlayer(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck ( IsViewPresentAndVisible ( pCmdUI, m_RightPlayerView ) );
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateContentPlaylist(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck ( IsViewPresentAndVisible ( pCmdUI, m_RightPlayListView ) );
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateContentStaff(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck ( IsViewPresentAndVisible ( pCmdUI, m_RightStaffView ) );
}

void CMainFrame::OnUpdateContentTracks(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck ( IsViewPresentAndVisible ( pCmdUI, m_RightEventView ) );
}

void CMainFrame::OnUpdateContentErrormessages(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck ( IsViewPresentAndVisible ( pCmdUI, m_RightErrorView ) );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateContentEventsShowhide(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_RightTextView != NULL && ! theApp.m_bSplitterWindow );	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateCorrectAddXgreset(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DisableMenuWhenNoMidifile(pCmdUI);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateEditFindagain(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( IsViewPresentAndVisible ( pCmdUI, m_RightEventView ) );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( IsViewPresentAndVisible ( pCmdUI, m_RightEventView ) );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateFileSavegeometry(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( ! theApp.m_bSplitterWindow );
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
bool CMainFrame::IsViewPresentAndVisible(CCmdUI *pCmdUI, CView *pView)
{
	if ( pView == NULL )
	{
		return false;
	}

	return ( pView->IsWindowVisible ( ) != FALSE );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateFileRecord(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
	DisableMenuWhenNoMidifile(pCmdUI);
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateViewSF2Toolbar(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck ( IsSF2BarVisible ( ) );
}
