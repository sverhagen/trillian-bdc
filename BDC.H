// BDC.h : main header file for the BDC DLL
//

#if !defined(AFX_BDC_H__38936207_A22C_454C_B634_7C400D022E05__INCLUDED_)
#define AFX_BDC_H__38936207_A22C_454C_B634_7C400D022E05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

/////////////////////////////////////////////////////////////////////////////
// CBDCApp
// See BDC.cpp for the implementation of this class
//

#ifdef _DEBUG
#define VERSION "Version 0.20 DEBUG"
#endif
#ifndef _DEBUG
#define VERSION "Version 0.20"
#endif

class CBDCApp : public CWinApp
{
public:
	void DrawToday();
	void EnableICQImport();
	void DisableICQImport();
	void ClearToday();
	void UpdateCounter();
	void UpdateIcon(CBirthday*);
	void MakeIconLeft( list_entry_t* pEntry, int nIcon );
	void MakeIconRight( list_entry_t* pEntry, MEDIUM medium );
	void UpdateText(CBirthday*);
	void Alert(CString, void* data=NULL);
	void ClearBirthday(CBirthday *);
	void DrawBirthday(CBirthday *);
	void ClearKnown(CBirthday*);
	void DrawKnown(CBirthday*, bool bSorted=true);
	int listCallback(int windowID, char *subwindow, char *event, void *data, void *userData);
	VOID CALLBACK TimerProcMember(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
	int systrayCallback(char *event, void *data);
	int menuCallback(char *event, void *data);
	int main( char*event, void*data );
	int prefDialog(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
	CBDCApp();
	virtual ~CBDCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBDCApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBDCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	int m_todayID;
	HWND m_prefswnd;
	HBITMAP m_pbmpListentry_today;
	HBITMAP m_pbmpListentry_vcf;
	HBITMAP m_pbmpListentry_wab;
	HBITMAP m_pbmpListentry_known;
	HBITMAP m_pbmpListentry_bd;
	HBITMAP m_pbmpListentry_bd_confirmed;
	int m_knowngroupID;
	int m_sectionID;
	plugin_function_send m_plugin_send;
	CProcessor * m_pproc;
	CString m_homedir;
	void AssignString(char **pstrDestination, const char *strSource);
	void CBDCApp::AddMenu(int id, char* text, menu_entry_t** pmenu_entry, void* pdata=NULL);
	void CBDCApp::FreeMenus(menu_entry_t** pmenu_entry);
	void CBDCApp::ClearItem(int* pid);
	void CBDCApp::FreeIcons(list_entry_t* list_entry);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BDC_H__38936207_A22C_454C_B634_7C400D022E05__INCLUDED_)
