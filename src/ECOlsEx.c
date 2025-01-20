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
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <tchar.h>

#ifdef _WIN64
#include <wtypesbase.h>
#endif

#include <winioctl.h>
//#include <intrin.h>

#include "Ols\stdafx.h"
#include "Ols\OlsIoctl.h"
#include "Ols\OlsApi.h"
#include "Ols\OlsDll.h"
#include "Ols\OlsDef.h"
#include "Ols\Driver.h"


char WINAPI *GetDriverNameEx(char ver[255])
{
	strcat(ver, OLS_DRIVER_ID);
	return ver;
}

char WINAPI *GetDriverVersionEx(char ver[255])
{
	BYTE a=0,b=0,c=0,d=0;
	char s[255], t[255];

	GetDriverVersion(&a,&b,&c,&d);

	sprintf(s, "%i", a); strcpy(t, s); strcat(t, ".");
	sprintf(s, "%i", b); strcat(t, s); strcat(t, ".");
	sprintf(s, "%i", c); strcat(t, s); strcat(t, ".");
	sprintf(s, "%i", d); strcat(t, s);

	strcat(ver,t);

	return ver;
}

char WINAPI *GetDllVersionEx(char ver[255])
{
	BYTE a=0,b=0,c=0,d=0;
	char s[255], t[255];

	GetDllVersion(&a,&b,&c,&d);

	sprintf(s, "%i", a); strcpy(t, s); strcat(t, ".");
	sprintf(s, "%i", b); strcat(t, s); strcat(t, ".");
	sprintf(s, "%i", c); strcat(t, s); strcat(t, ".");
	sprintf(s, "%i", d); strcat(t, s);

	strcat(ver,t);

	return ver;
}

char WINAPI *CpuidPxEx(char id[4096])
{
	DWORD a=0, b=0, c=0, d=0, MaxCpuid=0;
	char s[255]="", t[4096]="";


	sprintf(t, "\r\n\r\n  Index     EAX       EBX       ECX       EDX");
	CpuidPx(0, &MaxCpuid, &b, &c, &d, 1);

	for (int i = 0x00000000; i <= MaxCpuid; i++) 
	{ 
		if (CpuidPx(i, &a,&b,&c,&d, 1)) 
		{ 
			sprintf(s, "\r\n  ");   strcat(t, s);
			sprintf(s, "%08x ", (unsigned int)i); strcat(t, s); strcat(t, " ");
			sprintf(s, "%08x ", (unsigned int)a); strcat(t, s); strcat(t, " ");
			sprintf(s, "%08x ", (unsigned int)b); strcat(t, s); strcat(t, " ");
			sprintf(s, "%08x ", (unsigned int)c); strcat(t, s); strcat(t, " ");
			sprintf(s, "%08x ", (unsigned int)d); strcat(t, s);
		} 
	}
	strcat(id, t);
	return id;
}

