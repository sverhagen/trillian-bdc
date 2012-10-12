// Birthday.cpp: implementation of the CBirthday class.
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

CBirthday::CBirthday()
{
	m_confirmed = true;
	m_alerted_this_year = false;
	m_listid_bd = -1;
	m_listid_known = -1;
	m_xmlname = "birthday";
	m_recent_alert = CDate(1,1,0);
}

CBirthday::CBirthday(CXMLString& xml)
{
	m_confirmed = true;
	m_alerted_this_year = false;
	parse(xml);
	m_listid_bd = -1;
	m_listid_known = -1;
	m_xmlname = "birthday";
}

CBirthday::~CBirthday()
{
}

CInternetSession* CBirthday::m_pSession = NULL;

CString CBirthday::GetTooltip()
{
	switch( m_medium )
	{
		case MEDIUM_ICQ:
			return m_name + "\nICQ "+m_mediumname+"\n" + m_date.GetDateString() + "\n" + GetSuffix(SUFFIX_AGE);
		case MEDIUM_VCARD:
			return m_name + "\nvCard import\n" + m_date.GetDateString() + "\n" + GetSuffix(SUFFIX_AGE);
		case MEDIUM_WAB:
			return m_name + "\nWAB import\n" + m_date.GetDateString() + "\n" + GetSuffix(SUFFIX_AGE);
		default:
			return m_name + "\n" + m_date.GetDateString() + "\n" + GetSuffix(SUFFIX_AGE);
	}
}

CString CBirthday::GetText(SUFFIX sx/* = SUFFIX_NONE */, bool left/* = false */)
{
	if( sx == SUFFIX_NONE )
		return m_name;
	else
	{
		CString suffix = GetSuffix(sx);
		if( left )
		{
			if( suffix.GetLength() )
			{
				suffix += " ";
			}
			return suffix+m_name;
		}
		else
		{
			if( suffix.GetLength() )
			{
				suffix = " " + suffix;
			}
			return m_name+suffix;
		}
	}
}

CString CBirthday::GetSuffix(SUFFIX sx)
{
	char cBuffer[10];
	switch( sx )
	{
		case SUFFIX_YOB:
			_itoa_s( m_date.GetYear(), cBuffer, 10 );
			return CString(cBuffer);
			break;
		case SUFFIX_YOB_SHORT:
			_itoa_s( m_date.GetYear(), cBuffer, 10 );
			return CString( "'" + CString(cBuffer).Right(2) );
			break;
		case SUFFIX_AGE:
			if( m_date.GetAge() != (signed)CDate().GetYear() )
			{
				_itoa_s( m_date.GetAge(), cBuffer, 10 );
				return CString(cBuffer)+" years";
			}
			break;
		case SUFFIX_AGE_SHORT:
			if( m_date.GetAge() != (signed)CDate().GetYear() )
			{
				_itoa_s( m_date.GetAge(), cBuffer, 10 );
				return CString(cBuffer)+"y";
			}
			break;
		case SUFFIX_DATE:
			return CString("("+m_date.GetShortDateString()+")");
			break;
		case SUFFIX_NONE:
		default:
			return "";
	}
	return "";
}

CString CBirthday::construct()
{
	CString ret = "<"+m_xmlname+">\r\n";
	
	ret += "<name>"+CXMLString::Encode(m_name)+"</name>\r\n";
	switch( m_medium )
	{
		case MEDIUM_ICQ:
			ret += "<medium>ICQ</medium>\r\n";
			break;
		case MEDIUM_VCARD:
			ret += "<medium>vCard</medium>\r\n";
			break;
		case MEDIUM_WAB:
			ret += "<medium>WAB</medium>\r\n";
			break;
		default:
			ret += "<medium>no</medium>\r\n";
	}
	ret += "<alerted>"+(CString)(m_alerted_this_year?"true":"false")+"</alerted>\r\n";
	ret += "<confirmed>"+(CString)(m_confirmed?"true":"false")+"</confirmed>\r\n";
	ret += "<nameOnMedium>"+m_mediumname+"</nameOnMedium>\r\n";

	m_recent_alert.SetName( "recentalert" );
	ret += m_recent_alert.construct();
	ret += m_date.construct();

	ret += "</"+m_xmlname+">\r\n";

	return ret;
}

