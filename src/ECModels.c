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
#include "Ols\OlsApi.h"
#include "ECOlsEx.h"
#include "ECMessage.h"
#include "ECModels.h"
#include "ECFanController.h"

extern int NvGetTemp();
extern BOOL fan_update(const unsigned char value);

//extern int GetByteOrWordFromEC(byte value, int doublebyte);
/*
enum
{
        MODEL_UNSUPPORTED = 0,
	MODEL_ACER_ASPIRE_5742G,
	MODEL_ACER_TRAVELMATE_4000,
	MODEL_ASUS_F3JC,
	MODEL_COMPAQ_R3000,
	MODEL_FUJITSU_SIEMENS_AMILOD,
	MODEL_HP_COMPAQ_EVO_N1020,
	MODEL_HP_COMPAQ_NX9010,
	MODEL_HP_PAVILION_DV5000,
	MODEL_HP_PAVILION_ZD8000,
	MODEL_NC_CLEVO_D400E,
        MODEL_SAMSUNG_N130,
        MODEL_SONY_VAIO_CS21S,
	MODEL_SONY_VAIO_FE31B,
	MODEL_TOSHIBA_SATELLITE_SA60,
        MODEL_END
};
*/


// Here are stored the register number for temperatures
int Temp1_reg = 0, Temp2_reg = 0, Temp3_reg = 0;
int Temp4_reg = 0, Temp5_reg = 0, Temp6_reg = 0;

// Here are stored the byte/word setting for get temperatures
int Temp1_reg_double = 0, Temp2_reg_double = 0, Temp3_reg_double = 0;
int Temp4_reg_double = 0, Temp5_reg_double = 0, Temp6_reg_double = 0;

// Here are stored the register number for get fan speed
int Fan1_reg = 0, Fan2_reg = 0;

// Here are stored the register number for set fan speed
int Fan1C_reg = 0, Fan2C_reg = 0;

// Here are stored the byte/word setting for get fan speed
int Fan1_reg_double = 1, Fan2_reg_double = 1;

// Here are stored the register number for battery
int BatV_reg = 0, BatA_reg = 0, BatAM_reg = 0, BatAD_reg = 0;

// Here are stored the model number for selected PC
int LaptopModel = -1;


BOOL BatPresent = FALSE;
BOOL FanOn      = FALSE;



int get_single_key(HKEY h, LPCSTR key)
{
	char buf[256];
	DWORD bufSize = 256;
	int key_reg = 0;

	bufSize = sizeof(buf);

	if (RegQueryValueEx(h,  (LPCSTR)key, NULL, NULL,
				(LPBYTE)buf, &bufSize) == ERROR_SUCCESS)
        {
		sscanf(buf, "%i", &key_reg);
	}

	return key_reg;
}


