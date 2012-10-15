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

// Date.h: interface for the CDate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAY_H__7812B84A_F9CF_4672_BD7A_58759FBC4C1F__INCLUDED_)
#define AFX_DAY_H__7812B84A_F9CF_4672_BD7A_58759FBC4C1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDate : public CSerializer
{
public:
	bool IsDateOfYear();
	bool IsDateOfYear(CDate&);
	int GetAge();
	bool AfterAndWithinDays(CDate d, BYTE n);
	int GetDayCount();
	int operator>(const CDate &comp) const;
	int operator<(const CDate &comp) const;
	bool operator!=(const CDate &comp) const;
	bool operator==(const CDate &comp) const;
	bool WithinDays( CDate d, BYTE n );
	int Compare( CDate& );
	CDate(CXMLString&);
	CDate(CTime&);
	void handlevalue(CString name, CString parameters, CXMLString& xml);
	CString GetShortDateString();
	CString GetDateString();
	BYTE GetMonth();
	BYTE GetDay();
	UINT GetYear();
	CString construct();
	CDate operator+(BYTE plus_days);
	CDate operator-(BYTE min_days);
	bool ValidDate();
	CDate( BYTE d, BYTE m, UINT y );
	CDate();
	virtual ~CDate();

protected:
	BYTE m_day;
	BYTE m_month;
	UINT m_year;
	BYTE CountFebruary(int offset_years);
};

#endif // !defined(AFX_DAY_H__7812B84A_F9CF_4672_BD7A_58759FBC4C1F__INCLUDED_)
