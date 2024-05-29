#pragma once

#include "AudioLoopback.h"

//
///////////////////////////////////////////////////////////////////////////////////
// CAudioLoopbackThread
//
///////////////////////////////////////////////////////////////////////////////////
class CAudioLoopbackThread : public CWinThread
{
	DECLARE_DYNCREATE(CAudioLoopbackThread)

	protected:
		CAudioLoopbackThread();           // constructeur prot�g� utilis� par la cr�ation dynamique
		virtual ~CAudioLoopbackThread();

	public:
		virtual BOOL InitInstance();
		virtual int ExitInstance();

	protected:
		DECLARE_MESSAGE_MAP()
};


