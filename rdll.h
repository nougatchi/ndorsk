#ifndef RDLL_H
#define RDLL_H
#include <windows.h>

struct vector3d {
    float x;
    float y;
    float z;
};

struct vector2d {
    float x;
    float y;
};

struct rdll_edict {
    vector3d* xyz;
    vector3d* angles;
    vector3d* acceleration;

    // user variables
    void* puser0; // ptr
    void* puser1;
    void* puser2;
    float fuser0; // float
    float fuser1;
    float fuser2;
    int iuser0; // int
    int iuser1;
    int iuser2;
};

struct rdll_response {
    void* object;
    int type;
    bool success;
};

union rdll_ukwndata {
    rdll_edict edict;
    rdll_response response;
    vector3d v3d;
    vector2d v2d;
};

class runtimedll {
public:
    runtimedll(char* file);
    ~runtimedll();
    bool ok;
    /** \brief Calls a function in the rdll with an edict and expects a rdll_response pointer.
     *
     * \param name char* The name of the function.
     * \param edict rdll_edict* The edict.
     * \return rdll_response*  The response.
     *
     */
    rdll_response* call_e(char* name, rdll_edict* edict);
    /** \brief Calls a function in the rdll by name and expects a rdll_response pointer.
     *
     * \param name char* The name of the function.
     * \return rdll_response* The response.
     *
     */
    rdll_response* call_r(char* name);
    void call(char* name); // should use call_e or call_r, but for simple testing this will do
    PROC getproc(char* name);
private:
    HINSTANCE hInstLib;
};

#endif // RDLL_H
