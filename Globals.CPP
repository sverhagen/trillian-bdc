#include "stdafx.h"
#include "incl.h"

CBDCApp theApp;

int __declspec(dllexport)plugin_version(void)
{
	// Currently supporting version 1 of plugins
	return 1;
}

int __declspec(dllexport)plugin_main(char *event, void *data) 
{   
	return theApp.main( event, data );
} 

int menuCallback(int windowID, char *subwindow, char *event, void *data, void *userData)
{	
	return theApp.menuCallback( event, data );
}

int listCallback(int windowID, char *subwindow, char *event, void *data, void *userData)
{	
	return theApp.listCallback( windowID, subwindow, event, data, userData );
}

int systrayCallback(int windowID, char *subwindow, char *event, void *data, void *userData)
{	
	return theApp.systrayCallback( event, userData );
}

VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	theApp.TimerProcMember( hwnd, uMsg, idEvent, dwTime );
}

int prefDialog(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	return theApp.prefDialog(hWnd, message, wparam, lparam);
}
