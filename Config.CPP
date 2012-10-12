// Config.cpp: implementation of the CConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "incl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConfig::CConfig(CString homedir)
{
	m_drawLeftInfo = false;
	m_drawRightIcons = true;
	m_keepUntilConfirmed = true;
	m_daysAdvance = 5;
	m_warnAnnoyingly = false;
	m_warnDaily = true;
	m_homedir = homedir;
	m_xmlname = "config";
	// Persistency
	ReadXML( m_homedir+"bdc.xml");
}

CString CConfig::construct()
{
	char buf[10];

	CString ret = "<"+m_xmlname+">\r\n";
	
	_itoa_s(m_daysAdvance, buf, 10);
	ret += "<daysInAdvance>"+CXMLString::Encode((CString)buf)+"</daysInAdvance>\r\n";
	
	ret += "<warn>\r\n";
	ret += "<daily>"+(CString)(m_warnDaily?"true":"false")+"</daily>\r\n";
	ret += "<annoyingly>"+(CString)(m_warnAnnoyingly?"true":"false")+"</annoyingly>\r\n";
	ret += "<confirmation>"+(CString)(m_keepUntilConfirmed?"true":"false")+"</confirmation>\r\n";
	ret += "<drawRightIcons>"+(CString)(m_drawRightIcons?"true":"false")+"</drawRightIcons>\r\n";
	ret += "<drawLeftInfo>"+(CString)(m_drawLeftInfo?"true":"false")+"</drawLeftInfo>\r\n";
	ret += "<suffix>";
	switch( m_suffix )
	{
		case SUFFIX_AGE:
			ret+="AGE";
			break;
		case SUFFIX_DATE:
			ret+="DATE";
			break;
		case SUFFIX_AGE_SHORT:
			ret+="AGE_SHORT";
			break;
		case SUFFIX_YOB:
			ret+="YOB";
			break;
		case SUFFIX_YOB_SHORT:
			ret+="YOB_SHORT";
			break;
		case SUFFIX_NONE:
		default:
			ret+="NONE";
			break;
	}
	ret += "</suffix>\r\n";
	ret += "</warn>\r\n";
	
	ret += "<comments>"+CXMLString::Encode("This file holds the configuration for the BDC Birthday Calendar plugin for Trillian")+"</comments>\r\n";

	ret += "</"+m_xmlname+">\r\n";

	return ret;
}

void CConfig::handlevalue(CString name, CString parameters, CXMLString & xml)
{
	if( name.Compare( "daysInAdvance" )==0 )
		m_daysAdvance = atoi( xml.nextstring() );
	else if( name.Compare( "annoyingly" )==0 )
		m_warnAnnoyingly = xml.nextstring().CompareNoCase( "true" )==0;
	else if( name.Compare( "confirmation" )==0 )
		m_keepUntilConfirmed = xml.nextstring().CompareNoCase( "true" )==0;
	else if( name.Compare( "suffix" )==0 )
	{
		CString sx = xml.nextstring();
		if( sx.CompareNoCase( "NONE" )==0 ) m_suffix = SUFFIX_NONE;
		else if( sx.CompareNoCase( "NONE" )==0 ) m_suffix = SUFFIX_NONE;
		else if( sx.CompareNoCase( "DATE" )==0 ) m_suffix = SUFFIX_DATE;
		else if( sx.CompareNoCase( "AGE" )==0 ) m_suffix = SUFFIX_AGE;
		else if( sx.CompareNoCase( "AGE_SHORT" )==0 ) m_suffix = SUFFIX_AGE_SHORT;
		else if( sx.CompareNoCase( "YOB" )==0 ) m_suffix = SUFFIX_YOB;
		else if( sx.CompareNoCase( "YOB_SHORT" )==0 ) m_suffix = SUFFIX_YOB_SHORT;
	}
	else if( name.Compare( "drawRightIcons" )==0 )
		m_drawRightIcons = xml.nextstring().CompareNoCase( "true" )==0;
	else if( name.Compare( "drawLeftInfo" )==0 )
		m_drawLeftInfo = xml.nextstring().CompareNoCase( "true" )==0;
	else if( name.Compare( "daily" )==0 )
		m_warnDaily = xml.nextstring().CompareNoCase( "true" )==0;
	else
		parse(xml);
}

void CConfig::Store()
{
	// Persistency
	WriteXML( m_homedir+"bdc.xml");
}

void CConfig::PrefsShow(HWND hWnd)
{
	// Store config
	SetDlgItemInt( hWnd, IDC_DAYSINADVANCE, m_daysAdvance, false );
	CheckDlgButton( hWnd, IDC_WARNDAILY, m_warnDaily );
	CheckDlgButton( hWnd, IDC_WARNANNOYINGLY, m_warnAnnoyingly );
	CheckDlgButton( hWnd, IDC_KEEPUNTILCONFIRMED, m_keepUntilConfirmed );
	CheckDlgButton( hWnd, IDC_DRAWRIGHTICONS, m_drawRightIcons );
	CheckDlgButton( hWnd, IDC_DRAWLEFTINFO, m_drawLeftInfo );
	CheckDlgButton( hWnd, IDC_SUFFIX_NONE , m_suffix == SUFFIX_NONE );
	CheckDlgButton( hWnd, IDC_SUFFIX_AGE , m_suffix == SUFFIX_AGE_SHORT );
	CheckDlgButton( hWnd, IDC_SUFFIX_DATE , m_suffix == SUFFIX_DATE );
	CheckDlgButton( hWnd, IDC_SUFFIX_YOB , m_suffix == SUFFIX_YOB );
	CheckDlgButton( hWnd, IDC_SUFFIX_YOB_SHORT , m_suffix == SUFFIX_YOB_SHORT );
	EnableWindow(
		GetDlgItem( hWnd, IDC_DRAWLEFTINFO ), m_suffix != SUFFIX_NONE );
}

