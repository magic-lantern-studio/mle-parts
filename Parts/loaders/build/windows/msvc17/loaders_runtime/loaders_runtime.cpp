// loaders_runtime.cpp : Defines the exported functions for the DLL.
//

#include "framework.h"
#include "loaders_runtime.h"


// This is an example of an exported variable
LOADERSRUNTIME_API int nloadersruntime=0;

// This is an example of an exported function.
LOADERSRUNTIME_API int fnloadersruntime(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
Cloadersruntime::Cloadersruntime()
{
    return;
}
