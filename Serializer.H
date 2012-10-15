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

// Serializer.h: interface for the Serializer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALIZER_H__79C7911F_A60F_4C35_928D_1F4BFA4BB7B5__INCLUDED_)
#define AFX_SERIALIZER_H__79C7911F_A60F_4C35_928D_1F4BFA4BB7B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSerializer  
{
protected:
	void ReadXML( CString filename );
	void WriteXML( CString filename );
	virtual void handlevalue(CString name, CString parameters, CXMLString&);
	virtual void parse(CXMLString&);
public:
	CString m_xmlname;
	void SetName(CString);
	virtual CString construct()=0;
	CSerializer();
	CSerializer(CXMLString&);

};

#endif // !defined(AFX_SERIALIZER_H__79C7911F_A60F_4C35_928D_1F4BFA4BB7B5__INCLUDED_)
