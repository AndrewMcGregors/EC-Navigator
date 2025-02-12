#include "dll-load.h"
#include "ECNvidia.h"

/* path to library */

#define PATH_DLL   "C:\\Windows\\System32\\nvapi.dll"
#define PATH_NVAPI "c:\\Windows\\System32\\nvapi.dll"


/* Pointers to DLL functions */

NvStatus (*NvAPI_Initialize)();
NvStatus (*NvAPI_EnumPhysicalGPUs)(NvPhysicalGpuHandleArray*, NvU32*);
NvStatus (*NvAPI_EnumNvidiaDisplayHandle)(NvU32, NvDisplayHandle*);
NvStatus (*NvAPI_GetInterfaceVersionString)(NvAPI_ShortString*);
NvStatus (*NvAPI_GetDisplayDriverVersion)(NvDisplayHandle*, TNvDisplayDriverVersion*);
NvStatus (*NvAPI_GetPhysicalGPUsFromDisplay)(NvDisplayHandle*, NvPhysicalGpuHandleArray*, NvU32*);
NvStatus (*NvAPI_GetErrorMessage)(NvStatus, NvAPI_ShortString*);
NvStatus (*NvAPI_GPU_GetFullName)(NvPhysicalGpuHandle, NvAPI_ShortString*);
NvStatus (*NvAPI_GPU_GetThermalSettings)(NvPhysicalGpuHandle, NvU32, NvGPUThermalSettings*);
NvStatus (*NvAPI_GPU_GetBusType)(NvPhysicalGpuHandle, NV_GPU_BUS_TYPE*);
NvStatus (*NvAPI_GPU_GetSystemType)(NvPhysicalGpuHandle, NV_SYSTEM_TYPE*);
NvStatus (*NvAPI_GPU_GetVbiosVersionString)(NvPhysicalGpuHandle, NvAPI_ShortString*);
NvStatus (*NvAPI_SYS_GetChipSetInfo)(TNvChipsetInfo*);
NvStatus (*NvAPI_Unload)();




void nvapi_init_all(HWND hwnd);
void nvapi_close_all();