void get_registry_key()
{
	char *RegPathForEC = "SOFTWARE\\Andrew McGregor Productions\\EC Navigator";
	//char buf[256];
	//DWORD bufSize = 256;
	HKEY hKey;
	//char s[32];

	//bufSize = sizeof(buf);

	if (RegOpenKeyEx(HKEY_CURRENT_USER, RegPathForEC, 0, 
			 KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
        {

		Temp1_reg = get_single_key(hKey, "Temp1" );
		Temp2_reg = get_single_key(hKey, "Temp2" );
		Temp3_reg = get_single_key(hKey, "Temp3" );
		Temp4_reg = get_single_key(hKey, "Temp4" );
		Temp5_reg = get_single_key(hKey, "Temp5" );
		Temp6_reg = get_single_key(hKey, "Temp6" );

		Fan1_reg  = get_single_key(hKey, "Fan1"  );
		Fan2_reg  = get_single_key(hKey, "Fan2"  );

		BatV_reg  = get_single_key(hKey, "BatV"  );
		BatA_reg  = get_single_key(hKey, "BatA"  );
		BatAM_reg = get_single_key(hKey, "BatAM" );
		BatAD_reg = get_single_key(hKey, "BatAD" );

		Temp1_reg_double = get_single_key(hKey, "Temp1_double" );
		Temp2_reg_double = get_single_key(hKey, "Temp2_double" );
		Temp3_reg_double = get_single_key(hKey, "Temp3_double" );
		Temp4_reg_double = get_single_key(hKey, "Temp4_double" );
		Temp5_reg_double = get_single_key(hKey, "Temp5_double" );
		Temp6_reg_double = get_single_key(hKey, "Temp6_double" );

		Fan1_reg_double  = get_single_key(hKey, "Fan1_double"  );
		Fan2_reg_double  = get_single_key(hKey, "Fan2_double"  );

		LaptopModel  = get_single_key(hKey, "LaptopModel" );


		//sprintf(s, "RegEnumKeyEx LastError %i.", RegEnumKeyEx(hKey, 0,
			    //(LPBYTE)buf, &bufSize, NULL, NULL, NULL, NULL));
		//MessageBox(NULL, s, buf, MB_OK);

		//ERROR_MORE_DATA 	234 (0xEA) More data is available.
		//ERROR_ACCESS_DENIED 	5 (0x5)    Access is denied.

		RegCloseKey(hKey);
	}
	//else	ec_note(" RegPathForEC not found!");
	//else	MessageBox(NULL, " RegPathForEC not found!", "get_registry_key", MB_OK);
}



void set_single_key(HKEY h, LPCSTR key, int val)
{
	char buf[8];
	DWORD bufSize = 3;

	sprintf(buf, "%i", val);
	buf[bufSize] = 0;

	if (RegSetValueEx(h, (LPCSTR)key, 0, REG_SZ,
			     (BYTE *)buf, bufSize+1) != ERROR_SUCCESS)
	{
		ec_note("RegSetValueEx failed!");
		//MessageBox(NULL, " RegSetValueEx failed!", "set_registry_key", MB_OK);
	}
}


void set_registry_key()
{
	char *RegPathForEC = "SOFTWARE\\Andrew McGregor Productions\\EC Navigator";
	//char buf[8];
	//DWORD bufSize = 3;
	HKEY hKey;
	//char s[32];


	if (RegCreateKeyEx(HKEY_CURRENT_USER, RegPathForEC, 0, NULL, 0,
			   KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
	{

		set_single_key(hKey, "Temp1" , Temp1_reg);
		set_single_key(hKey, "Temp2" , Temp2_reg);
		set_single_key(hKey, "Temp3" , Temp3_reg);
		set_single_key(hKey, "Temp4" , Temp4_reg);
		set_single_key(hKey, "Temp5" , Temp5_reg);
		set_single_key(hKey, "Temp6" , Temp6_reg);

		set_single_key(hKey, "Fan1"  , Fan1_reg );
		set_single_key(hKey, "Fan2"  , Fan2_reg );

		set_single_key(hKey, "BatV"  , BatV_reg );
		set_single_key(hKey, "BatA"  , BatA_reg );
		set_single_key(hKey, "BatAM" , BatAM_reg);
		set_single_key(hKey, "BatAD" , BatAD_reg);

		set_single_key(hKey, "Temp1_double" , Temp1_reg_double);
		set_single_key(hKey, "Temp2_double" , Temp2_reg_double);
		set_single_key(hKey, "Temp3_double" , Temp3_reg_double);
		set_single_key(hKey, "Temp4_double" , Temp4_reg_double);
		set_single_key(hKey, "Temp5_double" , Temp5_reg_double);
		set_single_key(hKey, "Temp6_double" , Temp6_reg_double);

		set_single_key(hKey, "Fan1_double"  , Fan1_reg_double );
		set_single_key(hKey, "Fan2_double"  , Fan2_reg_double );

		set_single_key(hKey, "LaptopModel" , LaptopModel );


		RegCloseKey(hKey);		

	}
	//else	MessageBox(NULL, " RegPathForEC not created!", "set_registry_key", MB_OK);
	else	ec_note("RegPathForEC not created!");

}



void InitializeCombo(HWND hdlg, int cbox, int reg)
{
	int i;
	char s[255];
        HWND h;

	h = GetDlgItem(hdlg, cbox);
	SendMessage(h, CB_RESETCONTENT, 0, 0);
	for (i=0; i<256; i++)
	{
		sprintf(s, "%3i", i);
		SendMessage(h, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);
	}
	SendMessage(h, CB_SETCURSEL, reg, 0);
}


void InitializeModels(HWND hdlg)
{
	//int i = 0;
	//char s[255];
        HWND h;


	h = GetDlgItem(hdlg, IDC_CB_MODELS);
	SendMessage(h, CB_RESETCONTENT, 0, 0);

	SendMessage(h, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)" -- Custom values --");
	SendMessage(h, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Acer Aspire 5742G");
	SendMessage(h, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Acer Travelmate 4000/4100");
	SendMessage(h, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Asus F3JC");
	SendMessage(h, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Compaq R3000");
	SendMessage(h, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Fujitsu Siemens Amilo D");
	SendMessage(h, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"HP Compaq EVO N1020");
	SendMessage(h, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"HP Compaq NX9010");
	SendMessage(h, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"HP Pavilion DV 5000");
	SendMessage(h, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"HP Pavilion ZD 8000");
	SendMessage(h, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Notebook Computer Clevo D400E");
	SendMessage(h, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Samsung N130");
	SendMessage(h, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Sony Vaio VGN CS21S");
	SendMessage(h, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Sony Vaio VGN FE31B");
	SendMessage(h, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Toshiba Satellite SA60");

	SendMessage(h, CB_SETCURSEL, LaptopModel, 0);

	InitializeCombo(hdlg, IDC_CB_TEMP1,  Temp1_reg);
	InitializeCombo(hdlg, IDC_CB_TEMP2,  Temp2_reg);
	InitializeCombo(hdlg, IDC_CB_TEMP3,  Temp3_reg);

	InitializeCombo(hdlg, IDC_CB_FAN1,   Fan1_reg);
	InitializeCombo(hdlg, IDC_CB_FAN2,   Fan2_reg);

	InitializeCombo(hdlg, IDC_CB_BATV,  BatV_reg);
	InitializeCombo(hdlg, IDC_CB_BATA,  BatA_reg);
	InitializeCombo(hdlg, IDC_CB_BATAM, BatAM_reg);
	InitializeCombo(hdlg, IDC_CB_BATAD, BatAD_reg);

	ec_message(hdlg, IDC_CBUT_TEMP1_DBL, BM_SETCHECK, Temp1_reg_double, 0);
	ec_message(hdlg, IDC_CBUT_TEMP2_DBL, BM_SETCHECK, Temp2_reg_double, 0);
	ec_message(hdlg, IDC_CBUT_TEMP3_DBL, BM_SETCHECK, Temp3_reg_double, 0);

	ec_message(hdlg, IDC_CBUT_FAN1_DBL,  BM_SETCHECK, Fan1_reg_double, 0);
	ec_message(hdlg, IDC_CBUT_FAN2_DBL,  BM_SETCHECK, Fan2_reg_double, 0);

};


void InitializeModelsItems(HWND hdlg)
{
        //HWND h;

	ec_message(hdlg, IDC_CB_TEMP1, CB_SETCURSEL, Temp1_reg, 0);
	ec_message(hdlg, IDC_CB_TEMP2, CB_SETCURSEL, Temp2_reg, 0);
	ec_message(hdlg, IDC_CB_TEMP3, CB_SETCURSEL, Temp3_reg, 0);

	ec_message(hdlg, IDC_CB_FAN1, CB_SETCURSEL, Fan1_reg, 0);
	ec_message(hdlg, IDC_CB_FAN2, CB_SETCURSEL, Fan2_reg, 0);

	ec_message(hdlg, IDC_CB_BATV,  CB_SETCURSEL, BatV_reg,  0);
	ec_message(hdlg, IDC_CB_BATA,  CB_SETCURSEL, BatA_reg,  0);
	ec_message(hdlg, IDC_CB_BATAM, CB_SETCURSEL, BatAM_reg, 0);
	ec_message(hdlg, IDC_CB_BATAD, CB_SETCURSEL, BatAD_reg, 0);

	ec_message(hdlg, IDC_CBUT_TEMP1_DBL, BM_SETCHECK, Temp1_reg_double, 0);
	ec_message(hdlg, IDC_CBUT_TEMP2_DBL, BM_SETCHECK, Temp2_reg_double, 0);
	ec_message(hdlg, IDC_CBUT_TEMP3_DBL, BM_SETCHECK, Temp3_reg_double, 0);

	ec_message(hdlg, IDC_CBUT_FAN1_DBL, BM_SETCHECK, Fan1_reg_double, 0);
	ec_message(hdlg, IDC_CBUT_FAN2_DBL, BM_SETCHECK, Fan2_reg_double, 0);


}

int check_reg(int reg)
{
	if (!((reg >= 0) && (reg < 256)))
		return 999;
	return reg;
}

void InitializeCustomItems(HWND hdlg)
{

	if (LaptopModel == MODEL_UNSUPPORTED)
	{

		Temp1_reg = ec_message(hdlg, IDC_CB_TEMP1, CB_GETCURSEL, 0, 0);
		Temp2_reg = ec_message(hdlg, IDC_CB_TEMP2, CB_GETCURSEL, 0, 0);
		Temp3_reg = ec_message(hdlg, IDC_CB_TEMP3, CB_GETCURSEL, 0, 0);

		Fan1_reg = check_reg(ec_message(hdlg, IDC_CB_FAN1, CB_GETCURSEL, 0, 0));
		Fan2_reg = check_reg(ec_message(hdlg, IDC_CB_FAN2, CB_GETCURSEL, 0, 0));

		BatV_reg = ec_message(hdlg, IDC_CB_BATV, CB_GETCURSEL, 0, 0);
		BatA_reg = ec_message(hdlg, IDC_CB_BATA, CB_GETCURSEL, 0, 0);
		BatAM_reg = ec_message(hdlg, IDC_CB_BATAM, CB_GETCURSEL, 0, 0);
		BatAD_reg = ec_message(hdlg, IDC_CB_BATAD, CB_GETCURSEL, 0, 0);

		Temp1_reg_double = ec_message(hdlg, IDC_CBUT_TEMP1_DBL, BM_GETCHECK, 0, 0);
		Temp2_reg_double = ec_message(hdlg, IDC_CBUT_TEMP2_DBL, BM_GETCHECK, 0, 0);
		Temp3_reg_double = ec_message(hdlg, IDC_CBUT_TEMP3_DBL, BM_GETCHECK, 0, 0);

		Fan1_reg_double = ec_message(hdlg, IDC_CBUT_FAN1_DBL, BM_GETCHECK, 0, 0);
		Fan2_reg_double = ec_message(hdlg, IDC_CBUT_FAN2_DBL, BM_GETCHECK, 0, 0);


	}
}


void InitializeEC(HWND hdlg)
{

	if (LaptopModel == MODEL_UNSUPPORTED)
	{

		//Temp1_reg = 0;
		//Temp2_reg = 0;
		//Temp3_reg = 0;

		//Fan1_reg = 0;	Fan1_reg_double = 1;

		//BatV_reg = 0;	BatA_reg = 0;
		//BatAD_reg = 0;	BatAM_reg = 0;

		//not here because items have not jet been created
		//InitializeCustomItems(hdlg);
	}
	else if (LaptopModel == MODEL_SONY_VAIO_CS21S)
	{
		Temp1_reg = 88;		Temp2_reg = 84;
		Temp3_reg = 76;		Temp3_reg_double = 1; // or 63?

		Temp6_reg = 999; // fake for Nvidia Gpu Temp

		Fan1_reg = 102;		Fan1_reg_double = 1;


		BatV_reg = 74;	BatA_reg = 72;
		BatAD_reg = 0;	BatAM_reg = 114;
		//BatteryProgressBar.Maximum = 4800; //used for mAh measure
	}
	else if (LaptopModel == MODEL_ASUS_F3JC)
	{
		Temp1_reg = 166;	Temp2_reg = 167;
		Temp6_reg = 999;	// fake for Nvidia Gpu Temp

		Fan1_reg = 147;		Fan1_reg_double = 1;

		BatV_reg  = 178;	BatA_reg  = 180;
		BatAD_reg = 240;	BatAM_reg = 244;	//243 on certain batteries

	}
	else if (LaptopModel == MODEL_SONY_VAIO_FE31B)
	{
		Temp1_reg = 64;		Temp2_reg = 66;	
		Temp4_reg = 67;		Temp5_reg = 65;
		Temp6_reg = 999;	// fake for Nvidia Gpu Temp

		Fan1_reg = 70;		Fan1_reg_double = 1;
	}
	else if (LaptopModel == MODEL_TOSHIBA_SATELLITE_SA60)
	{
		Temp1_reg = 156;
		Temp3_reg = 0;

		Fan1_reg = 252;		Fan1_reg_double = 0;	
		Fan2_reg = 253;		Fan2_reg_double = 0;

		BatV_reg  = 0;		BatA_reg  = 168;
		BatAD_reg = 172;	BatAM_reg = 170;
	}
	else if (LaptopModel == MODEL_HP_PAVILION_ZD8000)
	{
		Temp1_reg = 88;		Temp2_reg = 94;

		Fan1_reg = 65;		Fan1_reg_double = 0;

		BatV_reg  = 197;	BatA_reg  = 195;
		//BatAD_reg = 172;	BatAM_reg = 170;
	}
	else if (LaptopModel == MODEL_HP_PAVILION_DV5000)
	{
		Temp1_reg = 176;	Temp2_reg = 177;
		Temp3_reg = 0;

	}
	else if (LaptopModel == MODEL_HP_COMPAQ_EVO_N1020)
	{
		Temp1_reg = 219;	Temp2_reg = 220;
		Temp3_reg = 224;	Temp3_reg_double = 1;

		Fan1_reg = 72;		Fan1_reg_double = 0;
		Fan1_reg = 73;		Fan1_reg_double = 0;

		BatV_reg  = 165;	BatA_reg  = 161;
		BatAD_reg = 137;	BatAM_reg = 141;
	}
	else if (LaptopModel == MODEL_HP_COMPAQ_NX9010)
	{
		Temp1_reg = 85;
		Temp3_reg = 143;

		BatV_reg  = 92;		BatA_reg  = 90;
		BatAM_reg = 88;

	}
	else if (LaptopModel == MODEL_ACER_TRAVELMATE_4000)
	{
		Temp1_reg = 88;		Temp2_reg = 94;
		Temp3_reg = 0;

		Fan1_reg  = 92;		Fan1_reg_double = 0;

		BatV_reg  = 197;	BatA_reg  = 195;
		BatAD_reg = 220;	BatAM_reg = 216;
	}
	else if (LaptopModel == MODEL_FUJITSU_SIEMENS_AMILOD)
	{
		Temp1_reg = 65;		Temp2_reg = 66;
		Temp3_reg = 0;

		Fan1_reg  = 139;	Fan1_reg_double = 0;

		BatV_reg  = 56;		BatA_reg  = 54;
		BatAD_reg = 2;		BatAM_reg = 4;
	}
	else if (LaptopModel == MODEL_NC_CLEVO_D400E)
	{
		Temp1_reg = 10;		Temp1_reg_double = 1;
		Temp3_reg = 0;

		//SetByteOnEC(7, 12);
	}
	else if (LaptopModel == MODEL_SAMSUNG_N130)
	{
		Temp1_reg = 192;	Temp2_reg = 194;
		Temp4_reg = 195;

		Fan1_reg  = 0;		Fan1_reg_double = 0;

		BatV_reg  = 165;	BatA_reg  = 161;  // 163 remaining m-ampere to complete
		BatAD_reg = 176;	BatAM_reg = 178;
	}
	else if (LaptopModel == MODEL_COMPAQ_R3000)
	{
		Temp1_reg = 176;	Temp2_reg = 171;
		Temp3_reg = 0;

		// 14.8 V on reg 200
		BatV_reg  = 198;	BatA_reg  = 194;
		BatAD_reg = 202;	BatAM_reg = 204;
	}
	else if (LaptopModel == MODEL_ACER_ASPIRE_5742G)
	{
		Temp1_reg = 178;	Temp2_reg = 180;
		Temp3_reg = 0;

		Fan2_reg  = 0;		Fan1_reg_double = 1;
		Fan1_reg = 999;		// fake for Cpu Fan Speed

		BatV_reg  = 200;	BatA_reg  = 194;
		BatAD_reg = 202;	BatAM_reg = 204;

		FanOn = FALSE;		fan_update(0x76);
	}
	else
	{
		Temp1_reg = 0;		Temp2_reg = 0;
		Temp3_reg = 0;

		Fan1_reg = 0;		Fan1_reg_double = 1;

		BatV_reg = 0;		BatA_reg = 0;
		BatAD_reg = 0;		BatAM_reg = 0;

	}
	

};


void ECReadTemp(HWND hwnd)
{
	char s[255];
	int i = 0;
	byte b = 0;
	HWND hTemp;

	// this add all temperarures
	hTemp = GetDlgItem(hwnd, IDC_LISTBOX_TEMP);
	SendMessage(hTemp, LB_RESETCONTENT, 0, 0);

	b = Temp1_reg;   // acquire register number to read CPU temp

	if (b > 0)
	{
		//i  = GetByteOrWordFromEC(b, Temp1registerCheckBox.Checked);
		i  = GetByteOrWordFromEC(b, Temp1_reg_double);


		//correct temperature x Notebook Computer Clevo D400E which is in Kelvin
		if (LaptopModel == MODEL_NC_CLEVO_D400E)
			i = (i - 2732) / 10;


		//s = i.ToString();
		sprintf(s, "CPU     %3i °C", i);
		//s = "CPU     " + s + " °C";
		if ((i > 0) && (i < 200))
			SendMessage(hTemp, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);

		//start or stop fan x ACER_ASPIRE_5742G basing on cpu temperature
		if (LaptopModel == MODEL_ACER_ASPIRE_5742G)
		{
			if ((i > 49) && (i < 200) && (FanOn == FALSE))
			{
				fan_update(0x77);
				FanOn = TRUE;
			}
			else if ((i < 45) && (i > 0) && (FanOn == TRUE))
			{
			        fan_update(0x76);
				FanOn = FALSE;
			}
		}
		//start fan x SONY_VAIO_FE31B basing on cpu temperature
		if (LaptopModel == MODEL_SONY_VAIO_FE31B)
		{
			if ((i > 49) && (i < 200) && (FanOn == FALSE))
			{
				SetByteOnEC(92, 11);
				FanOn = TRUE;
			}

		}
/*
		if (LaptopModel == MODEL_HP_PAVILION_ZD8000)
		{
			if ((i > 49) && (i < 200) && (FanOn == FALSE))
			{
				SetByteOnEC(65, 23);
				FanOn = TRUE;
			}
			else if ((i < 47) && (i > 0) && (FanOn == TRUE))
			{
			        SetByteOnEC(65, 7);
				FanOn = FALSE;
			}
		}
*/

	}

	b = Temp2_reg;   // acquire register number to read Board temp

	if (b > 0)
	{
		i  = GetByteOrWordFromEC(b, Temp2_reg_double);

		sprintf(s, "Board   %3i °C", i);
		if ((i > 0) && (i < 200))
			SendMessage(hTemp, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);

	}

	b = Temp3_reg;   // acquire register number to read Battery temp

	if (b > 0)
	{
		i  = GetByteOrWordFromEC(b, Temp3_reg_double);

		//correct bat temp x some modls which is reported in Kelvin
		if ((LaptopModel == MODEL_HP_COMPAQ_EVO_N1020) ||
		    (LaptopModel == MODEL_NC_CLEVO_D400E) ||
		    (LaptopModel == MODEL_SONY_VAIO_CS21S))
                	i = (i - 2732) / 10;

		sprintf(s, "Bat     %3i °C", i);
		if ((i > 0) && (i < 200))
			SendMessage(hTemp, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);

	}

	b = Temp4_reg;   // acquire register number to read Board temp

	if (b > 0)
	{
		i  = GetByteOrWordFromEC(b, Temp4_reg_double);

		sprintf(s, "Aux1    %3i °C", i);
		if ((i > 0) && (i < 200))
			SendMessage(hTemp, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);

	}

	b = Temp5_reg;   // acquire register number to read Board temp

	if (b > 0)
	{
		i  = GetByteOrWordFromEC(b, Temp5_reg_double);

		sprintf(s, "Aux2    %3i °C", i);
		if ((i > 0) && (i < 200))
			SendMessage(hTemp, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);

	}
	b = Temp6_reg;   // acquire info to read GPU temp

	if (b > 0)
	{
		i = (int)NvGetTemp();
		//i = 0;
		
		sprintf(s, "GPU     %3i °C", i);
		if ((i > 0) && (i < 200))
			SendMessage(hTemp, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);

	}
};


void ECReadFan(HWND hwnd)
{
	char s[255];
	int i = 0,  j = 0;
	byte b = 0;
	HWND hFan;


	// this add all fans
	hFan = GetDlgItem(hwnd, IDC_LISTBOX_FAN);
	SendMessage(hFan, LB_RESETCONTENT, 0, 0);

	b = Fan1_reg;   // acquire register number to read CPU temp

	if (b > 0)
	{
		i  = GetByteOrWordFromEC(b, Fan1_reg_double);

		//correct fan speed x F3JC
		if (LaptopModel == MODEL_ASUS_F3JC)
		{
			if ((i > 100) && (i < 50000))
                        {
                                j = i / 10;
                                i = (23438 / j)*10;
                        }
			else i = 0;
		}
		//correct fan speed x Acer Travelmate 4000/4100
		if (LaptopModel == MODEL_ACER_TRAVELMATE_4000)
                	i = 10 * i;

		//correct fan speed x Toshiba Satellite SA60
		if (LaptopModel == MODEL_TOSHIBA_SATELLITE_SA60)
                	i = 10 * (255 - i);

		//correct fan speed x HP Pavilion ZD 8000
		if (LaptopModel == MODEL_HP_PAVILION_ZD8000)
                {
			i = (int)(i & 16);  // b1 AND 00010000
			i = i * 80;
                 }

		//correct fan speed x HP Compaq Evo N1020
		if (LaptopModel == MODEL_HP_COMPAQ_EVO_N1020)
                	i = 12 * i;

		//correct fan speed x Fujitsu Siemens Amilo D...
		if (LaptopModel == MODEL_FUJITSU_SIEMENS_AMILOD)
			i = 600 * i;

		//correct fan speed x ACER ASPIRE 5742G...
		if (LaptopModel == MODEL_ACER_ASPIRE_5742G)
		{
			if ((FanOn == TRUE))
			{
				i = 3000;
			}
			else
			{
			        i = 0;
			}
		}

		sprintf(s, "Fan1     %4i Rpm", i);
		if ((i > 0) && (i < 5000))
			SendMessage(hFan, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);

	}

	b = Fan2_reg;   // acquire register number to read Board temp

	if (b > 0)
	{
		i  = GetByteOrWordFromEC(b, Fan2_reg_double);

		//correct fan speed x Toshiba Satellite SA60
		if (LaptopModel == MODEL_TOSHIBA_SATELLITE_SA60)
                	i = 10 * (255 - i);

		//correct fan speed x HP Compaq Evo N1020
		if (LaptopModel == MODEL_HP_COMPAQ_EVO_N1020)
                	i = 12 * i;

		sprintf(s, "Fan2     %4i Rpm", i);
		if ((i > 0) && (i < 5000))
			SendMessage(hFan, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);

	}

};


void ECReadBat(HWND hwnd)
{
	char s[255];
	int i = 0, j = 0, k = 0;
	byte b = 0;
	int watt = 0, amp = 0, volt = 0;
	HWND hBat, hPbarBat;


	// this add batt
	hBat = GetDlgItem(hwnd, IDC_LISTBOX_BAT);
	hPbarBat = GetDlgItem(hwnd, IDC_PBAR_BAT);
	SendMessage(hBat, LB_RESETCONTENT, 0, 0);

	b = BatV_reg;   // acquire register number to read battery voltage

	if (b > 0)
	{
		volt  = GetByteOrWordFromEC(b, 1);

		//correct bat voltage x Toshiba Satellite SA60
		if (LaptopModel == MODEL_TOSHIBA_SATELLITE_SA60)
			volt = 14800;

		if ((volt > 0) && (volt < 65535))
		     BatPresent = TRUE;
		else BatPresent = FALSE;

		sprintf(s, "BatV     %5i mV", volt);
		if ((BatPresent))
			SendMessage(hBat, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);

	}

	b = BatA_reg;   // acquire register number to read battery amperage

	if (b > 0)
	{
		amp  = GetByteOrWordFromEC(b, 1);

		sprintf(s, "BatA     %5i mA", amp);
		if (BatPresent)
			SendMessage(hBat, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);

		watt = (int)(volt / 1000) * amp ;
		sprintf(s, "BatW     %5i mWh", watt);

		if (BatPresent)
			SendMessage(hBat, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);
	}

	b = BatAM_reg;   // acquire register number to read battery max amperage

	if (b > 0)
	{
		if (LaptopModel == MODEL_SAMSUNG_N130)
		{
			i = GetByteOrWordFromEC(b, 0);
			k = GetByteOrWordFromEC(b+1, 0);
			i = i*256 + k;
		}
		else	i  = GetByteOrWordFromEC(b, 1);

		if (LaptopModel == MODEL_HP_COMPAQ_NX9010)
			i  = i + amp;

		sprintf(s, "BatAMax  %5i mAh", i);
		if (BatPresent)
			SendMessage(hBat, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);

		if ((amp > 0) && (i > 0))
			j = (INT)(((amp*10)/i)*10);

		sprintf(s, "BatChrg  %5i %%", j);

		if (BatPresent)
			SendMessage(hBat, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);

		if (BatPresent)
			SendMessage(hPbarBat, PBM_SETPOS, (WPARAM)j, 0);
	}

	b = BatAD_reg;   // acquire register number to read battery designed amperage

	if (b > 0)
	{
		if (LaptopModel == MODEL_SAMSUNG_N130)
		{
			j = GetByteOrWordFromEC(b, 0);
			k = GetByteOrWordFromEC(b+1, 0);
			j = j*256 + k;
		}
		else	j  = GetByteOrWordFromEC(b, 1);

		sprintf(s, "BatAOri  %5i mAh", j);
		if (BatPresent)
			SendMessage(hBat, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);

		if (j > 100)
			j = (int)(100 - i/(j/100));

		sprintf(s, "BatWL    %5i %%", j);
		if (BatPresent)
			SendMessage(hBat, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)s);

	}
};




