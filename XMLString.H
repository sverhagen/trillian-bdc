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
