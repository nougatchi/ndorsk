#include "rdll.h"
#include <stdio.h>

runtimedll::runtimedll(char* file) {
    hInstLib = LoadLibrary(file);
    if(hInstLib != NULL) {
        ok = true;
    } else {
        printf("unable to load %s in runtime\n",file);
        ok = false;
    }
}

runtimedll::~runtimedll() {
    if(ok) {
        FreeLibrary(hInstLib);
    }
}

typedef rdll_response* (__cdecl *CALL_E_PROC)(rdll_edict*);

rdll_response* runtimedll::call_e(char* name, rdll_edict* edict) {
    if(ok) {
        CALL_E_PROC procs = (CALL_E_PROC)GetProcAddress(hInstLib,name);
        if(NULL != procs) {
            return (procs)(edict);
        } else {
            printf("unable to run %s(edict)\n",name);
            return nullptr;
        }
    } else {
        return nullptr;
    }
}

void runtimedll::call(char* name) {
    if(ok) {
        PROC procs = (PROC)GetProcAddress(hInstLib,name);
        if(NULL != procs) {
            (procs);
            return;
        } else {
            printf("unable to run %s()\n",name);
        }
    }
}

PROC runtimedll::getproc(char* name) {
    if(ok) {
        PROC procs = (PROC)GetProcAddress(hInstLib,name);
        return procs;
    }
    return nullptr;
}
