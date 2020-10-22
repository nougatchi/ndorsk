#ifndef CVARS_H
#define CVARS_H
#include <string>
#include <vector>

#define CVAR_GLOBAL 0b00000001
#define CVAR_SERVER 0b00000010
#define CVAR_CLIENT 0b00000100
#define CVAR_REPLIC 0b00001000

class cvar {
public:
        std::string name;
        void* value;
        int flags;
        std::string mtype;
        void f_set(float &v);
        void s_set(std::string &v);
};

class cvars {
public:
        cvar* getcvar(std::string name);
        void setcvar(std::string name, void* value);
        cvar* add_cvar(std::string name, int flags, void* def, std::string mtype = "unknown");
        void dbg_printcvars();
        std::vector<cvar> autocomplete(std::string v);
        void init();
        cvars();
protected:
        std::vector<cvar> vars;
};

#endif
