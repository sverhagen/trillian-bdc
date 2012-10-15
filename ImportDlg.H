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

#if !defined(AFX_IMPORTDLG_H__45936EB3_47C9_4D71_9D15_C69F16A9D36F__INCLUDED_)
#define AFX_IMPORTDLG_H__45936EB3_47C9_4D71_9D15_C69F16A9D36F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImportDlg dialog

enum DLG_REPLIES
{
	DLG_RETRY,
	DLG_SKIP,
	DLG_CANCEL
};

class CImportDlg : public CDialog
{
// Construction
public:
	~CImportDlg();
	static int contactCallback(int windowID, char *subwindow, char *event, void *data, void *userData);
	CImportDlg(CWnd* pParent, CProcessor* proc);

// Dialog Data
	//{{AFX_DATA(CImportDlg)
	enum { IDD = IDD_IMPORT };
	CButton	m_cancel;
	CButton	m_skip;
	CButton	m_retry;
	CListBox	m_report;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	static UINT Handle(LPVOID pParam);
	POSITION m_position;
	static UINT ThreadFunction(LPVOID pParam);
	DLG_REPLIES m_lastbutton;
	static CList<contactlist_entry_t,contactlist_entry_t&>* m_pcontactstohandle;
	CProcessor* m_pproc;

	// Generated message map functions
	//{{AFX_MSG(CImportDlg)
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnRetry();
	afx_msg void OnSkip();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTDLG_H__45936EB3_47C9_4D71_9D15_C69F16A9D36F__INCLUDED_)