char *GetDeviceNameEx(DWORD vendor, DWORD id, char DeviceName[255])
{
	char s[255];

        switch (vendor)
        {

	    case 0x121a:	/* 3DFX */
        	switch (id)
        	{
		   case 0x0003: strcpy(s, "3DFX Voodoo Banshee");
		   break;

		   default:     strcpy(s, "Unknown 3DFX device type");
		   break;
        	}
	    break;

	    case 0x10b9:	/* Ali */
        	switch (id)
        	{
		   case 0x1533: strcpy(s, "Ali M1533 PCI to ISA adapter");
		   break;

		   case 0x5229: strcpy(s, "Ali M5229 EIDE bus master controller");
		   break;

		   case 0x5451: strcpy(s, "Ali M5451 AC-Link audio device");
		   break;

		   case 0x5457: strcpy(s, "Ali M5457 AC-Link modem device");
		   break;

		   case 0x7101: strcpy(s, "Ali M7101 power management controller");
		   break;

		   default:     strcpy(s, "Unknown Ali device type");
		   break;
        	}
	    break;

	    case 0x1022:	/* AMD */
        	switch (id)
        	{
		   case 0x1100: strcpy(s, "AMD Athlon 64/Opteron HT hub");
		   break;

		   case 0x1101: strcpy(s, "AMD Athlon 64/Opteron address map");
		   break;

		   case 0x1102: strcpy(s, "AMD Athlon 64/Opteron DRAM controller");
		   break;

		   case 0x1103: strcpy(s, "AMD miscellaneus controller");
		   break;

		   default:     strcpy(s, "Unknown AMD device type");
		   break;
        	}
	    break;

	    case 0x1002:	/* Ati */
        	switch (id)
        	{
		   case 0x3150: strcpy(s, "Ati Mobility Radeon X600");
		   break;

		   case 0x4337: strcpy(s, "Ati Radeon 345M IGP");
		   break;

		   case 0x4341: strcpy(s, "Ati AC97 audio controller");
		   break;

		   case 0x4342: strcpy(s, "Ati standard PCI to PCI adapter");
		   break;

		   case 0x4345: strcpy(s, "Ati EHCI USB 2.0 controller");
		   break;

		   case 0x4347: strcpy(s, "Ati OHCI USB controller");
		   break;

		   case 0x4348: strcpy(s, "Ati OHCI USB controller");
		   break;

		   case 0x4349: strcpy(s, "Ati PCI IDE bus master controller");
		   break;

		   case 0x434c: strcpy(s, "Ati AC97 modem");
		   break;

		   case 0x434d: strcpy(s, "Ati standard PCI to ISA adapter");
		   break;

		   case 0x4353: strcpy(s, "Ati SM bus");
		   break;

		   case 0x4370: strcpy(s, "Ati SB400 AC97 audio controller");
		   break;

		   case 0x4371: strcpy(s, "Ati SB400 PCI to PCI bridge");
		   break;

		   case 0x4373: strcpy(s, "Ati SB400 EHCI USB 2.0 controller");
		   break;

		   case 0x4374:
		   case 0x4375: strcpy(s, "Ati SB400 OHCI USB controller");
		   break;

		   case 0x4376: strcpy(s, "Ati SB400 IDE IDE controller");
		   break;

		   case 0x4377: strcpy(s, "Ati SB400 PCI to ISA bridge");
		   break;

		   case 0x4378: strcpy(s, "Ati SB400 AC97 modem controller");
		   break;

		   case 0x4437: strcpy(s, "Ati Mobility Radeon 7000 IGP");
		   break;

		   case 0x4c66: strcpy(s, "Ati Mobility Radeon 9000 AGP");
		   break;

		   case 0x5833: strcpy(s, "Ati standard PCI to CPU host adapter");
		   break;

		   case 0x5835: strcpy(s, "Ati Mobility Radeon 9000 IGP");
		   break;

		   case 0x5838: strcpy(s, "Ati RS300M AGP");
		   break;

		   case 0x5950: strcpy(s, "Ati RS480 host bridge");
		   break;

		   case 0x5955: strcpy(s, "Ati Mobility Radeon Xpress 200");
		   break;

		   case 0x5a37:
		   case 0x5a3f: strcpy(s, "Ati RS480 PCI bridge");
		   break;

		   case 0x68e0: strcpy(s, "Ati Mobility Radeon HD 5000");
		   break;

		   case 0x7010: strcpy(s, "Ati RS200 PCI to AGP bridge");
		   break;

		   case 0xcab2: strcpy(s, "Ati standard CPU host adapter");
		   break;

		   case 0xcab3: strcpy(s, "Ati standard PCI to CPU adapter");
		   break;

		   case 0xcbb2: strcpy(s, "Ati RS200 CPU to PCI adapter");
		   break;

		   default:     strcpy(s, "Unknown Ati device type");
		   break;
        	}
	    break;

	    case 0x8086:	/* Intel */
        	switch (id)
        	{
		   case 0x0045: strcpy(s, "Intel PCI to PCI bridge");
		   break;

		   case 0x2448:
		   case 0x244e: strcpy(s, "Intel 82801 PCI bridge");
		   break;

		   case 0x24c0: strcpy(s, "Intel 82801D LPC interface controller");
		   break;

		   case 0x24c3: strcpy(s, "Intel 82801D SMBus controller");
		   break;

		   case 0x24c5: strcpy(s, "Intel 82801D AC97 Audio controller (ICH4)");
		   break;

		   case 0x24c6: strcpy(s, "Intel 82801D AC97 Modem controller (ICH4)");
		   break;

		   case 0x24c2:
		   case 0x24c4:
		   case 0x24c7: strcpy(s, "Intel 82801D USB host controller");
		   break;

		   case 0x24cd: strcpy(s, "Intel 82801D Enhanced host controller");
		   break;

		   case 0x24cb: strcpy(s, "Intel 82801D Ultra ATA storage controller");
		   break;

		   case 0x2560: strcpy(s, "Intel 82845G CPU to I/O controller");
		   break;

		   case 0x2561: strcpy(s, "Intel 82845G CPU to AGP controller");
		   break;

		   case 0x2580: strcpy(s, "Intel 915 CPU to I/O controller");
		   break;

		   case 0x2581: strcpy(s, "Intel 915 PCI Express root");
		   break;

		   case 0x2640: strcpy(s, "Intel 82801F LPC interface controller");
		   break;

		   case 0x2658: 
		   case 0x2659:
		   case 0x265a:
		   case 0x265b: strcpy(s, "Intel 82801F USB host controller");
		   break;

		   case 0x265c: strcpy(s, "Intel 82801F Enhanced host controller");
		   break;

		   case 0x2660: strcpy(s, "Intel 82801F PCI Express root port");
		   break;

		   case 0x266a: strcpy(s, "Intel 82801F SMBus controller (ICH6)");
		   break;

		   case 0x266d: strcpy(s, "Intel 82801F AC97 Modem controller (ICH6)");
		   break;

		   case 0x266e: strcpy(s, "Intel 82801F AC97 Audio controller (ICH6)");
		   break;

		   case 0x266f: strcpy(s, "Intel 82801F Ultra ATA storage controller");
		   break;

		   case 0x27a0: strcpy(s, "Mobile Intel 945 CPU to DRAM controller");
		   break;

		   case 0x27a1: strcpy(s, "Mobile Intel 945 PCI Express root");
		   break;

		   case 0x27a2:
		   case 0x27ae:
		   case 0x27a6: strcpy(s, "Mobile Intel 945 Express chipset family");
		   break;

		   case 0x27b9: strcpy(s, "Intel 82801G LPC interface controller");
		   break;

		   case 0x27c8: 
		   case 0x27c9:
		   case 0x27ca:
		   case 0x27cb: strcpy(s, "Intel 82801G USB host controller (ICH7)");
		   break;

		   case 0x27cc: strcpy(s, "Intel 82801G enhanced host controller");
		   break;

		   case 0x27c4: strcpy(s, "Intel 82801 SATA storage controller (ICH7)");
		   break;

		   case 0x27d0:
		   case 0x27d2:
		   case 0x27d4:
		   case 0x27d6: strcpy(s, "Intel 82801G PCI Express root port (ICH7)");
		   break;

		   case 0x27d8: strcpy(s, "Intel ICH7 HD audio controller");
		   break;

		   case 0x27da: strcpy(s, "Intel 82801 SMBus controller");
		   break;

		   case 0x27df: strcpy(s, "Intel 82801G ATA storage controller (ICH7)");
		   break;

		   case 0x2919: strcpy(s, "Intel ICH9 LPC interface controller");
		   break;

		   case 0x2934:
		   case 0x2935: 
		   case 0x2936:
		   case 0x2937:
		   case 0x2938: strcpy(s, "Intel ICH9 USB host controller");
		   break;

		   case 0x293a:
		   case 0x293c: strcpy(s, "Intel ICH9 Enhanced host controller");
		   break;

		   case 0x293e: strcpy(s, "Intel ICH9 HD Audio controller");
		   break;

		   case 0x2a40: strcpy(s, "Intel Mobile PM45 CPU to DRAM controller");
		   break;

		   case 0x2940:
		   case 0x2944:
		   case 0x2948:
		   case 0x2a41: strcpy(s, "Intel ICH9 PCI Express root port");
		   break;

		   case 0x2c62:
		   case 0x2d01:
		   case 0x2d10:
		   case 0x2d11:
		   case 0x2d12:
		   case 0x2d13: strcpy(s, "Intel CPU to PCI standard host controller");
		   break;

		   case 0x383c: 
		   case 0x3834: strcpy(s, "Intel 3400 series Enhanced host controller");
		   break;

		   case 0x3b09: strcpy(s, "Intel LPC interface controller");
		   break;

		   case 0x3b29: strcpy(s, "Intel AHCI SATA standard controller");
		   break;

		   case 0x3b44:
		   case 0x3b45:
		   case 0x3b42: strcpy(s, "Intel PCI to PCI bridge");
		   break;

		   case 0x3b56: strcpy(s, "Intel high definition audio controller");
		   break;

		   case 0x3b64: strcpy(s, "Intel management engine interface");
		   break;

		   default:     strcpy(s, "Unknown Intel device type");
		   break;
        	}

	    break;

	    case 0x11ab:	/* Marvell */
        	switch (id)
        	{
		   case 0x4354: strcpy(s, "Marvell Yukon 88E8084 ethernet adapter");
		   break;

		   default:     strcpy(s, "Unknown Marvell device type");
		   break;
        	}
	    break;

	    case 0x100b:	/* National Semiconductor */
        	switch (id)
        	{
		   case 0x0020: strcpy(s, "NS DP83815 fast ethernet adapter");
		   break;

		   default:     strcpy(s, "Unknown NS device type");
		   break;
        	}
	    break;

	    break;

	    case 0x1033:		/* NEC */
        	switch (id)
        	{
		   case 0x0035: strcpy(s, "NEC PCI to USB open host controller");
		   break;

		   case 0x00e0: strcpy(s, "NEC USB enhanced host controller");
		   break;

		   default:     strcpy(s, "Unknown NEC device type");
		   break;
        	}
	    break;

	    case 0x10de:		/* Nvidia */
        	switch (id)
        	{
		   case 0x01d7: strcpy(s, "Nvidia GeForce Go 7300");
		   break;

		   case 0x01d8: strcpy(s, "Nvidia GeForce Go 7400");
		   break;

		   case 0x06e9: strcpy(s, "Nvidia GeForce 9300 M GS");
		   break;

		   default:     strcpy(s, "Unknown Nvidia device type");
		   break;
        	}
	    break;


	    case 0x10ec:	/* Realtek */
        	switch (id)
        	{
		   case 0x8136: strcpy(s, "Realtek PCIe FE fast ethernet adapter");
		   break;

		   case 0x8139: strcpy(s, "Realtek RTL8139 fast ethernet adapter");
		   break;

		   case 0x8168: strcpy(s, "Realtek RTL8168 fast ethernet adapter");
		   break;

		   default:     strcpy(s, "Unknown Realtek device type");
		   break;
        	}
	    break;

	    case 0x1180:	/* Ricoh */
        	switch (id)
        	{
		   case 0x0592: strcpy(s, "Ricoh Memory Stick host controller");
		   break;

		   case 0x0822: strcpy(s, "Ricoh SD host controller");
		   break;

		   case 0x0832: strcpy(s, "Ricoh OHCI 1394 firewire controller");
		   break;

		   default:     strcpy(s, "Unknown Ricoh device type");
		   break;
        	}
	    break;

	    case 0x1039:	/* SIS */
        	switch (id)
        	{
		   case 0x0001: strcpy(s, "SIS virtual CPU to AGP bridge");
		   break;

		   case 0x0008: strcpy(s, "SIS CPU to ISA bridge");
		   break;

		   case 0x0646: strcpy(s, "SIS 645DX CPU to PCI bridge");
		   break;

		   case 0x5513: strcpy(s, "SIS 5513 EIDE controller");
		   break;

		   case 0x7001: strcpy(s, "SIS 5597 USB controller");
		   break;

		   case 0x7002: strcpy(s, "SIS 7002 USB 2.0 enhanced controller");
		   break;

		   case 0x7012: strcpy(s, "SIS 7012 audio controller");
		   break;

		   default:     strcpy(s, "Unknown SIS device type");
		   break;
        	}
	    break;

	    case 0x104c:	/* Texas Instruments */
        	switch (id)
        	{
		   case 0x8017: strcpy(s, "Texas Instruments 1394 firewire controller");
		   break;

		   case 0x8026: strcpy(s, "Texas Instruments 1394 firewire controller");
		   break;

		   case 0xac41: strcpy(s, "Texas Instruments 4410 cardbus controller");
		   break;

		   case 0xac50: strcpy(s, "Texas Instruments 1410 cardbus controller");
		   break;

		   case 0xac55: strcpy(s, "Texas Instruments 1520 cardbus controller");
		   break;

		   default:     strcpy(s, "Unknown Texas Instruments device type");
		   break;
        	}
	    break;

	    case 0x1106:	/* VIA */
        	switch (id)
        	{
		   case 0x0571: strcpy(s, "VIA VT82C586/686 EIDE controller");
		   break;

		   case 0x0586: strcpy(s, "VIA VT82C586 PCI to ISA bridge");
		   break;

		   case 0x0598: strcpy(s, "VIA VT82C598 Apollo MVP3 CPU to PCI bridge");
		   break;

		   case 0x0691: strcpy(s, "VIA VT82C691 Apollo Pro system controller");
		   break;

		   case 0x3038: strcpy(s, "VIA VT6202 USB host controller");
		   break;

		   case 0x3040: strcpy(s, "VIA VT82C572/586 PM controller");
		   break;

		   case 0x3044: strcpy(s, "VIA 1394 firewire host controller");
		   break;

		   case 0x3104: strcpy(s, "VIA VT6202 enhanced USB 2.0 host controller");
		   break;

		   case 0x8598: strcpy(s, "VIA VT82C598 CPU to AGP bridge");
		   break;

		   case 0x8691: strcpy(s, "VIA VT82C691 Apollo PCI bridge");
		   break;

		   default:     strcpy(s, "Unknown VIA device type");
		   break;
        	}
	    break;

	    case 0x14e4:	/* Broadcom */
        	switch (id)
        	{
		   case 0x1692: strcpy(s, "Broadcom Netlink ethernet adapter ");
		   break;

		   default:     strcpy(s, "Unknown Broadcom device type");
		   break;
        	}
	    break;


	    default:     strcpy(s, "Unknown device type");
	    break;

        }

	strcpy(DeviceName, s);
	return DeviceName;
}


