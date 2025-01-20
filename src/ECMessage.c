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
#include "ECMessage.h"


extern HWND hListboxMsg;


LRESULT ec_message(HWND hwindow, int item, UINT message, WPARAM w, LPARAM l)
{
	HWND h;
	LRESULT lres;

	h = GetDlgItem(hwindow, item);
	lres = SendMessage(h, message, (WPARAM) w, (LPARAM)l);
	return lres;
}



LRESULT ec_note(char *c)
{
	LRESULT lres;

	lres = SendMessage(hListboxMsg, LB_INSERTSTRING, 0, (LPARAM)(LPCSTR)c);


	return lres;
}






