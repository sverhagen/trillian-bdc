// ImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "incl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportDlg dialog


CImportDlg::CImportDlg(CWnd* pParent, CProcessor* proc)
	: CDialog(CImportDlg::IDD, pParent)
{
	m_pproc = proc;
	if( m_pcontactstohandle )
		m_position = m_pcontactstohandle->GetHeadPosition();
	//{{AFX_DATA_INIT(CImportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportDlg)
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_SKIP, m_skip);
	DDX_Control(pDX, IDC_RETRY, m_retry);
	DDX_Control(pDX, IDC_REPORT, m_report);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportDlg, CDialog)
	//{{AFX_MSG_MAP(CImportDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RETRY, OnRetry)
	ON_BN_CLICKED(IDC_SKIP, OnSkip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportDlg message handlers

void CImportDlg::OnClose() 
{
	if( m_cancel.IsWindowEnabled() )
		CDialog::OnClose();
}


BOOL CImportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_skip.EnableWindow( FALSE );
	m_retry.SetWindowText("&Start");
	m_pproc->DisableICQImport();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CList<contactlist_entry_t,contactlist_entry_t&>* CImportDlg::m_pcontactstohandle = NULL;
int CImportDlg::contactCallback(int windowID, char *subwindow, char *event, void *data, void *userData)
{
	struct contactlist_entry_t	*pentry = (struct contactlist_entry_t *)data;

	if(!strcmp(event, "enum_start"))
	{
		if( !m_pcontactstohandle )
			m_pcontactstohandle = new CList<contactlist_entry_t,contactlist_entry_t&>();
	}
	else if(!strcmp(event, "enum_finish"))
	{
		AfxBeginThread( ThreadFunction, userData );
	}
	else if(!strcmp(event, "enum_add"))
	{
		if( m_pcontactstohandle )
			m_pcontactstohandle->AddTail( *pentry );
	}
	return 0;
}

void CImportDlg::OnRetry() 
{
	m_retry.SetWindowText("&Retry");
	AfxBeginThread( Handle, this );
}

void CImportDlg::OnSkip() 
{
	m_pcontactstohandle->GetNext( m_position );
	AfxBeginThread( Handle, this );
}

void CImportDlg::OnCancel() 
{
	CDialog::OnCancel();
}

UINT CImportDlg::ThreadFunction(LPVOID pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( )); 
	CImportDlg* pd = new CImportDlg(AfxGetMainWnd(), (CProcessor*)pParam);
	pd->DoModal();
	delete pd;
	return 0; // error level
}

CImportDlg::~CImportDlg()
{
	if( m_pcontactstohandle )
	{
		delete m_pcontactstohandle;
		m_pcontactstohandle = NULL;
	}
	ASSERT(m_pproc);
	if( m_pproc )
		m_pproc->EnableICQImport();
	m_pproc = NULL;
}

UINT CImportDlg::Handle(LPVOID pParam)
{
	CImportDlg* pDlg = (CImportDlg*)pParam;

	pDlg->m_skip.EnableWindow(FALSE);
	pDlg->m_retry.EnableWindow(FALSE);
	pDlg->m_cancel.EnableWindow(FALSE);
	while( pDlg->m_pcontactstohandle && pDlg->m_position )
	{
		if( pDlg->m_position )
		{
			contactlist_entry_t cle = pDlg->m_pcontactstohandle->GetAt( pDlg->m_position );
			if( !pDlg->m_pproc->ICQExists(&cle) )
			{
				// Load the icq info
				CBirthday* b = new CBirthday();
				switch( b->LoadICQ( cle.real_name, cle.name ) )
				{
					case IMPORT_BADREPLY:
						pDlg->m_report.SetTopIndex(
							pDlg->m_report.AddString(CString(cle.real_name) + "  --  Bad reply, try again (after a bit)") );
						delete b;
						pDlg->SetForegroundWindow();
						pDlg->SetFocus();
						pDlg->m_retry.SetFocus();
						MessageBeep(MB_ICONHAND);
						pDlg->m_skip.EnableWindow();
						pDlg->m_retry.EnableWindow();
						pDlg->m_cancel.EnableWindow();
						return 0;
					case IMPORT_BADICQ:
						pDlg->m_report.SetTopIndex(
							pDlg->m_report.AddString(CString(cle.real_name) + "  --  Not a registered ICQ number") );
						delete b;
						break;
					case IMPORT_NOBIRTHDAY:
						pDlg->m_report.SetTopIndex(
							pDlg->m_report.AddString(CString(cle.real_name) + "  --  No birthday information found") );
						delete b;
						break;
					case IMPORT_SUCCESS:
						pDlg->m_report.SetTopIndex(
							pDlg->m_report.AddString(CString(cle.real_name) + "  --  Successfully imported") );
						pDlg->m_pproc->Add( b );
						break;
				}
			}
			pDlg->m_pcontactstohandle->GetNext( pDlg->m_position );
		}
	}
	pDlg->m_report.SetTopIndex(
		pDlg->m_report.AddString("") );
	pDlg->m_report.SetTopIndex(
		pDlg->m_report.AddString(">> ALL DONE! <<") );
	pDlg->m_cancel.SetWindowText("&Close");
	pDlg->m_cancel.EnableWindow();

	return 0;
}
