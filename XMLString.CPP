// XMLString.cpp: implementation of the XMLString class.
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

#define chars_per_read 1000

CXMLString::CXMLString(CFile* pf)
{
	// returncount to registrate that still data is coming out of the file
	int nReturnCount;
	// We are going to retrieve one character per cycle and hold it in czBuffer
	char czBuffer[chars_per_read];

	// Get the first character
	nReturnCount = pf->Read( &czBuffer, chars_per_read );
	// Now get all the other characters
	while( nReturnCount )
	{
		// We add the retreived character to the string
		m_value += ((CString)czBuffer).Left(nReturnCount);
		// And get the next one
		nReturnCount = pf->Read( &czBuffer, chars_per_read );
	}

    m_position = 0;
}

int CXMLString::leftover()
{
    return m_value.GetLength() - m_position;
}

bool CXMLString::equals(CString test)
{
    if( leftover() >= test.GetLength() )
        return m_value.Mid(m_position,test.GetLength()).Compare(test)==0;
    return false;
}

bool CXMLString::equals(char ch)
{
    return value()==ch;
}

char CXMLString::value()
{
    return m_value[m_position];
}

void CXMLString::nextchar()
{
    if( leftover()>0 )
        m_position++;
}

CString CXMLString::nextstring()
{
    int start = m_position;
    while( m_value[++m_position]!='<' );
    return Decode(m_value.Mid(start,m_position-start));
}

CString CXMLString::Encode(CString cURL)
{
	// Replaces all unsafe characters with HTML escape
	// sequences according to W3C specifications
	// For instance: " " --> "%20"

	char hexVals[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
		'A', 'B', 'C', 'D', 'E', 'F' };

	CString cNewURL = "";
	
	// Walk through the provided URL
	for( int i=0; i < cURL.GetLength(); i++ )
	{ 
		unsigned char currentChar = cURL.GetAt(i);
		if( IsUnsafe( currentChar ) )
		{
			// Replace unsafe characters by their safe escape code
			cNewURL.AppendChar('%');
			cNewURL.AppendChar(hexVals[currentChar/16]);
			cNewURL.AppendChar(hexVals[currentChar%16]);
		}
		else
			// Just copy safe characters
			cNewURL.AppendChar(currentChar);
	}

	return cNewURL;
}

CString CXMLString::Decode(CString cSource)
{
	// Replaces all HTML escape sequences with the corresponding
	// unsafe characters according to W3C specifications
	// For instance: "%20" --> " "

	CString cResult = "";
	// Walk through the provided URL
	for( int i=0; i < cSource.GetLength(); i++ )
	{ 
		if( cSource[i]=='%' )
		{
			char c = (char) strtoul( "0x"+cSource.Mid(i+1,2), NULL, 16 );
			i+=2;
			cResult += (CString)c;
		}
		else
			// Just copy safe characters
			cResult += cSource[i];
	}

	return cResult;
}

bool CXMLString::IsUnsafe(unsigned char cCompare)
{
	// This function checks to see if a char is URL unsafe
	// Returns bool result. True = unsafe, False = safe
    //   reserved       = ";" | "/" | "?" | ":" | "@" | "&" | "=" | "+"
    //   extra          = "!" | "*" | "'" | "(" | ")" | ","
    //   safe           = "$" | "-" | "_" | "."
    //   unsafe         = CTL | SP | <"> | "#" | "%" | "<" | ">"

	CString unsafeString;
	unsafeString += ";/?:@&=+"; // reserved
	unsafeString += "\"#%<>";   // unsafe
	//unsafeString += "\\^[]`";
	//unsafeString += "()";
	if( (unsafeString.Find(cCompare)==-1) && (cCompare>32) && (cCompare<123) )
		return false;// found no unsafe chars, return false
	else
		return true;// found unsafe chars, return true
}
