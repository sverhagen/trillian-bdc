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

// Processor.cpp: implementation of the CProcessor class.
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

CProcessor::CProcessor(CString homedir, CBDCApp* parent)
{
	m_pApp = parent;
	m_pbds = new CBirthdays(homedir);
	m_pconfig = new CConfig(homedir);

	// Set the timer for our alerts
	m_hours = -1;
	m_maintimer = SetTimer( NULL, 0, 120000 /*2mins*/, &TimerProc );
}

CProcessor::~CProcessor()
{
	delete m_pbds;
	m_pbds = NULL;
	delete m_pconfig;
	m_pconfig = NULL;
	// Stop the timer for our alerts
	KillTimer( NULL, m_maintimer );
}

void CProcessor::PrefsShow(HWND hwnd)
{
	ASSERT(m_pconfig);
	if( m_pconfig )
		m_pconfig->PrefsShow(hwnd);
}

void CProcessor::PrefsAction(HWND hwnd)
{
	ASSERT(m_pconfig);
	if( m_pconfig )
	{
		m_pconfig->PrefsAction(hwnd);
		DepopulateKnowns();
		PopulateKnowns();
		DepopulateBirthdays();
		ProcessAction(true);
	}
}

void CProcessor::Add(CBirthday * pbd)
{
	ASSERT(m_pbds);
	if( m_pbds )
	{
		m_pbds->Add(pbd);
		ProcessAction(true);
		m_pbds->Store();
	}
	// Add to list
	m_pApp->DrawKnown( pbd );
	m_pApp->UpdateCounter();
}

void CProcessor::Remove(CBirthday * pbd)
{
	if( pbd->m_listid_bd >= 0 )
	{
		m_pApp->ClearBirthday(pbd);
	}
	if( pbd->m_listid_known >= 0 )
	{
		m_pApp->ClearKnown(pbd);
	}

	ASSERT(m_pbds);
	if( m_pbds )
	{
		m_pbds->Remove( pbd );
		m_pbds->Store();
	}
	delete pbd;
	m_pApp->UpdateCounter();
}

void CProcessor::Store()
{
	ASSERT(m_pbds);
	if( m_pbds )
		m_pbds->Store();
}

int CProcessor::GetNextID(CDate* pd)
{
	ASSERT(m_pbds);
	if( m_pbds )
		return m_pbds->GetNextID(pd);
	else
		return -2;
}

int CProcessor::GetPreviousID(CBirthday * pbd)
{
	ASSERT(m_pbds);
	if( m_pbds )
		return m_pbds->GetPreviousID(pbd);
	else
		return -2;
}

void CProcessor::Confirm(CBirthday *pbd)
{
	pbd->m_confirmed = true;

	if( !pbd->m_date.AfterAndWithinDays( CDate(),
			m_pconfig->m_daysAdvance ) )
		pbd->m_alerted_this_year = false;

	Store();
	ProcessAction(true);
}

