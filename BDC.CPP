// BDC.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "incl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CBDCApp

BEGIN_MESSAGE_MAP(CBDCApp, CWinApp)
	//{{AFX_MSG_MAP(CBDCApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBDCApp construction

int sb=1;
char* p1 = "ICQ";
char* p2 = "select";
char* p3 = "online";

CBDCApp::CBDCApp()
{
	m_prefswnd = NULL;
	m_pproc = NULL;
	m_pbmpListentry_bd = NULL;
	m_pbmpListentry_bd_confirmed = NULL;
	m_pbmpListentry_today = NULL;
	m_pbmpListentry_known = NULL;
	m_pbmpListentry_vcf = NULL;
	m_pbmpListentry_wab = NULL;
	m_knowngroupID = -1;
	m_sectionID = -1;
	m_todayID = -1;
}

CBDCApp::~CBDCApp()
{
	CBirthday::FinalCleanup();
	CProcessor::FinalCleanup();
}

void CBDCApp::AssignString(char **pstrDestination, const char *strSource)
{
	int len = strlen(strSource)+1;
	*pstrDestination = (char *)malloc(len);
	strcpy_s(*pstrDestination, len, strSource);
}

int CBDCApp::main(char *event, void *data)
{
	if( !_stricmp(event,"load") )
	{ 
	   struct plugin_info_t *pi = (struct plugin_info_t *)data;

	   if( pi )
	   {
			// Fill in plugin information
			strcpy_s(pi->guid, MYGUID); 
			strcpy_s(pi->name, "Birthday Calendar"); 
			strcpy_s(pi->company, "Totaal Software"); 
			strcpy_s(pi->version, VERSION); 
			strcpy_s(pi->description, "Keep track of your friends' birthdays.");
         
			// Extract what we need and store locally
			m_plugin_send = pi->plugin_send;
			m_homedir = CString( pi->config_directory );
	   }
	}
	else if( !_stricmp(event, "initialLoad"))
	{
		// First time a plugin is loaded
		// Can do install wizardish stuff here
	}
	else if( !_stricmp(event, "start") )
	{
		// Initialize
		if (m_pbmpListentry_vcf == NULL)
			m_pbmpListentry_vcf = LoadBitmap(m_hInstance, MAKEINTRESOURCE(IDB_VCF));
		if (m_pbmpListentry_wab == NULL)
			m_pbmpListentry_wab = LoadBitmap(m_hInstance, MAKEINTRESOURCE(IDB_WAB));
		if (m_pbmpListentry_bd == NULL)
			m_pbmpListentry_bd = LoadBitmap(m_hInstance, MAKEINTRESOURCE(IDB_BALLOONS));
		if (m_pbmpListentry_bd_confirmed == NULL)
			m_pbmpListentry_bd_confirmed = LoadBitmap(m_hInstance, MAKEINTRESOURCE(IDB_BALLOONS_CONFIRMED));
		if (m_pbmpListentry_known == NULL)
			m_pbmpListentry_known = LoadBitmap(m_hInstance, MAKEINTRESOURCE(IDB_BALLOONS_KNOWN));
		if (m_pbmpListentry_today == NULL)
			m_pbmpListentry_today = LoadBitmap(m_hInstance, MAKEINTRESOURCE(IDB_TODAY));
		if (m_pproc == NULL)
			m_pproc = new CProcessor(m_homedir,this);

		struct plugin_prefs_t pp;
		trillianInitialize(pp);
		// Initialize preferences.
		pp.enabled		= 1;
		pp.pref_name	= "Birthday Calendar"; 
		pp.sub_entry	= NULL; 
		m_plugin_send(MYGUID, "prefsInitialize", &pp); 

		// Populate contact list

		// Add the main section.
		{
			struct list_entry_t ple; 
			trillianListInitialize(ple);
			ple.previous_id		= -2;	// This controls placement of new entry, before (-2) or last (-1)
			ple.section			= 1;	// This is a section
			ple.expanded		= 1;	
			ple.tooltip			= "Birthday Calendar";
			ple.text			= "Birthdays";
			ple.real_name		= "birthdays";
int i=			m_plugin_send(MYGUID, "listAddEntry", (void *)&ple); 
				
			// Set our global sectionID for later. Note that we get the
			// sectionID from the structure after it's been populated
			m_sectionID = ple.section_id;
		}

		// Add the 'known' group
		// Flip section/group around
		// Group ID is the RETURN value, not in struct
		{
			struct list_entry_t ple; 
			trillianListInitialize(ple);
			ple.group			= 1;
			ple.section			= 0;
			ple.tooltip			= "Known Birthdays";
			ple.text			= "Known Birthdays"; 
			ple.section_id		= m_sectionID;

			// menu item: "ADD"
			ple.menu_entry = (struct menu_entry_t *)malloc(sizeof(struct menu_entry_t));
			trillianInitialize((*ple.menu_entry));
			
			ple.menu_entry->menu_id = MENU_ADD;
			ple.menu_entry->type = MENU_TEXT;
			AssignString(&ple.menu_entry->text, "&Add...");
			ple.menu_entry->callback = ::menuCallback;

			m_knowngroupID = m_plugin_send(MYGUID, "listAddEntry", (void *)&ple);

			free(ple.menu_entry->text);
			free(ple.menu_entry);
		}

		ASSERT(m_pproc);
		if( m_pproc )
		{
			m_pproc->PopulateKnowns();
			m_pproc->ProcessAction();
		}
	}
	else if( !_stricmp(event,"stop") && m_plugin_send )
	{
		// Remove list items
		ASSERT(m_pproc);
		if( m_pproc )
			m_pproc->DepopulateKnowns();
		// Remove the 'known' group
		{
			struct list_entry_t ple;
			trillianListInitialize(ple);
			ple.unique_id = m_knowngroupID;
			ple.section_id = -1;
			ple.parent_id = -1;
			ple.previous_id = -1;
			m_plugin_send(MYGUID, "listRemoveEntry", (void *)&ple);
		}
		// Remove the section
		{
			struct list_entry_t ple;
			trillianListInitialize(ple);
			ple.unique_id = -1;
			ple.section_id = m_sectionID;
			ple.parent_id = -1;
			ple.previous_id = -1;
			m_plugin_send(MYGUID, "listRemoveEntry", (void *)&ple);
		}
		// Clear up some variables
		if(m_pbmpListentry_vcf)
		{
			DeleteObject(m_pbmpListentry_vcf);
			m_pbmpListentry_vcf = NULL;
		}
		if(m_pbmpListentry_wab)
		{
			DeleteObject(m_pbmpListentry_wab);
			m_pbmpListentry_wab = NULL;
		}
		if(m_pbmpListentry_bd)
		{
			DeleteObject(m_pbmpListentry_bd);
			m_pbmpListentry_bd = NULL;
		}
		if(m_pbmpListentry_bd_confirmed)
		{
			DeleteObject(m_pbmpListentry_bd_confirmed);
			m_pbmpListentry_bd_confirmed = NULL;
		}
		if(m_pbmpListentry_known)
		{
			DeleteObject(m_pbmpListentry_known);
			m_pbmpListentry_known = NULL;
		}
		if(m_pbmpListentry_today)
		{
			DeleteObject(m_pbmpListentry_today);
			m_pbmpListentry_today = NULL;
		}
		if(m_pproc)
		{
			delete m_pproc;
			m_pproc = NULL;
		}
	}
	else if( !_stricmp(event, "unload") )
	{
		m_plugin_send = NULL;
	}
	else if( !_stricmp(event, "prefsShow") )
	{
		struct plugin_prefs_show_t	*pps = (struct plugin_prefs_show_t *)data;

		// Trillian is telling us to show our preferences
		// dialog do so only if we find our name

		if( !strcmp(pps->pref_name, "Birthday Calendar") )
		{
			int prefDialog();

			strcpy_s(pps->prefs_info.name, "Birthday Calendar");
			strcpy_s(pps->prefs_info.description, "Keep track of your friends' birthdays.");
			pps->prefs_info.bitmap = LoadBitmap(m_hInstance, MAKEINTRESOURCE(IDB_PREFSLOGO));

			if( pps->show==1 )
			{
				if( m_prefswnd==NULL )
				{
					struct dialog_entry_t det;
					trillianInitialize(det);						
					det.hwnd = CreateDialog(m_hInstance, MAKEINTRESOURCE(IDD_PREFS_DIALOG), pps->hwnd, (DLGPROC)::prefDialog);
					m_plugin_send(MYGUID, "dialogAdd", &det);   
					m_prefswnd = det.hwnd;

					SetWindowPos(m_prefswnd, 0, pps->x, pps->y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

					ASSERT(m_pproc);
					if( m_pproc )
						m_pproc->PrefsShow(m_prefswnd);
				}
				ShowWindow(m_prefswnd, SW_SHOW);
			}else
			if( pps->show==0 )
			{
				if( m_prefswnd!=NULL )
					ShowWindow(m_prefswnd, SW_HIDE);
			}
		}
	}
	else if( !_stricmp(event, "prefsAction") )
	{
		struct plugin_prefs_action_t *ppa
			= (struct plugin_prefs_action_t *)data;

		// Take action:
		// 0 - Cancel
		// 1 - Apply
		// 2 - OK

		if( ppa->type==0 )
		{
			if( m_prefswnd!=NULL )
			{
				struct dialog_entry_t det;
				trillianInitialize( det );
				det.hwnd = m_prefswnd;
				m_plugin_send(MYGUID, "dialogRemove", &det);
				DestroyWindow(m_prefswnd);
				m_prefswnd = NULL;
			}
		}
		if( (ppa->type==1) || (ppa->type==2) )
		{
			ASSERT(m_pproc);
			if( m_prefswnd && m_pproc )
			{
				m_pproc->PrefsAction(m_prefswnd);
			}
		}
		if( ppa->type==2 )
		{
			if( m_prefswnd!=NULL )
			{
				struct dialog_entry_t det;
				trillianInitialize( det );
				det.hwnd = m_prefswnd;
				m_plugin_send(MYGUID, "dialogRemove", &det);
				DestroyWindow(m_prefswnd);
				m_prefswnd = NULL;
			}
		}		
	}

	return 0; 
}

int CBDCApp::menuCallback(char *event, void *data)
{
	struct menu_entry_t *item;
	item = (struct menu_entry_t *)data;

	if( !strcmp(event, "menu-select") )
	{
		switch( (MENU)(UINT)(void*)item->menu_id )
		{
			case MENU_ADD:
				{
					ASSERT(m_pproc);
					if( m_pproc )
					{
						AFX_MANAGE_STATE(AfxGetStaticModuleState( )); 
						CAddBirthdayDlg bddlg( AfxGetMainWnd() );
						if( bddlg.DoModal() == IDOK )
						{
							CBirthday * pbd = new CBirthday;
							// Store the data
								pbd->m_name = bddlg.m_name;
								pbd->m_date = CDate(bddlg.m_day,bddlg.m_month,bddlg.m_year);
								m_pproc->Add( pbd );
						}
					}
				}
				break;
			case MENU_REMOVE:
				{
					CBirthday * pbd = (CBirthday*)item->data;
					// Remove from data
					ASSERT(m_pproc);
					if( m_pproc ) m_pproc->Remove( pbd );
				}
				break;
			case MENU_EDIT:
				{
					CBirthday * pbd = (CBirthday*)item->data;
					ASSERT(m_pproc);
					if( m_pproc ) m_pproc->Edit(pbd);
				}
				break;
			case MENU_RELOAD:
				{
					CBirthday * pbd = (CBirthday*)item->data;
					ASSERT(m_pproc);
					if( m_pproc ) m_pproc->Reload(pbd);
				}
				break;
			case MENU_CONFIRM:
				{
					CBirthday * pbd = (CBirthday*)item->data;
					ASSERT(m_pproc);
					if( m_pproc ) m_pproc->Confirm(pbd);
				}
				break;
			default:
				break;
		}

	}
	return 0;
}

void CBDCApp::AddMenu(int id, char* text, menu_entry_t** pmenu_entry, void* pdata)
{
	menu_entry_t** pentry = pmenu_entry;
	while( *pentry != NULL )
	{
		pentry = &(*pentry)->next_menu;
	}

	*pentry = (struct menu_entry_t *)malloc(sizeof(struct menu_entry_t));
	trillianInitialize((**pentry));

	(*pentry)->menu_id = id;
	(*pentry)->type = MENU_TEXT;
	AssignString(&(*pentry)->text, text);
	(*pentry)->callback = ::menuCallback;
	(*pentry)->data = (void*)pdata;
}

void CBDCApp::FreeMenus(menu_entry_t** pmenu_entry)
{
	if( (*pmenu_entry)->next_menu != NULL )
	{
		FreeMenus(&(*pmenu_entry)->next_menu);
	}
	if( (*pmenu_entry)->text )
		free((*pmenu_entry)->text);
	free(*pmenu_entry);
}

void CBDCApp::DrawKnown(CBirthday *pbd, bool bSorted/*=true*/)
{
	struct list_entry_t ple;

	trillianListInitialize(ple);

	ple.section_id		= m_sectionID;
	ple.parent_id		= m_knowngroupID;
	ple.unique_id		= -1; 
	ple.group			= 0;
	ple.section			= 0; 
	ple.inline_editing	= 0;
	ple.drag_and_drop	= 0; 
	ple.expanded		= 1; 
	ple.callback		= ::listCallback;

	ASSERT( m_pproc );
	if( bSorted && m_pproc )
		ple.previous_id	= m_pproc->GetPreviousID(pbd);
	else
		ple.previous_id	= -1;

	AssignString(&ple.tooltip, pbd->GetTooltip());
	ASSERT(m_pproc);
	if( m_pproc )
	{
		AssignString(&ple.text, pbd->GetText(m_pproc->GetSuffix(), m_pproc->DrawLeftInfo()));
	}
	else
	{
		AssignString(&ple.text, pbd->GetText(SUFFIX_NONE));
	}

	ple.data			= (void*)pbd;

	// left icon
	ple.left_icons		= (struct list_bmp_t *)malloc(sizeof(struct list_bmp_t));
	trillianInitialize((*ple.left_icons));
	
	ple.left_icons->num_states	= 1;

	ple.left_icons->bitmap		= (HBITMAP *)malloc(sizeof(HBITMAP));
	ple.left_icons->bitmap[0]	= m_pbmpListentry_known;

	ple.left_icons->location	= (RECT *)malloc(sizeof(RECT));
	ple.left_icons->location[0].left	=  0;
	ple.left_icons->location[0].top		=  0;
	ple.left_icons->location[0].bottom	= 18;
	ple.left_icons->location[0].right	= 18;

	ple.num_left_icons = 1;

	// right icon
	MakeIconRight( &ple,
		pbd->m_medium);

	AddMenu(MENU_ADD, "&Add...", &ple.menu_entry);
	AddMenu(MENU_EDIT, "&Edit...", &ple.menu_entry, pbd);
	AddMenu(MENU_REMOVE, "&Remove...", &ple.menu_entry, pbd);

	if( pbd->m_medium == MEDIUM_ICQ )
	{
		AddMenu(MENU_RELOAD, "Re&load...", &ple.menu_entry, pbd);
	}

	// You'll need the UniqueID for any further manipulation, so store it somewhere nice
	pbd->m_listid_known = m_plugin_send(MYGUID, "listAddEntry", (void *)&ple); 

	FreeMenus(&ple.menu_entry);
	free(ple.text);
	free(ple.tooltip);
	free(ple.left_icons->bitmap);
	free(ple.left_icons->location);
	free(ple.left_icons);

	if(ple.right_icons)
	{
		if(ple.right_icons->bitmap)
			free(ple.right_icons->bitmap);
		if(ple.right_icons->location)
			free(ple.right_icons->location);
		free(ple.right_icons);
	}
}

void CBDCApp::DrawBirthday(CBirthday *pbd)
{
	struct list_entry_t ple;

	trillianListInitialize(ple);

	ple.previous_id		= -2; 
	ple.section_id		= m_sectionID;
	ple.unique_id		= -1; 
	ple.group			= 0;
	ple.section			= 0; 
	ple.inline_editing	= 0;
	ple.drag_and_drop	= 0; 
	ple.expanded		= 1; 
	ple.callback		= ::listCallback;

	AssignString(&ple.tooltip, pbd->GetTooltip());
	ASSERT(m_pproc);
	if( m_pproc )
	{
		AssignString(&ple.text, pbd->GetText(m_pproc->GetSuffix(), m_pproc->DrawLeftInfo()));
	}
	else
	{
		AssignString(&ple.text, pbd->GetText(SUFFIX_NONE));
	}

	ple.data			= (void*)pbd;

	// icon
	MakeIconLeft( &ple, pbd->m_confirmed?1:0);

	// right icon
	MakeIconRight( &ple, pbd->m_medium);

	if(pbd->m_date.IsDateOfYear())
	{
		ple.font.flags = 0x1;
		ple.font.skin_name = "recent-online";
		ple.font.select_fore = "recent-online-fore";
		ple.font.hover_fore = "recent-online-fore";
		ple.font.normal_fore = "recent-online-fore";
	}

	AddMenu(MENU_EDIT, "&Edit...", &ple.menu_entry, pbd);
	AddMenu(MENU_CONFIRM, "&Confirm...", &ple.menu_entry, pbd);
	AddMenu(MENU_REMOVE, "&Remove...", &ple.menu_entry, pbd);

	if( pbd->m_medium == MEDIUM_ICQ )
	{
		AddMenu(MENU_RELOAD, "&Reload...", &ple.menu_entry, pbd);
	}

	// You'll need the UniqueID for any further manipulation, so store it somewhere nice
	pbd->m_listid_bd = m_plugin_send(MYGUID, "listAddEntry", (void *)&ple);

	// Free what needs to be freed
	FreeMenus(&ple.menu_entry);
	free(ple.text);
	free(ple.tooltip);
	free(ple.left_icons->bitmap);
	free(ple.left_icons->location);
	free(ple.left_icons);

	if(ple.right_icons)
	{
		if(ple.right_icons->bitmap)
			free(ple.right_icons->bitmap);
		if(ple.right_icons->location)
			free(ple.right_icons->location);
		free(ple.right_icons);
	}
}

void CBDCApp::ClearKnown(CBirthday *pbd)
{
	ClearItem(&pbd->m_listid_known);
}

void CBDCApp::ClearBirthday(CBirthday *pbd)
{
	ClearItem(&pbd->m_listid_bd);
}

void CBDCApp::ClearToday()
{
	ClearItem(&m_todayID);
}

void CBDCApp::ClearItem(int* pid)
{
	if( *pid != -1 )
	{
		struct list_entry_t ple;
		trillianListInitialize(ple);
		ple.unique_id = *pid;
		ple.section_id = -1;
		ple.parent_id = -1;
		ple.previous_id = -1;
		m_plugin_send(MYGUID, "listRemoveEntry", (void *)&ple);
		// Clear field in data that linked to listed entry
		*pid = -1;
	}
}

VOID CALLBACK CBDCApp::TimerProcMember(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	ASSERT(m_pproc);
	if( m_pproc )
		m_pproc->ProcessAction();
}

int CBDCApp::systrayCallback(char *event, void *data)
{
	// Empty; just to allow for clearing systray alert
	return 0;
}

int CBDCApp::prefDialog(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch( message )
	{
		case WM_COMMAND:
			switch( (int)LOWORD(wparam) )
			{
				case IDC_SUFFIX_NONE:
					EnableWindow(
						GetDlgItem( hWnd, IDC_DRAWLEFTINFO ), FALSE);
					break;
				case IDC_SUFFIX_AGE:
				case IDC_SUFFIX_YOB:
				case IDC_SUFFIX_YOB_SHORT:
				case IDC_SUFFIX_DATE:
					EnableWindow(
						GetDlgItem( hWnd, IDC_DRAWLEFTINFO ), TRUE);
					break;
				case IDC_IMPORTICQ:
					{
						struct contactlist_enum_t	cet2;
						trillianInitialize(cet2);
						cet2.connection_id = -1;			// No specific CID requested.
						cet2.medium = "ICQ";
						cet2.callback = CImportDlg::contactCallback;
						cet2.data = m_pproc;

						m_plugin_send(MYGUID, "contactlistEnumerate", &cet2);
					}
					return 0;
				case IDC_IMPORTVCF:
					{
						ASSERT(m_pproc);
						if(m_pproc) m_pproc->ImportVCF();
					}
					return 0;
				case IDC_IMPORTWAB:
					{
						ASSERT(m_pproc);
						if(m_pproc) m_pproc->ImportWAB();
					}
					return 0;
				default:
					break;
			}
		default:
			break;
	}

	ASSERT(m_pproc);
	if( m_pproc ) return m_pproc->prefDialog(hWnd, message, wparam, lparam, m_hInstance);
	return 0;
}

int CBDCApp::listCallback(int windowID, char *subwindow, char *event, void *data, void *userData)
{
	if( !strcmp(event, "list_leftDoubleClick") )
	{
		ASSERT(m_pproc);
		if( m_pproc )
			m_pproc->Edit( (CBirthday*) userData );
	}
	return 0;
}

void CBDCApp::Alert(CString text, void* data/*=NULL*/)
{
	alert_t at;
	trillianInitialize(at);
	// Make a systray alert
	at.text = text.GetBuffer(0);
	at.type = "default";
	at.callback = ::systrayCallback;
	at.data = data;

	m_plugin_send(MYGUID, "systraySetAlert",&at);
}

void CBDCApp::UpdateText(CBirthday *pbd)
{
	struct list_entry_t ple;
	trillianListInitialize(ple);
	// Fill new list item structure
	AssignString(&ple.tooltip, pbd->GetTooltip());
	ple.tooltip = (char *)malloc(strlen(pbd->GetTooltip())+1);
	ASSERT(m_pproc);
	if( m_pproc )
	{
		AssignString(&ple.text, pbd->GetText(m_pproc->GetSuffix(), m_pproc->DrawLeftInfo()));
	}
	else
	{
		AssignString(&ple.text, pbd->GetText(SUFFIX_NONE));
	}

	// Set the text and the tooltip
	if( pbd->m_listid_known >= 0 )
	{
		ple.unique_id = pbd->m_listid_known;
		m_plugin_send(MYGUID, "listUpdateText", (void *)&ple);
		m_plugin_send(MYGUID, "listUpdateTooltip", (void *)&ple);
	}
	if( pbd->m_listid_bd >= 0 )
	{
		ple.unique_id = pbd->m_listid_bd;
		m_plugin_send(MYGUID, "listUpdateText", (void *)&ple);
		m_plugin_send(MYGUID, "listUpdateTooltip", (void *)&ple);
	}
	free( ple.tooltip );
	free( ple.text );
}

void CBDCApp::UpdateCounter()
{
	ASSERT( m_pproc );
	if(m_pproc)
	{
		struct list_entry_t ple;
		trillianListInitialize(ple);
		CString c("Known Birthdays ("+m_pproc->GetKnownCount()+")");
		// Fill new list item structure
		ple.tooltip = c.GetBuffer(0);
		ple.text = c.GetBuffer(0);
		ple.unique_id = m_knowngroupID;
		// Set the text and the tooltip
		m_plugin_send(MYGUID, "listUpdateText", (void *)&ple);
		m_plugin_send(MYGUID, "listUpdateTooltip", (void *)&ple);
	}
}

void CBDCApp::MakeIconLeft(list_entry_t *pEntry, int nIcon)
{
	pEntry->left_icons = (struct list_bmp_t *)malloc(sizeof(struct list_bmp_t));
	trillianInitialize((*pEntry->left_icons));
	
	pEntry->left_icons->num_states	= 1;
	pEntry->left_icons->bitmap		= (HBITMAP *)malloc(sizeof(HBITMAP));
	pEntry->left_icons->location		= (RECT *)malloc(sizeof(RECT));

	switch( nIcon )
	{
		case 1:
			pEntry->left_icons->bitmap[0] = m_pbmpListentry_bd_confirmed;
			break;
		default:
			pEntry->left_icons->bitmap[0] = m_pbmpListentry_bd;
	}

	pEntry->left_icons->location[0].left	=  0;
	pEntry->left_icons->location[0].top		=  0;
	pEntry->left_icons->location[0].bottom	= 18;
	pEntry->left_icons->location[0].right	= 18;

	pEntry->num_left_icons = 1;
}

void CBDCApp::MakeIconRight(list_entry_t *pEntry, MEDIUM med)
{
	ASSERT(m_pproc);
	if( m_pproc && m_pproc->DrawRightIcons() )
	{
		if( med==MEDIUM_ICQ )
		{
			pEntry->right_icons		= (struct list_bmp_t *)malloc(sizeof(struct list_bmp_t));
			trillianInitialize((*pEntry->right_icons));
			
			pEntry->right_icons->num_states	= 1;

			pEntry->right_icons->skin_based = &sb;
			pEntry->right_icons->skin_name = &p1;
			pEntry->right_icons->skin_state = &p2;
			pEntry->right_icons->skin_type = &p3;

			pEntry->num_right_icons = 1;
		}
		else if( med==MEDIUM_VCARD )
		{
			pEntry->right_icons		= (struct list_bmp_t *)malloc(sizeof(struct list_bmp_t));
			trillianInitialize((*pEntry->right_icons));
			
			pEntry->right_icons->num_states	= 1;

			pEntry->right_icons->bitmap		= (HBITMAP *)malloc(sizeof(HBITMAP));
			pEntry->right_icons->bitmap[0]	= m_pbmpListentry_vcf;
			pEntry->right_icons->location	= (RECT *)malloc(sizeof(RECT));

			pEntry->right_icons->location[0].left	=  0;
			pEntry->right_icons->location[0].top	=  0;
			pEntry->right_icons->location[0].bottom	= 18;
			pEntry->right_icons->location[0].right	= 18;

			pEntry->num_right_icons = 1;
		}
		else if( med==MEDIUM_WAB )
		{
			pEntry->right_icons		= (struct list_bmp_t *)malloc(sizeof(struct list_bmp_t));
			trillianInitialize((*pEntry->right_icons));
			
			pEntry->right_icons->num_states	= 1;

			pEntry->right_icons->bitmap		= (HBITMAP *)malloc(sizeof(HBITMAP));
			pEntry->right_icons->bitmap[0]	= m_pbmpListentry_wab;
			pEntry->right_icons->location	= (RECT *)malloc(sizeof(RECT));

			pEntry->right_icons->location[0].left	=  0;
			pEntry->right_icons->location[0].top	=  0;
			pEntry->right_icons->location[0].bottom	= 18;
			pEntry->right_icons->location[0].right	= 18;

			pEntry->num_right_icons = 1;
		}
	}
}

void CBDCApp::UpdateIcon(CBirthday * pbd)
{
	if( pbd->m_listid_bd >= 0 )
	{
		{
			struct list_entry_t ple;

			trillianListInitialize(ple);

			ple.unique_id = pbd->m_listid_bd; 

			// icon
			MakeIconLeft( &ple,
				pbd->m_confirmed?1:0);
			MakeIconRight( &ple,
				pbd->m_medium);

			// You'll need the UniqueID for any further manipulation, so store it somewhere nice
			m_plugin_send(MYGUID, "listUpdateIcon", (void *)&ple);

			// Free what needs to be freed
			free(ple.left_icons->bitmap);
			free(ple.left_icons->location);
			free(ple.left_icons);
			if(ple.right_icons)
			{
				if(ple.right_icons->bitmap)
					free(ple.right_icons->bitmap);
				if(ple.right_icons->location)
					free(ple.right_icons->location);
				free(ple.right_icons);
			}
		}
		{
			struct list_entry_t ple;

			trillianListInitialize(ple);

			ple.unique_id = pbd->m_listid_bd; 

			if(pbd->m_date.IsDateOfYear())
			{
				ple.font.flags = 0x1;
				ple.font.skin_name = "recent-online";
				ple.font.select_fore = "recent-online-fore";
				ple.font.hover_fore = "recent-online-fore";
				ple.font.normal_fore = "recent-online-fore";
			}
			else
			{
			}
			m_plugin_send(MYGUID, "listUpdateFont", (void *)&ple);
		}
	}
}

void CBDCApp::EnableICQImport()
{
	if( m_prefswnd )
	{
		EnableWindow(
			GetDlgItem( m_prefswnd, IDC_IMPORTICQ ),
			true );
	}
}

void CBDCApp::DisableICQImport()
{
	if( m_prefswnd )
	{
		EnableWindow(
			GetDlgItem( m_prefswnd, IDC_IMPORTICQ ),
			false );
	}
}

void CBDCApp::DrawToday()
{
	struct list_entry_t ple; 
	trillianListInitialize(ple);

	ple.section_id		= m_sectionID;
	ple.parent_id		= m_knowngroupID;
	ple.unique_id		= -1; 
	ple.group			= 0;
	ple.section			= 0; 
	ple.inline_editing	= 0;
	ple.drag_and_drop	= 0; 
	ple.expanded		= 1; 

	ple.font.flags = 0x1;
	ple.font.skin_name = "recent-online";
	ple.font.select_fore = "recent-online-fore";
	ple.font.hover_fore = "recent-online-fore";
	ple.font.normal_fore = "recent-online-fore";

	ASSERT( m_pproc );
	if( m_pproc )
		ple.previous_id	= m_pproc->GetNextID(&CDate());
	else
		ple.previous_id	= -1;

	CString c = CDate().GetDateString() + " (today)";
	ple.tooltip			= c.GetBuffer(0);
	ple.text			= c.GetBuffer(0);
	ple.data			= NULL;

	// icon
	ple.left_icons = (struct list_bmp_t *)malloc(sizeof(struct list_bmp_t));
	trillianInitialize((*ple.left_icons));

	ple.left_icons->num_states	= 1;
	ple.left_icons->bitmap		= (HBITMAP *)malloc(sizeof(HBITMAP));
	ple.left_icons->location		= (RECT *)malloc(sizeof(RECT));

	ple.left_icons->bitmap[0] = m_pbmpListentry_today;

	ple.left_icons->location[0].left	=  0;
	ple.left_icons->location[0].top		=  0;
	ple.left_icons->location[0].bottom	= 18;
	ple.left_icons->location[0].right	= 18;

	ple.num_left_icons = 1;

	// You'll need the UniqueID for any further manipulation, so store it somewhere nice
	m_todayID = m_plugin_send(MYGUID, "listAddEntry", (void *)&ple);

	// Free what needs to be freed
	free(ple.left_icons->bitmap);
	free(ple.left_icons->location);
	free(ple.left_icons);
}
