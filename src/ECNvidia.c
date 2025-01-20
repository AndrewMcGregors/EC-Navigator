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

//#include "ECNvidia.h"
#include "dll-init.h"



NvGPUThermalSettings thermal = {0, 0, {{0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}} };

BOOL NvAPI_Loaded = FALSE;




char *NvGetTermal(char id[4096])
{
    char t[4096] = "";

    NvStatus ret = NVSTATUS_OK;
    NvAPI_ShortString s = "Failure";

    if (NvAPI_Loaded == FALSE)
	return NULL;

    sprintf(t, "\r\n\r\n "); 

    ret = NvAPI_GetInterfaceVersionString(&s);
    if (ret != NVSTATUS_OK)
    {
	NvAPI_GetErrorMessage(ret, &s);    
    }

    strcat(t, "GetInterfaceVersionString: ");  strcat(t, s);  strcat(t, "\r\n ");  

    int i=0;
    NvDisplayHandle hDisplay_a[NVAPI_MAX_PHYSICAL_GPUS * 2];

    ret = NvAPI_EnumNvidiaDisplayHandle(i, &hDisplay_a[i]);
    if (ret != NVSTATUS_OK)
    {
	NvAPI_GetErrorMessage(ret, &s);
	strcat(t, "EnumNvidiaDisplayHandle: ");

    }
    else sprintf(s, "EnumNvidiaDisplayHandle: %i Display Handle (0x%i)", 
		     i+1, (int)(HANDLE)hDisplay_a[i]);

    strcat(t, s); strcat(t, "\r\n ");

    NvU32 cnt = 0;
    NvPhysicalGpuHandle phys[NVAPI_MAX_PHYSICAL_GPUS];

    ret = NvAPI_GetPhysicalGPUsFromDisplay(hDisplay_a[0], &phys, &cnt);
    //ret = NvAPI_GetPhysicalGPUsFromDisplay(NVAPI_DEFAULT_HANDLE, &phys, &cnt);
    if (ret != NVSTATUS_OK)
    {
	NvAPI_GetErrorMessage(ret, &s);
	strcat(t, "GetPhysicalGPUsFromDisplay: ");
    }
    else sprintf(s, "GetPhysicalGPUsFromDisplay: %i GPU found.", cnt);

    strcat(t, s); strcat(t, "\r\n ");

    ret = NvAPI_EnumPhysicalGPUs(&phys, &cnt);
    if (ret != NVSTATUS_OK)
    {
        NvAPI_GetErrorMessage(ret, &s);
	strcat(t, "EnumPhysicalGPUs: ");
    }
    else sprintf(s, "EnumPhysicalGPUs: %i GPU found.", cnt);

    strcat(t, s); strcat(t, "\r\n ");

    ret = NvAPI_GPU_GetFullName(phys[0], &s);
    if (ret != NVSTATUS_OK)
    {
        NvAPI_GetErrorMessage(ret, &s);
    }

    strcat(t, "GPU_GetFullName: "); strcat(t, s); strcat(t, "\r\n ");

    NV_GPU_BUS_TYPE bt = 0;

    ret = NvAPI_GPU_GetBusType(phys[0], &bt);
    if (ret != NVSTATUS_OK)
    {
        NvAPI_GetErrorMessage(ret, &s);
	strcat(t, "GPU_GetBusType: ");
    }
    else sprintf(s, "GPU_GetBusType: %i (PCI=1, AGP=2, PCI_EXPRESS=3)", bt);

    strcat(t, s); strcat(t, "\r\n ");

	thermal.Version = NV_GPU_THERMAL_SETTINGS_VER;
	thermal.Count = NVAPI_MAX_THERMAL_SENSORS_PER_GPU;

	for (int c=0; c<NVAPI_MAX_THERMAL_SENSORS_PER_GPU; c++)
	{
		thermal.Sensor[c].Controller = UNKNOWN_CONTROLLER;
		thermal.Sensor[c].CurrentTemp = 0;
		thermal.Sensor[c].Target = GPU;
	}

	ret = NvAPI_GPU_GetThermalSettings(phys[0], ALL, &thermal);
    if (ret != NVSTATUS_OK)
    {
        NvAPI_GetErrorMessage(ret, &s);
	strcat(t, "GPU_GetThermalSettings: ");  strcat(t, s); strcat(t, "\r\n ");
    }
    else
    {
	sprintf(s, "TempCur %3i C\r\n ", thermal.Sensor[0].CurrentTemp);
	strcat(t, "GPU_GetThermalSettings: "); strcat(t, s);

	sprintf(s, "TempMin %3i C\r\n ", thermal.Sensor[0].DefaultMinTemp);
	strcat(t, "GPU_GetThermalSettings: "); strcat(t, s);

	sprintf(s, "TempMax %3i C\r\n ", thermal.Sensor[0].DefaultMaxTemp);
	strcat(t, "GPU_GetThermalSettings: "); strcat(t, s);
	//sprintf(s, "\r\n "); strcat(t, s);
    }

    TNvDisplayDriverVersion ddv = {0,0,0,"-","-"};

    ddv.version = NV_DISPLAY_DRIVER_VERSION_VER;
    ret = NvAPI_GetDisplayDriverVersion(NVAPI_DEFAULT_HANDLE, &ddv);
    if (ret != NVSTATUS_OK)
    {
        NvAPI_GetErrorMessage(ret, &s);
	strcat(t, "GetDisplayDriverVersion: ");   strcat(t, s); strcat(t, "\r\n ");
    }
    else
    {
	sprintf(s, "Driver version %i\r\n ", ddv.drvVersion);
	strcat(t, "GetDisplayDriverVersion: "); strcat(t, s);

	sprintf(s, "Version %i\r\n ", ddv.version);
	strcat(t, "GetDisplayDriverVersion: "); strcat(t, s);

	sprintf(s, "Build %s\r\n ", ddv.szBuildBranchString);
	strcat(t, "GetDisplayDriverVersion: "); strcat(t, s);

	sprintf(s, "Adapter %s\r\n ", ddv.szAdapterString);
	strcat(t, "GetDisplayDriverVersion: "); strcat(t, s);
    }

    TNvChipsetInfo ci = {0,0,0,"-","-",0,0,0,"-"};

    ci.version = NV_CHIPSET_INFO_VER;
    ret = NvAPI_SYS_GetChipSetInfo(&ci);
    if (ret != NVSTATUS_OK)
    {
        NvAPI_GetErrorMessage(ret, &s);
	strcat(t, "SYS_GetChipSetInfo: ");   strcat(t, s); strcat(t, "\r\n ");
    }
    else
    {
	sprintf(s, "vendorId %x\r\n ", ci.vendorId);
	strcat(t, "SYS_GetChipSetInfo: "); strcat(t, s);

	sprintf(s, "deviceId %x\r\n ", ci.deviceId);
	strcat(t, "SYS_GetChipSetInfo: "); strcat(t, s);

	sprintf(s, "Chipset %s ", ci.szVendorName); 
	strcat(s, ci.szChipsetName);
	strcat(t, "SYS_GetChipSetInfo: "); strcat(t, s);  sprintf(s, "\r\n "); strcat(t, s);

	sprintf(s, "Subvendor %s\r\n ", ci.szSubSysVendorName); 
	strcat(t, "SYS_GetChipSetInfo: "); strcat(t, s);
    }


    NV_SYSTEM_TYPE st = 0;

    ret = NvAPI_GPU_GetSystemType(phys[0], &st);
    if (ret != NVSTATUS_OK)
    {
        NvAPI_GetErrorMessage(ret, &s);
	strcat(t, "GPU_GetSystemType: ");
    }
    else sprintf(s, "GPU_GetSystemType: %i (LAPTOP=1, DESKTOP=2)", st);

    strcat(t, s); strcat(t, "\r\n ");

    ret = NvAPI_GPU_GetVbiosVersionString(phys[0], &s);
    if (ret != NVSTATUS_OK)
    {
        NvAPI_GetErrorMessage(ret, &s);
    }

    strcat(t, "GPU_GetVbiosVersionString: "); strcat(t, s); strcat(t, "\r\n ");


    strcat(id, t);
    return id;
}




BOOL GetNvapiStatus()
{
    return NvAPI_Loaded;
}

/*
NvStatus NvPhysicalGPUs()
{
}
*/




int NvGetTemp()
{
	NvU32 cnt = 0;
	NvPhysicalGpuHandle phys[NVAPI_MAX_PHYSICAL_GPUS];

	if (GetNvapiStatus() == FALSE)
		return 0;

	if (NvAPI_EnumPhysicalGPUs(&phys, &cnt) != NVSTATUS_OK)
		return 0;

	thermal.Version = NV_GPU_THERMAL_SETTINGS_VER;
	thermal.Count = NVAPI_MAX_THERMAL_SENSORS_PER_GPU;

	for (int c=0; c<NVAPI_MAX_THERMAL_SENSORS_PER_GPU; c++)
	{
		thermal.Sensor[c].Controller = UNKNOWN_CONTROLLER;
		thermal.Sensor[c].CurrentTemp = 0;
		thermal.Sensor[c].Target = GPU;
	}


	if (NvAPI_GPU_GetThermalSettings(phys[0], ALL, &thermal) != NVSTATUS_OK)
		return 0;

	return (int)thermal.Sensor[0].CurrentTemp;
}