char WINAPI *PciEx(char id[4096])
{
	char s[255]="", t[4096]="";
	DWORD address, value;


	sprintf(t, "\r\n\r\n Bus Dev Fun Vendor Device Description");
	SetPciMaxBusIndex(7);

	for(DWORD bus = 0; bus <= 255; bus++) 
	{ 
		for(DWORD dev = 0; dev < 32; dev++) 
		{ 
			for(DWORD fun = 0; fun < 8; fun++) 
			{ 
				address = PciBusDevFunc(bus, dev, fun); 
				// No Error Check 
				value = ReadPciConfigDword(address, 0x00); 

				if((value & 0xFFFF) != 0xFFFF &&
				   (value & 0xFFFF) != 0x0000) 
				{ 
					sprintf(s, "\r\n ");       		strcat(t, s);
					sprintf(s, "%02x  ",(unsigned int)bus); strcat(t, s);
					sprintf(s, "%02x  ",(unsigned int)dev); strcat(t, s);
					sprintf(s, "%02x  ",(unsigned int)fun); strcat(t, s);

					sprintf(s, "%04x   ",(unsigned int)
							     (value & 0x0000FFFF));
					strcat(t, s);
					sprintf(s, "%04x   ",(unsigned int)
							     ((value >> 16) & 0x0000FFFF));
					strcat(t, s);

					GetDeviceNameEx((value & 0x0000FFFF),
							(value >> 16) & 0x0000FFFF, s);
					strcat(t, s);
				} 
			} 
		} 
	}
	strcat(id, t);
	return id;
}

