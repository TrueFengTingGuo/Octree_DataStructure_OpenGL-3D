#ifndef SCENE_GRAPH_H_
#define SCENE_GRAPH_H_

#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene_node.h"
#include "resource.h"
#include "camera.h"
#include "octree_resouce.h"
#include "player.h"
#include "racing_pathway.h"
#include "bullet.h"
#include "gun.h"

const int PlayerObject = 1;
const int SceneNodeObject = 2;
const int BlockerObject = 3;
const int BulletObject = 4;
const int WeaponObject = 5;

namespace game {

    // Class that manages all the objects in a scene
    class SceneGraph {

        private:
            // Background color
            glm::vec3 background_color_;

            // Scene nodes to render
            std::vector<SceneNode *> node_;
            Octree *tree = nullptr;

        public:
            // Constructor and destructor
            SceneGraph(void);
            ~SceneGraph();

            // Background color
            void SetBackgroundColor(glm::vec3 color);
            glm::vec3 GetBackgroundColor(void) const;
            
            // Create a scene node from the specified resources
            SceneNode *CreateNode(bool addToGraph, int ObjectType,std::string node_name, Resource *geometry, Resource *material, Resource *texture = NULL);
            // Add an already-created node
            void AddNode(SceneNode *node);
            // Find a scene node with a specific name
            SceneNode *GetNode(std::string node_name) const;

            bool DeleteNodeUsingPosition(SceneNode* node);

            // Get node const iterator
            std::vector<SceneNode *>::const_iterator begin() const;
            std::vector<SceneNode *>::const_iterator end() const;
            SceneNode* playerControlledObject = nullptr;

            // Draw the entire scene
            void Draw(Camera *camera);

            // Update entire scene
            void Update(float deltaTime);

            float collisionReslove(SceneNode* objectA, SceneNode* objectB);

    }; // class SceneGraph

} // namespace game

#endif // SCENE_GRAPH_H_