void CProcessor::ProcessAction(bool bForce/*=false*/)
{
	CString today;
	CString reminder;
	bool bChanged = false;
	bool bFirst = false;
	// Check if we entered a new hour
	if( (m_hours!=CTime::GetCurrentTime().GetHour()) || bForce )
	{
		m_hours=CTime::GetCurrentTime().GetHour();
		// Search for a birthday person
		POSITION pos = m_pbds->GetHead();
		while( pos )
		{
			CBirthday* pbd = m_pbds->GetNext( pos );

			// ARRANGE STATUSES
			if( pbd->m_date.AfterAndWithinDays( CDate(),
					m_pconfig->m_daysAdvance ) )
			{
				if( !pbd->m_alerted_this_year )
				{
					pbd->m_alerted_this_year = true;
					bFirst = true;
					pbd->m_confirmed = false;
					bChanged = true;
				}
			}
			else
			{
				if( pbd->m_alerted_this_year )
				{
					pbd->m_alerted_this_year = false;
					bChanged = true;
				}
			}

			// ARRANGE ALERTS

			// An alert could be given if the birthday is in the time that
			// is still to come or it is in the past but not yet confirmed
			if( pbd->m_date.AfterAndWithinDays( CDate(),
					m_pconfig->m_daysAdvance )
				|| (!pbd->m_confirmed && m_pconfig->m_keepUntilConfirmed) )
			{
				// Only alert if not already alerted today
				if( pbd->m_recent_alert!=CDate() )
				{
					pbd->m_recent_alert = CDate();
					if( pbd->m_date.IsDateOfYear() )
					{
						if( !pbd->m_confirmed || !m_pconfig->m_keepUntilConfirmed )
						{
							// TODAY
							if( m_pconfig->m_warnAnnoyingly )
							{
								if( today.GetLength() )
									today += ", ";
								today += pbd->m_name;
							}
							else
								m_pApp->Alert("Today's birthday: "+pbd->m_name);
						}
					}
					else
					{
						if( pbd->m_alerted_this_year && !bFirst )
						{
							if( m_pconfig->m_warnDaily
									&& (!pbd->m_confirmed || !m_pconfig->m_keepUntilConfirmed) )
							{
								// UPCOMING
								// ALREADY ALERTED (REMIND)
								if( m_pconfig->m_warnAnnoyingly )
								{
									if( reminder.GetLength() )
										reminder += ", ";
									reminder += pbd->m_name;
								}
								else
									m_pApp->Alert("Birthday reminder: "+pbd->m_name);
							}
						}
						else
							if( pbd->m_date.AfterAndWithinDays( CDate(),
									m_pconfig->m_daysAdvance ) )
							{
								// UPCOMING
								// FIRST ALERT
								if( m_pconfig->m_warnAnnoyingly )
								{
									if( reminder.GetLength() )
										reminder += ", ";
									reminder += pbd->m_name;
								}
								else
									m_pApp->Alert("Upcoming birthday: "+pbd->m_name);
							}
							else
								if( m_pconfig->m_warnDaily )
								{
									if( m_pconfig->m_warnAnnoyingly )
									{
										if( reminder.GetLength() )
											reminder += ", ";
										reminder += pbd->m_name;
									}
									else
										m_pApp->Alert("Forgotten birthday: "+pbd->m_name);
								}
					}
					bChanged = true;
				}
			}

			// ARRANGE LIST ENTRY
			if( pbd->m_date.AfterAndWithinDays( CDate(),
					m_pconfig->m_daysAdvance )
				|| (!pbd->m_confirmed && m_pconfig->m_keepUntilConfirmed) )
			{
				if( pbd->m_listid_bd >= 0 )
					m_pApp->UpdateIcon(pbd);
				else
					m_pApp->DrawBirthday(pbd);
			}
			else
			{
				if( pbd->m_listid_bd >= 0 )
					m_pApp->ClearBirthday( pbd );
			}
		}
		m_pApp->ClearToday();
		m_pApp->DrawToday();
	}
	if( bChanged ) Store();
	if( today.GetLength() || reminder.GetLength() )
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState( )); 
		CAnnoyDlg d( AfxGetMainWnd() );
		if( today.GetLength() )
			d.m_text = "TODAY'S BIRTHDAY(S):\r\n"+today+"\r\n\r\n";
		else
			d.m_text = "NO TODAY'S BIRTHDAYS\r\n\r\n";
		if( reminder.GetLength() )
			d.m_text += "OTHER CURRENT BIRTHDAY(S):\r\n"+reminder;
		d.DoModal();
	}
}

void CProcessor::PopulateKnowns()
{
	// Add actual list items
	ASSERT(m_pbds);
	if( m_pbds )
	{
		POSITION pos = m_pbds->GetHead();
		while( pos )
		{
			CBirthday* pbd = m_pbds->GetNext( pos );
			m_pApp->DrawKnown( pbd, false );
		}
	}
	m_pApp->UpdateCounter();
}

void CProcessor::DepopulateBirthdays()
{
	// Add actual list items
	ASSERT(m_pbds);
	if( m_pbds )
	{
		POSITION pos = m_pbds->GetHead();
		while( pos )
		{
			CBirthday* pbd = m_pbds->GetNext(pos);
			m_pApp->ClearBirthday( pbd );
		}
	}
}

void CProcessor::DepopulateKnowns()
{
	// Add actual list items
	ASSERT(m_pbds);
	if( m_pbds )
	{
		POSITION pos = m_pbds->GetHead();
		while( pos )
		{
			CBirthday* pbd = m_pbds->GetNext(pos);
			m_pApp->ClearKnown( pbd );
		}
	}
}

void CProcessor::Edit(CBirthday * pbd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( )); 
	CSetBirthdayDlg bddlg( AfxGetMainWnd(), pbd->m_name, pbd->m_date.GetDay(), pbd->m_date.GetMonth(), pbd->m_date.GetYear() );
	if( bddlg.DoModal() == IDOK )
	{
		// Update the data
		pbd->m_name = bddlg.m_name;

		if( pbd->m_date != CDate(bddlg.m_day,bddlg.m_month,bddlg.m_year) )
		{
			// UPDATE ALL

			m_pApp->ClearKnown( pbd );
			// This garantees re-sorting
			CBirthday* pnew = new CBirthday;
			*pnew = *pbd;
			Remove( pbd );

			pnew->m_date = CDate(bddlg.m_day,bddlg.m_month,bddlg.m_year);
			Add( pbd=pnew );
		}

		// Persistency
		Store();

		// ONLY UPDATE SOME TEXT
		m_pApp->UpdateText( pbd );

		ProcessAction(true);
	}
}

