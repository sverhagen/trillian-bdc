// AddBirthdayDlg.h: interface for the CAddBirthdayDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDBIRTHDAYDLG_H__156A2EF6_57B4_43F0_BEE0_B289328B6372__INCLUDED_)
#define AFX_ADDBIRTHDAYDLG_H__156A2EF6_57B4_43F0_BEE0_B289328B6372__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAddBirthdayDlg : public CSetBirthdayDlg  
{
public:
	CAddBirthdayDlg(CWnd* pParent);   // standard constructor
	CAddBirthdayDlg(CWnd* pParent, CString name, BYTE day, BYTE month, UINT year);   // standard constructor

protected:
	virtual BOOL OnInitDialog();
};

#endif // !defined(AFX_ADDBIRTHDAYDLG_H__156A2EF6_57B4_43F0_BEE0_B289328B6372__INCLUDED_)
