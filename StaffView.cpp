//
///////////////////////////////////////////////////////////////////////////////////////
// StaffView.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "StaffView.h"

#include "WaitTreatment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	NOTE_TREBLE_UP		80
#define	NOTE_TREBLE_DOWN	60
#define	NOTE_BASS_DOWN		40

//
///////////////////////////////////////////////////////////////////////////////////////
// CStaffView
//
///////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CStaffView, CScrollView)

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
CStaffView::CStaffView()
{
	m_bFirstTime		= true;
	m_iMesure			= 0;
	m_iLastMesure		= 0;
	m_bScale			= false;

	//		Initialize all other values
	ComputeSizeForScreen ( NULL, NULL, &m_ScreenParms );
	ComputeSizeForPrinter ( NULL, NULL, &m_PrinterParms );
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
CStaffView::~CStaffView()
{
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CStaffView, CScrollView)
	//{{AFX_MSG_MAP(CStaffView)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_STAFF_HELP, OnStaffHelp)
	ON_COMMAND(ID_STAFF_SCALE, OnStaffScale)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_WM_MOVE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////////
// CStaffView drawing
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::OnDraw(CDC* pDC)
{
	CMidiDoc *pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );

	// TODO
	DrawAll(pDC, NULL, &m_ScreenParms );

}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawAll( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView )
{
	DrawFrameWork ( pDC, pInfo, parmsView );

	DrawNotes ( pDC, pInfo, parmsView );
}