void CConfig::PrefsAction(HWND hwnd)
{
	// Store config
	m_daysAdvance =
		GetDlgItemInt( hwnd, IDC_DAYSINADVANCE, NULL, false );
	m_warnDaily =
		IsDlgButtonChecked( hwnd, IDC_WARNDAILY )>0;
	m_warnAnnoyingly =
		IsDlgButtonChecked( hwnd, IDC_WARNANNOYINGLY )>0;
	m_keepUntilConfirmed =
		IsDlgButtonChecked( hwnd, IDC_KEEPUNTILCONFIRMED )>0;
	m_drawRightIcons =
		IsDlgButtonChecked( hwnd, IDC_DRAWRIGHTICONS )>0;
	m_drawLeftInfo =
		IsDlgButtonChecked( hwnd, IDC_DRAWLEFTINFO )>0;
	if( IsDlgButtonChecked( hwnd, IDC_SUFFIX_NONE )>0 ) m_suffix = SUFFIX_NONE;
	else if( IsDlgButtonChecked( hwnd, IDC_SUFFIX_DATE )>0 ) m_suffix = SUFFIX_DATE;
	else if( IsDlgButtonChecked( hwnd, IDC_SUFFIX_AGE )>0 ) m_suffix = SUFFIX_AGE_SHORT;
	else if( IsDlgButtonChecked( hwnd, IDC_SUFFIX_YOB )>0 ) m_suffix = SUFFIX_YOB;
	else if( IsDlgButtonChecked( hwnd, IDC_SUFFIX_YOB_SHORT )>0 ) m_suffix = SUFFIX_YOB_SHORT;
	Store();
}

int CConfig::prefDialog(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam, HMODULE hHandle)
{
	switch( message )
	{

		case WM_INITDIALOG:
			CheckDlgButton( hWnd, IDC_INSTRUCTIONS, 1 );
			SetDlgItemText( hWnd, IDC_VERSION, VERSION );
			{
				//Load the credits resource
				HGLOBAL hLic;
				LPVOID lpLic;
				
				try
				{
					hLic = LoadResource(hHandle, FindResource(hHandle, (char*)IDR_INSTRUCTIONS, "Text"));

					if(!hLic) throw 0;

					lpLic = LockResource(hLic);
					
					if(!lpLic) throw 0;

					SetDlgItemText( hWnd, IDC_INFO, (char*)lpLic );
				}
				catch(...)
				{
					SetDlgItemText( hWnd, IDC_INFO, "The text for this control could not be found" );
				}
			}
			break;

		case WM_COMMAND:
			switch( (int)LOWORD(wparam) )
			{
				case IDC_INSTRUCTIONS:
				{
					//Load the credits resource
					HGLOBAL hLic;
					LPVOID lpLic;
					
					try
					{
						hLic = LoadResource(hHandle, FindResource(hHandle, (char*)IDR_INSTRUCTIONS, "Text"));

						if(!hLic) throw 0;

						lpLic = LockResource(hLic);
						
						if(!lpLic) throw 0;

						SetDlgItemText( hWnd, IDC_INFO, (char*)lpLic );
					}
					catch(...)
					{
						SetDlgItemText( hWnd, IDC_INFO, "The text for this control could not be found" );
					}
				}
				break;
				case IDC_DISCLAIMER:
				{
					//Load the credits resource
					HGLOBAL hLic;
					LPVOID lpLic;
					
					try
					{
						hLic = LoadResource(hHandle, FindResource(hHandle, (char*)IDR_DISCLAIMER, "Text"));

						if(!hLic) throw 0;

						lpLic = LockResource(hLic);
						
						if(!lpLic) throw 0;

						SetDlgItemText( hWnd, IDC_INFO, (char*)lpLic );
					}
					catch(...)
					{
						SetDlgItemText( hWnd, IDC_INFO, "The text for this control could not be found" );
					}
				}
				break;
				case IDC_VERSIONHISTORY:
				{
					//Load the credits resource
					HGLOBAL hLic;
					LPVOID lpLic;
					
					try
					{
						hLic = LoadResource(hHandle, FindResource(hHandle, (char*)IDR_VERSIONHISTORY, "Text"));

						if(!hLic) throw 0;

						lpLic = LockResource(hLic);
						
						if(!lpLic) throw 0;

						SetDlgItemText( hWnd, IDC_INFO, (char*)lpLic );
					}
					catch(...)
					{
						SetDlgItemText( hWnd, IDC_INFO, "The text for this control could not be found" );
					}
				}
				break;
				default:
					break;

			}
			break;
		default:
			break;
	}

	return 0;
}
