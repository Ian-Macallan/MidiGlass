// STAFFVIEWPARMS.h: interface for the CStaffViewParms class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STAFFVIEWPARMS_H__3E8D9185_ED1F_11D2_8148_444553540000__INCLUDED_)
#define AFX_STAFFVIEWPARMS_H__3E8D9185_ED1F_11D2_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
class CStaffViewParms  
{
	public:
		CStaffViewParms();
		virtual ~CStaffViewParms();

		//		Font used
		CFont				* m_pFont;

		//		Pens used
		CPen				* m_pLightPen;
		CPen				* m_pNormalPen;
		CPen				* m_pStrongPen;
		CPen				* m_pHeavyPen;

		//		The frame height
		long				m_iFrameHeight;
	
		//		The frame width
		long				m_iFrameWidth;
	
		long				m_iNoteHeight;

		long				m_iTextHeight;

		//		The Pens Width
		long				m_iLightPenWidth;
		long				m_iNormalPenWidth;
		long				m_iStrongPenWidth;
		long				m_iHeavyPenWidth;

		//		The width of a chararcter
		long				m_iTextWidth;

		//		The deformation of the note
		long				m_iNoteMul;
		long				m_iNoteDiv;

		//		The queue for black
		long				m_iNoteQueue;

		//		The minimum size between note when duration are different
		long				m_iMinColumnSpace;

		//		Note width
		long				m_iNoteWidth;

		//		Dot width
		long				m_iDotWidth;

		//		Space between columns
		long				m_iRondeSpace;

		//		Space between columns
		long				m_iWhiteSpace;

		//		Space between columns
		long				m_iBlackSpace;

		//		Space between columns
		long				m_iCrocheSpace;

		//		Space between columns
		long				m_iDoubleCrocheSpace;

		//		Space between columns
		long				m_iTripleCrocheSpace;

		//		Left Margin
		long				m_iLeftMargin;

		//		Right Margin
		long				m_iRightMargin;

		//		Top Margin
		long				m_iTopMargin;

		//		Bottom Margin
		long				m_iBottomMargin;

		//		Space between mesure
		long				m_iMesureSpace;

		//		Spaces between double bars
		long				m_iKeySignatureSpace;

		//		Spaces between double bars
		long				m_iDoubleBarsSpace;

		//		Space between Low and High Staff
		long				m_iSpaceBetweenLowHigh;

		//		Space between staff
		long				m_iSpaceBetweenStaff;

		//		Tolerance for the notes
		unsigned long		m_iNoteTolerance;


};

#endif // !defined(AFX_STAFFVIEWPARMS_H__3E8D9185_ED1F_11D2_8148_444553540000__INCLUDED_)
