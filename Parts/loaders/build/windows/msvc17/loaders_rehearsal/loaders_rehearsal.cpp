// loaders_runtime.cpp : Defines the exported functions for the DLL.
//

#include "framework.h"
#include "loaders_rehearsal.h"


// This is an example of an exported variable
LOADERSREHEARSAL_API int nloadersruntime=0;

// This is an example of an exported function.
LOADERSREHEARSAL_API int fnloadersruntime(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
Cloadersruntime::Cloadersruntime()
{
    return;
}
