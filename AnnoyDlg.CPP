// AnnoyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "incl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnnoyDlg dialog


CAnnoyDlg::CAnnoyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnnoyDlg::IDD, pParent)
{
	m_timer = 0;
	//{{AFX_DATA_INIT(CAnnoyDlg)
	m_text = _T("");
	//}}AFX_DATA_INIT
}


void CAnnoyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnnoyDlg)
	DDX_Control(pDX, IDC_ANIMATE, m_animate);
	DDX_Text(pDX, IDC_ALERTTEXT, m_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnnoyDlg, CDialog)
	//{{AFX_MSG_MAP(CAnnoyDlg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnnoyDlg message handlers

BOOL CAnnoyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_animate.Open( IDR_BALLOONSAVI );
	m_animate.Play(0,-1,-1);

	MessageBeep(MB_ICONHAND);
	m_timer = SetTimer( 1, 3000, NULL );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAnnoyDlg::OnTimer(UINT nIDEvent) 
{
	if( m_timer == nIDEvent )
	{
		MessageBeep(MB_ICONHAND);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CAnnoyDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if( m_timer>0 )
		KillTimer(m_timer);
}
