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

// BDC.h : main header file for the BDC DLL
//

#if !defined(AFX_BDC_H__C08C317F_057B_4049_9262_064ABAE49EF0__INCLUDED_)
#define AFX_BDC_H__C08C317F_057B_4049_9262_064ABAE49EF0__INCLUDED_

// Every plugin has it's own GUID
#define MYGUID "3BE629FA-BC5F-4ae7-AF3B-32E661019A11"

enum MENU
{
	MENU_ADD = 2,
	MENU_REMOVE = 3,
	MENU_EDIT = 4,
	MENU_CONFIRM = 5,
	MENU_RELOAD = 6
};

/*int __declspec(dllexport)plugin_version(void);
int __declspec(dllexport)plugin_main(char *event, void *data);
int WINAPI DllMain(HANDLE hinstDLL, unsigned long dwReason, void *reserved);
*/
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
int systrayCallback(int windowID, char *subwindow, char *event, void *data, void *userData);
int listCallback(int windowID, char *subwindow, char *event, void *data, void *userData);
int menuCallback(int windowID, char *subwindow, char *event, void *data, void *userData);
int prefDialog(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
/////////////////////////////////////////////////////////////////////////////
// The one and only CBDCApp object


#endif // !defined(AFX_BDC_H__C08C317F_057B_4049_9262_064ABAE49EF0__INCLUDED_)
