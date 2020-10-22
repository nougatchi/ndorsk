#ifndef LIGHTMAN_H
#define LIGHTMAN_H
#include <array>
#include <irrlicht.h>

using namespace irr;

// ty irr doc
class lightman : public scene::ILightManager
{
    typedef enum
    {
        NO_MANAGEMENT,
        LIGHTS_NEAREST_NODE,
        LIGHTS_IN_ZONE
    }
    LightManagementMode;

    LightManagementMode Mode;
    LightManagementMode RequestedMode;

    public:
        lightman(ISceneManager*);
        virtual ~lightman();

        virtual void OnPreRender(core::array<scene::ISceneNode*> & lightList)
        {

            // Store the light list. I am free to alter this list until the end of OnPostRender().
            SceneLightList = &lightList;
        }

        // Called after the last scene node is rendered.
        virtual void OnPostRender()
        {
            // Since light management might be switched off in the event handler, we'll turn all
            // lights on to ensure that they are in a consistent state. You wouldn't normally have
            // to do this when using a light manager, since you'd continue to do light management
            // yourself.
            for (u32 i = 0; i < SceneLightList->size(); i++)
                (*SceneLightList)[i]->setVisible(true);
        }

        virtual void OnRenderPassPreRender(scene::E_SCENE_NODE_RENDER_PASS renderPass)
        {
            // I don't have to do anything here except remember which render pass I am in.
            CurrentRenderPass = renderPass;
        }

        virtual void OnRenderPassPostRender(scene::E_SCENE_NODE_RENDER_PASS renderPass)
        {
            // I only want solid nodes to be lit, so after the solid pass, turn all lights off.
            if (scene::ESNRP_SOLID == renderPass)
            {
                for (u32 i = 0; i < SceneLightList->size(); ++i)
                    (*SceneLightList)[i]->setVisible(false);
            }
        }

        // This is called before the specified scene node is rendered
        virtual void OnNodePreRender(scene::ISceneNode* node)
        {
            CurrentSceneNode = node;

            // This light manager only considers solid objects, but you are free to manipulate
            // lights during any phase, depending on your requirements.
            if (scene::ESNRP_SOLID != CurrentRenderPass)
                return;

            // And in fact for this example, I only want to consider lighting for cube scene
            // nodes.  You will probably want to deal with lighting for (at least) mesh /
            // animated mesh scene nodes as well.
            if (node->getType() != scene::ESNT_CUBE)
                return;

            if (LIGHTS_NEAREST_NODE == Mode)
            {
                // This is a naive implementation that prioritises every light in the scene
                // by its proximity to the node being rendered.  This produces some flickering
                // when lights orbit closer to a cube than its 'zone' lights.
                const core::vector3df nodePosition = node->getAbsolutePosition();

                // Sort the light list by prioritising them based on their distance from the node
                // that's about to be rendered.
                core::array<LightDistanceElement> sortingArray;
                sortingArray.reallocate(SceneLightList->size());

                u32 i;
                for(i = 0; i < SceneLightList->size(); ++i)
                {
                    scene::ISceneNode* lightNode = (*SceneLightList)[i];
                    const f64 distance = lightNode->getAbsolutePosition().getDistanceFromSQ(nodePosition);
                    sortingArray.push_back(LightDistanceElement(lightNode, distance));
                }

                sortingArray.sort();

                // The list is now sorted by proximity to the node.
                // Turn on the three nearest lights, and turn the others off.
                for(i = 0; i < sortingArray.size(); ++i)
                    sortingArray[i].node->setVisible(i < 3);
            }
            else if(LIGHTS_IN_ZONE == Mode)
            {
                // Empty scene nodes are used to represent 'zones'.  For each solid mesh that
                // is being rendered, turn off all lights, then find its 'zone' parent, and turn
                // on all lights that are found under that node in the scene graph.
                // This is a general purpose algorithm that doesn't use any special
                // knowledge of how this particular scene graph is organised.
                for (u32 i = 0; i < SceneLightList->size(); ++i)
                {
                    if ((*SceneLightList)[i]->getType() != scene::ESNT_LIGHT)
                        continue;
                    scene::ILightSceneNode* lightNode = static_cast<scene::ILightSceneNode*>((*SceneLightList)[i]);
                    video::SLight & lightData = lightNode->getLightData();

                    if (video::ELT_DIRECTIONAL != lightData.Type)
                        lightNode->setVisible(false);
                }

                scene::ISceneNode * parentZone = findZone(node);
                if (parentZone)
                    turnOnZoneLights(parentZone);
            }
        }

        // Called after the specified scene node is rendered
        virtual void OnNodePostRender(scene::ISceneNode* node)
        {
            // I don't need to do any light management after individual node rendering.
        }

    protected:
        scene::ISceneManager * SceneManager;
        core::array<scene::ISceneNode*> * SceneLightList;
        scene::E_SCENE_NODE_RENDER_PASS CurrentRenderPass;
        scene::ISceneNode * CurrentSceneNode;
        // Find the empty scene node that is the parent of the specified node
        scene::ISceneNode * findZone(scene::ISceneNode * node)
        {
            if (!node)
                return 0;

            if (node->getType() == scene::ESNT_EMPTY)
                return node;

            return findZone(node->getParent());
        }

        // Turn on all lights that are children (directly or indirectly) of the
        // specified scene node.
        void turnOnZoneLights(scene::ISceneNode * node)
        {
            core::list<scene::ISceneNode*> const & children = node->getChildren();
            for (core::list<scene::ISceneNode*>::ConstIterator child = children.begin();
                child != children.end(); ++child)
            {
                if ((*child)->getType() == scene::ESNT_LIGHT)
                    (*child)->setVisible(true);
                else // Assume that lights don't have any children that are also lights
                    turnOnZoneLights(*child);
            }
        }


        // A utility class to aid in sorting scene nodes into a distance order
        class LightDistanceElement
        {
        public:
            LightDistanceElement() {};

            LightDistanceElement(scene::ISceneNode* n, f64 d)
                : node(n), distance(d) { }

            scene::ISceneNode* node;
            f64 distance;

            // Lower distance elements are sorted to the start of the array
            bool operator < (const LightDistanceElement& other) const
            {
                return (distance < other.distance);
            }
        };

    private:
};

#endif // LIGHTMAN_H