void CBirthday::handlevalue(CString name, CString parameters, CXMLString& xml)
{
	if( name.Compare( "name" )==0 )
		m_name = xml.nextstring();
	else if( name.Compare( "nameOnMedium" )==0 )
		m_mediumname = xml.nextstring();
	else if( name.Compare( "confirmed" )==0 )
		m_confirmed = xml.nextstring().CompareNoCase( "true" )==0;
	else if( name.Compare( "alerted" )==0 )
		m_alerted_this_year = xml.nextstring().CompareNoCase( "true" )==0;
	else if( name.Compare( "medium" )==0 )
	{
		CString med = xml.nextstring();
		if( med.CompareNoCase( "ICQ" )==0 )
			m_medium = MEDIUM_ICQ;
		else if( med.CompareNoCase( "vCard" )==0 )
			m_medium = MEDIUM_VCARD;
		else if( med.CompareNoCase( "WAB" )==0 )
			m_medium = MEDIUM_WAB;
		else
			m_medium = MEDIUM_NONE;
	}
	else if( name.Compare( "recentalert" )==0 )
		m_recent_alert = CDate(xml);
	else if( name.Compare( "date" )==0 )
		m_date = CDate(xml);
	else
		parse(xml);
}

IMPORT CBirthday::LoadICQ(char * icqno, char * name)
{
	#define HTTPBUFLEN    512 // Size of HTTP Buffer...
	char httpbuff[HTTPBUFLEN];
	CString resulthtml;

	TRY
	{
		if( m_pSession == NULL )
			m_pSession = new CInternetSession();
		CStdioFile* remotefile = m_pSession->OpenURL(
			"http://people.icq.com/people/full_details_show.php?uin="
				+CString(icqno),
			1,
			INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_EXISTING_CONNECT
		);
		if( remotefile )
		{
			int numbytes;
			while (numbytes = remotefile->Read(httpbuff, HTTPBUFLEN))
				resulthtml += httpbuff;

			remotefile->Close();
			delete remotefile;

			// Look for error
			if( resulthtml.Find("Error Page") >= 0 )
			{
				return IMPORT_BADICQ;
			}

			int pos, poss;
			// Look for year
			if( (pos = resulthtml.Find("Year")) < 0 )
			{
				return IMPORT_BADREPLY;
			}
			if( ((pos = resulthtml.Find("<div class=\"udu-flvl\">", pos)+22) < 0)
				|| ((poss = resulthtml.Find("<", pos)) < 0) )
			{
				return IMPORT_BADREPLY;
			}
		    CString year = resulthtml.Mid(pos, poss-pos);

			// Look for month
			if( (pos = resulthtml.Find("Month")) < 0 )
			{
				return IMPORT_BADREPLY;
			}
			if( ((pos = resulthtml.Find("<div class=\"udu-flvl\">", pos)+22) < 0)
				|| ((poss = resulthtml.Find("<", pos)) < 0) )
			{
				return IMPORT_BADREPLY;
			}
		    CString month = resulthtml.Mid(pos, poss-pos);

			// Look for day
			if( (pos = resulthtml.Find("Day")) < 0 )
			{
				return IMPORT_BADREPLY;
			}
			if( ((pos = resulthtml.Find("<div class=\"udu-flvl\">", pos)+22) < 0)
				|| ((poss = resulthtml.Find("<", pos)) < 0) )
			{
				return IMPORT_BADREPLY;
			}
		    CString day = resulthtml.Mid(pos, poss-pos);

			// Convert month
			if     ( month.CompareNoCase( "January"   )==0 ) month="1";
			else if( month.CompareNoCase( "February"  )==0 ) month="2";
			else if( month.CompareNoCase( "March"     )==0 ) month="3";
			else if( month.CompareNoCase( "April"     )==0 ) month="4";
			else if( month.CompareNoCase( "May"       )==0 ) month="5";
			else if( month.CompareNoCase( "June"      )==0 ) month="6";
			else if( month.CompareNoCase( "July"      )==0 ) month="7";
			else if( month.CompareNoCase( "August"    )==0 ) month="8";
			else if( month.CompareNoCase( "September" )==0 ) month="9";
			else if( month.CompareNoCase( "October"   )==0 ) month="10";
			else if( month.CompareNoCase( "November"  )==0 ) month="11";
			else if( month.CompareNoCase( "December"  )==0 ) month="12";
			else
			{
				return IMPORT_NOBIRTHDAY;
			}

			// Create the actual date
			m_date = CDate(atoi(day), atoi(month), atoi(year));
			if( !m_date.ValidDate() )
			{
				return IMPORT_NOBIRTHDAY;
			}

			m_name = CString(name);
			m_mediumname = CString(icqno);
			m_medium = MEDIUM_ICQ;
		}
		else
		{
			return IMPORT_BADREPLY;
		}
	}
	CATCH_ALL(error)
	{
		return IMPORT_BADREPLY;
	}
	END_CATCH_ALL;
	return IMPORT_SUCCESS;
}

