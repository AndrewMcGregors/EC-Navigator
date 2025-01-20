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

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>


#include "resource.h"
#include "ECbackground.h"

//extern HINSTANCE my_hinstance;


char *get_ec_filename(char *s)
{
        int c = strlen(s) - 1;

        while (c > 0)
        {
                if (s[c] == '/' || s[c] == '\\')
                   return &s[c+1];
                c--;
        }
        return s;
}


void get_ec_path(char *s, int size)
{
	char *p;

        //GetModuleFileName(my_hinstance, s, size);
        GetModuleFileName(GetModuleHandle(NULL), s, size);
        p=get_ec_filename(s);
        *p=0;
}



HBRUSH load_bkground(char file_bmp[24])
{
        char t_path[512];

        get_ec_path(t_path, 512);
 	strcat(t_path, file_bmp);

	HANDLE h = LoadImage(NULL,(LPCSTR)t_path, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
        if (GetLastError())
	{
                return (HBRUSH)GetStockObject(GRAY_BRUSH);
	}
	return CreatePatternBrush((HBITMAP)h);
}



HBITMAP load_bkground2(char file_bmp[24])
{
        char t_path[512];

        get_ec_path(t_path, 512);
 	strcat(t_path, file_bmp);

	HANDLE h = LoadImage(NULL,(LPCSTR)t_path, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
        if (GetLastError())
	{
                return  NULL;
	}
	return (HBITMAP)h;
}






