#ifndef PATHWAY_H_
#define PATHWAY_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "scene_node.h"
#include "player.h"
namespace game {

    // Abstraction of an asteroid
    class RacingPathObject : public SceneNode {

    public:
        RacingPathObject(glm::vec3 position);
        // Create asteroid from given resources
        RacingPathObject(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture);

        // Destructor
        ~RacingPathObject();

        //setter
        void SetAccelrationCircle(bool);
        // Update geometry configuration
        void Update(float deltaTime) override ;
        bool isCollided(SceneNode* comparedObject)override;
        glm::vec3 giveAccelration(SceneNode* targetObject);

    private:
        bool accelrationCircle = false;
    }; // class RacingPathObject

} // namespace game

#endif // PATHWAY_H_