IMPORT CBirthday::LoadVCF(CString vcf_file)
{
	// We create a file exception
	CFileException e;
	CStdioFile file;

	if ( !file.Open(
			vcf_file,
			CFile::typeBinary | CFile::modeRead | CFile::shareDenyNone,
			&e ) )
	{
		return IMPORT_BADFILE;
	}
	else
	{
		#define chars_per_read 100
		// returncount to registrate that still data is coming out of the file
		int nReturnCount;
		// We are going to retrieve one character per cycle and hold it in czBuffer
		char czBuffer[chars_per_read];
		// The resulting string
		CString result;

		// Get the first character
		nReturnCount = file.Read( &czBuffer, chars_per_read );
		// Now get all the other characters
		while( nReturnCount )
		{
			// We add the retreived character to the string
			result += ((CString)czBuffer).Left(nReturnCount);
			// And get the next one
			nReturnCount = file.Read( &czBuffer, chars_per_read );
		}

		// Process birthday information

		int pos;
		// Look for birth date label
		if( ((pos = result.Find("BDAY")) < 0)
			|| ((pos = result.Find(':',pos)) < 0) )
		{
			return IMPORT_NOBIRTHDAY;
		}
		CString date = result.Mid( pos+1 );
		pos = date.Find( '\r' );
		date = date.Mid( 0, pos );

		// Look for the full name label
		if( ((pos = result.Find("FN")) < 0)
			|| ((pos = result.Find(':',pos)) < 0) )
		{
			return IMPORT_NONAME;
		}
		CString name = result.Mid( pos+1 );
		pos = name.Find( '\r' );
		name = name.Mid( 0, pos );

		m_medium = MEDIUM_VCARD;
		m_name = name;
		m_date = CDate(atoi(date.Mid(6,2)), atoi(date.Mid(4,2)), atoi(date.Mid(0,4)));
		return IMPORT_SUCCESS;
	}
}

void CBirthday::FinalCleanup()
{
	if( m_pSession )
	{
		delete m_pSession;
		m_pSession = NULL;
	}
}

bool CBirthday::Reload()
{
	if( m_medium == MEDIUM_ICQ )
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState( )); 
		CBirthday b;
		if( b.LoadICQ( m_mediumname.GetBuffer(0),
			m_name.GetBuffer(0) )
				!= IMPORT_SUCCESS )
		{
			AfxMessageBox(
				"Birthday could not be updated",
				MB_OK|MB_SYSTEMMODAL,
				0 );
			return false;
		}
		else
		{
			AfxMessageBox(
				"Birthday updated",
				MB_OK|MB_SYSTEMMODAL,
				0 );
			if( m_date == b.m_date )
				return false;
			else
			{
				m_date = b.m_date;
				return true;
			}
		}
	}
	return false;
}
