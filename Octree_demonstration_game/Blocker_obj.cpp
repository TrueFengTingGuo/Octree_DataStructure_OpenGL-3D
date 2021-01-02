using namespace std;

#include <string>
#include <iostream>
#include "Blocker_obj.h"

namespace game {

    Blocker::Blocker(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture) : SceneNode(name, geometry, material, texture) {
        SetScale(glm::vec3(2.0f));
    
    }


    Blocker::~Blocker() {
    }


    void Blocker::Update(float deltatime) {
        SceneNode::Update(deltatime);
    }

} // namespace game
