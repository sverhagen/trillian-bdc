// AddBirthdayDlg.cpp: implementation of the CAddBirthdayDlg class.
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

CAddBirthdayDlg::CAddBirthdayDlg(CWnd* pParent)
	: CSetBirthdayDlg(pParent)
{
}

CAddBirthdayDlg::CAddBirthdayDlg(CWnd* pParent, CString name, BYTE day, BYTE month, UINT year)
	: CSetBirthdayDlg(pParent, name, day, month, year)
{
}

BOOL CAddBirthdayDlg::OnInitDialog()
{
	BOOL b = CSetBirthdayDlg::OnInitDialog();
	SetWindowText( "Add new birthday" );
	return b;
}
