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
#include <winuser.h>


#include <commctrl.h>
#include <commdlg.h>

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

#include "dll-init.h"
#include "ECMessage.h"



/* Global vars */
 
HMODULE *dll_handle = NULL;
HMODULE *nvapi_handle = NULL;

extern BOOL NvAPI_Loaded;


/* Function tables */
/*
static dllimport_t dll_imports[] = 
{

  { "NvAPI_Initialize",			&NvAPI_Initialize		},
  { "NvAPI_EnumPhysicalGPUs",		&NvAPI_EnumPhysicalGPUs		},
  { "NvAPI_GPU_GetThermalSettings",	&NvAPI_GPU_GetThermalSettings	},
  { "NvAPI_GetInterfaceVersionString",	&NvAPI_GetInterfaceVersionString},

  { NULL,				NULL			}
};
*/

static nvapiimport_t nvapi_import_table[] = 
{
  { 0x0150E828,			&NvAPI_Initialize			},
  { 0x9ABDD40D,			&NvAPI_EnumNvidiaDisplayHandle		},
  { 0xE5AC921F,			&NvAPI_EnumPhysicalGPUs			},
  { 0x01053FA5,			&NvAPI_GetInterfaceVersionString	},
  { 0xF951A4D1,			&NvAPI_GetDisplayDriverVersion  	},
  { 0x34EF9506,			&NvAPI_GetPhysicalGPUsFromDisplay  	},
  { 0x6C2D048C,			&NvAPI_GetErrorMessage		  	},
  { 0x1BB18724,			&NvAPI_GPU_GetBusType			},
  { 0xbaaabfcc,			&NvAPI_GPU_GetSystemType		},
  { 0xCEEE8E9F,			&NvAPI_GPU_GetFullName  		},
  { 0xE3640A56,			&NvAPI_GPU_GetThermalSettings		},
  { 0xa561fd7d,			&NvAPI_GPU_GetVbiosVersionString	},
  { 0x53DABBCA,			&NvAPI_SYS_GetChipSetInfo	  	},
  { 0xd22bdd7e,			&NvAPI_Unload				},

  { 0,				NULL					}
};

/* Functions */


/*
void dll_close_all()
{

    if (dll_handle != NULL) 
    {
	dynaload_close(dll_handle);
	dll_handle = NULL;
    }

}


void dll_init_all()
{

    dll_handle = dynaload_module(PATH_DLL, dll_imports);

    if (dll_handle == NULL)
    {
	MessageBox(NULL, "DLL: unable to load.", PATH_DLL, MB_OK);
	return;
    }
    else MessageBox(NULL, "DLL: Successfully loaded.", PATH_DLL, MB_OK);


    if (NvAPI_Initialize() == NVSTATUS_OK)
	    MessageBox(NULL, "Ok ", "DDL: Initialize ", MB_OK);
    else    MessageBox(NULL, "Ko ", "DDL: Initialize ", MB_OK);


}
*/



void nvapi_init_all(HWND hlistboxmsg)
{
	NvStatus ret = NVSTATUS_OK;
	NvAPI_ShortString err_string;

	nvapi_handle = nvapiload_module(PATH_NVAPI, nvapi_import_table);

	if (nvapi_handle == NULL)
	{
		ec_note("NVAPI: unable to load!");
		return;
	}

	NvAPI_Loaded = TRUE;
	ec_note("NvAPI loaded.");

	NvAPI_ShortString s = "Ok";

	ret = NvAPI_GetInterfaceVersionString(&s);
	if (ret == NVSTATUS_OK)
		ec_note(s);
		//SendMessage(hlistboxmsg, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);
	else
	{
		NvAPI_GetErrorMessage(ret, &err_string);
		ec_note("NVAPI: unable to init!");
		ec_note(err_string);
		
	}

}


void nvapi_close_all()
{
    if (nvapi_handle != NULL) 
    {
	if (NvAPI_Unload() != NVSTATUS_OK)
		ec_note("NVAPI: unable to close!");
	NvAPI_Loaded = FALSE;
	dynaload_close(nvapi_handle);
	nvapi_handle = NULL;
    }
}


