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

// Birthdays.h: interface for the CBirthdays class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BIRTHDAYS_H__5C34D7C8_CB6F_409E_A253_CD9D28212A5A__INCLUDED_)
#define AFX_BIRTHDAYS_H__5C34D7C8_CB6F_409E_A253_CD9D28212A5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBirthdays : CSerializer
{
public:
	int GetNextID(CDate*);
	int GetCount();
	int GetPreviousID(CBirthday*);
	void Store();
	virtual void handlevalue(CString name, CString parameters, CXMLString&);
	void Add( CBirthday* );
	void Remove( CBirthday* );
	CBirthday* GetNext(POSITION&);
	POSITION GetHead();
	CBirthdays(CString homedir);
	virtual ~CBirthdays();
	virtual CString construct();
protected:
	CString m_homedir;
	CTypedPtrMap<CMapPtrToPtr, CBirthday*, CBirthday*> m_list;
	CTypedPtrList<CPtrList, CBirthday*> m_listSorted;
};

#endif // !defined(AFX_BIRTHDAYS_H__5C34D7C8_CB6F_409E_A253_CD9D28212A5A__INCLUDED_)