void WINAPI BeepEx(DWORD heigth, DWORD time)
{
	DWORD freq = 1193180000 / heigth; // heigth 440000 = 440Hz 
 
	WriteIoPortByte(0x43, 0xB6); 
	WriteIoPortByte(0x42, (BYTE)(freq & 0xFF)); 
	WriteIoPortByte(0x42, (BYTE)(freq >> 9)); 
 
	Sleep(100); 
 
	WriteIoPortByte(0x61, (ReadIoPortByte(0x61) | 0x03)); 
 
	Sleep(time); 			// time 1000 = 1 second
 
	WriteIoPortByte(0x61, (ReadIoPortByte(0x61) & 0xFC)); 
}


/********************************************************

   function for manage IsaBusMutex

 ********************************************************/

HANDLE ghMutex;

void OpenIsaBusMutex()
{
        ghMutex = CreateMutex(NULL, FALSE, NULL);

	/* NULL,  default security attributes;
	   FALSE, initially not owned;
	   NULL,  unnamed mutex */

	if (ghMutex == NULL)
	{
		MessageBox(NULL, "Oooops, mutex initialization failed!", "Mutex", MB_OK);
	}
}

DWORD WaitIsaBusMutex()
{
	DWORD dwWaitResult;

        dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);

	if (dwWaitResult != WAIT_OBJECT_0)
	{
		MessageBox(NULL, "Oooops, the thread can't get the mutex!", "Mutex", MB_OK);
		return 0;
	}

	return 1;
}

