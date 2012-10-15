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
