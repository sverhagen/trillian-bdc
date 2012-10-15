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

// Birthdays.cpp: implementation of the CBirthdays class.
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

CBirthdays::CBirthdays(CString homedir)
{
	m_homedir = homedir;
	m_xmlname = "birthdays";
	// Persistency
	ReadXML( m_homedir+"birthdays.xml");
}

CBirthdays::~CBirthdays()
{
	POSITION pos = GetHead();
	while( pos )
	{
		delete GetNext( pos );
	}
	m_list.RemoveAll();
	m_listSorted.RemoveAll();
}

POSITION CBirthdays::GetHead()
{
	return m_listSorted.GetHeadPosition();
}

CBirthday* CBirthdays::GetNext(POSITION& pos)
{
	return m_listSorted.GetNext( pos );
}

void CBirthdays::Remove(CBirthday * p)
{
	m_list.RemoveKey( p );

	POSITION nPos = m_listSorted.GetHeadPosition();
	while( nPos )
	{
		if( m_listSorted.GetAt( nPos )==p )
		{
			m_listSorted.RemoveAt( nPos );
			break;
		}
		m_listSorted.GetNext( nPos );
	}
}

CString CBirthdays::construct()
{
	CString ret = "<"+m_xmlname+">\r\n";

	POSITION pos = GetHead();
	while( pos )
	{
		ret += GetNext( pos )->construct();
	}
	
	ret += "<comments>"+CXMLString::Encode("This file holds the birthday data for the BDC Birthday Calendar plugin for Trillian")+"</comments>\r\n";

	ret += "</"+m_xmlname+">\r\n";

	return ret;
}

void CBirthdays::handlevalue(CString name, CString parameters, CXMLString& xml)
{
	if( name.Compare( "birthday" )==0 )
	{
		CBirthday* pbd = new CBirthday( xml );
		Add( pbd );
	}
	else
		parse(xml);
}

void CBirthdays::Store()
{
	// Persistency
	WriteXML( m_homedir+"birthdays.xml");
}

void CBirthdays::Add(CBirthday * pbd)
{
	m_list.SetAt( pbd, pbd );
	POSITION nPos = m_listSorted.GetHeadPosition();
	while( nPos &&
			pbd->m_date.Compare( m_listSorted.GetAt(nPos)->m_date )<0 )
		m_listSorted.GetNext(nPos);
	if( nPos )
		m_listSorted.InsertBefore( nPos, pbd );
	else
		m_listSorted.AddTail( pbd );
}

int CBirthdays::GetPreviousID(CBirthday *pbd)
{
	int nPreviousID = -2;
	POSITION nPos = m_listSorted.GetHeadPosition();
	while( nPos &&
			pbd->m_date.Compare( m_listSorted.GetAt(nPos)->m_date )<0 )
		nPreviousID = m_listSorted.GetNext(nPos)->m_listid_known;
	return nPreviousID;
}

int CBirthdays::GetNextID(CDate * pb)
{
	int nPreviousID = -2;
	POSITION nPos = m_listSorted.GetHeadPosition();
	while( nPos &&
			pb->Compare( m_listSorted.GetAt(nPos)->m_date )<0 )
		nPreviousID = m_listSorted.GetNext(nPos)->m_listid_known;
	return nPreviousID;
}

int CBirthdays::GetCount()
{
	return m_list.GetCount();
}