void ReleaseIsaBusMutex()
{
        if (! ReleaseMutex(ghMutex))
	{
		MessageBox(NULL, "Oooops, mutex release failed!", "Mutex", MB_OK);
	}
}

void CloseIsaBusMutex()
{
        CloseHandle(ghMutex);
}


/********************************************************

   function for manage Embedded Controller

 ********************************************************/

/*

enum ECStatus : byte
{
	OutputBufferFull = 0x01,    // EC_OBF
	InputBufferFull = 0x02,     // EC_IBF
                                    // 0x04 is ignored
	Command = 0x08,             // CMD
	BurstMode = 0x10,           // BURST
	SCIEventPending = 0x20,     // SCI_EVT
	SMIEventPending = 0x40      // SMI_EVT
                                    // 0x80 is ignored
}


enum ECCommand : byte
{
	Read = 0x80,            // RD_EC
	Write = 0x81,           // WR_EC
	BurstEnable = 0x82,     // BE_EC
	BurstDisable = 0x83,    // BD_EC
	Query = 0x84            // QR_EC
}

*/


byte ECStatus_OutputBufferFull = 0x01;
byte ECStatus_InputBufferFull  = 0x02;

byte ECCommand_Read  = 0x80;
byte ECCommand_Write = 0x81;


const int CommandPort 	= 0x66;            	//EC_SC
const int DataPort 	= 0x62;               	//EC_DATA

