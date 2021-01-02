using namespace std;

#include <string>
#include <iostream>
#include "gun.h"

namespace game {

    Gun::Gun(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture) : SceneNode(name, geometry, material, texture) {
        SetScale(glm::vec3(0.2f));
    }

    Gun::~Gun() {

    }


    void Gun::Update(float deltatime) {
        if (currentCD > 0) {
            currentCD -= deltatime;
            
        }
        else {
            canBeFire = true;
        }
        //cout << currentCD << endl;
        SceneNode::Update(deltatime);
    }

    bool Gun::Fire()
    {
        
        if (canBeFire) {
            cout << "yes" << endl;
            canBeFire = false;
            currentCD = totalCD;
            return true;
        }
 
        return canBeFire;
    }

} // namespace game
