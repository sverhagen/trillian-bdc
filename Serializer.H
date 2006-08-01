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
