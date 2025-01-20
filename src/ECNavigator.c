 /*
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +                                                                  +
 +                                                                  +
 + EC Navigator An Embedded Controller viewer and hardware          +
 +                                                                  +
 +              resource monitoring                                 +
 +                                                                  +
 +              This file is part of the EC Navigator distribution. +
 +                                                                  +
 +                                                                  +
 +                                                                  +
 +                                                                  +  
 + Author:  Andrew McGregor, <andrew.drunkyduck@gmail.com>          +
 +                                                                  +
 +          Copyright 2024 Andrew McGregor.                         +
 +                                                                  +
 +                                                                  +
 +                                                                  +
 +                                                                  +  
 +  This program is free software; you can redistribute it          +
 +  and/or modify it under the terms of the GNU General Public      +
 +  License as published by the Free Software Foundation;           +
 +  either version 2 of the License, or any later version.          +
 +                                                                  +
 +  This program is distributed in the hope that it will be useful  +
 +  but WITHOUT ANY WARRANTY; without even the implied warranty of  +
 +  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the   +
 +  GNU General Public License for more details.                    +
 +                                                                  +
 +  You should have received a copy of the GNU General Public       +
 +  License along with this program; if not, write to the Free      +
 +  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139,   +
 +  USA.                                                            +
 +                                                                  +
 +                                                                  +
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

#include <windows.h>
#include <windowsx.h>

#include <commctrl.h>
#include <commdlg.h>
#include <process.h>

#ifndef PROGRESS_CLASS
#define PROGRESS_CLASS L"msctls_progress32"
#endif

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef _WIN64
#include <wtypesbase.h>
#endif

#include "resource.h"
#include "Ols\OlsApi.h"
#include "ECOlsEx.h"
#include "ECModels.h"
#include "ECCpuUsage.h"
#include "ECBackground.h"
#include "ECMessage.h"

#include "dll-init.h"



int overlapped_style = WS_OVERLAPPEDWINDOW &~WS_SIZEBOX &~WS_MAXIMIZEBOX &~WS_SYSMENU;

char no_driver[255] = "Please load driver!";

enum { INFO_EC = 0, INFO_CPU, INFO_MSR, INFO_PCI, INFO_NVAPI };

int info = 0;
int dimFont = 6;

const char g_szClassName[] = "myWindowClass";
HFONT hFont;
HWND hListboxMsg;

extern char *NvGetTermal(char id[4096]);
extern int  LaptopModel;
extern BOOL GetNvapiStatus();



static BOOL CALLBACK settings_dlgproc(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam)
{
        HWND h;
	int c;

        switch (message)
        {

              case WM_INITDIALOG:
        		h = GetDlgItem(hdlg, IDC_BKGRND);
        		SendMessage(h,  STM_SETIMAGE,(WPARAM)IMAGE_BITMAP,
					(LPARAM)(HWND)load_bkground2("ECNavigator.bmp"));

			InitializeModels(hdlg);

			//h = GetDlgItem(hdlg, IDC_CB_SETTINGS);
			//SendMessage(h, CB_SETCURSEL, LaptopModel, 0);

			SetFocus(hdlg);
			SetActiveWindow(hdlg);
    			UpdateWindow(hdlg);
              return TRUE;
                
              case WM_COMMAND:
                   switch (LOWORD(wParam))
                   {

                        case IDC_CB_MODELS:
                        if (HIWORD(wParam) == CBN_SELCHANGE)
                        {

				InitializeModelsItems(hdlg);
    				UpdateWindow(hdlg);

                        }
                        return FALSE;

                        case IDOK:
                        {
				h = GetDlgItem(hdlg, IDC_CB_MODELS);
				c = SendMessage(h, CB_GETCURSEL, 0, 0);

				LaptopModel =  c;

				InitializeCustomItems(hdlg);
				InitializeEC(hdlg);
				InitializeModelsItems(hdlg);

                        }
                        return FALSE;
                        
                        case IDCANCEL:
                        	EndDialog(hdlg, 0);
                        return TRUE;

                }

                break; // End case WM_COMMAND

        }
        return FALSE;
}


int settings_dlg_open(HWND hwnd)
{
        int ret;
        
        //ret = DialogBox(my_hinstance, TEXT("myDlg"), hwnd, (DLGPROC)my_dlgproc);
        ret = DialogBox(GetModuleHandle(NULL),  TEXT("settings_dlg"),
						hwnd, (DLGPROC)settings_dlgproc);
        
        return ret;
}

static BOOL CALLBACK info_dlgproc(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam)
{
        HWND h;
	char t[4096] = "";

        switch (message)
        {

              case WM_INITDIALOG:
        		h = GetDlgItem(hdlg, IDC_BKGRND);
        		SendMessage(h,  STM_SETIMAGE,(WPARAM)IMAGE_BITMAP,
					(LPARAM)(HWND)load_bkground2("ECNavigator.bmp"));


        		SendMessage(GetDlgItem(hdlg, IDC_EDIT_INFO), WM_SETFONT,
					      (WPARAM)hFont, 0);
			SetFocus(hdlg);
			SetActiveWindow(hdlg);
    			UpdateWindow(hdlg);
              return TRUE;
                
              case WM_COMMAND:
                   switch (LOWORD(wParam))
                   {
                        case IDOK:
                        {
				switch (info)
                        	{
				   case INFO_EC:

					if (!GetDllStatus())
	                		{

					    if (LaptopModel != MODEL_ACER_TRAVELMATE_4000)
						SendMessage(GetDlgItem(hdlg, IDC_EDIT_INFO),
						WM_SETTEXT,0, (LPARAM)(LPCSTR)GetECTable(t));
					    else ec_note("No EC Table here.");
					}
					else ec_note(no_driver);
				   break;

				   case INFO_CPU:

					strcpy(t, "CPU Table");
					if (!GetDllStatus())
	                		{
					SendMessage(GetDlgItem(hdlg, IDC_EDIT_INFO),
						    WM_SETTEXT, 0,
					   	   (LPARAM)(LPCSTR)CpuidPxEx(t));
					}
					else ec_note(no_driver);
				   break;

				   case INFO_PCI:

					strcpy(t, "PCI Table");
					if (!GetDllStatus())
	                		{
					SendMessage(GetDlgItem(hdlg, IDC_EDIT_INFO), WM_SETTEXT,
						    0, (LPARAM)(LPCSTR)PciEx(t));
					}
					else ec_note(no_driver);
				   break;

				   case INFO_NVAPI:

					strcpy(t, "Nvidia Table");
					if (GetNvapiStatus())
	                		{
					SendMessage(GetDlgItem(hdlg, IDC_EDIT_INFO),
						    WM_SETTEXT, 0,
						   (LPARAM)(LPCSTR)NvGetTermal(t));
					}
					else ec_note(no_driver);
				   break;
                                }                    
                        }
                        return FALSE;
                        
                        case IDCANCEL:
                        	EndDialog(hdlg, 0);
                        return TRUE;

                }

                break; // End case WM_COMMAND

        }
        return FALSE;
}



int info_dlg_open(HWND hwnd)
{
        int ret;
        
        ret = DialogBox(GetModuleHandle(NULL),  TEXT("info_dlg"),
						hwnd, (DLGPROC)info_dlgproc);
        
        return ret;
}



// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {

	case WM_CREATE:
	{
        	HWND  hStatus, hListboxTemp, hListboxFan, hListboxBat;
		HWND  hProgBarBat, hProgBarCpuUsage;


    		hFont = CreateFont(0,dimFont,0,0,0,0,0,0,0,0,0,0,0,TEXT("Courier New"));

		hListboxMsg = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				WC_LISTBOX, 				/* Classname */
				"" ,					/* No name */
				WS_CHILD | WS_VISIBLE | WS_VSCROLL,	/* Style */
				30, 30, 220, 70,			/* Position */
				hwnd, 					/* child to hwnd */
				(HMENU)IDC_LISTBOX_MSG,
				//NULL,
				GetModuleHandle(NULL),			/* Instance handle */
				NULL);

        	SendMessage(hListboxMsg, WM_SETFONT, (WPARAM)hFont, 0);

		SendMessage(hListboxMsg, LB_RESETCONTENT, 0, 0);
		SendMessage(hListboxMsg, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)"System ready.");
		SendMessage(hListboxMsg, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Awaiting orders.");

		hProgBarCpuUsage = CreateWindowEx( WS_EX_CLIENTEDGE,
				PROGRESS_CLASS, 			/* Classname */
				"" ,					/* No name */
				WS_CHILD | WS_VISIBLE | PBS_SMOOTH,	/* Style */
				30, 90, 220, 10,			/* Position */
				hwnd, 					/* child to hwnd */
				(HMENU)IDC_PBAR_CPU_USAGE,
				//NULL,
				GetModuleHandle(NULL),			/* Instance handle */
				NULL);

		SendMessage(hProgBarCpuUsage, PBM_SETRANGE, 0, MAKELPARAM(0,100));
        	SendMessage(hProgBarCpuUsage, PBM_SETPOS, (WPARAM)(INT)0, 0);

		hListboxFan = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				WC_LISTBOX, 				/* Classname */
				"" ,					/* No name */
				WS_CHILD | WS_VISIBLE | WS_VSCROLL,	/* Style */
				30, 120, 160, 70,			/* Position */
				hwnd, 					/* child to hwnd */
				(HMENU)IDC_LISTBOX_FAN,
				//NULL,
				GetModuleHandle(NULL),			/* Instance handle */
				NULL);

        	SendMessage(hListboxFan, WM_SETFONT, (WPARAM)hFont, 0); 

		hListboxTemp = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				"LISTBOX", 				/* Classname */
				"" ,					/* No name */
				WS_CHILD | WS_VISIBLE | WS_VSCROLL,	/* Style */
				200, 120, 130, 110,			/* Position */
				hwnd, 					/* child to hwnd */
				(HMENU)IDC_LISTBOX_TEMP,
				//NULL,
				GetModuleHandle(NULL),			/* Instance handle */
				NULL);

        	SendMessage(hListboxTemp, WM_SETFONT, (WPARAM)hFont, 0);

		hListboxBat = CreateWindowEx( WS_EX_CLIENTEDGE,
				"LISTBOX", 				/* Classname */
				"" ,					/* No name */
				WS_CHILD | WS_VISIBLE | WS_VSCROLL,	/* Style */
				30, 190, 160, 150,			/* Position */
				hwnd, 					/* child to hwnd */
				(HMENU)IDC_LISTBOX_BAT,
				//NULL,
				GetModuleHandle(NULL),			/* Instance handle */
				NULL);

        	SendMessage(hListboxBat, WM_SETFONT, (WPARAM)hFont, 0);


		hProgBarBat = CreateWindowEx( WS_EX_CLIENTEDGE,
				PROGRESS_CLASS, 			/* Classname */
				"" ,					/* No name */
				WS_CHILD | WS_VISIBLE | PBS_SMOOTH,	/* Style */
				30, 340, 160, 10,			/* Position */
				hwnd, 					/* child to hwnd */
				(HMENU)IDC_PBAR_BAT,
				//NULL,
				GetModuleHandle(NULL),			/* Instance handle */
				NULL);

		SendMessage(hProgBarBat, PBM_SETRANGE, 0, MAKELPARAM(0,100));
        	SendMessage(hProgBarBat, PBM_SETPOS, (WPARAM)(INT)0, 0);

		hStatus = CreateWindowEx( WS_EX_CLIENTEDGE,
				STATUSCLASSNAME, 			/* Classname */
				"" ,					/* No name */
				WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,	/* Style */
				0, 0, 0, 0,				/* size & pos */
				hwnd, 					/* child to hwnd */
				(HMENU)IDC_STATUS,
				//NULL,
				GetModuleHandle(NULL),			/* Instance handle */
				NULL);

		int statuswidhts[] = {140, -1};
		SendMessage(GetDlgItem(hwnd, IDC_STATUS), SB_SETPARTS,
				       sizeof(statuswidhts)/sizeof(int), (LPARAM)statuswidhts);
		SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)(LPCSTR)" ***");
		SendMessage(hStatus, SB_SETTEXT, 1, (LPARAM)(LPCSTR)" ***");

		
		if (SetTimer(hwnd, ID_TIMER, 1000, NULL) == 0)
			  ec_note("SetTimer failed!");

		InitializeEC(hwnd);

		SendMessage(hwnd, WM_COMMAND, (WPARAM)(INT)IDM_LOAD_OLS, 0);

		get_registry_key();
    	}
        break;


	case WM_TIMER:
	{
   		if (!GetDllStatus())
	        {
			ECReadTemp(hwnd);
			ECReadFan(hwnd);
			ECReadBat(hwnd);

			HWND h = GetDlgItem(hwnd, IDC_PBAR_CPU_USAGE);

			int i = GetCPULoad();
			if ((i > 0) && (i <= 100))
			     SendMessage(h, PBM_SETPOS, (WPARAM)(INT)i, 0);

    		}
    	}
        break;


	case WM_COMMAND:


                switch (LOWORD(wParam))
                {
                        case IDM_LOAD_OLS:
	                {
				//HMODULE *hModule;
				char s[255]  = "OLS Version: ";

   				if (InitializeOls())
	                	{
				SendMessage(GetDlgItem(hwnd, IDC_STATUS), SB_SETTEXT, 0,
					   (LPARAM)(LPCSTR)GetDllVersionEx(s));

				strcpy(s, "OLS Driver: ");
				SendMessage(GetDlgItem(hwnd, IDC_STATUS), SB_SETTEXT, 1,
					   (LPARAM)(LPCSTR)GetDriverNameEx(s));

				SendMessage(GetDlgItem(hwnd, IDC_LISTBOX_MSG), LB_ADDSTRING, 0,
					   (LPARAM)(LPCSTR)"Ols loaded.");
 
				strcpy(s, "OLS Driver: ");
				SendMessage(GetDlgItem(hwnd, IDC_LISTBOX_MSG), LB_ADDSTRING, 0,
					   (LPARAM)(LPCSTR)GetDriverNameEx(s)); 
				}
    				else ec_note("Initialize Ols failed!");

			}
                        break;

                        case IDM_CPU_INFO:
	                {
				info = INFO_CPU;
                        	info_dlg_open(hwnd);
			}
                        break;

                        case IDM_EC_INFO:
	                {
				info = INFO_EC;
                        	info_dlg_open(hwnd);
			}
                        break;

                        case IDM_PCI_INFO:
	                {
				info = INFO_PCI;
                        	info_dlg_open(hwnd);
			}
                        break;

                        case IDM_NVAPI_INFO:
	                {
				info = INFO_NVAPI;
				info_dlg_open(hwnd);
                        break;
			}

                        case IDM_SETTINGS:

				settings_dlg_open(hwnd);
                        break;

                        case IDM_SENSOR_INFO:
	                {
				//char t[4096] = "TEMP report";
				if (!GetDllStatus())
	                	{
					InitializeEC(hwnd);

					ECReadTemp(hwnd);
					ECReadFan(hwnd);
					ECReadBat(hwnd);
				}
				else    ec_note(no_driver);
			}
                        break;

                        case IDM_LOAD_NVAPI:

				nvapi_init_all(hListboxMsg);

                        break;


                        case IDM_EXIT:
				DeinitializeOls();
				nvapi_close_all();
				//dll_close_all();
				set_registry_key();
                        	PostMessage(hwnd, WM_CLOSE, 0, 0);
                        break;
    		}
        break;

        case WM_CLOSE:
		DestroyWindow(hwnd);
        break;

        case WM_DESTROY:
		KillTimer(hwnd, ID_TIMER);
		DeleteObject(hFont);
		PostQuitMessage(0);
        break;

        default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}






int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    HMENU menu;
    MSG Msg;



    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    //wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wc.hbrBackground = (HBRUSH)load_bkground("ECNavigator.bmp");
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    menu = LoadMenu(hInstance, TEXT("MainMenu"));

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        		WS_EX_CLIENTEDGE,
        		g_szClassName,			/* Classname */
        		"EC Navigator",			/* Windows title */
        		overlapped_style,		/* Window setting */
        		CW_USEDEFAULT, CW_USEDEFAULT,	/* Windows position */
			370, 450,			/* Windows size */
        		HWND_DESKTOP,          		/* Window is child to desktop */
			menu,				/* Windows menu */
			hInstance,           		/* Program Instance handler */
			NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    OpenIsaBusMutex();


    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    CloseIsaBusMutex();

    //DestroyWindow(hwnd);

    UnregisterClass(g_szClassName, hInstance);

    return Msg.wParam;
}