void CProcessor::Reload(CBirthday *pbd)
{
	if( pbd->Reload() )
	{
		// UPDATE ALL

		m_pApp->ClearKnown( pbd );
		// This garantees re-sorting
		CBirthday* pnew = new CBirthday;
		*pnew = *pbd;
		Remove( pbd );
		Add( pbd=pnew );

		// Persistency
		Store();

		ProcessAction(true);
	}
}

int CProcessor::prefDialog(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam, HMODULE hHandle)
{
	ASSERT(m_pconfig);
	if( m_pconfig ) return m_pconfig->prefDialog(hWnd, message, wparam, lparam, hHandle);
	return 0;
}

CString CProcessor::GetKnownCount()
{
	char buffer[10];
	_itoa_s( m_pbds->GetCount(), buffer, 10 );
	return CString(buffer);
}

void CProcessor::EnableICQImport()
{
	m_pApp->EnableICQImport();
}

void CProcessor::DisableICQImport()
{
	m_pApp->DisableICQImport();
}

void CProcessor::ImportVCF()
{
	// Set up the open dialog box
	AFX_MANAGE_STATE(AfxGetStaticModuleState( )); 
	CFileDialog aFileDlg( TRUE, "*.vcf" );
	aFileDlg.m_ofn.nFilterIndex = 1; // Set to all txt files

	aFileDlg.m_ofn.lpstrFilter = "Business Card (vCard) Files\0*.vcf\0All files\0*.*\0\0";
	aFileDlg.m_ofn.lpstrTitle = "Import vCard files";
	aFileDlg.m_ofn.Flags |= OFN_ALLOWMULTISELECT;

	// Evaluate the open dialog box
	if(aFileDlg.DoModal() == IDOK)
	{
		CString err_nobirthday;
		CString err_badfile;
		CString err_noname;
		CString success;

		POSITION pos = aFileDlg.GetStartPosition();
		while( pos )
		{
			CString file = aFileDlg.GetNextPathName(pos);
			CBirthday* pbd = new CBirthday;
			switch( pbd->LoadVCF(file) )
			{
				case IMPORT_SUCCESS:
					if( success.GetLength() ) success += ", ";
					success += pbd->m_name + "(" + file + ")";
					Add(pbd);
					break;
				case IMPORT_NOBIRTHDAY:
				case IMPORT_BADFILE:
				case IMPORT_NONAME:
				default:
					AfxMessageBox( "Problem importing file:\n"+file, MB_OK|MB_SYSTEMMODAL, 0 );
					delete pbd;
			}
		}
	}
}

enum {
    ieidPR_DISPLAY_NAME = 0,
	ieidPR_BIRTHDAY,
    ieidPR_ENTRYID,
	ieidPR_OBJECT_TYPE,
    ieidMax
};

static const SizedSPropTagArray(ieidMax, ptaEid)=
{
    ieidMax,
    {
        PR_DISPLAY_NAME,
		PR_BIRTHDAY,
        PR_ENTRYID,
		PR_OBJECT_TYPE,
    }
};
typedef HRESULT (WINAPI *fWABOpen)(LPADRBOOK*,LPWABOBJECT*,LPWAB_PARAM,DWORD);

