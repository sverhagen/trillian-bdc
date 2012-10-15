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

// XMLString.h: interface for the XMLString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLSTRING_H__6FB57473_2B09_4EFA_8827_6083E9349AAC__INCLUDED_)
#define AFX_XMLSTRING_H__6FB57473_2B09_4EFA_8827_6083E9349AAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXMLString  
{
private:
	static bool IsUnsafe(unsigned char);
	CString m_value;
	int m_position;
public:
	static CString Decode(CString);
	static CString Encode(CString);
	CString nextstring();
	void nextchar();
	char value();
	bool equals(char);
	bool equals(CString);
	int leftover();
	CXMLString(CFile*);
};

#endif // !defined(AFX_XMLSTRING_H__6FB57473_2B09_4EFA_8827_6083E9349AAC__INCLUDED_)
