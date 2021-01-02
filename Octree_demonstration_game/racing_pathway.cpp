#include "racing_pathway.h"
#include <iostream>


namespace game {
    RacingPathObject::RacingPathObject(glm::vec3 position) : SceneNode(position) {}

    RacingPathObject::RacingPathObject(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture = NULL) : SceneNode(name, geometry, material, texture) {
        SetMass(1000.0f);
        SetRestitution(-0.6f);
    }


    RacingPathObject::~RacingPathObject() {
    }


    void RacingPathObject::SetAccelrationCircle(bool newAccelrationCircle)
    {

        accelrationCircle = newAccelrationCircle;
    }

    void RacingPathObject::Update(float deltatime) {
        SceneNode::Update(deltatime);
        
    }

    bool RacingPathObject::isCollided(SceneNode* comparedObject)
    {
        
        glm::vec3 forward = GetOrientation() * glm::vec3(0.0f, 0.0f, 1.0f) * glm::inverse(GetOrientation());
        float pointToPlaneDistance = (glm::dot(forward, GetPosition() - comparedObject->GetPosition())) / glm::length(forward);
        //std::cout << "pointToPlaneDistance: " << pointToPlaneDistance << std::endl;
        if (glm::abs(pointToPlaneDistance) < 10.0f) {
            float distance = glm::abs(glm::length(comparedObject->GetPosition() - GetPosition()));
            float circileRadius = glm::pow((distance * distance - pointToPlaneDistance * pointToPlaneDistance), 1.0f / 2.0f);

            //std::cout << "circileRadius: "<< circileRadius << std::endl;
            if (circileRadius > 9.0f && circileRadius < 10.5f) {
            
                return true;

            }
            else if (circileRadius < 9.0f && comparedObject->GetName().find("PlayerSpaceship") != std::string::npos) {
                Player* PathObject = dynamic_cast<Player*>(comparedObject);
                PathObject->AddScore(1);
            }
        }

        return false;
    }

    glm::vec3 RacingPathObject::giveAccelration(SceneNode* targetObject)
    {
        glm::vec3 forward = GetOrientation() * glm::vec3(0.0f, 0.0f, 1.0f) * glm::inverse(GetOrientation());
        glm::vec3 amp = glm::vec3(0.0f);
        glm::vec3 givenAccelration = glm::vec3(0.0f);

        float pointToPlaneDistance = (glm::dot(forward, GetPosition() - targetObject->GetPosition())) / glm::length(forward);
        //std::cout << "pointToPlaneDistance: " << pointToPlaneDistance << std::endl;
        if (glm::abs(pointToPlaneDistance) < 1.0f) {
            float distance = glm::abs(glm::length(targetObject->GetPosition() - GetPosition()));
            float circileRadius = glm::pow((distance * distance - pointToPlaneDistance * pointToPlaneDistance), 1.0f / 2.0f);

           
            if (circileRadius < 9.0f ) {

                if (accelrationCircle) {
                    
                    amp = glm::vec3(100.0f);
                    givenAccelration = GetOrientation() * glm::vec3(0.0f, 0.0f, -1.0f) * glm::inverse(GetOrientation());

                    //std::cout << "givenAccelration: " << givenAccelration.x << " , " << givenAccelration.y << " , " << givenAccelration.z << " , " << std::endl;
                    return givenAccelration * amp;
                }

            }
        }


        return givenAccelration;
        
    }


} // namespace game
