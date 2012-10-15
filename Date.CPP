/**
 * Copyright 2003-2012 Totaal Software / Sander Verhagen
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License. You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 */

// Date.cpp: implementation of the CDate class.
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

CDate::CDate()
{
	m_day = CTime::GetCurrentTime().GetDay();
	m_month = CTime::GetCurrentTime().GetMonth();
	m_year = CTime::GetCurrentTime().GetYear();
	m_xmlname = "date";
}

CDate::~CDate()
{

}

CDate::CDate(BYTE d, BYTE m, UINT y)
{
	m_day = d;
	m_month = m;
	m_year = y;
	m_xmlname = "date";
}

CDate::CDate(CTime& t)
{
	m_day = t.GetDay();
	m_month = t.GetMonth();
	m_year = t.GetYear();
	m_xmlname = "date";
}

bool CDate::ValidDate()
{
	if( m_day==0 ) return false;
	switch( m_month )
	{
		case 2:
			if(m_year % 400==0)
			{
				if(m_day>29) return false;
			}
			else if(m_year % 100==0)
			{
				if(m_day>28) return false;
			}
			else if(m_year % 4==0)
			{
				if(m_day>29) return false;
			}
			else if(m_day>28) return false;
			break;
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			if( m_day>31 ) return false;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			if( m_day>30 ) return false;
			break;
		default:
			return false;
	}
	return true;
}

CDate CDate::operator+(BYTE plus_days)
{
	if( plus_days < 0 )
		return *this - -1*plus_days;
	int day = m_day + plus_days;
	int month = m_month;
	int offset_year = 0;
	while(true)
	{
		switch( m_month )
		{
			case 2:
				if( day>CountFebruary(offset_year) )
				{
					day-=CountFebruary(offset_year); month++;
				}
				else
					return CDate(day, month, m_year);
				break;
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				if( day>31 )
				{
					day-=31; month++;
				}
				else
					return CDate(day, month, m_year);
				break;
			case 4:
			case 6:
			case 9:
			case 11:
				if( day>30 )
				{
					day-=30; month++;
				}
				else
					return CDate(day, month, m_year);
				break;
		}
		if( month > 12 )
		{
			month=1; offset_year++;
		}
	}
}

CDate CDate::operator-(BYTE min_days)
{
	if( min_days < 0 )
		return *this + -1*min_days;
	int day = m_day - min_days;
	int month = m_month;
	int offset_year = 0;
	while(true)
	{
		switch( m_month )
		{
			case 3:
				if( day<=0 )
				{
					day+=CountFebruary(offset_year); month--;
				}
				else
					return CDate(day, month, m_year);
				break;
			case 2:
			case 4:
			case 6:
			case 8:
			case 9:
			case 11:
			case 1:
				if( day<=0 )
				{
					day+=31; month--;
				}
				else
					return CDate(day, month, m_year);
				break;
			case 5:
			case 7:
			case 10:
			case 12:
				if( day<=0 )
				{
					day+=30; month--;
				}
				else
					return CDate(day, month, m_year);
				break;
		}
		if( month == 0 )
		{
			month=12; offset_year--;
		}
	}
}

BYTE CDate::CountFebruary(int offset_years)
{
	int year = CTime::GetCurrentTime().GetYear();
	year += offset_years;

	if (year % 400==0) return 29;
	if (year % 100==0) return 28;
	if (year % 4==0) return 29;
	
	return 28;
}

CString CDate::construct()
{
	char buf[10];
	CString ret = "<"+m_xmlname+">\r\n";
	_itoa_s(m_day, buf, 10);
	ret += "<day>"+CXMLString::Encode((CString)buf)+"</day>\r\n";
	_itoa_s(m_month, buf, 10);
	ret += "<month>"+CXMLString::Encode((CString)buf)+"</month>\r\n";
	_itoa_s(m_year, buf, 10);
	ret += "<year>"+CXMLString::Encode((CString)buf)+"</year>\r\n";
	ret += "</"+m_xmlname+">\r\n";
	return ret;
}

BYTE CDate::GetDay()
{
	return m_day;
}

BYTE CDate::GetMonth()
{
	return m_month;
}

UINT CDate::GetYear()
{
	return m_year;
}

