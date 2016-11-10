#ifndef __NPS_DLL_Example_h
#define __NPS_DLL_Example_h

#ifdef NPS_DLL_EXAMPLE_EXPORTS
#define NPS_DLL_EXAMPLE_API __declspec(dllexport)
#else
#define NPS_DLL_EXAMPLE_API __declspec(dllimport)
#endif

#include <Authif.h>

#ifdef __cplusplus
extern "C" {
#endif
	DWORD NPS_DLL_EXAMPLE_API WINAPI RadiusExtensionProcess2(
		_Inout_  PRADIUS_EXTENSION_CONTROL_BLOCK pECB
		);
	
#ifdef __cplusplus
}
#endif

int startSocket();
void LogMessage(const char * szMsg);




#endif /* __NPS_DLL_Example_h */