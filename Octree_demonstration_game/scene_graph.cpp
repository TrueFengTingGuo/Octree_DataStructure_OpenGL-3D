#include <stdexcept>
#include <iostream>
#include <fstream>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "scene_graph.h"




namespace game {

SceneGraph::SceneGraph(void){

    SceneNode* lowerBound = new SceneNode(glm::vec3(-1000,1000,1000));
    SceneNode* upperBound = new SceneNode(glm::vec3(1000, -1000, -1000));
    tree = new Octree(lowerBound, upperBound);

    background_color_ = glm::vec3(0.0, 0.0, 0.0);

    
}


SceneGraph::~SceneGraph(){
}


void SceneGraph::SetBackgroundColor(glm::vec3 color){

    background_color_ = color;
}


glm::vec3 SceneGraph::GetBackgroundColor(void) const {

    return background_color_;
}
 

SceneNode *SceneGraph::CreateNode(bool addToGraph, int ObjectType, std::string node_name, Resource *geometry, Resource *material, Resource *texture){
    SceneNode* scn = nullptr;
    // Create scene node with the specified resources
    if (ObjectType == PlayerObject) {
        scn = new Player(node_name, geometry, material, texture);
    }
    else if (ObjectType == SceneNodeObject) {
    
        scn = new SceneNode(node_name, geometry, material, texture);
    }
    else if (ObjectType == BulletObject) {

        scn = new Bullet(node_name, geometry, material, texture);
    }
    else if (ObjectType == WeaponObject) {

        scn = new Gun(node_name, geometry, material, texture);
    }
    // Add node to the scene
    if (scn != nullptr && addToGraph) {
        AddNode(scn);
    }
    
    return scn;
}


void SceneGraph::AddNode(SceneNode *node){

    node_.push_back(node);
    tree->insert(node);
}


SceneNode *SceneGraph::GetNode(std::string node_name) const {

    // Find node with the specified name
    for (int i = 0; i < node_.size(); i++){
        if (node_[i]->GetPosition() == node_[i]->GetPosition()){
            return node_[i];
        }
    }
    return NULL;

}

bool SceneGraph::DeleteNodeUsingPosition(SceneNode* node) {

    // Find node with the specified name
    for (int i = 0; i < node_.size(); i++) {
        if (node_[i]->GetName() == node->GetName()) {
            cout << "DeleteNodeUsingPosition - name " << node_[i]->GetName() << endl;
            tree->deleteSelectedNode(node);
            node_.erase(node_.begin()+i);
            return 1;
        }
    }
    
    return 0;

}
std::vector<SceneNode *>::const_iterator SceneGraph::begin() const {

    return node_.begin();
}


std::vector<SceneNode *>::const_iterator SceneGraph::end() const {

    return node_.end();
}


void SceneGraph::Draw(Camera *camera){

    // Clear background
    glClearColor(background_color_[0], 
                 background_color_[1],
                 background_color_[2], 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw all scene nodes

   
    /*
    for (int i = 0; i < node_.size(); i++){
        node_[i]->Draw(camera);
    }
    */
    if (playerControlledObject==nullptr) {
        for (int i = 0; i < node_.size(); i++) {
            node_[i]->Draw(camera);
        }
    }
    else {
        vector<SceneNode*> controlledUpdateList;
        tree->SearchInRadius(playerControlledObject->GetPosition(), 100.0f, &controlledUpdateList);
        for (int i = 0; i < controlledUpdateList.size(); i++) {
            controlledUpdateList[i]->Draw(camera);
        }
        playerControlledObject->Draw(camera);
    }
    
}


void SceneGraph::Update(float deltatime){
    vector<SceneNode*> controlledUpdateList;



    if (playerControlledObject != nullptr) {
        tree->updateNodeInRange(playerControlledObject->GetPosition(), 100.0f);
        


        //detect collision
        controlledUpdateList.clear();
        tree->SearchInRadius(playerControlledObject->GetPosition(), 100.0f, &controlledUpdateList);
        //cout << controlledUpdateList.size() << endl;


        //player collision interaction
        for (int i = 0; i < controlledUpdateList.size(); i++) {
            //cout << "Name: "<< controlledUpdateList[i]->GetName() << endl;

            if (controlledUpdateList[i]->GetName().find("BlockerInstance") != std::string::npos) {
                RacingPathObject* PathObject = dynamic_cast<RacingPathObject*>(controlledUpdateList[i]);

                //handle collision
                if (PathObject->isCollided(playerControlledObject)) {
                    //cout << "Name: "<< controlledUpdateList[i]->GetName() << endl;
                    collisionReslove(playerControlledObject, PathObject);
                    exit(1);

                }

                //pass through the ring to get speed up
                playerControlledObject->SetAcceleration(playerControlledObject->GetAcceleration() + PathObject->giveAccelration(playerControlledObject));
                //std::cout << "playerControlledObject: " << playerControlledObject->GetAcceleration().x << " , " << playerControlledObject->GetAcceleration().y << " , " << playerControlledObject->GetAcceleration().z << " , " << std::endl;

            }
            else {
                if (controlledUpdateList[i]->GetName().find("Bullet") != std::string::npos) {
                    //ignore the collision
                    //cout << "Name: "<< controlledUpdateList[i]->GetName() << endl;

                }
                else {
                    if (playerControlledObject->isCollided(controlledUpdateList[i])) {


                        collisionReslove(playerControlledObject, controlledUpdateList[i]);
                    }
                }
            }




        }

        //bullet collision interaction
        for (int i = 0; i < controlledUpdateList.size(); i++) {
            if (controlledUpdateList[i]->GetName().find("Bullet") != std::string::npos) {

                //if it's bullet
                Bullet* bullet = dynamic_cast<Bullet*>(controlledUpdateList[i]);
                for (int j = 0; j < controlledUpdateList.size(); j++) {

                    //if find a blocker
                    if (controlledUpdateList[j]->GetName().find("AsteroidInstance") != std::string::npos) {
                        if (bullet->isCollided(controlledUpdateList[j])) {

                            //delete both objects 
                            DeleteNodeUsingPosition(bullet);
                            DeleteNodeUsingPosition(controlledUpdateList[j]);
                            continue;
                            //collisionReslove(bullet, controlledUpdateList[j]);
                        }
                    }
                }
            }

        }


    }


    //update
    if (playerControlledObject == nullptr) {

        for (int i = 0; i < node_.size(); i++) {
            node_[i]->Update(deltatime);
        }
    }
    else {   


        playerControlledObject->Update(deltatime);
        tree->SearchInRadius(playerControlledObject->GetPosition(), 100.0f, &controlledUpdateList);
        //cout << "im at"  << " (" << playerControlledObject->GetPosition().x << "," << playerControlledObject->GetPosition().y << "," << playerControlledObject->GetPosition().z << ")"  <<"Can see  " << controlledUpdateList.size() << endl;
        for (int i = 0; i < controlledUpdateList.size(); i++) {

            //bullet target blocker gameobject
            if (controlledUpdateList[i]->GetName().find("Bullet") != std::string::npos) {
                Bullet* bullet = dynamic_cast<Bullet*>(controlledUpdateList[i]);
                vector<SceneNode*> searchedForBullet;

                //search near objects for bullet
                tree->SearchInRadius(bullet->GetPosition(), 50.0f, &searchedForBullet);
                for (int j = 0; j < searchedForBullet.size(); j++) {

                    //if find a blocker
                    if (searchedForBullet[j]->GetName().find("AsteroidInstance") != std::string::npos) {
                        bullet->targetThisNode(searchedForBullet[j]);
                    }
  

                }
            }

            controlledUpdateList[i]->Update(deltatime);
            
        }



    }
    
    
    
    /*

    */
}

float SceneGraph::collisionReslove(SceneNode* objectA, SceneNode* objectB) {

    glm::vec3 relativeV = objectB->GetVelocity() - objectA->GetVelocity();
    glm::vec3 normal = objectB->GetPosition() - objectA->GetPosition();
    float velN = glm::normalize(glm::dot(relativeV, normal));

    // cout << velN;
    if (velN > 0.0f) {
        return 0;
    }

    float e = glm::min(objectA->GetRestitution(), objectB->GetRestitution());
    cout << e << endl;
    //float e = objectA->getRestitution()+ objectB->getRestitution();
    float j = -(1 + e) * velN;

    float objectA_reverseMass = objectA->GetReverseMass();
    float objectB_reverseMass = objectB->GetReverseMass();

    float sumOfReverseMass = objectA_reverseMass + objectB_reverseMass;
    if (sumOfReverseMass != 0) {
        j /= sumOfReverseMass;
    }

    glm::vec3 impluse = j * normal;

    //cout << impluse.x << impluse.y << impluse.z;
    objectA->SetVelocity(objectA->GetVelocity() - impluse * objectA_reverseMass);
    objectB->SetVelocity(objectB->GetVelocity() + impluse * objectB_reverseMass);

    return j;
}


} // namespace game
