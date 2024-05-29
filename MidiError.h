//
////////////////////////////////////////////////////////////////////////
// MidiError.h: interface for the CMidiError class.
//
////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDIERROR_H__8718BB64_BD3F_11D2_8147_444553540000__INCLUDED_)
#define AFX_MIDIERROR_H__8718BB64_BD3F_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
class CMidiError : public CObject
{
		DECLARE_DYNAMIC(CMidiError)

	protected:
		bool		m_MidiError;
		char		*m_FullErrorText;
		char		*m_LastErrorText;
		size_t		m_FullErrorLen;
		size_t		m_LastErrorLen;

	public:
		CMidiError();
		virtual ~CMidiError();

		void Free();

		virtual void SetMidiSuccess();
		virtual void SetMidiError();

		virtual const char *GetLastErrorText() const;
		virtual const char *GetFullErrorText() const;

		virtual void SetMidiErrorText( const char *pText );

		virtual bool MidiSuccess();
		virtual bool MidiError();

};

#endif // !defined(AFX_MIDIERROR_H__8718BB64_BD3F_11D2_8147_444553540000__INCLUDED_)
