#include <stdio.h>
#include <stdlib.h>
#include <irrlicht.h>
#include <mem.h>
#include <string>
#include "rdll.h"
#include "derecv.h"
#include "audio.h"
#include "cvars.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef NDORSK_DBG
#define DBG_DMP_CVARS_MAIN() \
    cv.dbg_printcvars();
#else
#define DBG_DMP_CVARS_MAIN() \
    // do nothing
#endif

int main(int argc, char** args)
{
    cvars cv;
    video::E_DRIVER_TYPE drv = video::EDT_OPENGL;

    bool args_gammode = false;
    bool args_setval = false;
    bool fullscreen = false;
    bool vsync = false;
    bool stencil = true;
    unsigned int flags = 0b000000;

    unsigned int resolution_x = 640;
    unsigned int resolution_y = 480;

    std::string cgam = "dorsk";
    for(int i = 0; i<argc; i++) {
        // prepare for hell
        if(args_gammode == true) {
            cgam = std::string(args[i]);
            args_gammode = false;
            continue;
        } else {
                    if(strcmp(args[i],"-r_d3d9") == 0) {
                printf("EDT_DIRECT3D9\n");
                drv = video::EDT_DIRECT3D9;
            } else if(strcmp(args[i],"-r_d3d8") == 0) {
                printf("EDT_DIRECT3D8\n");
                drv = video::EDT_DIRECT3D8;
            } else if(strcmp(args[i],"-r_soft") == 0) {
                printf("EDT_SOFTWARE\n");
                drv = video::EDT_SOFTWARE;
            } else if(strcmp(args[i],"-r_burn") == 0) {
                printf("EDT_BURNINGSVIDEO\n");
                drv = video::EDT_BURNINGSVIDEO;
            } else if(strcmp(args[i],"-r_null") == 0) {
                printf("EDT_NULL\n");
                drv = video::EDT_NULL;
            } else if(strcmp(args[i],"-d_game") == 0) {
                args_gammode = true;
            } else if(strcmp(args[i],"-h_sync") == 0) {
                printf("VSYNC\n");
                vsync = true;
            } else if(strcmp(args[i],"-h_full") == 0) {
                printf("FULLSCREEN\n");
                fullscreen = true;
            } else if(strcmp(args[i],"-h_stcl") == 0) {
                printf("STENCIL disabled\n");
                stencil = false;
            } else if(strcmp(args[i],"-h_resl") == 0) {
                args_setval = true;
            }
        }
    }

    cv = cvars();
    cv.init();
    cv.add_cvar("p_cvars",CVAR_GLOBAL,(void*)&cv,"pointer");

    derecv evh;
    cv.add_cvar("p_event",CVAR_GLOBAL,(void*)&evh,"pointer");
    daudio snd = daudio();
    cv.add_cvar("p_sound",CVAR_GLOBAL,(void*)&snd,"pointer");

    IrrlichtDevice *device = createDevice( drv, dimension2d<u32>(resolution_x, resolution_y), 16, fullscreen, stencil, vsync, 0);
    cv.add_cvar("p_device",CVAR_GLOBAL,(void*)device,"pointer");

    cv.add_cvar("r_renderer",CVAR_CLIENT,(void*)&drv,"int");
    cv.add_cvar("r_fullscreen",CVAR_CLIENT,(void*)&fullscreen,"bool");
    cv.add_cvar("r_vsync",CVAR_CLIENT,(void*)&vsync,"bool");
    cv.add_cvar("r_stencil",CVAR_CLIENT,(void*)&stencil,"bool");
    cv.add_cvar("r_resolution_x",CVAR_CLIENT,(void*)&resolution_x,"int");
    cv.add_cvar("r_resolution_y",CVAR_CLIENT,(void*)&resolution_y,"int");
    cv.add_cvar("d_game",CVAR_CLIENT,(void*)&cgam,"string");

    if(device == 0) {
        printf("could not create window");
        DBG_DMP_CVARS_MAIN();
        return 1;
    }

    device->setWindowCaption(L"ndorsk");
    printf("ndorsk\n");
    printf("the n stands for new!\n");

    printf("cgam: %s\n",cgam.c_str());

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    cv.add_cvar("p_driver",CVAR_GLOBAL,(void*)driver,"pointer");
    cv.add_cvar("p_scene",CVAR_GLOBAL,(void*)smgr,"pointer");
    cv.add_cvar("p_gui",CVAR_GLOBAL,(void*)guienv,"pointer");


    // begin to load game data
    IFileSystem* fs = device->getFileSystem();
    cv.add_cvar("p_fs",CVAR_GLOBAL,(void*)fs,"pointer");
    fs->addFileArchive("shared");
    if(strcmp(cgam.c_str(),"dorsk") != 0) {
        fs->addFileArchive("dorsk");
    }
    fs->addFileArchive(cgam.c_str());
    runtimedll rdll = runtimedll("s_dorsk.dll");
    rdll.call("GetRDLL");
    cv.add_cvar("d_rdll",CVAR_GLOBAL,&rdll.ok,"bool");
    cv.add_cvar("p_rdll",CVAR_GLOBAL,&rdll,"pointer");

#ifdef NDORSK_DBG
    smgr->addCubeSceneNode();


    //part npart = part();
#endif // NDORSK_DBG
    int fps = 0;
    cv.add_cvar("d_fps",CVAR_GLOBAL,(void*)&fps,"int");
    cv.dbg_printcvars();
    while(device->run() && driver) {
        if(device->isWindowActive()) {
            driver->beginScene(true, true, SColor(255,100,101,140));

            smgr->drawAll();

            int dfps = driver->getFPS();
            if(fps != dfps) {
                fps = dfps;
            }

            guienv->drawAll();

            driver->endScene();
        } else {
            // if device is not active (showing as primary window), yield to eat up less CPU cycles
            device->yield();
        }
    }
    device->drop(); // die

    return 0;
}
