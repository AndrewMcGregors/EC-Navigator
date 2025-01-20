char WINAPI *GetDriverVersionEx(char ver[255]);
char WINAPI *GetDriverNameEx(char ver[255]);
char WINAPI *GetDllVersionEx(char ver[255]);
char WINAPI *CpuidPxEx(char id[4096]);
char WINAPI *PciEx(char id[4096]);

//   functions for manage ECTable
char WINAPI *GetECTable(char id[4096]);
int  WINAPI GetByteOrWordFromEC(byte reg, BOOL doublebyte);
void WINAPI SetByteOnEC(byte reg, byte value);

//   functions for manage IsaBusMutex
void OpenIsaBusMutex();
void WaitIsaBusMutex();
void ReleaseIsaBusMutex();
void CloseIsaBusMutex();

//  other functions
void WINAPI BeepEx(DWORD heigth, DWORD time);




