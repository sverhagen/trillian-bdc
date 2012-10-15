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
