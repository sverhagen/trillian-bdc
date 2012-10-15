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

// SetBirthday.cpp : implementation file
//

#include "stdafx.h"
#include "incl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetBirthdayDlg dialog


CSetBirthdayDlg::CSetBirthdayDlg(CWnd* pParent)
	: CDialog(CSetBirthdayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetBirthdayDlg)
	m_name = "";
	//}}AFX_DATA_INIT
	m_month = (BYTE)CTime::GetCurrentTime().GetMonth();
	m_day = (BYTE)CTime::GetCurrentTime().GetDay();
	m_year = CTime::GetCurrentTime().GetYear();
}

CSetBirthdayDlg::CSetBirthdayDlg(CWnd* pParent, CString name, BYTE day, BYTE month, UINT year)
	: CDialog(CSetBirthdayDlg::IDD, pParent)
{
	m_name = name;
	m_month = month;
	m_day = day;
	m_year = year;
}


void CSetBirthdayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetBirthdayDlg)
	DDX_Text(pDX, IDC_NAME, m_name);
	//}}AFX_DATA_MAP
	if( pDX->m_bSaveAndValidate )
	{
		m_day = (BYTE)GetDlgItemInt( IDC_BD_DAY );
		m_year = (UINT)GetDlgItemInt( IDC_BD_YEAR );
		m_month = ((CComboBox*)GetDlgItem( IDC_BD_MONTH ))->GetCurSel()+1;
	}
	else
	{
		SetDlgItemInt( IDC_BD_DAY, (int)m_day, FALSE );
		SetDlgItemInt( IDC_BD_YEAR, (int)m_year, FALSE );
		((CComboBox*)GetDlgItem( IDC_BD_MONTH ))->SetCurSel( m_month-1 );
	}
}


BEGIN_MESSAGE_MAP(CSetBirthdayDlg, CDialog)
	//{{AFX_MSG_MAP(CSetBirthdayDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetBirthdayDlg message handlers


void CSetBirthdayDlg::OnOK()
{
	bool bError = false;

	UpdateData();

	m_name.TrimLeft();
	m_name.TrimRight();
	if( m_name.GetLength()==0 ) bError = true;
	if( m_day==0 ) bError = true;
	switch( m_month )
	{
		case 2:
			if( m_day>29 ) bError = true;
			break;
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			if( m_day>31 ) bError = true;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			if( m_day>30 ) bError = true;
			break;
		default:
			bError = true;
	}
	if( m_year>(UINT)CTime::GetCurrentTime().GetYear() ) bError = true;

	if( bError )
		MessageBeep(MB_ICONHAND);
	else
		CDialog::OnOK();
}

BOOL CSetBirthdayDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	((CEdit*)GetDlgItem( IDC_BD_DAY ))->LimitText(2);
	((CEdit*)GetDlgItem( IDC_BD_YEAR ))->LimitText(4);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