const int RWTimeout 	= 400;               	// spins original 500
const int FailuresBeforeSkip = 20;
const int MaxRetries = 10;

const int EcTimeout = 200;                	// original 200

int waitReadFailures = 0;


BYTE ReadIoPort(WORD port)
{

	return ReadIoPortByte(port);
}

void WriteIoPort(WORD port, BYTE value)
{

	WriteIoPortByte(port, value);
}


BOOL WaitForEcStatus(byte status, int isSet)
{
	int timeout = RWTimeout;

	while (timeout > 0)
	{
		timeout--;
		byte value = ReadIoPort(CommandPort);

		if (isSet)
		{
			value = (byte)~value;
		}

		if (((byte)status & value) == 0)
		{
                        return TRUE;
		}
	}
	return FALSE;
}


BOOL WaitRead()
{
	if (waitReadFailures > FailuresBeforeSkip)
	{
		return TRUE;
	}
	else if (WaitForEcStatus(ECStatus_OutputBufferFull, 1))
	{
		waitReadFailures = 0;
		return TRUE;
	}
	else
	{
		waitReadFailures++;
		return FALSE;
	}
}


BOOL WaitWrite()
{
	return WaitForEcStatus(ECStatus_InputBufferFull, 0);
}



BOOL TryWriteByte(byte reg, byte value)
{
	if (WaitWrite())
	{
		WriteIoPort(CommandPort, (byte)ECCommand_Write);

		if (WaitWrite())
		{
			WriteIoPort(DataPort, reg);

			if (WaitWrite())
			{
				WriteIoPort(DataPort, value);
                                return TRUE;
			}
		}
	}

	return FALSE;
}


