#include "stdafx.h"
#include "MidiGlassApp.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static	char szRegisteredActions [ TOOLS_NUMBER ][ MAX_STRING ];

void CMainFrame::OnToolsTool1() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL1 );
}

void CMainFrame::OnToolsTool2() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL2 );
	
}

void CMainFrame::OnToolsTool3() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL3 );
	
}

void CMainFrame::OnToolsTool4() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL4 );
	
}

void CMainFrame::OnToolsTool5() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL5 );
	
}

void CMainFrame::OnToolsTool6() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL6 );
	
}

void CMainFrame::OnToolsTool7() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL7 );
	
}

void CMainFrame::OnToolsTool8() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL8 );
	
}

void CMainFrame::OnToolsTool9() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL9 );	
}

void CMainFrame::OnToolsTool10() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL10 );	
}

void CMainFrame::OnToolsTool11() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL11 );	
}

void CMainFrame::OnToolsTool12() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL12 );	
}

void CMainFrame::OnToolsTool13() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL13 );	
}

void CMainFrame::OnToolsTool14() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL14 );	
}

void CMainFrame::OnToolsTool15() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL15 );	
}

void CMainFrame::OnToolsTool16() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL16 );	
}

void CMainFrame::OnToolsTool17() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL17 );
	
}

void CMainFrame::OnToolsTool18() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL18 );
	
}

void CMainFrame::OnToolsTool19() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL19 );
	
}

void CMainFrame::OnToolsTool20() 
{
	// TODO
	OnToolsToolNumber ( ID_TOOLS_TOOL20 );
	
}

void CMainFrame::OnToolsToolNumber(UINT iID)
{
	CMidiDoc			*pMididoc	= GetMidiDoc ();

	HINSTANCE			hInst;

	if ( pMididoc == NULL )
	{
		return;
	}

	//	First Get The Menu
	CMenu	* pMenu		= GetMenu ();
	if ( pMenu == NULL )
	{
		return;
	}

	char		szAction [ MAX_REGISTRY_STRING ];
	char		szMenu [ MAX_MENU_STRING ];
	pMenu->GetMenuString ( iID, szMenu, sizeof ( szMenu ), MF_BYCOMMAND );

	switch ( iID )
	{
		case ID_TOOLS_TOOL1 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 0 ] );
			break;
		}
		case ID_TOOLS_TOOL2 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 1 ] );
			break;
		}
		case ID_TOOLS_TOOL3 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 2 ] );
			break;
		}
		case ID_TOOLS_TOOL4 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 3 ] );
			break;
		}
		case ID_TOOLS_TOOL5 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 4 ] );
			break;
		}
		case ID_TOOLS_TOOL6 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 5 ] );
			break;
		}
		case ID_TOOLS_TOOL7 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 6 ] );
			break;
		}
		case ID_TOOLS_TOOL8 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 7 ] );
			break;
		}
		case ID_TOOLS_TOOL9 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 8 ] );
			break;
		}
		case ID_TOOLS_TOOL10 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 9 ] );
			break;
		}
		case ID_TOOLS_TOOL11 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 10 ] );
			break;
		}
		case ID_TOOLS_TOOL12 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 11 ] );
			break;
		}
		case ID_TOOLS_TOOL13 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 12 ] );
			break;
		}
		case ID_TOOLS_TOOL14 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 13 ] );
			break;
		}
		case ID_TOOLS_TOOL15 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 14 ] );
			break;
		}
		case ID_TOOLS_TOOL16 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 15 ] );
			break;
		}
		case ID_TOOLS_TOOL17 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 16 ] );
			break;
		}
		case ID_TOOLS_TOOL18 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 17 ] );
			break;
		}
		case ID_TOOLS_TOOL19 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 18 ] );
			break;
		}
		case ID_TOOLS_TOOL20 :
		{
			strcpy_s ( szAction, sizeof ( szAction ), szRegisteredActions [ 19 ] );
			break;
		}
	}

	hInst = 
		ShellExecute ( NULL, szAction, 
						pMididoc->GetPathName( ), "", "", SW_SHOWDEFAULT );
}


