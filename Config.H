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

// Config.h: interface for the CConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIG_H__6920AB03_BB3D_47FC_946B_552775F24D40__INCLUDED_)
#define AFX_CONFIG_H__6920AB03_BB3D_47FC_946B_552775F24D40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum SUFFIX
{
	SUFFIX_NONE,
	SUFFIX_AGE,
	SUFFIX_AGE_SHORT,
	SUFFIX_DATE,
	SUFFIX_YOB,
	SUFFIX_YOB_SHORT
};

class CConfig : public CSerializer  
{
public:
	int prefDialog(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam, HMODULE hHandle);
	void PrefsAction(HWND hwnd);
	void PrefsShow(HWND hWnd);
	void Store();
	bool m_drawRightIcons;
	bool m_drawLeftInfo;
	bool m_warnDaily;
	bool m_warnAnnoyingly;
	bool m_keepUntilConfirmed;
	SUFFIX m_suffix;
	BYTE m_daysAdvance;
	virtual CString construct();
	virtual void handlevalue(CString name, CString parameters, CXMLString&);
	CConfig(CString homedir);

protected:
	CString m_homedir;
};

#endif // !defined(AFX_CONFIG_H__6920AB03_BB3D_47FC_946B_552775F24D40__INCLUDED_)
