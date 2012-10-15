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

#if !defined(AFX_ANNOYDLG_H__329198DE_68D2_4DFE_9514_DF4D532D04CB__INCLUDED_)
#define AFX_ANNOYDLG_H__329198DE_68D2_4DFE_9514_DF4D532D04CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnnoyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnnoyDlg dialog

class CAnnoyDlg : public CDialog
{
// Construction
public:
	CAnnoyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnnoyDlg)
	enum { IDD = IDD_ANNOY_DIALOG };
	CAnimateCtrl	m_animate;
	CString	m_text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnnoyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	UINT m_timer;

	// Generated message map functions
	//{{AFX_MSG(CAnnoyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANNOYDLG_H__329198DE_68D2_4DFE_9514_DF4D532D04CB__INCLUDED_)