BOOL TryReadByte(byte reg, byte *value)
{
	if (WaitWrite())
	{
		WriteIoPort(CommandPort, (byte)ECCommand_Read);

		if (WaitWrite())
		{
			WriteIoPort(DataPort, reg);

			if (WaitWrite() && WaitRead())
			{
                                *value = ReadIoPort(DataPort);
                                return TRUE;
			}
		}
	}

	*value = 0;
	return FALSE;
}


void WriteByte(byte reg, byte value)
{
	int writes = 0;

	while (writes < MaxRetries)
	{
		if (TryWriteByte(reg, value))
		{
                            return;
		}
		writes++;
	}
}



byte ReadByte(byte reg)
{
	byte result = 0;
	int reads = 0;

	while (reads < MaxRetries)
	{
		if (TryReadByte(reg, &result))
		{
			return result;
		}
		reads++;
	}
	return result;
}


int WINAPI GetByteOrWordFromEC(byte reg, BOOL doublebyte)
{
	byte b0, b1, b2, b3;
	int i = 0;

	if (WaitIsaBusMutex())
	{
                b0 = ReadByte(reg);
                b3 = ReadByte(reg);
                if (b0 != b3)
                    b0 = ReadByte(reg);

                i = b0;
                if (doublebyte)
                {
                    reg++;
                    b1 = ReadByte(reg);
                    b2 = ReadByte(reg);
                    if (b1 != b2)
                        b1 = ReadByte(reg);

                    i |= (int)(b1 << 8);
                }
		ReleaseIsaBusMutex();
	}
	return i;
}




void WINAPI SetByteOnEC(byte reg, byte value)
{

	if (WaitIsaBusMutex())
	{
                WriteByte(reg, value);
		ReleaseIsaBusMutex();
	}
}




char WINAPI *GetECTable(char id[4096])
{
	byte i, j, k, n;
	int h, z;
	char s[255]="", s1[255]="";
	char t[4096]= "  <o>  000 001 002 003 004 005 006 007";

 	strcat(t, " 008 009 010 011 012 013 014 015\r\n");
 	strcat(t, "  ...  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---\r\n");

	if (WaitIsaBusMutex())
	{
		z = 0;
		for (j = 0; j < 16; j++)
		{
			sprintf(s, "  %03i ", z);

			for (i = 0; i < 16; i++)
			{
                                h = (int)i + (int)j * 16;
                                n = (byte)h;
                                z = h + 1;
                                //k = ReadByte(n);
				k = GetByteOrWordFromEC(n, FALSE);
				//k = i+ j;

				sprintf(s1, " %03i", k);
				strcat(s, s1);

                                //s = s + " ";
			}

			strcat(s, "\r\n");
			strcat(t, s);
		}

		ReleaseIsaBusMutex();
	}

	strcat(id, t);
	return id;
}


