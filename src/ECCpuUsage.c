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
#include <winbase.h>

#include <commctrl.h>
#include <commdlg.h>
#include <process.h>

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "resource.h"


extern WINBOOL WINAPI GetSystemTimes(LPFILETIME lpIdleTime,
				     LPFILETIME lpKernelTime,
				     LPFILETIME lpUserTime);




unsigned long long oldFullTicks = 0;
unsigned long long oldIdleTicks = 0;


float partialCPULoad(unsigned long long newIdleTicks, unsigned long long newFullTicks)
{

    unsigned long long partialFullTicks = newFullTicks - oldFullTicks;
    unsigned long long partialIdleTicks = newIdleTicks - oldIdleTicks;

    float ret;

    if (partialFullTicks)
	 ret = 1.0 - (((float)partialIdleTicks) / partialFullTicks);
    else ret = 0.0;

    oldFullTicks = newFullTicks;
    oldIdleTicks = newIdleTicks;

    return ret;
}



unsigned long long From32To64(LPFILETIME f)
{
    return (((unsigned long long )(f->dwHighDateTime)) << 32) | 
	    ((unsigned long long )(f->dwLowDateTime ));
}


int GetCPULoad()
{
    FILETIME idleTime, kernelTime, userTime;
    float f = 0.19;
    unsigned long long a,b,c;

    if (GetSystemTimes(&idleTime, &kernelTime, &userTime))
    {

	a = From32To64(&idleTime); 
	b = From32To64(&kernelTime); 
	c = From32To64(&userTime);

	f = partialCPULoad(a, b + c);

	return (int)((double)(100.0*f));
    }

    return (int)(0);
}