void CProcessor::ImportWAB()
{
	HINSTANCE hinstLib;
	HRESULT hRes;
	LPADRBOOK lpAdrBook;
	LPWABOBJECT lpWABObject;
	DWORD Reserved2 = NULL;

	fWABOpen procWABOpen;

    {
        TCHAR  szWABDllPath[MAX_PATH];
        DWORD  dwType = 0;
        ULONG  cbData = sizeof(szWABDllPath);
        HKEY hKey = NULL;

        *szWABDllPath = '\0';
        
        // First we look under the default WAB DLL path location in the
        // Registry. 
        // WAB_DLL_PATH_KEY is defined in wabapi.h
        //
        if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, WAB_DLL_PATH_KEY, 0, KEY_READ, &hKey))
            RegQueryValueEx( hKey, "", NULL, &dwType, (LPBYTE) szWABDllPath, &cbData);

        if(hKey) RegCloseKey(hKey);

        // if the Registry came up blank, we do a loadlibrary on the wab32.dll
        // WAB_DLL_NAME is defined in wabapi.h
        //
        hinstLib = LoadLibrary( (lstrlen(szWABDllPath)) ? szWABDllPath : WAB_DLL_NAME );
    }

	if (hinstLib != NULL)
	{
		procWABOpen = (fWABOpen) GetProcAddress(hinstLib, "WABOpen");

		if (procWABOpen != NULL)
		{
			hRes = (procWABOpen)(&lpAdrBook,&lpWABObject,NULL,Reserved2); // WABOpen
			_ASSERTE(hRes == S_OK);
			if (hRes != S_OK) return;

			ULONG lpcbEntryID;
			ENTRYID *lpEntryID;
			hRes = lpAdrBook->GetPAB(
				&lpcbEntryID,
				&lpEntryID
			);
			_ASSERTE(hRes == S_OK);
			if (hRes != S_OK) return;

			ULONG ulFlags = MAPI_BEST_ACCESS;
			ULONG ulObjType = NULL;
			LPUNKNOWN lpUnk = NULL;
			hRes = lpAdrBook->OpenEntry(
				lpcbEntryID,
				lpEntryID,
				NULL,
				ulFlags,
				&ulObjType,
				&lpUnk
			);

			ulFlags = NULL;
			//IABTable *lpTable;
			
			if (ulObjType == MAPI_ABCONT)
			{
				IABContainer *lpContainer = static_cast <IABContainer *>(lpUnk);
				LPMAPITABLE lpTable = NULL;
				hRes = lpContainer->GetContentsTable(
					ulFlags,
					&lpTable
				);

				_ASSERT(lpTable);
				ULONG ulRows, ulFound = 0, ulExisted = 0;
				hRes = lpTable->GetRowCount(0,&ulRows);
				_ASSERTE(hRes == S_OK);

				SRowSet *lpRows;

				hRes = lpTable->SetColumns( (LPSPropTagArray)&ptaEid, 0 );

				hRes = lpTable->QueryRows(
					ulRows,		// Get all Rows
					0,
					&lpRows
				);

				for(ULONG i=0;i<lpRows->cRows;i++)
				{
					bool bBirthdayProcessed = false;
					CBirthday* pbd = new CBirthday;
					SRow *lpRow = &lpRows->aRow[i];
					for(ULONG j=0;j<lpRow->cValues;j++)
					{
						SPropValue *lpProp = &lpRow->lpProps[j];
						if(lpProp->ulPropTag == PR_BIRTHDAY)
						{
							SYSTEMTIME st;
							FileTimeToSystemTime(&lpProp->Value.ft,&st);

							pbd->m_date = CDate((BYTE)st.wDay, (BYTE)st.wMonth, (UINT)st.wYear);
							bBirthdayProcessed = true;
						}
						if(lpProp->ulPropTag == PR_DISPLAY_NAME_A)
						{
							pbd->m_name = lpProp->Value.lpszA;
						}
					}
					if( bBirthdayProcessed && pbd->m_date.ValidDate() )
					{
						ulFound++;
						if( !WABExists(pbd) )
						{
							pbd->m_medium = MEDIUM_WAB;
							Add( pbd );
						}
						else
						{
							delete pbd;
							ulExisted++;
						}
					}
					else
						delete pbd;

					lpWABObject->FreeBuffer(lpRow);
				}
				CString c;
				c.Format( "Processed %d contacts\r\n\r\nFound %d birthdays of which\r\n%d already existed",
					ulRows, ulFound, ulExisted );
				AfxMessageBox( c, MB_OK|MB_SYSTEMMODAL, 0 );

				lpWABObject->FreeBuffer(lpRows);
			}
			if(lpAdrBook)
				lpAdrBook->Release();

			if(lpWABObject)
				lpWABObject->Release();

		}
// This would be nice but leads to crashing Trillian
//		FreeLibrary(hinstLib);
	}
}

bool CProcessor::WABExists(CBirthday *pbd)
{
	// See if already exists
	POSITION pos = m_pbds->GetHead();
	while( pos )
	{
		CBirthday* pbd1 = m_pbds->GetNext( pos );
		if( (pbd1->m_medium==MEDIUM_WAB)
			&& (pbd1->m_name==pbd->m_name)
			&& (pbd1->m_date==pbd->m_date) )
			return true;
	}
	return false;
}

bool CProcessor::ICQExists(contactlist_entry_t *pentry)
{
	// See if already exists
	POSITION pos = m_pbds->GetHead();
	while( pos )
	{
		CBirthday* pbd = m_pbds->GetNext( pos );
		if( (pbd->m_medium==MEDIUM_ICQ)
			&& (pbd->m_mediumname.CompareNoCase( pentry->real_name )==0) )
			return true;
	}
	return false;
}


void CProcessor::FinalCleanup()
{
}


bool CProcessor::DrawRightIcons()
{
	ASSERT(m_pconfig);
	if( m_pconfig )
		return m_pconfig->m_drawRightIcons;
	else
		return true;
}

bool CProcessor::DrawLeftInfo()
{
	ASSERT(m_pconfig);
	if( m_pconfig )
		return m_pconfig->m_drawLeftInfo;
	else
		return true;
}

SUFFIX CProcessor::GetSuffix()
{
	ASSERT(m_pconfig);
	if( m_pconfig )
		return m_pconfig->m_suffix;
	else
		return SUFFIX_NONE;
}
