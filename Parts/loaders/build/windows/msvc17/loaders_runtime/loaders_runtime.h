// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the LOADERSRUNTIME_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// LOADERSRUNTIME_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LOADERSRUNTIME_EXPORTS
#define LOADERSRUNTIME_API __declspec(dllexport)
#else
#define LOADERSRUNTIME_API __declspec(dllimport)
#endif

// This class is exported from the dll
class LOADERSRUNTIME_API Cloadersruntime {
public:
	Cloadersruntime(void);
	// TODO: add your methods here.
};

extern LOADERSRUNTIME_API int nloadersruntime;

LOADERSRUNTIME_API int fnloadersruntime(void);
