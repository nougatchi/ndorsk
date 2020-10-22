#include "cvars.h"

void cvars::init() {
    add_cvar("d_version",CVAR_GLOBAL,0);
    add_cvar("r_type",CVAR_CLIENT,0);
}

cvar* cvars::add_cvar(std::string name, int flags, void* def, std::string mtype) {
    cvar nv = cvar();
    nv.name = name;
    nv.flags = flags;
    nv.value = def;
    nv.mtype = mtype;
    vars.push_back(nv);
    return &nv;
}

cvar* cvars::getcvar(std::string name) {
    for(int i = 0; i<(int)vars.size(); i++) {
        if(vars[i].name == name) {
            return &vars[i];
        }
    }
    return nullptr;
}

void cvars::setcvar(std::string name, void* value) {
    cvar* cv = getcvar(name);
    if(cv != nullptr) {
        cv->value = value;
    }
}

void cvars::dbg_printcvars() {
    #ifdef NDORSK_DBG
    for(int i = 0; i<(int)vars.size(); i++) {
        printf("CVAR %s FLAGS 0x%x VADDR 0x%p TYPE %s",vars[i].name.c_str(),vars[i].flags,vars[i].value,vars[i].mtype.c_str());
        if(vars[i].value != nullptr) {
            if(vars[i].mtype != "string") {
                if(vars[i].mtype != "pointer") {
                    printf("\n\tPossible Values:");
                    printf("\n\tfloat: %f",*(float*)vars[i].value);
                    printf("\n\tbool : %d",*(bool*)vars[i].value);
                    printf("\n\tint  : %i",*(int*)vars[i].value);
                }
            } else {
                printf("\n\tstr  : %s",(*(std::string*)vars[i].value).c_str());
            }
        }
        printf("\n");
    }
    #else
    printf("Sorry! Cant print debug cvars without NDORSK_DBG set as define compile time.\n");
    #endif
}

cvars::cvars() {
    vars = std::vector<cvar>();
}


std::vector<cvar> cvars::autocomplete(std::string v) {
    std::vector<cvar> autocompletes = std::vector<cvar>();
    for(int i = 0; i<(int)vars.size(); i++) {
        if(vars[i].name.substr(0,v.length()) == v) {
            autocompletes.push_back(vars[i]);
        }
    }
    return autocompletes;
}