CString CDate::GetDateString()
{
	CString retval;

	CString help;
	if( m_year )
	{
		help.Format(" %u", m_year);
	}
	else
	{
		help = "";
	}
	
	switch( m_month )
	{
		case 1:
			retval.Format( "January %u%s", m_day, help ); break;
		case 2:
			retval.Format( "February %u%s", m_day, help ); break;
		case 3:
			retval.Format( "March %u%s", m_day, help ); break;
		case 4:
			retval.Format( "April %u%s", m_day, help ); break;
		case 5:
			retval.Format( "May %u%s", m_day, help ); break;
		case 6:
			retval.Format( "June %u%s", m_day, help ); break;
		case 7:
			retval.Format( "July %u%s", m_day, help ); break;
		case 8:
			retval.Format( "August %u%s", m_day, help ); break;
		case 9:
			retval.Format( "September %u%s", m_day, help ); break;
		case 10:
			retval.Format( "October %u%s", m_day, help ); break;
		case 11:
			retval.Format( "November %u%s", m_day, help ); break;
		case 12:
			retval.Format( "December %u%s", m_day, help ); break;
		default:
			retval = "Erroneous date";
	}
	return retval;
}

CString CDate::GetShortDateString()
{
	CString retval;

	CString help;
	if( m_year )
	{
		help.Format("%u", m_year);
		help = help.Right(2);
		help = " '" + help;
	}
	else
	{
		help = "";
	}
	
	switch( m_month )
	{
		case 1:
			retval.Format( "Jan. %u%s", m_day, help ); break;
		case 2:
			retval.Format( "Feb. %u%s", m_day, help ); break;
		case 3:
			retval.Format( "Mar. %u%s", m_day, help ); break;
		case 4:
			retval.Format( "Apr. %u%s", m_day, help ); break;
		case 5:
			retval.Format( "May %u%s", m_day, help ); break;
		case 6:
			retval.Format( "June %u%s", m_day, help ); break;
		case 7:
			retval.Format( "July %u%s", m_day, help ); break;
		case 8:
			retval.Format( "Aug. %u%s", m_day, help ); break;
		case 9:
			retval.Format( "Sep. %u%s", m_day, help ); break;
		case 10:
			retval.Format( "Oct. %u%s", m_day, help ); break;
		case 11:
			retval.Format( "Nov. %u%s", m_day, help ); break;
		case 12:
			retval.Format( "Dec. %u%s", m_day, help ); break;
		default:
			retval = "Erroneous date";
	}
	return retval;
}

void CDate::handlevalue(CString name, CString parameters, CXMLString &xml)
{
	if( name.Compare( "day" )==0 )
		m_day = atoi( xml.nextstring() );
	else if( name.Compare( "month" )==0 )
		m_month = atoi( xml.nextstring() );
	else if( name.Compare( "year" )==0 )
		m_year = atoi( xml.nextstring() );
	else
		parse(xml);
}

CDate::CDate(CXMLString& xml)
{
	m_year = 0; // For compatibility with older version
	parse(xml);
	m_xmlname = "date";
}

int CDate::Compare(CDate & d)
{
	return (d.m_month*100+d.m_day)-(m_month*100+m_day);
}

bool CDate::AfterAndWithinDays(CDate d, BYTE n)
{
	if( d > *this )
	{
		return false;
	}
	else
	{
		return WithinDays(d,n);
	}
}

bool CDate::WithinDays(CDate d, BYTE n)
{
	if( d > *this )
	{
		return (d.GetDayCount()-GetDayCount())<=n;
	}
	else
	{
		return (GetDayCount()-d.GetDayCount())<=n;
	}
}

int CDate::operator>(const CDate &comp) const
{
	return ((m_day>comp.m_day) && (m_month==comp.m_month)
		|| (m_month>comp.m_month));
}

int CDate::operator<(const CDate &comp) const
{
	return ((m_day<comp.m_day) && (m_month==comp.m_month)
		|| (m_month<comp.m_month));
}

bool CDate::operator==(const CDate &comp) const
{
	return (m_day==comp.m_day) && (m_month==comp.m_month) && (m_year==comp.m_year);
}

bool CDate::operator!=(const CDate &comp) const
{
	return !((m_day==comp.m_day) && (m_month==comp.m_month) && (m_year==comp.m_year));
}

int CDate::GetDayCount()
{
	int days = 0;
	switch( m_month )
	{
		case 12:
			days += 30;
		case 11:
			days += 31;
		case 10:
			days += 30;
		case 9:
			days += 31;
		case 8:
			days += 31;
		case 7:
			days += 30;
		case 6:
			days += 31;
		case 5:
			days += 30;
		case 4:
			days += 31;
		case 3:
			days += CountFebruary(0);
		case 2:
			days += 31;
		default:
			days += m_day;
			return days;
	}
}

int CDate::GetAge()
{
	return CTime::GetCurrentTime().GetYear() - m_year - 
		((CDate()<*this)?1:0);
}

bool CDate::IsDateOfYear(CDate & comp)
{
	return (m_day==comp.m_day) && (m_month==comp.m_month);
}

bool CDate::IsDateOfYear()
{
	return IsDateOfYear(CDate());
}
