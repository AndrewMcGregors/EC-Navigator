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


void InitializeEC(HWND hdlg);
void InitializeModels(HWND hdlg);
void InitializeModelsItems(HWND hdlg);
void InitializeCustomItems(HWND hdlg);
void ECReadTemp(HWND hwnd);
void ECReadFan(HWND hwnd);
void ECReadBat(HWND hwnd);
void get_registry_key();
void set_registry_key();