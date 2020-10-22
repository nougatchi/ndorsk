#include <stdio.h>
#include "lightman.h"

using namespace irr;

lightman::lightman(ISceneManager* sceneManager) : Mode(NO_MANAGEMENT), RequestedMode(NO_MANAGEMENT),
        SceneManager(sceneManager), SceneLightList(0),
        CurrentRenderPass(scene::ESNRP_NONE), CurrentSceneNode(0)
{

}

lightman::~lightman()
{

}