//
///////////////////////////////////////////////////////////////////////////////////////
// CStaffView diagnostics
//
///////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CStaffView::AssertValid() const
{
	CScrollView::AssertValid();
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

//
///////////////////////////////////////////////////////////////////////////////////////
// CStaffView message handlers
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO

	SetViewParameters ( );

	if ( m_bFirstTime || ( lHint & RIGHT_MASK_STAFF ) || lHint == 0 )
	{
		m_bFirstTime = false;
		DisplayValues (NULL, NULL, &m_ScreenParms);
	}
	else
	{
		ForceToRedraw (NULL, NULL, &m_ScreenParms);
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	// TODO
	SetViewParameters ();

}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DisplayValues(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	ForceToRedraw ( pDC, pInfo, parmsView);
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawFrameWork(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView )
{
	long		iLine;
	long		iMaxStaff;

	iMaxStaff = ComputeFrameWorkNumber (pDC, pInfo, parmsView);

	for ( iLine = 0; iLine < 1; iLine++ )
	{
		if ( TestLineInPage ( pDC, pInfo, parmsView, iLine ) )
		{
			DrawOneFrameWork ( pDC, pInfo, parmsView, ComputeCy ( pDC, pInfo, parmsView, iLine ) );
		}
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////
//		Draw one frame work
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawOneFrameWork(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCy)
{
	RECT			rectFrame;
	long			iX;

	GetStaffViewSize (pDC, pInfo, parmsView, &rectFrame );

	//		Test if space is enought to draw
	if ( parmsView->m_iLeftMargin + parmsView->m_iDoubleBarsSpace >= rectFrame.right - parmsView->m_iLeftMargin - parmsView->m_iDoubleBarsSpace )
	{
		return;
	}

	SetStrongPen ( pDC, pInfo, parmsView );

	//		Draw the full starting bar
	pDC->MoveTo ( parmsView->m_iLeftMargin, iCy );
	pDC->LineTo ( parmsView->m_iLeftMargin, iCy + 8 * parmsView->m_iNoteHeight + parmsView->m_iSpaceBetweenLowHigh + 1 );

	SetNormalPen ( pDC, pInfo, parmsView );

	//		Draw starting bar for high
	pDC->MoveTo ( parmsView->m_iLeftMargin + parmsView->m_iDoubleBarsSpace, iCy );
	pDC->LineTo ( parmsView->m_iLeftMargin + parmsView->m_iDoubleBarsSpace, iCy + 4 * parmsView->m_iNoteHeight );

	//		Draw the framework for High
	for ( iX = 0; iX < 5; iX++ )
	{
		pDC->MoveTo ( parmsView->m_iLeftMargin + parmsView->m_iDoubleBarsSpace, iCy + iX * parmsView->m_iNoteHeight );
		pDC->LineTo ( rectFrame.right - parmsView->m_iRightMargin, iCy + iX * parmsView->m_iNoteHeight );
	}

	//		Draw Ending bar high
	pDC->MoveTo ( rectFrame.right - parmsView->m_iRightMargin, iCy );
	pDC->LineTo ( rectFrame.right - parmsView->m_iRightMargin, iCy + 4 * parmsView->m_iNoteHeight );

	//		Draw Low
	iCy = iCy + parmsView->m_iSpaceBetweenLowHigh + 4 * parmsView->m_iNoteHeight;

	//		Draw starting bar for low
	pDC->MoveTo ( parmsView->m_iLeftMargin + parmsView->m_iDoubleBarsSpace, iCy );
	pDC->LineTo ( parmsView->m_iLeftMargin + parmsView->m_iDoubleBarsSpace, iCy + 4 * parmsView->m_iNoteHeight );

	//		Draw the framework for low
	for ( iX = 0; iX < 5; iX++ )
	{
		pDC->MoveTo ( parmsView->m_iLeftMargin + parmsView->m_iDoubleBarsSpace, iCy + iX * parmsView->m_iNoteHeight );
		pDC->LineTo ( rectFrame.right - parmsView->m_iRightMargin, iCy + iX * parmsView->m_iNoteHeight );
	}

	//		Draw Ending bar for low
	pDC->MoveTo ( rectFrame.right - parmsView->m_iRightMargin, iCy );
	pDC->LineTo ( rectFrame.right - parmsView->m_iRightMargin, iCy + 4 * parmsView->m_iNoteHeight );

}

//
///////////////////////////////////////////////////////////////////////////////////////
//		Draw notes
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawNotes( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView )
{
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	CMidiTrack			*pMiditrack;
	CMidiList			*pMidilist;
	CMidiList			*pNext;

	int					iTrackNumber;
	long				iCx			= parmsView->m_iLeftMargin;
	long				iLine		= 0;
	long				iMesure		= 0;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		if ( TestLineInPage ( pDC, pInfo, parmsView, iLine )  )
		{
			//		Draw Title
			DrawTitle ( pDC, pInfo, parmsView );

			//		Draw Track name
			DrawTrackName ( pDC, pInfo, parmsView );
		}

		//		Get the current Track
		iTrackNumber = pMididoc->GetCurrentTrack ();

		//		Get the track address associated to the track number
		pMiditrack = pMidifile->GetMidiTrackAddress ( iTrackNumber );
		if ( pMiditrack != NULL )
		{
			pMidilist	= pMiditrack->GetFirstAddress ();

			//		Now display the notes
			bool				bSeparator = false;

			if ( pMidilist != NULL )
			{
				m_cNN	=	pMidilist->GetNN ( );
				m_cDD	=	pMidilist->GetDD ( );
			}

			while ( pMidilist != NULL )
			{
				pNext = 
					DrawOneMesure(pDC, pInfo, parmsView, iCx, iLine, pMidilist, bSeparator);
				pMidilist = pNext;

				bSeparator = true;

			}
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
CMidiList *CStaffView::DrawOneMesure (	CDC *pDC, CPrintInfo* pInfo,  CStaffViewParms *parmsView, long &iCx, long &iLine, 
										 CMidiList *pList, bool bSeparator )
{
	int					iMesure;
	int					iBeat;
	int					iTimeDivision;
	unsigned long		iDuration;
	int					iAddColumn;
	int					iX;

	//		Reset the Mesure table
	//		NN represent the number of sub Beat
	memset ( m_cMesureHiTable, 0, sizeof ( m_cMesureHiTable ) );
	memset ( m_cMesureLoTable, 0, sizeof ( m_cMesureLoTable ) );

	iMesure			= pList->GetMesure ( );
	iBeat			= pList->GetBeat ( );
	iTimeDivision	= pList->GetTimeDivision ( );
	iAddColumn		= 0;

	if ( bSeparator )
	{
		if ( iCx + parmsView->m_iMesureSpace + parmsView->m_iKeySignatureSpace + parmsView->m_iNoteWidth * 3 > RightEdge (pDC, pInfo, parmsView) )
		{
			AddNewFrameWork(pDC, pInfo, parmsView, iCx, iLine);
		}
		else
		{
			iCx = iCx + parmsView->m_iMesureSpace / 2;
			if ( TestLineInPage ( pDC, pInfo, parmsView, iLine ) )
			{
				SetNormalPen ( pDC, pInfo, parmsView );
				DrawSeparator (pDC, pInfo, parmsView, iCx, ComputeCy (pDC, pInfo, parmsView, iLine ) );
			}
			iCx = iCx + parmsView->m_iMesureSpace / 2;
		}
	}

	if ( m_cNN != pList->GetNN ( ) || m_cDD !=	pList->GetDD ( ) || iMesure	==	0 )
	{
		if ( TestLineInPage ( pDC, pInfo, parmsView, iLine )  )
		{
			iCx		= iCx + parmsView->m_iKeySignatureSpace / 2;

			char	szText [ MAX_INT_STRING ];

			m_cNN = pList->GetNN ( );
			m_cDD = pList->GetDD ( );

			sprintf_s ( szText, sizeof ( szText ), "%d", m_cNN );
			SetAnsiVarFont ( pDC, pInfo, parmsView );
			pDC->TextOut ( iCx - parmsView->m_iTextWidth, ComputeCy (pDC, pInfo, parmsView, iLine ) + parmsView->m_iTextHeight / 4, szText );

			sprintf_s ( szText, sizeof ( szText ), "%d", pList->Power ( m_cDD ) );
			SetAnsiVarFont ( pDC, pInfo, parmsView );
			pDC->TextOut ( iCx - parmsView->m_iTextWidth, ComputeCy (pDC, pInfo, parmsView, iLine ) + parmsView->m_iTextHeight / 4 + parmsView->m_iTextHeight, szText );
		}

		iCx		= iCx + parmsView->m_iKeySignatureSpace / 2;
	}

	if ( TestLineInPage ( pDC, pInfo, parmsView, iLine )  )
	{
		char	szText [ MAX_INT_STRING ];
		sprintf_s ( szText, sizeof ( szText ), "%d", iMesure + 1 );
		DrawMesureText ( pDC, pInfo, parmsView, iCx, ComputeCy (pDC, pInfo, parmsView,  iLine ), szText );
	}

	long oldCx			= 0;
	long oldLine		= 0;
	long increment		= 0;

	while ( pList != NULL )
	{
		if ( pList->GetEventIndex () == MIDI_EVENT_NOTEON )
		{
			iDuration = pList->GetNoteDuration ();

			for ( iX = iBeat * 8; iX < iBeat * 8 + ( long ) iDuration / 15; iX++ )
			{
				if ( iX < MAX_INSIDE_MESURE )
				{
					m_cMesureHiTable [ iX ] = 1;
				}
			}

			//	Display Silences if needed
			DisplaySilences ( pDC, pInfo, parmsView, iCx, iLine, iBeat * 8 );

			//	Display the note
			if ( TestLineInPage ( pDC, pInfo, parmsView, iLine ) )
			{
				DrawOneNote ( pDC, pInfo, parmsView, iCx + increment, iLine, pList );
			}

			iAddColumn = max ( iAddColumn, 2 * parmsView->m_iNoteWidth );
		}

		pList = pList->GetNext ();

		if ( pList != NULL )
		{
			//		Note the same mesure
			//		So exit to caller
			if ( pList->GetMesure () != iMesure )
			{
				m_iLastMesure = iMesure;
				ComputeCxAndLine ( pDC, pInfo, parmsView, iCx, iLine, iAddColumn );

				//	Display Silences if needed
//				DisplaySilences ( pDC, pInfo, parmsView, iCx, iLine, m_cNN );

				return pList;
			}

			//
			//		Same mesure but not same subdivision
			//		A space will be inserted
			if ( pList->GetBeat () != iBeat )
			{
				ComputeCxAndLine ( pDC, pInfo, parmsView, iCx, iLine, iAddColumn );
				iAddColumn = 0;
			}
			//		Same mesure and same subdivision
			else
			{
				//		Not the Same place 
				if ( pList->GetTimeDivision () != iTimeDivision )
				{
					ComputeCxAndLine ( pDC, pInfo, parmsView, iCx, iLine, iAddColumn );
					iAddColumn = 0;
				}
				else
				{
					if ( iDuration != pList->GetNoteDuration () )
					{
//						iCx += m_iMinColumnSpace;
					}
				}
			}

			iMesure			= pList->GetMesure ( );
			iBeat			= pList->GetBeat ( );
			iTimeDivision	= pList->GetTimeDivision ( );

			if ( iCx == oldCx && iLine == oldLine )
			{
				//increment = increment + 1;
			}
			else
			{
				increment = 0;
			}

			oldCx	= iCx;
			oldLine	= iLine;
		}
	}

	//	Display Silences if needed
//	DisplaySilences ( pDC, pInfo, parmsView, iCx, iLine, m_cNN );

	m_iLastMesure = iMesure;

	return pList;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawOneNoteVertQueue ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, int iCx, int iCy )
{
	CPoint from1 ( iCx + parmsView->m_iNoteWidth - 1, iCy + parmsView->m_iNoteHeight / 2 );
	CPoint to1   ( iCx + parmsView->m_iNoteWidth - 1, iCy - parmsView->m_iNoteQueue );

	pDC->MoveTo ( from1 );
	pDC->LineTo ( to1 );

}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawOneNoteHorzQueue ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, int iCx, int iCy, int number )
{
	bool bArc = false;

	int space = parmsView->m_iCrocheSpace;
	if ( number == 1 )
	{
		space = parmsView->m_iCrocheSpace;
	}
	else if ( number == 2 )
	{
		space = parmsView->m_iDoubleCrocheSpace;
	}
	else if ( number == 3 )
	{
		space = parmsView->m_iTripleCrocheSpace;
	}

	//	One
	CPoint from1 ( iCx + parmsView->m_iNoteWidth - 1, iCy - parmsView->m_iNoteQueue );
	CPoint to1   ( iCx + parmsView->m_iNoteWidth + space, iCy - parmsView->m_iNoteQueue + parmsView->m_iNoteQueue / 4 );
	CRect rect1 ( from1.x, from1.y, to1.x, to1.y );
	if ( bArc )
	{
		pDC->Arc (	rect1, to1, from1 );
	}
	else
	{
		pDC->MoveTo ( from1 );
		pDC->LineTo ( to1 );
	}

	if ( number <= 1 )
	{
		return;
	}

	//	Two
	CPoint from2 ( iCx + parmsView->m_iNoteWidth - 1, iCy - 3 * parmsView->m_iNoteQueue / 4 );
	CPoint to2   ( iCx + parmsView->m_iNoteWidth + space, iCy - 3 * parmsView->m_iNoteQueue / 4 + parmsView->m_iNoteQueue / 4 );
	CRect rect2 ( from2.x, from2.y, to2.x, to2.y );
		if ( bArc )
	{
		pDC->Arc ( rect2, to2, from2 );
	}
	else
	{
		pDC->MoveTo ( from2 );
		pDC->LineTo ( to2 );
	}
	
	if ( number <= 2 )
	{
		return;
	}

	//	Three
	CPoint from3 ( iCx + parmsView->m_iNoteWidth - 1, iCy - 2 * parmsView->m_iNoteQueue / 4 );
	CPoint to3   ( iCx + parmsView->m_iNoteWidth + space, iCy - 2 * parmsView->m_iNoteQueue / 4 + parmsView->m_iNoteQueue / 4 );
	CRect rect3 ( from3.x, from3.y, to3.x, to3.y );
		if ( bArc )
	{
		pDC->Arc ( rect3, to3, from3 );
	}
	else
	{
		pDC->MoveTo ( from3 );
		pDC->LineTo ( to3 );
		}

}

//
///////////////////////////////////////////////////////////////////////////////////////
//		Draw One note on the view
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawOneNote(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, const long iCx, long &iLine, CMidiList *pList)
{
	unsigned int		cNote;
	int					iLevel;
	int					iNote;
	bool				bBemol;
	bool				bDiese;
	unsigned long		iDuration;
	unsigned int		cHeight;
	char				szText [ MAX_INT_STRING ];
	int					iCy;

	cNote		= pList->GetValue1 ();
	sprintf_s ( szText, sizeof ( szText ), "%x", cNote );

	iCy			= ComputeCy (pDC, pInfo, parmsView, iLine );
	iLevel		= cNote / 12;
	iNote		= cNote % 12;
	bBemol		= pList->IsBemol ( cNote );
	bDiese		= pList->IsDiese ( cNote );
	iDuration	= pList->GetNoteDuration ( );
	cHeight		= pList->GetHeight ( cNote );

	if ( iDuration >= NOTE_BLANCHE )
	{
		pDC->SelectStockObject ( WHITE_BRUSH );
	}
	else
	{
		pDC->SelectStockObject ( BLACK_BRUSH );
	}

	SetNormalPen ( pDC, pInfo, parmsView );
	if ( iLevel >= 5 )	// Greater than Do
	{
		iCy = iCy + 4 * parmsView->m_iNoteHeight - ( parmsView->m_iNoteHeight * ( ( iLevel - 5 ) * 7 + cHeight - 1 ) ) / 2;
		pDC->MoveTo ( iCx + 1, iCy + 1 );
		Round ( pDC,  iCx + 1, iCy + 1, iCx + parmsView->m_iNoteWidth , iCy + parmsView->m_iNoteHeight );
		if ( bBemol )
		{
			DrawBemol ( pDC, pInfo, parmsView, iCx, iCy );
		}

		DrawAdditionalFrame ( pDC, pInfo, parmsView, iCx, iLine, pList );
	}
	else
	{
		iCy = iCy + ( parmsView->m_iSpaceBetweenLowHigh + 8 * parmsView->m_iNoteHeight );

		iCy = iCy - ( parmsView->m_iNoteHeight * ( ( iLevel - 2 ) * 7 + cHeight - 2 ) ) / 2 + 4 * parmsView->m_iNoteHeight;
		pDC->MoveTo ( iCx + 1, iCy + 1 );
		Round ( pDC,  iCx + 1, iCy + 1, iCx + parmsView->m_iNoteWidth , iCy + parmsView->m_iNoteHeight );
		if ( bBemol )
		{
			DrawBemol ( pDC, pInfo, parmsView, iCx, iCy );
		}

		DrawAdditionalFrame ( pDC, pInfo, parmsView, iCx, iLine, pList );
	}

	if ( iDuration <= NOTE_BLANCHE - parmsView->m_iNoteTolerance )
	{

		if ( iDuration > NOTE_NOIRE - parmsView->m_iNoteTolerance )
		{
			//		This is a Black: A queue at least
			SetNormalPen ( pDC, pInfo, parmsView );

			//	One
			DrawOneNoteVertQueue ( pDC, pInfo, parmsView, iCx, iCy );

			if ( iDuration >= 3 * NOTE_NOIRE / 2 - parmsView->m_iNoteTolerance )
			{
				DrawDot ( pDC, pInfo, parmsView, iCx + 3 * parmsView->m_iNoteWidth / 2, iCy );
			}

		}
		else
		{

			SetNormalPen ( pDC, pInfo, parmsView );

			DrawOneNoteVertQueue ( pDC, pInfo, parmsView, iCx, iCy );

			if ( iDuration > NOTE_CROCHE - parmsView->m_iNoteTolerance )
			{
				SetStrongPen ( pDC, pInfo, parmsView );

				DrawOneNoteHorzQueue ( pDC, pInfo, parmsView, iCx, iCy, 1 );

				if ( iDuration >= 3 * NOTE_CROCHE / 2 - parmsView->m_iNoteTolerance )
				{
					DrawDot ( pDC, pInfo, parmsView, iCx + 3 * parmsView->m_iNoteWidth / 2, iCy );
				}
			}
			else
			{
				if ( iDuration > NOTE_DOUBLE_CROCHE - parmsView->m_iNoteTolerance )
				{
					SetStrongPen ( pDC, pInfo, parmsView );

					DrawOneNoteHorzQueue ( pDC, pInfo, parmsView, iCx, iCy, 2 );

					if ( iDuration >= 3 * NOTE_DOUBLE_CROCHE / 2 - parmsView->m_iNoteTolerance )
					{
						DrawDot ( pDC, pInfo, parmsView, iCx + 3 * parmsView->m_iNoteWidth / 2, iCy );
					}
				}
				else
				{
					SetStrongPen ( pDC, pInfo, parmsView );

					DrawOneNoteHorzQueue ( pDC, pInfo, parmsView, iCx, iCy, 3 );

					if ( iDuration >= 3 * NOTE_TRIPLE_CROCHE / 2 - parmsView->m_iNoteTolerance )
					{
						DrawDot ( pDC, pInfo, parmsView, iCx + 3 * parmsView->m_iNoteWidth / 2, iCy );
					}
				}
			}
		}
	}
	else
	{
		if ( iDuration <= NOTE_RONDE - parmsView->m_iNoteTolerance )
		{
			if ( iDuration >= 3 * NOTE_BLANCHE / 2 - parmsView->m_iNoteTolerance )
			{
				DrawDot ( pDC, pInfo, parmsView, iCx + 3 * parmsView->m_iNoteWidth / 2, iCy );
			}
		}
		else
		{
			if ( iDuration >= 3 * NOTE_RONDE / 2 - parmsView->m_iNoteTolerance )
			{
				DrawDot ( pDC, pInfo, parmsView, iCx + 3 * parmsView->m_iNoteWidth / 2, iCy );
			}
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawSeparator(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iCy)
{
	//		Draw Ending bar
	pDC->MoveTo ( iCx, iCy );
	pDC->LineTo ( iCx, iCy + 4 * parmsView->m_iNoteHeight );

	//		Draw Low
	iCy = iCy + parmsView->m_iSpaceBetweenLowHigh + 4 * parmsView->m_iNoteHeight;

	pDC->MoveTo ( iCx, iCy );
	pDC->LineTo ( iCx, iCy + 4 * parmsView->m_iNoteHeight );

}

//
///////////////////////////////////////////////////////////////////////////////////////
//	Compute the physical line associated to a logical line
//
///////////////////////////////////////////////////////////////////////////////////////
long CStaffView::ComputeCy (CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iLine )
{
	long			iRealLine;

	//		Place the line in the page
	if ( ComputeFrameWorkNumber (pDC, pInfo, parmsView) != 0 )
	{
		iLine = iLine % ComputeFrameWorkNumber (pDC, pInfo, parmsView);
	}

	//		Now compute Cy
	iRealLine = parmsView->m_iTopMargin 
		+ iLine * ( parmsView->m_iSpaceBetweenStaff + parmsView->m_iSpaceBetweenLowHigh + 8 * parmsView->m_iNoteHeight ); 

	return iRealLine;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
bool CStaffView::VerifyColumnInRect(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx)
{
	RECT		rectFrame;

	GetStaffViewSize (pDC, pInfo, parmsView, &rectFrame );
	if (    iCx < LeftEdge (pDC, pInfo, parmsView) 
		 || iCx > RightEdge (pDC, pInfo, parmsView)  )
	{
		return false;
	}

	return true;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
long CStaffView::LeftEdge(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	RECT		rectFrame;

	GetStaffViewSize (pDC, pInfo, parmsView, &rectFrame );

	return rectFrame.left + parmsView->m_iLeftMargin;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
long CStaffView::RightEdge(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	RECT		rectFrame;

	GetStaffViewSize (pDC, pInfo, parmsView,  &rectFrame );

	return rectFrame.right - parmsView->m_iRightMargin;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
long CStaffView::TopEdge(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	RECT		rectFrame;

	GetStaffViewSize (pDC, pInfo, parmsView,  &rectFrame );

	return rectFrame.top + parmsView->m_iTopMargin;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
long CStaffView::BottomEdge(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	RECT		rectFrame;

	GetStaffViewSize (pDC, pInfo, parmsView, &rectFrame );

	return rectFrame.bottom - parmsView->m_iBottomMargin;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
long CStaffView::FrameWorkWidth(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	return 0;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
long CStaffView::FrameWorkHeight(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView )
{
	return parmsView->m_iSpaceBetweenStaff + parmsView->m_iSpaceBetweenLowHigh 
			+ 4 * parmsView->m_iNoteHeight + 4 * parmsView->m_iNoteHeight;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
long CStaffView::ComputeEndOfLine(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, int iLine)
{
	return ( iLine + 1 ) 
			* ( parmsView->m_iSpaceBetweenStaff + parmsView->m_iSpaceBetweenLowHigh 
				+ 4 * parmsView->m_iNoteHeight + 4 * parmsView->m_iNoteHeight ) 
				+ parmsView->m_iTopMargin + parmsView->m_iBottomMargin;

}
 
//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
long CStaffView::ComputeFrameWorkNumber( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	RECT		rectFrame;

	GetStaffViewSize (pDC, pInfo, parmsView, &rectFrame );

	long			iMax;

	iMax = rectFrame.bottom - parmsView->m_iTopMargin - parmsView->m_iBottomMargin;
	if ( iMax <= 0 )
	{
		return 0;
	}

	return iMax / FrameWorkHeight (pDC, pInfo, parmsView);

}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawMesureText(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iCy, const char *pText)
{
	pDC->TextOut ( iCx, 
		iCy + 8 * parmsView->m_iNoteHeight + parmsView->m_iSpaceBetweenLowHigh + parmsView->m_iSpaceBetweenStaff / 3, pText );
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::ForceToRedraw(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	RECT		rectFrame;

	GetStaffViewSize (pDC, pInfo, parmsView, &rectFrame );

	InvalidateRect ( &rectFrame, TRUE );
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawTitle(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		SetAnsiVarFont ( pDC, pInfo, parmsView );
		pDC->TextOut ( parmsView->m_iLeftMargin, parmsView->m_iTextHeight, pMididoc->GetPathName ( ) );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawTrackName(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	CMidiTrack			*pMiditrack;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		pMiditrack = 
			pMidifile->GetMidiTrackAddress ( pMididoc->GetCurrentTrack () );
		if ( pMiditrack != NULL )
		{
			char		*pText = pMiditrack->GetTrackName ( );
			if ( pText != NULL )
			{
				SetAnsiVarFont ( pDC, pInfo, parmsView );
				pDC->TextOut ( parmsView->m_iLeftMargin, parmsView->m_iTextHeight * 3, pText );
			}
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
CFont *CStaffView::SetAnsiVarFont( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView )
{
	if ( parmsView->m_pFont != NULL )
	{
		if ( pDC != NULL )
		{
			pDC->SelectObject ( parmsView->m_pFont );
		}
		return parmsView->m_pFont;
	}

	return CreateAnsiVarFont( pDC, pInfo, parmsView );
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
CFont *CStaffView::CreateAnsiVarFont( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView )
{
	CFont		*pFont;
	LOGFONT		logFont;

	if ( pDC != NULL )
	{
		pFont = GetFont ( );
		if ( pFont != NULL )
		{
			pFont->GetLogFont ( & logFont );
		}
	}

	if ( parmsView->m_pFont != NULL )
	{
		delete parmsView->m_pFont;
		parmsView->m_pFont	= NULL;
	}

	logFont.lfHeight			= parmsView->m_iTextHeight;
	logFont.lfWidth				= parmsView->m_iTextWidth;
	logFont.lfEscapement		= 0;
    logFont.lfOrientation		= 0;
	logFont.lfWeight			= FW_NORMAL;
	logFont.lfItalic			= FALSE;
	logFont.lfUnderline			= FALSE;
    logFont.lfStrikeOut			= FALSE;
    logFont.lfCharSet			= DEFAULT_CHARSET;
	logFont.lfOutPrecision		= OUT_DEFAULT_PRECIS;
    logFont.lfClipPrecision		= CLIP_DEFAULT_PRECIS;
	logFont.lfPitchAndFamily	= VARIABLE_PITCH | FF_SWISS;
    logFont.lfQuality			= DEFAULT_QUALITY; 
	strcpy_s ( logFont.lfFaceName, sizeof ( logFont.lfFaceName ), "MS Sans Serif" ); 

	parmsView->m_pFont = new CFont ( );
	( parmsView->m_pFont ) ->CreateFontIndirect ( &logFont );

	if ( pDC != NULL )
	{
		pDC->SelectObject ( parmsView->m_pFont );
	}

	return parmsView->m_pFont;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::GetStaffViewSize(CDC *pDC, CPrintInfo *pInfo, CStaffViewParms *parmsView, RECT *pRect)
{
	pRect->top		= 0;
	pRect->left		= 0;
	pRect->right	= parmsView->m_iFrameWidth;
	pRect->bottom	= parmsView->m_iFrameHeight;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::ComputeAllSize(CDC *pDC, CPrintInfo *pInfo, CStaffViewParms *parmsView, long iHeight)
{
	//		The height of a text
	//		Space between lines
	parmsView->m_iNoteHeight				= iHeight;

	//
	//		Pen width
	parmsView->m_iLightPenWidth				= 1;
	if ( parmsView->m_iLightPenWidth == 0 )
	{
		parmsView->m_iLightPenWidth			= 1;
	}

	parmsView->m_iNormalPenWidth			= parmsView->m_iNoteHeight / 8;
	if ( parmsView->m_iNormalPenWidth == 0 )
	{
		parmsView->m_iNormalPenWidth		= 1;
	}

	parmsView->m_iStrongPenWidth			= parmsView->m_iNoteHeight / 3;
	if ( parmsView->m_iStrongPenWidth == 0 )
	{
		parmsView->m_iStrongPenWidth		= 1;
	}

	parmsView->m_iHeavyPenWidth				= parmsView->m_iNoteHeight / 2;
	if ( parmsView->m_iHeavyPenWidth == 0 )
	{
		parmsView->m_iHeavyPenWidth			= 1;
	}

	//
	//		The Text height
	parmsView->m_iTextHeight				= parmsView->m_iNoteHeight * 2;

	//		The width of a chararcter
	parmsView->m_iTextWidth					= 5 * parmsView->m_iTextHeight / 8;

	//		The deformation of the note
	parmsView->m_iNoteMul					= 5;
	parmsView->m_iNoteDiv					= 3;

	//		The queue for black
	parmsView->m_iNoteQueue					= 3 * parmsView->m_iNoteHeight;

	//		The minimum size between note when duration are different
	parmsView->m_iMinColumnSpace			= 2;

	//		Note width
	parmsView->m_iNoteWidth					= 
		( parmsView->m_iNoteMul * parmsView->m_iNoteHeight ) / parmsView->m_iNoteDiv;

	//		The width of a dot.
	parmsView->m_iDotWidth					= parmsView->m_iNoteWidth / 3;

	//		Space between columns
	parmsView->m_iRondeSpace				= 16 * parmsView->m_iNoteWidth;

	//		Space between columns
	parmsView->m_iWhiteSpace				= 8 * parmsView->m_iNoteWidth;

	//		Space between columns
	parmsView->m_iBlackSpace				= parmsView->m_iNoteWidth + 1;

	//		Space between columns
	parmsView->m_iCrocheSpace				= parmsView->m_iNoteWidth + 1;

	//		Space between columns
	parmsView->m_iDoubleCrocheSpace			= parmsView->m_iNoteWidth + 1;
	
	//		Space between columns
	parmsView->m_iTripleCrocheSpace			= parmsView->m_iNoteWidth + 1;

	//		Left Margin
	parmsView->m_iLeftMargin				= parmsView->m_iTextWidth * 10;

	//		Right Margin
	parmsView->m_iRightMargin				= parmsView->m_iLeftMargin;

	//		Top Margin
	parmsView->m_iTopMargin					= parmsView->m_iNoteHeight * 20;

	//		Bottom Margin
	parmsView->m_iBottomMargin				= parmsView->m_iTopMargin;

	//		Space between mesure
	parmsView->m_iMesureSpace				= 4 * parmsView->m_iNoteWidth;

	//		Spaces between double bars
	parmsView->m_iKeySignatureSpace			= parmsView->m_iNoteWidth * 10;

	//		Spaces between double bars
	parmsView->m_iDoubleBarsSpace			= 4;

	//		Space between Low and High Staff
	parmsView->m_iSpaceBetweenLowHigh		= parmsView->m_iNoteHeight * 4;

	//		Space between staff
	parmsView->m_iSpaceBetweenStaff			= parmsView->m_iNoteHeight * 14;

	//		Tolerance for the notes
	parmsView->m_iNoteTolerance				= 6;

	//		Set the frame width and height
	if ( pInfo != NULL )
	{
		parmsView->m_iFrameWidth	= pInfo->m_rectDraw.right	- pInfo->m_rectDraw.left;
		parmsView->m_iFrameHeight	= pInfo->m_rectDraw.bottom	- pInfo->m_rectDraw.top;
	}
	else
	{
		//		Default five lines
		parmsView->m_iFrameWidth	= 5 * 512;
		parmsView->m_iFrameHeight	= ComputeEndOfLine ( pDC, pInfo, parmsView, 5 );
	}

	//		Create the font if necessary
	//
	CreateAnsiVarFont ( pDC, pInfo, parmsView );

	//		Create the pens
	CreateLightPen ( pDC, pInfo, parmsView );
	CreateNormalPen ( pDC, pInfo, parmsView );
	CreateStrongPen ( pDC, pInfo, parmsView );
	CreateHeavyPen ( pDC, pInfo, parmsView );

}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
CPen *CStaffView::CreateStrongPen( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	if ( parmsView->m_pStrongPen != NULL )
	{
		delete parmsView->m_pStrongPen;
		parmsView->m_pStrongPen = NULL;
	}

	parmsView->m_pStrongPen = new CPen ( );
	( parmsView->m_pStrongPen )->CreatePen ( PS_SOLID, parmsView->m_iStrongPenWidth, COLORREF ( 0x00000000 ) );
	if ( pDC != NULL )
	{
		pDC->SelectObject ( parmsView->m_pStrongPen );
	}

	return parmsView->m_pStrongPen;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
CPen *CStaffView::SetStrongPen( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	if ( parmsView->m_pStrongPen != NULL )
	{
		if ( pDC != NULL )
		{
			pDC->SelectObject ( parmsView->m_pStrongPen );
		}
		return parmsView->m_pStrongPen;
	}

	return CreateStrongPen( pDC, pInfo, parmsView);
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
CPen *CStaffView::CreateNormalPen(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	if ( parmsView->m_pNormalPen != NULL )
	{
		delete parmsView->m_pNormalPen;
		parmsView->m_pNormalPen = NULL;
	}

	parmsView->m_pNormalPen = new CPen ( );
	( parmsView->m_pNormalPen )->CreatePen ( PS_SOLID, parmsView->m_iNormalPenWidth, COLORREF ( 0x00000000 ) );

	if ( pDC != NULL )
	{
		pDC->SelectObject ( parmsView->m_pNormalPen );
	}

	return parmsView->m_pNormalPen;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
CPen *CStaffView::SetNormalPen(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	if ( parmsView->m_pNormalPen != NULL )
	{
		if ( pDC != NULL )
		{
			pDC->SelectObject ( parmsView->m_pNormalPen );
		}

		return parmsView->m_pNormalPen;
	}

	return CreateNormalPen( pDC, pInfo, parmsView );
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
CPen *CStaffView::CreateLightPen(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	if ( parmsView->m_pLightPen != NULL )
	{
		delete parmsView->m_pLightPen;
		parmsView->m_pLightPen = NULL;
	}

	parmsView->m_pLightPen = new CPen ( );
	( parmsView->m_pLightPen )->CreatePen ( PS_SOLID, parmsView->m_iLightPenWidth, COLORREF ( 0x00000000 ) );

	if ( pDC != NULL )
	{
		pDC->SelectObject ( parmsView->m_pLightPen );
	}

	return parmsView->m_pLightPen;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
CPen *CStaffView::SetLightPen(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	if ( parmsView->m_pLightPen != NULL )
	{
		if ( pDC != NULL )
		{
			pDC->SelectObject ( parmsView->m_pLightPen );
		}

		return parmsView->m_pLightPen;
	}

	return CreateLightPen( pDC, pInfo, parmsView );
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
CPen *CStaffView::CreateHeavyPen(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	if ( parmsView->m_pHeavyPen != NULL )
	{
		delete parmsView->m_pHeavyPen;
		parmsView->m_pHeavyPen = NULL;
	}

	parmsView->m_pHeavyPen = new CPen ( );
	( parmsView->m_pHeavyPen )->CreatePen ( PS_SOLID, parmsView->m_iHeavyPenWidth, COLORREF ( 0x00000000 ) );

	if ( pDC != NULL )
	{
		pDC->SelectObject ( parmsView->m_pHeavyPen );
	}

	return parmsView->m_pHeavyPen;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
CPen *CStaffView::SetHeavyPen(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView)
{
	if ( parmsView->m_pHeavyPen != NULL )
	{
		if ( pDC != NULL )
		{
			pDC->SelectObject ( parmsView->m_pHeavyPen );
		}

		return parmsView->m_pHeavyPen;
	}

	return CreateHeavyPen( pDC, pInfo, parmsView );
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawUpSquare(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iLine)
{

	int			iCy;

	iCy = ComputeCy ( pDC, pInfo, parmsView, iLine );

	//		Draw Top square
	pDC->SelectStockObject ( BLACK_BRUSH );
	SetNormalPen ( pDC, pInfo, parmsView );

	pDC->MoveTo ( iCx, iCy + parmsView->m_iNoteHeight );
	pDC->Rectangle ( iCx, iCy + parmsView->m_iNoteHeight, iCx + parmsView->m_iNoteWidth, iCy + 3 * parmsView->m_iNoteHeight / 2 );

	//		Draw Bottom square
	iCy = iCy + parmsView->m_iSpaceBetweenLowHigh + 4 * parmsView->m_iNoteHeight;

	pDC->MoveTo ( iCx, iCy + parmsView->m_iNoteHeight );
	pDC->Rectangle ( iCx, iCy + parmsView->m_iNoteHeight, iCx + parmsView->m_iNoteWidth, iCy + 3 * parmsView->m_iNoteHeight / 2  );
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawDownSquare(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iLine)
{

	int			iCy;

	iCy = ComputeCy ( pDC, pInfo, parmsView, iLine );

	//		Draw Top square
	pDC->SelectStockObject ( BLACK_BRUSH );
	SetNormalPen ( pDC, pInfo, parmsView );

	pDC->MoveTo ( iCx, iCy + parmsView->m_iNoteHeight );
	pDC->Rectangle ( iCx, iCy + 3 * parmsView->m_iNoteHeight / 2, iCx + parmsView->m_iNoteWidth, iCy + 2 * parmsView->m_iNoteHeight );

	//		Draw Bottom square
	iCy = iCy + parmsView->m_iSpaceBetweenLowHigh + 4 * parmsView->m_iNoteHeight;

	pDC->MoveTo ( iCx, iCy + parmsView->m_iNoteHeight );
	pDC->Rectangle ( iCx, iCy + 3 * parmsView->m_iNoteHeight / 2, iCx + parmsView->m_iNoteWidth, iCy + 2 * parmsView->m_iNoteHeight  );
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawZigZag(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iLine)
{

	int			iCy;

	iCy = ComputeCy ( pDC, pInfo, parmsView, iLine );

	//		Draw Top square
	SetNormalPen ( pDC, pInfo, parmsView );

	iCy += parmsView->m_iNoteHeight;
	pDC->MoveTo ( iCx, iCy );
	iCx += parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;
	pDC->LineTo ( iCx, iCy );
	iCx -= parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;
	pDC->LineTo ( iCx, iCy );
	iCx += parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;
	pDC->LineTo ( iCx, iCy );

	//		Draw Bottom square
	iCy = iCy + parmsView->m_iSpaceBetweenLowHigh;
	iCx	-= parmsView->m_iNoteWidth;

	pDC->MoveTo ( iCx, iCy );
	iCy += parmsView->m_iNoteHeight;
	pDC->MoveTo ( iCx, iCy );
	iCx += parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;
	pDC->LineTo ( iCx, iCy );
	iCx -= parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;
	pDC->LineTo ( iCx, iCy );
	iCx += parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;
	pDC->LineTo ( iCx, iCy );

}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawSlashOne(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iLine)
{

	int			iCy;

	iCy = ComputeCy ( pDC, pInfo, parmsView, iLine );

	//		Draw a round
	DrawDot ( pDC, pInfo, parmsView, iCx, iCy + parmsView->m_iNoteHeight );

	//		Draw Top square
	SetNormalPen ( pDC, pInfo, parmsView );

	iCy += parmsView->m_iNoteHeight;
	iCx += parmsView->m_iNoteWidth;
	pDC->MoveTo ( iCx, iCy );
	iCx -= parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;
	iCy += parmsView->m_iNoteHeight;
	pDC->LineTo ( iCx, iCy );
	iCx += parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;

	//		Draw Bottom square
	iCy = iCy + parmsView->m_iSpaceBetweenLowHigh;
	iCx	-= parmsView->m_iNoteWidth;

	//		Draw a round
	DrawDot ( pDC, pInfo, parmsView, iCx, iCy + parmsView->m_iNoteHeight );

	//		Draw Top square
	SetNormalPen ( pDC, pInfo, parmsView );

	iCy += parmsView->m_iNoteHeight;
	iCx += parmsView->m_iNoteWidth;
	pDC->MoveTo ( iCx, iCy );
	iCx -= parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;
	iCy += parmsView->m_iNoteHeight;
	pDC->LineTo ( iCx, iCy );
	iCx += parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;

}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawSlashTwo(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iLine)
{

	int			iCy;

	iCy = ComputeCy ( pDC, pInfo, parmsView, iLine );

	//		Draw a round
	DrawDot ( pDC, pInfo, parmsView, iCx, iCy + parmsView->m_iNoteHeight );

	//		Draw a round
	DrawDot ( pDC, pInfo, parmsView, iCx - parmsView->m_iNoteWidth / 3, iCy + 3 * parmsView->m_iNoteHeight / 2 );

	SetNormalPen ( pDC, pInfo, parmsView );

	iCy += parmsView->m_iNoteHeight;
	iCx += parmsView->m_iNoteWidth;
	pDC->MoveTo ( iCx, iCy );
	iCx -= parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;
	iCy += parmsView->m_iNoteHeight;
	pDC->LineTo ( iCx, iCy );
	iCx += parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;

	//		Draw Bottom square
	iCy = iCy + parmsView->m_iSpaceBetweenLowHigh;
	iCx	-= parmsView->m_iNoteWidth;

	//		Draw a round
	DrawDot ( pDC, pInfo, parmsView, iCx, iCy + parmsView->m_iNoteHeight );

	//		Draw a round
	DrawDot ( pDC, pInfo, parmsView, iCx - parmsView->m_iNoteWidth / 3, iCy + 3 * parmsView->m_iNoteHeight / 2 );

	SetNormalPen ( pDC, pInfo, parmsView );

	iCy += parmsView->m_iNoteHeight;
	iCx += parmsView->m_iNoteWidth;
	pDC->MoveTo ( iCx, iCy );
	iCx -= parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;
	iCy += parmsView->m_iNoteHeight;
	pDC->LineTo ( iCx, iCy );
	iCx += parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;

}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawSlashThree(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iLine)
{

	int			iCy;

	iCy = ComputeCy ( pDC, pInfo, parmsView, iLine );

	//		Draw a round
	DrawDot ( pDC, pInfo, parmsView, iCx, iCy + parmsView->m_iNoteHeight );

	//		Draw a round
	DrawDot ( pDC, pInfo, parmsView, iCx - parmsView->m_iNoteWidth / 3, iCy + 3 * parmsView->m_iNoteHeight / 2 );

	//		Draw a round
	DrawDot ( pDC, pInfo, parmsView, iCx - 2 * parmsView->m_iNoteWidth / 3, iCy + 2 * parmsView->m_iNoteHeight);

	SetNormalPen ( pDC, pInfo, parmsView );

	iCy += parmsView->m_iNoteHeight;
	iCx += parmsView->m_iNoteWidth;
	pDC->MoveTo ( iCx, iCy );
	iCx -= parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;
	iCy += parmsView->m_iNoteHeight;
	pDC->LineTo ( iCx, iCy );
	iCx += parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;

	//		Draw Bottom square
	iCy = iCy + parmsView->m_iSpaceBetweenLowHigh;
	iCx	-= parmsView->m_iNoteWidth;

	//		Draw a round
	DrawDot ( pDC, pInfo, parmsView, iCx, iCy + parmsView->m_iNoteHeight );

	//		Draw a round
	DrawDot ( pDC, pInfo, parmsView, iCx - parmsView->m_iNoteWidth / 3, iCy + 3 * parmsView->m_iNoteHeight / 2 );

	//		Draw a round
	DrawDot ( pDC, pInfo, parmsView, iCx - 2 * parmsView->m_iNoteWidth / 3, iCy + 2 * parmsView->m_iNoteHeight);

	SetNormalPen ( pDC, pInfo, parmsView );

	iCy += parmsView->m_iNoteHeight;
	iCx += parmsView->m_iNoteWidth;
	pDC->MoveTo ( iCx, iCy );
	iCx -= parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;
	iCy += parmsView->m_iNoteHeight;
	pDC->LineTo ( iCx, iCy );
	iCx += parmsView->m_iNoteWidth;
	iCy += parmsView->m_iNoteHeight;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawBemol(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iCy)
{
	SetNormalPen ( pDC, pInfo, parmsView );

	iCx = iCx - 2 * parmsView->m_iNoteWidth / 3;
	iCy = iCy - parmsView->m_iNoteHeight / 2;
	pDC->MoveTo ( iCx, iCy - 2 * parmsView->m_iNoteHeight );
	pDC->LineTo ( iCx, iCy + parmsView->m_iNoteHeight );
	pDC->Arc (	iCx - parmsView->m_iNoteWidth, iCy,
				iCx + 2 * parmsView->m_iNoteWidth / 3, iCy + parmsView->m_iNoteHeight, 
				iCx, iCy + parmsView->m_iNoteHeight,
				iCx, iCy );

}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::ComputeCxAndLine(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long &iCx, long &iLine, long iAddCx)
{
	iCx += iAddCx;
	if ( iCx > RightEdge ( pDC, pInfo, parmsView ) )
	{
		AddNewFrameWork(pDC, pInfo, parmsView, iCx, iLine);
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::AddNewFrameWork(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long &iCx, long &iLine)
{
	//		For the printer or preview
	if ( pInfo != NULL )
	{
		if ( iLine % ComputeFrameWorkNumber (pDC, pInfo, parmsView) + 1 
				>= ComputeFrameWorkNumber (pDC, pInfo, parmsView) )
		{
			//	End the page
			pDC->EndPage ( );

			//	Start new page
			pDC->StartPage ( );
		}
	}

	iLine++;

	if ( TestLineInPage ( pDC, pInfo, parmsView, iLine ) )
	{
		DrawOneFrameWork ( pDC, pInfo, parmsView, ComputeCy ( pDC, pInfo, parmsView, iLine ) );
	}

	iCx = parmsView->m_iLeftMargin + parmsView->m_iMesureSpace / 2;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
bool CStaffView::TestLineInPage(CDC *pDC, CPrintInfo *pInfo, CStaffViewParms *parmsView,
								long iLine)
{
	//	If printer or preview
	if ( pInfo != NULL )
	{
		unsigned iPage =
			( iLine / ComputeFrameWorkNumber (pDC, pInfo, parmsView) ) + 1;
		
		//		For preview
		if ( pInfo->m_bPreview )
		{
			if ( iPage == pInfo->m_nCurPage )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		//		For the printer always in the page
		else
		{
			return true;
		}
	}

	//		For the screen only the first page
	else
	{
		unsigned iPage = 
			( iLine / ComputeFrameWorkNumber (pDC, pInfo, parmsView) );
		if ( iPage == 0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::ComputeSizeForScreen(CDC *pDC, CPrintInfo *pInfo, CStaffViewParms *parmsView)
{
	ComputeAllSize ( pDC, pInfo, parmsView, 8 );
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::ComputeSizeForPrinter(CDC *pDC, CPrintInfo *pInfo, CStaffViewParms *parmsView)
{
	ComputeAllSize ( pDC, pInfo, parmsView, 20 );
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DisplaySilences(CDC *pDC, CPrintInfo *pInfo, CStaffViewParms *parmsView, long &iCx, long &iLine, int iBeat)
{
	int					iX;
	int					iCount;

	iCount = 0;
	for ( iX = 0; iX < iBeat; iX++ )
	{
		if ( ! m_cMesureHiTable [ iX ] )
		{
			iCount++;
			m_cMesureHiTable [ iX ] = 1;

		}
		else
		{
			DisplaySilencesForCount(pDC, pInfo, parmsView, iCx, iLine, iCount);
			iCount = 0;
		}
	}

	DisplaySilencesForCount(pDC, pInfo, parmsView, iCx, iLine, iCount);
	iCount = 0;

}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DisplaySilencesForCount(CDC *pDC, CPrintInfo *pInfo, CStaffViewParms *parmsView, long &iCx, long &iLine, int iCount)
{
	while ( iCount > 32 )
	{

		ComputeCxAndLine ( pDC, pInfo, parmsView, iCx, iLine, parmsView->m_iBlackSpace );

		if ( TestLineInPage ( pDC, pInfo, parmsView, iLine ) )
		{
			DrawDownSquare ( pDC, pInfo, parmsView, iCx, iLine );
		}

		ComputeCxAndLine ( pDC, pInfo, parmsView, iCx, iLine, parmsView->m_iBlackSpace );

		iCount -= 32;

	}

	while ( iCount > 16 )
	{

		ComputeCxAndLine ( pDC, pInfo, parmsView, iCx, iLine, parmsView->m_iBlackSpace );

		if ( TestLineInPage ( pDC, pInfo, parmsView, iLine ) )
		{
			DrawDownSquare ( pDC, pInfo, parmsView, iCx, iLine );
		}

		ComputeCxAndLine ( pDC, pInfo, parmsView, iCx, iLine, parmsView->m_iBlackSpace );

		iCount -= 16;

	}

	while ( iCount > 8 )
	{
		ComputeCxAndLine ( pDC, pInfo, parmsView, iCx, iLine, parmsView->m_iBlackSpace );

		if ( TestLineInPage ( pDC, pInfo, parmsView, iLine ) )
		{
			DrawZigZag ( pDC, pInfo, parmsView, iCx, iLine );
		}

		ComputeCxAndLine ( pDC, pInfo, parmsView, iCx, iLine, parmsView->m_iBlackSpace );

		iCount -= 8;

	}

	while ( iCount > 4 )
	{
		ComputeCxAndLine ( pDC, pInfo, parmsView, iCx, iLine, parmsView->m_iBlackSpace );

		if ( TestLineInPage ( pDC, pInfo, parmsView, iLine ) )
		{
			DrawSlashOne ( pDC, pInfo, parmsView, iCx, iLine );
		}

		ComputeCxAndLine ( pDC, pInfo, parmsView, iCx, iLine, parmsView->m_iBlackSpace );

		iCount -= 4;

	}

	while ( iCount > 2 )
	{
		ComputeCxAndLine ( pDC, pInfo, parmsView, iCx, iLine, parmsView->m_iBlackSpace );

		if ( TestLineInPage ( pDC, pInfo, parmsView, iLine ) )
		{
			DrawSlashTwo ( pDC, pInfo, parmsView, iCx, iLine );
		}

		ComputeCxAndLine ( pDC, pInfo, parmsView, iCx, iLine, parmsView->m_iBlackSpace );

		iCount -= 2;

	}

	while ( iCount > 1 )
	{
		ComputeCxAndLine ( pDC, pInfo, parmsView, iCx, iLine, parmsView->m_iBlackSpace );

		iCx += ( parmsView->m_iBlackSpace / 2 );

		if ( TestLineInPage ( pDC, pInfo, parmsView, iLine ) )
		{
			DrawSlashThree ( pDC, pInfo, parmsView, iCx, iLine );
		}

		ComputeCxAndLine ( pDC, pInfo, parmsView, iCx, iLine, parmsView->m_iBlackSpace );

		iCount--;

	}

}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawDot(CDC *pDC, CPrintInfo *pInfo, CStaffViewParms *parmsView, long iCx, long iCy)
{
	SetNormalPen ( pDC, pInfo, parmsView );

	pDC->SelectStockObject ( BLACK_BRUSH );

	Round ( pDC, iCx, iCy, iCx + parmsView->m_iDotWidth, iCy + parmsView->m_iDotWidth );
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::Round ( CDC *pDC, long iCx1, long iCy1, long iCx2, long iCy2 )
{
//	pDC->RoundRect ( iCx1, iCy1, iCx2, iCy2, iCx3, iCy3 );
	pDC->Ellipse ( iCx1, iCy1, iCx2, iCy2 );
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO
	CRect		rect;
	GetClientRect ( &rect );
	ClientToScreen ( &rect );

	if ( rect.PtInRect ( point ) )
	{
		CMenu		menu;
		menu.LoadMenu ( IDR_MENU_STAFF );
		CMenu	*pContextMenu = menu.GetSubMenu ( 0 );
		pContextMenu->TrackPopupMenu (
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, 
			point.x, point.y, this );
		return;
	}
	
	
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::OnStaffHelp() 
{
	// TODO
	CAboutDlg	dlg;
	dlg.DoModal ();
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::SetViewParameters()
{
	ComputeSizeForScreen ( NULL, NULL, &m_ScreenParms );

	CSize sizeTotal;
	sizeTotal.cx	= m_ScreenParms.m_iFrameWidth;
	sizeTotal.cy	= m_ScreenParms.m_iFrameHeight;
	SetScrollSizes(MM_TEXT, sizeTotal);

	if ( m_bScale )
	{
		sizeTotal = GetTotalSize ( );

		SetScaleToFitSize ( sizeTotal );
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::OnStaffScale() 
{
	// TODO
	CSize sizeTotal;

	m_bScale = ! m_bScale;

	if ( m_bScale )
	{
		sizeTotal = GetTotalSize ( );
		SetScaleToFitSize ( sizeTotal );
	}
	else
	{
		sizeTotal.cx	= m_ScreenParms.m_iFrameWidth;
		sizeTotal.cy	= m_ScreenParms.m_iFrameHeight;
		SetScrollSizes(MM_TEXT, sizeTotal);
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::DrawAdditionalFrame(CDC *pDC, CPrintInfo *pInfo, CStaffViewParms *parmsView, long iCx, long iLine, CMidiList *pList)
{
	long			iCy;
	int				iX;
	unsigned char	cNote;
	unsigned int	cHeight;
	int				iLevel;
	int				iMax;

	cNote		= pList->GetValue1 ( );
	cHeight		= pList->GetHeight ( cNote );
	iLevel		= cNote / 12;

	iCy = ComputeCy (pDC, pInfo, parmsView, iLine );

	SetNormalPen ( pDC, pInfo, parmsView );

	//		Draw the framework for High
	if ( cNote == NOTE_TREBLE_DOWN )
	{
		iX = 5;
		pDC->MoveTo ( iCx - parmsView->m_iNoteWidth / 2, iCy + iX * parmsView->m_iNoteHeight );
		pDC->LineTo ( iCx + 3 * parmsView->m_iNoteWidth / 2, iCy + iX * parmsView->m_iNoteHeight );

		return;
	}
	
	if ( cNote >= NOTE_TREBLE_UP )
	{
		iMax = ( NOTE_TREBLE_UP - cNote ) / 2 - 1;

		for ( iX = iMax; iX < 0; iX++ )
		{
			pDC->MoveTo ( iCx - parmsView->m_iNoteWidth / 2, iCy + iX * parmsView->m_iNoteHeight );
			pDC->LineTo ( iCx + 3 * parmsView->m_iNoteWidth / 2, iCy + iX * parmsView->m_iNoteHeight );
		}

		return;
	}
	
	//		Draw Low
	iCy = iCy + parmsView->m_iSpaceBetweenLowHigh + 4 * parmsView->m_iNoteHeight;

	if ( cNote <= NOTE_BASS_DOWN )
	{

		iMax = ( NOTE_BASS_DOWN - cNote ) / 2 + 1;

		for ( iX = 5; iX < 5 + iMax; iX++ )
		{
			pDC->MoveTo ( iCx - parmsView->m_iNoteWidth / 2, iCy + iX * parmsView->m_iNoteHeight );
			pDC->LineTo ( iCx + 3 * parmsView->m_iNoteWidth / 2, iCy + iX * parmsView->m_iNoteHeight );
		}

		return;
	}
	
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO
	FriendActivate( bActivate, pActivateView, pDeactiveView, true);
	
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::OnMove(int x, int y)
{
	CScrollView::OnMove(x, y);

}

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CStaffView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

}
