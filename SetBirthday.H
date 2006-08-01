#if !defined(AFX_SETBIRTHDAY_H__D1966A9A_9943_4F40_BBE3_4C50B7337F7A__INCLUDED_)
#define AFX_SETBIRTHDAY_H__D1966A9A_9943_4F40_BBE3_4C50B7337F7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetBirthday.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetBirthdayDlg dialog

class CSetBirthdayDlg : public CDialog
{
// Construction
public:
	UINT m_month;
	UINT m_day;
	UINT m_year;
	CSetBirthdayDlg(CWnd* pParent);   // standard constructor
	CSetBirthdayDlg(CWnd* pParent, CString name, BYTE day, BYTE month, UINT year);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetBirthdayDlg)
	enum { IDD = IDD_SETBIRTHDAY };
	CString	m_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetBirthdayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnOK();

	// Generated message map functions
	//{{AFX_MSG(CSetBirthdayDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETBIRTHDAY_H__D1966A9A_9943_4F40_BBE3_4C50B7337F7A__INCLUDED_)
