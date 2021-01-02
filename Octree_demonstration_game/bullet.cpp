using namespace std;

#include <string>
#include <iostream>
#include "bullet.h"
int bulletNumber = 0;
namespace game {

    Bullet::Bullet(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture) : SceneNode((name + to_string(bulletNumber)), geometry, material, texture) {
        bulletNumber += 1;

        //cout << "bullet number " << bulletNumber << endl;
        SetScale(glm::vec3(0.9f));
    }

    Bullet::~Bullet() {

    }


    void Bullet::Update(float deltatime) {

        SceneNode::Update(deltatime);
    }

    void Bullet::Draw(Camera* camera) {

  
        SceneNode::Draw(camera);
    }

    void Bullet::targetThisNode(SceneNode* target) {
    
        glm::vec3 direction = target->GetPosition() - GetPosition();
        SetAcceleration(glm::normalize(direction)*glm::vec3(100.5f));
    }
} // namespace game
