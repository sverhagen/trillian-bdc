// Birthday.h: interface for the CBirthday class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BIRTHDAY_H__D58325F4_A226_4021_80B9_63BBE7A9D810__INCLUDED_)
#define AFX_BIRTHDAY_H__D58325F4_A226_4021_80B9_63BBE7A9D810__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum MEDIUM
{
	MEDIUM_NONE,
	MEDIUM_VCARD,
	MEDIUM_ICQ,
	MEDIUM_WAB
};

enum IMPORT
{
	IMPORT_SUCCESS,
	IMPORT_BADREPLY,
	IMPORT_BADICQ,
	IMPORT_NOBIRTHDAY,
	IMPORT_BADFILE,
	IMPORT_NONAME
};

class CBirthday : CSerializer
{
public:
	CString GetText(SUFFIX sx = SUFFIX_NONE, bool left = false);
	bool Reload();
	static void FinalCleanup();
	IMPORT LoadVCF(CString vcf_file);
	IMPORT LoadICQ( char* icqno, char * name );
	virtual void handlevalue(CString name, CString parameters, CXMLString&);
	virtual ~CBirthday();
	CString GetTooltip();
	CDate m_date;
	CString m_name;
	CString m_mediumname;
	MEDIUM m_medium;
	int m_listid_known;
	int m_listid_bd;
	bool m_confirmed;
	bool m_alerted_this_year;
	CDate m_recent_alert;
	CBirthday();
	CBirthday(CXMLString&);
	virtual CString construct();
private:
	static CInternetSession* m_pSession;
protected:
	CString GetSuffix(SUFFIX);
};

#endif // !defined(AFX_BIRTHDAY_H__D58325F4_A226_4021_80B9_63BBE7A9D810__INCLUDED_)
