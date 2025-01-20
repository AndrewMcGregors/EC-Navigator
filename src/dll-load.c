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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "ECNvidia.h"
#include "ECMessage.h"
#include "dll-load.h"


HMODULE *dynaload_module(const char *name, dllimport_t *table)
{
    HMODULE h;
    const dllimport_t *i;
    void *func;


    if ((h = LoadLibrary(name)) == NULL)
    {
	ec_note("DLL: library not found!");
	return(NULL);
    }


    if (table == NULL)
    {
	FreeLibrary(h);
	return((HMODULE *)h);
    }


    for (i = table; i->name != NULL; i++)
    {
	func = GetProcAddress(h, i->name);
	if (func == NULL)
        {
		//MessageBox(NULL, "DLL: function not found!", i->name, MB_OK);
		ec_note("DLL: function not found!");
		FreeLibrary(h);
		return(NULL);
	}


	*(char **)i->func = (char *)func;
    }

    return((HMODULE *)h);
}


void *(*nvapi_QueryInterface)(long int ID);


NvStatus NvNotImplemented()
{
    return NO_IMPLEMENTATION;
}


HMODULE *nvapiload_module(const char *name, nvapiimport_t *table)
{
    HMODULE h;
    const nvapiimport_t *t;
    int *(*initfunc)();
    void *func;
    int c = 0;
    char s[63];

    if ((h = LoadLibrary(name)) == NULL)
    {
	//MessageBox(NULL, "NVAPI: library not found!", "PATH_DLL", MB_OK);
	ec_note("NVAPI: library not found!");
	return(NULL);
    }


    if (table == NULL)
    {
	FreeLibrary(h);
	//return((HMODULE *)h);
	return(NULL);
    }


    func = GetProcAddress(h, "nvapi_QueryInterface");
    nvapi_QueryInterface = func;

    if (nvapi_QueryInterface == NULL)
    {
	MessageBox(NULL, "NVAPI: nvapi_QueryInterface not found!", "PATH_DLL", MB_OK);
	FreeLibrary(h);
	//return((HMODULE *)h);
	return(NULL);
    }

    for (t = table; t->name != 0; t++)
    {
	
	func = nvapi_QueryInterface(t->name);
	if (func == NULL)
        {
		func = &NvNotImplemented;
		ec_note("NVAPI: function not found!");
    		//sprintf(s, "0x%x", t->name);
		//MessageBox(NULL, "NVAPI: function not found!", s, MB_OK);
		//FreeLibrary(h);
		//return(NULL);
	}

	if (t->name == 0x0150E828)	//NvAPI_Initialize(); 
	{
		initfunc = func;
		if (initfunc())
		{
			//MessageBox(NULL, "NVAPI: function not initialized!", name, MB_OK);
			ec_note("NVAPI: function not initialized!");
			FreeLibrary(h);
			//return((HMODULE *)h);
			return(NULL);
		}
	}


	*(char **)t->func = (char *)func;
	c++;

    	sprintf(s, "%i function imported!", c);

    }

    return((HMODULE *)h);
}


void dynaload_close(void *handle)
{
    if (handle != NULL)
	FreeLibrary((HMODULE)handle);
}

