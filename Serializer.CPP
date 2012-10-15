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

// Serializer.cpp: implementation of the Serializer class.
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

CSerializer::CSerializer()
{
}

CSerializer::CSerializer(CXMLString& xml)
{
	parse(xml);
}

void CSerializer::handlevalue(CString name, CString parameters, CXMLString& xml)
{
	parse(xml);
}

void CSerializer::parse(CXMLString& xml)
{
	enum STATUS
	{
		do_nothing,
		do_tagname,
		do_tagpars,
		do_value
	} status = do_nothing;
	/*
			Lacking proper enumeration type in Java,
			let's do this as follows:
			status | means
			 0         | doing nothing
			 1         | tag started, ergo: doing tag name
			 2         | tagname done, ergo: doing parameters
			 3         | tag done, ergo: value
	*/
	CString name;
	CString parameters;
	while( xml.leftover()>0 && !xml.equals("</") )
	{
		switch( status )
		{
			case do_nothing:
				// If we are just getting started
				// Note that our position can be somewhere within a end tag
				// (due to a previous recursion call)
				// Change status when a new tag is found
				// while making sure that it is not an informational single-tag
				if( xml.equals('<') && !xml.equals("<?") ) status=do_tagname;
				break;
			case do_tagname:
				// If processing a start tag
				// Change status when a space occurs,
				// meaning a (new) parameter is found
				// Change status when the tag is finished
				if( xml.equals(' ') ) status=do_tagpars;
				else if( xml.equals("></") )
				{
					status = do_nothing;
					name = "";
					xml.nextchar();
				}
				else if( xml.equals('>') ) status=do_value;
				else name += (CString)xml.value();
				break;
			case do_tagpars:
				// If processing a parameter in a start tag
				// Change status when the tag is finished
				if( xml.equals('>') ) status=do_value;
				else if( xml.equals('/') && parameters.GetLength()==0 )
				{
					status = do_nothing;
					name = "";
					parameters = "";
				}
				else parameters += (CString)xml.value()+"";
				break;
			case do_value:
				// Start tag processing is all finished now
				// So lets call our function again to start
				// over, using the value that should exist
				// after the tag that is just finished
				// "Recursivity" takes place!
				handlevalue(name,parameters,xml);
				status = do_nothing;
				name = "";
				parameters = "";
				break;
		}
		xml.nextchar();
	}
}

void CSerializer::WriteXML(CString filename)
{
	// We create a file exception
	CFileException e;
	CStdioFile file;

	if ( !file.Open(
			filename,
			CFile::modeCreate | CFile::typeBinary | CFile::modeReadWrite | CFile::shareDenyWrite,
			&e ) )
		MessageBox( NULL, "File could not be written: "+filename, "File error", MB_OK );
	else
		file.WriteString( "<?xml version=\"1.0\"?>\r\n"+construct() );
}

void CSerializer::ReadXML(CString filename)
{
	// We create a file exception
	CFileException e;
	CStdioFile file;

	if ( !file.Open(
			filename,
			CFile::modeRead | CFile::shareDenyNone,
			&e ) )
	{
		MessageBox( NULL, "File could not be opened: "+filename+"\n\nA new file will be created", "File error", MB_OK );
		WriteXML( filename );
	}
	else
	{
		CXMLString xml( &file );
		parse( xml );
	}
}

void CSerializer::SetName(CString c)
{
	m_xmlname = c;
}
