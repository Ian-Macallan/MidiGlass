#include "stdafx.h"
#include "WaitTreatment.h"

#include "MidiGlassApp.h"

extern CMidiWorksApp theApp;

//
//////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////
int CWaitTreatment::m_count = 0;

//
//////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////
CWaitTreatment::CWaitTreatment(void)
{
}


//
//////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////
CWaitTreatment::~CWaitTreatment(void)
{
}

//
//////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////
void CWaitTreatment::BeginWait()
{
	if ( m_count <= 0 )
	{
		theApp.BeginWaitCursor();
	}
	m_count++;
}

//
//////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////
void CWaitTreatment::EndWait()
{
	if (  m_count == 1 )
	{
		theApp.EndWaitCursor();
	}
	m_count--;
	if ( m_count < 0 )
	{
		m_count = 0;
	}
}