void CMainFrame::OnToolsExplorer() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();

	HINSTANCE			hInst;

	if ( pMididoc == NULL )
	{
		return;
	}

	CString		strPathName = pMididoc->GetPathName( );
	char		szDirectory [ MAX_PATHNAME_STRING ];
	strcpy_s ( szDirectory,  sizeof ( szDirectory ), strPathName );

	bool bFinished = false;
	int iX = (int)strlen ( szDirectory );
	while ( ! bFinished && iX > 0 )
	{
		iX--;
		if ( szDirectory [ iX ] == '\\' )
		{
			bFinished = true;
		}
		szDirectory [ iX ] = 0;
	}

	hInst = 
		ShellExecute ( NULL, "open", 
						szDirectory, "", "", SW_SHOWDEFAULT );
}

void CMainFrame::AddToolsMenu()
{
	// TODO
	char			szValue [ MAX_REGISTRY_STRING ];
	long			iLength;
	LONG			iLong; 

	iLength = sizeof ( szValue ) - 1;
	iLong = RegQueryValue( HKEY_CLASSES_ROOT, ".mid",  szValue, &iLength );
	if ( iLong != ERROR_SUCCESS )
	{
		return;
	}

	//	First Get The Menu
	CMenu	* pMenu		= GetMenu ();
	if ( pMenu == NULL )
	{
		return;
	}

	//	Then The menu count
	UINT	iItemCount	= pMenu->GetMenuItemCount ( );

	//	Now Get The Sub Menu
	CMenu	* pSubMenu	= pMenu->GetSubMenu ( iItemCount - 2 );
	if ( pSubMenu == NULL )
	{
		return;
	}

	//	Now Enumerate Keys
	char		szPlace [ MAX_REGISTRY_STRING ];
	HKEY		hkResult;

	strcpy_s ( szPlace, sizeof ( szPlace ), szValue );
	strcat_s ( szPlace, sizeof ( szPlace ), "\\shell" );
	iLong = RegOpenKey( HKEY_CLASSES_ROOT, szPlace, &hkResult );
	if ( iLong != ERROR_SUCCESS )
	{
		return;
	}

	//		Reset the buffer for actions
	memset ( szRegisteredActions, 0, sizeof ( szRegisteredActions ) );
		
	//		Loop on the registry
	DWORD		dwIndex	= 0;
	char		szBuffer [ MAX_REGISTRY_STRING ];
	do
	{
		memset ( szBuffer, 0, sizeof ( szBuffer ) );
		iLong = RegEnumKey( hkResult, dwIndex, szBuffer, sizeof ( szBuffer ) - 1 );
		if ( iLong == ERROR_SUCCESS )
		{
			//		Get The value of the key
			memset ( szValue, 0, sizeof ( szValue ) );
			iLength = sizeof ( szValue ) - 1;
			
			LONG iLong2 = RegQueryValue( hkResult, szBuffer,  szValue, &iLength );
			if ( iLong2 != ERROR_SUCCESS || *szValue == '\0' )
			{
				strcpy_s ( szValue, sizeof ( szValue ), szBuffer );
			}

			UINT		iID;
			switch ( dwIndex )
			{
			case 0:
				strcpy_s (	szRegisteredActions [ 0 ],
							sizeof ( szRegisteredActions [ 0 ] ), szBuffer );
				iID = ID_TOOLS_TOOL1;
				break;
			case 1:
				strcpy_s (	szRegisteredActions [ 1 ],
							sizeof ( szRegisteredActions [ 1 ] ), szBuffer );
				iID = ID_TOOLS_TOOL2;
				break;
			case 2:
				strcpy_s (	szRegisteredActions [ 2 ],
							sizeof ( szRegisteredActions [ 2 ] ), szBuffer );
				iID = ID_TOOLS_TOOL3;
				break;
			case 3:
				strcpy_s (	szRegisteredActions [ 3 ],
							sizeof ( szRegisteredActions [ 3 ] ), szBuffer );
				iID = ID_TOOLS_TOOL4;
				break;
			case 4:
				strcpy_s (	szRegisteredActions [ 4 ],
							sizeof ( szRegisteredActions [ 4 ] ), szBuffer );
				iID = ID_TOOLS_TOOL5;
				break;
			case 5:
				strcpy_s (	szRegisteredActions [ 5 ],
							sizeof ( szRegisteredActions [ 5 ] ), szBuffer );
				iID = ID_TOOLS_TOOL6;
				break;
			case 6:
				strcpy_s (	szRegisteredActions [ 6 ],
							sizeof ( szRegisteredActions [ 6 ] ), szBuffer );
				iID = ID_TOOLS_TOOL7;
				break;
			case 7:
				strcpy_s (	szRegisteredActions [ 7 ],
							sizeof ( szRegisteredActions [ 7 ] ), szBuffer );
				iID = ID_TOOLS_TOOL8;
				break;
			case 8:
				strcpy_s (	szRegisteredActions [ 8 ],
							sizeof ( szRegisteredActions [ 8 ] ), szBuffer );
				iID = ID_TOOLS_TOOL9;
				break;
			case 9:
				strcpy_s (	szRegisteredActions [ 9 ],
							sizeof ( szRegisteredActions [ 9 ] ), szBuffer );
				iID = ID_TOOLS_TOOL10;
				break;
			case 10:
				strcpy_s (	szRegisteredActions [ 10 ],
							sizeof ( szRegisteredActions [ 10 ] ), szBuffer );
				iID = ID_TOOLS_TOOL11;
				break;
			case 11:
				strcpy_s (	szRegisteredActions [ 11 ],
							sizeof ( szRegisteredActions [ 11 ] ), szBuffer );
				iID = ID_TOOLS_TOOL12;
				break;
			case 12:
				strcpy_s (	szRegisteredActions [ 12 ],
							sizeof ( szRegisteredActions [ 12 ] ), szBuffer );
				iID = ID_TOOLS_TOOL13;
				break;
			case 13:
				strcpy_s (	szRegisteredActions [ 13 ],
							sizeof ( szRegisteredActions [ 13 ] ), szBuffer );
				iID = ID_TOOLS_TOOL14;
				break;
			case 14:
				strcpy_s (	szRegisteredActions [ 14 ],
							sizeof ( szRegisteredActions [ 14 ] ), szBuffer );
				iID = ID_TOOLS_TOOL15;
				break;
			case 15:
				strcpy_s (	szRegisteredActions [ 15 ],
							sizeof ( szRegisteredActions [ 15 ] ), szBuffer );
				iID = ID_TOOLS_TOOL16;
				break;
			case 16:
				strcpy_s (	szRegisteredActions [ 16 ],
							sizeof ( szRegisteredActions [ 16 ] ), szBuffer );
				iID = ID_TOOLS_TOOL17;
				break;
			case 17:
				strcpy_s (	szRegisteredActions [ 17 ],
							sizeof ( szRegisteredActions [ 17 ] ), szBuffer );
				iID = ID_TOOLS_TOOL18;
				break;
			case 18:
				strcpy_s (	szRegisteredActions [ 18 ],
							sizeof ( szRegisteredActions [ 18 ] ), szBuffer );
				iID = ID_TOOLS_TOOL19;
				break;
			case 19:
				strcpy_s (	szRegisteredActions [ 19 ],
							sizeof ( szRegisteredActions [ 19 ] ), szBuffer );
				iID = ID_TOOLS_TOOL20;
				break;
			}

			strcpy_s ( szBuffer, sizeof ( szBuffer ), szValue );
			if ( *szBuffer >= 'a' && *szBuffer <= 'z' )
			{
				*szBuffer -= 0x20;
			}

			pSubMenu->AppendMenu(MF_STRING, iID, szBuffer);
			dwIndex++;
		}
	}
	while ( iLong == ERROR_SUCCESS && dwIndex < TOOLS_NUMBER );

	iLong = RegCloseKey (hkResult);
	
}


