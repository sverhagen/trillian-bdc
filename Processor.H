// Processor.h: interface for the CProcessor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESSOR_H__718B16F1_439D_4863_B2F4_7829114FD00F__INCLUDED_)
#define AFX_PROCESSOR_H__718B16F1_439D_4863_B2F4_7829114FD00F__INCLUDED_

#include "Config.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBDCApp;
class CProcessor  
{
public:
	SUFFIX GetSuffix();
	int GetNextID(CDate);
	void Reload(CBirthday*);
	bool DrawRightIcons();
	bool DrawLeftInfo();
	void DepopulateBirthdays();
	static void FinalCleanup();
	bool WABExists(CBirthday* pbd);
	void ImportWAB();
	void ImportVCF();
	void DisableICQImport();
	void EnableICQImport();
	bool ICQExists(contactlist_entry_t *pentry);
	CString GetKnownCount();
	int prefDialog(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam, HMODULE hHandle);
	void Edit(CBirthday*);
	void Confirm(CBirthday* pbd);
	void DepopulateKnowns();
	void PopulateKnowns();
	void ProcessAction(bool bForce = false);
	int GetNextID(CDate*);
	int GetPreviousID(CBirthday*);
	void Store();
	void Remove( CBirthday*);
	void Add(CBirthday*);
	void PrefsAction(HWND);
	void PrefsShow(HWND);
	CProcessor(CString homedir, CBDCApp* parent);
	virtual ~CProcessor();
protected:
	CBDCApp* m_pApp;
	int m_hours;
	UINT m_maintimer;
	CBirthdays* m_pbds;
	CConfig* m_pconfig;
};

#endif // !defined(AFX_PROCESSOR_H__718B16F1_439D_4863_B2F4_7829114FD00F__INCLUDED_)
