#ifndef OCTREE_H_
#define OCTREE_H_

// Implemetation of Octree in c++ 
#include <iostream> 
#include <vector> 
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "scene_node.h"
#include "DebugMessageManager.h"

using namespace std;

#define TopLeftFront 0 
#define TopRightFront 1 
#define BottomRightFront 2 
#define BottomLeftFront 3 
#define TopLeftBack 4 
#define TopRightBack 5 
#define BottomRightBack 6 
#define BottomLeftBack 7 

namespace game{
    /* // Structure of a point 
    struct Point {
        int x;
        int y;
        int z;
        Point()
            : x(-1), y(-1), z(-1)
        {
        }

        Point(int a, int b, int c)
            : x(a), y(b), z(c)
        {
        }
    };*/
   

    // Octree class 
    class Octree {
        
        SceneNode* point;// if point == NULL, node is internal node. 
        bool emptyNode = true;// if emptyNode == true, node is empty. 

        // Represent the boundary of the cube 
        SceneNode* topLeftFront, * bottomRightBack;
        vector<Octree*> children;
        DebugMessageManager debugMessager;
    public:
        // Constructor 
        Octree()
        {
            // To declare empty Octree 
            point = nullptr;
            emptyNode = true;
        }

        // Constructor with one arguments : adding one scene node
        Octree(SceneNode* oneSceneNode)
        {
            // To declare point node 
            point = oneSceneNode;
            emptyNode = false;
        }

        // Constructor with six arguments 
        Octree(SceneNode* lowerBoundSceneNode, SceneNode* upperBoundSceneNode)
        {
            // This use to construct Octree 
            // with boundaries defined 

            /*
            if (upperBoundSceneNode->GetPosition().x < lowerBoundSceneNode->GetPosition().x
                || upperBoundSceneNode->GetPosition().y < lowerBoundSceneNode->GetPosition().y
                || upperBoundSceneNode->GetPosition().z < lowerBoundSceneNode->GetPosition().z) {
                cout << "bounday poitns are not valid" << endl;
                return;
            }
            */
            
            glm::vec3 direcationOfBoundary = lowerBoundSceneNode->GetPosition() - upperBoundSceneNode->GetPosition();
            if (direcationOfBoundary.x > 0 || direcationOfBoundary.y < 0 || direcationOfBoundary.z < 0) {

                cout << "bounday poitns are not valid" << endl;
                return;

            }


            point = nullptr;
            emptyNode = false;

            topLeftFront = lowerBoundSceneNode;
            bottomRightBack = upperBoundSceneNode;

            // Assigning null to the children 
            children.assign(8, nullptr);
            for (int i = TopLeftFront; i <= BottomLeftBack; ++i) { children[i] = new Octree(); }
                
        }

        
        // Function to insert a point in the octree 
        void insert(SceneNode* newSceneNode){

            //cout << "Start of insert" << endl;
            // If the point already exists in the octree 
            if (find(newSceneNode)) {
                cout << "Point already exist in the tree" << endl;
                return;
            }
            glm::vec3 LowerBoundCheck = topLeftFront->GetPosition() - newSceneNode->GetPosition();
            glm::vec3 UpperBoundCheck = bottomRightBack->GetPosition() - newSceneNode->GetPosition();
            // If the point is out of bounds 


            if ((LowerBoundCheck.x > 0 && LowerBoundCheck.y < 0&& LowerBoundCheck.z <0)|| (UpperBoundCheck.x < 0 && UpperBoundCheck.y > 0 && UpperBoundCheck.z > 0)){
                cout << "Point is out of bound" << endl;
                return;
            }

            // Binary search to insert the point 
            float midx = (topLeftFront->GetPosition().x
                + bottomRightBack->GetPosition().x)
                / 2;
            float midy = (topLeftFront->GetPosition().y
                + bottomRightBack->GetPosition().y)
                / 2;
            float midz = (topLeftFront->GetPosition().z
                + bottomRightBack->GetPosition().z)
                / 2;
            glm::vec3 midPoint = glm::vec3(midx, midy, midz);
            int pos = -1;

            //if the node is equal to the midPoint

            glm::vec3 SceneNodeMidPointDirecation = newSceneNode->GetPosition() - midPoint;

            // Checking the octant of 
            // the point 
            if (SceneNodeMidPointDirecation.x > 0) {
                if (SceneNodeMidPointDirecation.y > 0) {
                    if (SceneNodeMidPointDirecation.z > 0)
                        pos = TopRightFront;
                    else
                        pos = TopRightBack;
                }
                else {

                    //SceneNodeMidPointDirecation.x > 0 and SceneNodeMidPointDirecation.y < 0
                    if (SceneNodeMidPointDirecation.z > 0)
                        pos = BottomRightFront;
                    else
                        pos = BottomRightBack;
                }
            }
            else {
                //SceneNodeMidPointDirecation.x < 0 
                if (SceneNodeMidPointDirecation.y > 0) {
                    if (SceneNodeMidPointDirecation.z > 0)
                        pos = TopLeftFront;
                    else
                        pos = TopLeftBack;
                }
                else {

                    //SceneNodeMidPointDirecation.x < 0 and SceneNodeMidPointDirecation.y < 0
                    if (SceneNodeMidPointDirecation.z > 0)
                        pos = BottomLeftFront;
                    else
                        pos = BottomLeftBack;
                }
            }

            

            // If an internal node is encountered 
            if (children[pos]->point == nullptr && children[pos]->emptyNode == false) {
                //cout << "internal node search" << endl;
                children[pos]->insert(newSceneNode);
                return;
            }
            // If an empty node is encountered 
            else if (children[pos]->emptyNode == true && children[pos]->point == nullptr) {
                //cout << "Empty space so I add node into octree" << endl; 
                //cout << "topLeftFront (" << topLeftFront->GetPosition().x << "," << topLeftFront->GetPosition().y << "," << topLeftFront->GetPosition().z << ")" << endl;
                //cout << "new (" << newSceneNode->GetPosition().x << "," << newSceneNode->GetPosition().y << "," << newSceneNode->GetPosition().z << ")" << endl;
                //cout << "bottomRightBack (" << bottomRightBack->GetPosition().x << "," << bottomRightBack->GetPosition().y << "," << bottomRightBack->GetPosition().z << ")" << endl;
                
                
                if (newSceneNode->GetPosition() == topLeftFront->GetPosition()) {
                    glm::vec3 offsetDirecation = midPoint - topLeftFront->GetPosition();
                    float offsetAmount = 0.01 * glm::distance(newSceneNode->GetPosition(), topLeftFront->GetPosition());

                    newSceneNode->SetPosition(newSceneNode->GetPosition() - offsetAmount * offsetDirecation);
                }
                else if (newSceneNode->GetPosition() == bottomRightBack->GetPosition()) {
                    glm::vec3 offsetDirecation = midPoint - bottomRightBack->GetPosition();
                    float offsetAmount = 0.01 * glm::distance(newSceneNode->GetPosition(), topLeftFront->GetPosition());

                    newSceneNode->SetPosition(newSceneNode->GetPosition() - offsetAmount * offsetDirecation);
                }

                children[pos] = new Octree(newSceneNode);
                //cout << "prove that it have been added (" << children[pos]->point->GetPosition().x << "," << children[pos]->point->GetPosition().y << "," << children[pos]->point->GetPosition().z << ")" << endl;
                //cout << "end this adding" << endl;
                return;
            }

            //if (children[pos]->point != nullptr && children[pos]->emptyNode == false) means if already has a scene node
            else {
                //cout << endl << "Here i see two node in the same cue" << endl;
                
                SceneNode* oldSceneNode = children[pos]->point;

                delete children[pos];
                children[pos] = nullptr;

                //create new tree under the children[pos]
                if (pos == TopLeftFront) {
                    SceneNode* newBottomRightBack = new SceneNode(glm::vec3(midx, midy, midz));
                    children[pos] = new Octree(topLeftFront, newBottomRightBack);

                    //cout  << "TopLeftFront" << endl;
                }

                else if (pos == TopRightFront) {
                    SceneNode* newTopLeftFront = new SceneNode(glm::vec3(midx,
                        topLeftFront->GetPosition().y,
                        topLeftFront->GetPosition().z));
                    SceneNode* newBottomRightBack = new SceneNode(glm::vec3(bottomRightBack->GetPosition().x,
                        midy,
                        midz));
                    
                    children[pos] = new Octree(newTopLeftFront,newBottomRightBack);
                    //cout << "TopRightFront" << endl;
                }
                else if (pos == BottomRightFront) {

                    SceneNode* newTopLeftFront = new SceneNode(glm::vec3(midx,
                        midy,
                        topLeftFront->GetPosition().z));
                    SceneNode* newBottomRightBack = new SceneNode(glm::vec3(bottomRightBack->GetPosition().x,
                        bottomRightBack->GetPosition().y,
                        midz));

                    children[pos] = new Octree(newTopLeftFront, newBottomRightBack);


                    //cout << "BottomRightFront" << endl;
                }
                else if (pos == BottomLeftFront) {

                    SceneNode* newTopLeftFront = new SceneNode(glm::vec3(topLeftFront->GetPosition().x,
                        midy,
                        topLeftFront->GetPosition().z));
                    SceneNode* newBottomRightBack = new SceneNode(glm::vec3(midx,
                        bottomRightBack->GetPosition().y,
                        midz));

                    children[pos] = new Octree(newTopLeftFront, newBottomRightBack);
                    //cout << "BottomLeftFront" << endl;
                }
                else if (pos == TopLeftBack) {

                    SceneNode* newTopLeftFront = new SceneNode(glm::vec3(topLeftFront->GetPosition().x,
                        topLeftFront->GetPosition().y,
                        midz));
                    SceneNode* newBottomRightBack = new SceneNode(glm::vec3(midx,
                        midy,
                        bottomRightBack->GetPosition().z));

                    children[pos] = new Octree(newTopLeftFront, newBottomRightBack);
                    //cout << "TopLeftBack" << endl;
                }
                else if (pos == TopRightBack) {

                    SceneNode* newTopLeftFront = new SceneNode(glm::vec3(midx,
                        topLeftFront->GetPosition().y,
                        midz));
                    SceneNode* newBottomRightBack = new SceneNode(glm::vec3(bottomRightBack->GetPosition().x,
                        midy,
                        bottomRightBack->GetPosition().z));

                    children[pos] = new Octree(newTopLeftFront, newBottomRightBack);

                    //cout << "TopRightBack" << endl;

                }
                else if (pos == BottomRightBack) {

                    SceneNode* newTopLeftFront = new SceneNode(glm::vec3(midx,
                        midy,
                        midz));
                    SceneNode* newBottomRightBack = new SceneNode(glm::vec3(bottomRightBack->GetPosition().x,
                        bottomRightBack->GetPosition().y,
                        bottomRightBack->GetPosition().z));

                    children[pos] = new Octree(newTopLeftFront, newBottomRightBack);

                    //cout << "BottomRightBack" << endl;
                }
                else if (pos == BottomLeftBack) {

                    SceneNode* newTopLeftFront = new SceneNode(glm::vec3(topLeftFront->GetPosition().x,
                        midy,
                        midz));
                    SceneNode* newBottomRightBack = new SceneNode(glm::vec3(midx,
                        bottomRightBack->GetPosition().y,
                        bottomRightBack->GetPosition().z));

                    children[pos] = new Octree(newTopLeftFront, newBottomRightBack);
                    //cout << "BottomLeftBack" << endl;
                }
                
                //cout << "old (" << oldSceneNode->GetPosition().x << "," << oldSceneNode->GetPosition().y << "," << oldSceneNode->GetPosition().z << ")" << endl;
                //cout << "switch position" << endl;
                children[pos]->insert(oldSceneNode);
                oldSceneNode->SetPosition(oldSceneNode->GetPosition() + glm::vec3(0.1f));
                //cout << "new (" << newSceneNode->GetPosition().x << "," << newSceneNode->GetPosition().y << "," << newSceneNode->GetPosition().z << ")" << endl;
                children[pos]->insert(newSceneNode);
            }

        }

        // Function that returns true if the point 
        // (x, y, z) exists in the octree 
        bool find(SceneNode* searchedNode)
        {
            //cout << endl << "find a node" << endl;
            //cout << "this node is at (" << searchedNode->GetPosition().x << "," << searchedNode->GetPosition().y << "," << searchedNode->GetPosition().z << ")" << endl;
            glm::vec3 LowerBoundCheck = topLeftFront->GetPosition() - searchedNode->GetPosition();
            glm::vec3 UpperBoundCheck = bottomRightBack->GetPosition() - searchedNode->GetPosition();
            // If the point is out of bounds 

            if ((LowerBoundCheck.x > 0 && LowerBoundCheck.y < 0 && LowerBoundCheck.z < 0) || (UpperBoundCheck.x < 0 && UpperBoundCheck.y > 0 && UpperBoundCheck.z > 0)) {
                //cout << "end of find" << endl;
                return 0;
            }

            // Binary search to insert the point 
            float midx = (topLeftFront->GetPosition().x
                + bottomRightBack->GetPosition().x)
                / 2;
            float midy = (topLeftFront->GetPosition().y
                + bottomRightBack->GetPosition().y)
                / 2;
            float midz = (topLeftFront->GetPosition().z
                + bottomRightBack->GetPosition().z)
                / 2;
            glm::vec3 midPoint = glm::vec3(midx, midy, midz);
            int pos = -1;

            glm::vec3 SceneNodeMidPointDirecation = searchedNode->GetPosition() - midPoint;
            // Checking the octant of 
            // the point 
            if (SceneNodeMidPointDirecation.x > 0) {
                if (SceneNodeMidPointDirecation.y > 0) {
                    if (SceneNodeMidPointDirecation.z > 0)
                        pos = TopRightFront;
                    else
                        pos = TopRightBack;
                }
                else {

                    //SceneNodeMidPointDirecation.x > 0 and SceneNodeMidPointDirecation.y < 0
                    if (SceneNodeMidPointDirecation.z > 0)
                        pos = BottomRightFront;
                    else
                        pos = BottomRightBack;
                }
            }
            else {
                //SceneNodeMidPointDirecation.x < 0 
                if (SceneNodeMidPointDirecation.y > 0) {
                    if (SceneNodeMidPointDirecation.z > 0)
                        pos = TopLeftFront;
                    else
                        pos = TopLeftBack;
                }
                else {

                    //SceneNodeMidPointDirecation.x < 0 and SceneNodeMidPointDirecation.y < 0
                    if (SceneNodeMidPointDirecation.z > 0)
                        pos = BottomLeftFront;
                    else
                        pos = BottomLeftBack;
                }
            }
            
            //cout << " this node is at " << pos << endl;
            // If an internal node is encountered 
            if (children[pos]->point == nullptr && children[pos]->emptyNode == false) {
                //cout << "internal node is encountered" << endl;
                return children[pos]->find(searchedNode);
            }
            else if (children[pos]->emptyNode == true && children[pos]->point == nullptr) { // If an empty node is encountered 
                //cout << "empty is encountered" << endl;
                return 0;
            }
            else {
                //cout << "searchedNode (" << searchedNode->GetPosition().x << "," << searchedNode->GetPosition().y << "," << searchedNode->GetPosition().z << ")" << endl;
                //cout << "pos (" << children[pos]->point->GetPosition().x << "," << children[pos]->point->GetPosition().y << "," << children[pos]->point->GetPosition().z << ")" << endl;
                if (searchedNode->GetPosition().x == children[pos]->point->GetPosition().x
                    && searchedNode->GetPosition().y == children[pos]->point->GetPosition().y
                    && searchedNode->GetPosition().z == children[pos]->point->GetPosition().z) {
                    return 1;
                }
            }

            //cout << "found nothing" << endl;
            return 0;
        }

        void SearchInRadius(glm::vec3 pivot, float radius, std::vector<SceneNode*>* result) {

            //run the searching process
            return SearchInRadiusProcess(pivot,radius, result);
        }

        // collect all nodes that are in range
        // (x, y, z) exists in the octree 
        void SearchInRadiusProcess(glm::vec3 pivot,float radius, std::vector<SceneNode*> *result)
        {

            glm::vec3 LowerBoundCheck = topLeftFront->GetPosition() - pivot;
            glm::vec3 UpperBoundCheck = bottomRightBack->GetPosition() - pivot;
            // If the point is out of bounds 

            //outside the boundry
            if ((LowerBoundCheck.x > 0 && LowerBoundCheck.y < 0 && LowerBoundCheck.z < 0) || (UpperBoundCheck.x < 0 && UpperBoundCheck.y > 0 && UpperBoundCheck.z > 0)) {
                //cout << "end of find" << endl;
                return;
            }


            // Otherwise perform binary search 
            

            for (int pos = TopLeftFront; pos <= BottomLeftBack; pos++)
            {
                //cout << pos << " being searched" << endl;
               
                if (children[pos]->point == nullptr && children[pos]->emptyNode == false) {
                    
                    //find the mid point of a child
                    float midx = (children[pos]->topLeftFront->GetPosition().x
                        + children[pos]->bottomRightBack->GetPosition().x)
                        / 2;
                    float midy = (children[pos]->topLeftFront->GetPosition().y
                        + children[pos]->bottomRightBack->GetPosition().y)
                        / 2;
                    float midz = (children[pos]->topLeftFront->GetPosition().z
                        + children[pos]->bottomRightBack->GetPosition().z)
                        / 2;


                    glm::vec3 midPoint = glm::vec3(midx, midy, midz);
                    //here is the process which is trying to check if this child is overlapped with the searching range
                    float distanceFromMidpointToPivot = glm::distance(midPoint, pivot);
                    float distanceFromMidpointToItsTopLeftFront = glm::distance(midPoint, children[pos]->topLeftFront->GetPosition());

                    //cout << pivot.x << pivot.y << pivot.z << endl;
                    //cout << "distanceFromMidpointToPivot: " << distanceFromMidpointToPivot << "  distanceFromMidpointToItsTopLeftFront: " << distanceFromMidpointToItsTopLeftFront << endl;
                    //check to see if two radius are overlapped
                    if (distanceFromMidpointToPivot < radius + distanceFromMidpointToItsTopLeftFront) {
                        //cout << " inside of the radius" << endl;
                        //recursive to find node
                        children[pos]->SearchInRadiusProcess(pivot, radius, result);
                        continue;
                    }
                    else {
                    
                        //cout << "not inside of the radius" << endl;
                    }
                }

                // If an empty node is encountered 
                else if (children[pos]->emptyNode == true && children[pos]->point == nullptr) {
                    //cout << pos << ": search in radius, empty node and nullptr" << endl;
                    continue;
                }
                else {

                    //cout << " inside of the radius" << endl;
                    float distance = glm::distance(pivot, children[pos]->point->GetPosition());

                    //cout <<  children[pos]->point->GetPosition().x <<  children[pos]->point->GetPosition().y <<  children[pos]->point->GetPosition().z << endl;


                    //cout << "distance:" << distance <<endl;
                    if (distance < radius) {

                        //always update the player outside this octree for secure 
                        if (children[pos]->point->GetName() != "PlayerSpaceship") {
                            result->push_back(children[pos]->point);
                        }
                        
                    }
                        
                }

            }
            
            return;
        }

        bool updateNodeInRange(glm::vec3 pivot, float radius) {
            glm::vec3 LowerBoundCheck = topLeftFront->GetPosition() - pivot;
            glm::vec3 UpperBoundCheck = bottomRightBack->GetPosition() - pivot;
            // If the point is out of bounds 

            //outside the boundry
            if ((LowerBoundCheck.x > 0 && LowerBoundCheck.y < 0 && LowerBoundCheck.z < 0) || (UpperBoundCheck.x < 0 && UpperBoundCheck.y > 0 && UpperBoundCheck.z > 0)) {
                //cout << "end of find" << endl;
                return 0;
            }

            // Otherwise perform binary search 

             // Otherwise perform binary search 


            for (int pos = TopLeftFront; pos <= BottomLeftBack; pos++)
            {
                //cout << pos << " being searched" << endl;

                if (children[pos]->point == nullptr && children[pos]->emptyNode == false) {

                    //find the mid point of a child
                    float midx = (children[pos]->topLeftFront->GetPosition().x
                        + children[pos]->bottomRightBack->GetPosition().x)
                        / 2;
                    float midy = (children[pos]->topLeftFront->GetPosition().y
                        + children[pos]->bottomRightBack->GetPosition().y)
                        / 2;
                    float midz = (children[pos]->topLeftFront->GetPosition().z
                        + children[pos]->bottomRightBack->GetPosition().z)
                        / 2;


                    glm::vec3 midPoint = glm::vec3(midx, midy, midz);
                    //here is the process which is trying to check if this child is overlapped with the searching range
                    float distanceFromMidpointToPivot = glm::distance(midPoint, pivot);
                    float distanceFromMidpointToItsTopLeftFront = glm::distance(midPoint, children[pos]->topLeftFront->GetPosition());

                    //cout << pivot.x << pivot.y << pivot.z << endl;

                    //check to see if two radius are overlapped
                    if (distanceFromMidpointToPivot < radius + distanceFromMidpointToItsTopLeftFront) {
                        //cout << " inside of the radius" << endl;
                        //recursive to find node
                        children[pos]->updateNodeInRange(pivot, radius);
                        continue;
                    }
                    else {

                        //cout << "not inside of the radius" << endl;
                    }
                }

                // If an empty node is encountered 
                else if (children[pos]->emptyNode == true && children[pos]->point == nullptr) {
                    //cout << pos << ": search in radius, empty node and nullptr" << endl;
                    continue;
                }
                else {

                    //cout << " inside of the radius" << endl;
                    float distance = glm::distance(pivot, children[pos]->point->GetPosition());

                    //cout <<  children[pos]->point->GetPosition().x <<  children[pos]->point->GetPosition().y <<  children[pos]->point->GetPosition().z << endl;


                    //cout << "distance:" << distance <<endl;
                    if (distance < radius) {
                        
                        //always update the player outside this octree for secure 
                        if (children[pos]->point->GetName() != "PlayerSpaceship") {
                            SceneNode* oldSceneNode = children[pos]->point;
                            delete children[pos];
                            children[pos] = new Octree();
                            insert(oldSceneNode);
                        }
                        
                    }

                }

            }

            return 1;
        
        }

        //delete selected node
        bool deleteSelectedNode(SceneNode* searchedNode)
        {

            //cout << "this node is at (" << searchedNode->GetPosition().x << "," << searchedNode->GetPosition().y << "," << searchedNode->GetPosition().z << ")" << endl;
            glm::vec3 LowerBoundCheck = topLeftFront->GetPosition() - searchedNode->GetPosition();
            glm::vec3 UpperBoundCheck = bottomRightBack->GetPosition() - searchedNode->GetPosition();
            // If the point is out of bounds 

            if ((LowerBoundCheck.x > 0 && LowerBoundCheck.y < 0 && LowerBoundCheck.z < 0) || (UpperBoundCheck.x < 0 && UpperBoundCheck.y > 0 && UpperBoundCheck.z > 0)) {
                cout << "this node is at (" << searchedNode->GetPosition().x << "," << searchedNode->GetPosition().y << "," << searchedNode->GetPosition().z << ")" << endl;
                cout << "end of find" << endl;
                return 0;
            }

            // Binary search to insert the point 
            float midx = (topLeftFront->GetPosition().x
                + bottomRightBack->GetPosition().x)
                / 2;
            float midy = (topLeftFront->GetPosition().y
                + bottomRightBack->GetPosition().y)
                / 2;
            float midz = (topLeftFront->GetPosition().z
                + bottomRightBack->GetPosition().z)
                / 2;
            glm::vec3 midPoint = glm::vec3(midx, midy, midz);
            int pos = -1;

            glm::vec3 SceneNodeMidPointDirecation = searchedNode->GetPosition() - midPoint;
            // Checking the octant of 
            // the point 
            if (SceneNodeMidPointDirecation.x > 0) {
                if (SceneNodeMidPointDirecation.y > 0) {
                    if (SceneNodeMidPointDirecation.z > 0)
                        pos = TopRightFront;
                    else
                        pos = TopRightBack;
                }
                else {

                    //SceneNodeMidPointDirecation.x > 0 and SceneNodeMidPointDirecation.y < 0
                    if (SceneNodeMidPointDirecation.z > 0)
                        pos = BottomRightFront;
                    else
                        pos = BottomRightBack;
                }
            }
            else {
                //SceneNodeMidPointDirecation.x < 0 
                if (SceneNodeMidPointDirecation.y > 0) {
                    if (SceneNodeMidPointDirecation.z > 0)
                        pos = TopLeftFront;
                    else
                        pos = TopLeftBack;
                }
                else {

                    //SceneNodeMidPointDirecation.x < 0 and SceneNodeMidPointDirecation.y < 0
                    if (SceneNodeMidPointDirecation.z > 0)
                        pos = BottomLeftFront;
                    else
                        pos = BottomLeftBack;
                }
            }

            //cout << " this node is at " << pos << endl;
            // If an internal node is encountered 
            if (children[pos]->point == nullptr && children[pos]->emptyNode == false) {
                //cout << "internal node is encountered" << endl;
                return children[pos]->deleteSelectedNode(searchedNode);
            }
            else if (children[pos]->emptyNode == true && children[pos]->point == nullptr) { // If an empty node is encountered 
                //cout << "empty is encountered" << endl;
                return 0;
            }
            else {
                //cout << "searchedNode (" << searchedNode->GetPosition().x << "," << searchedNode->GetPosition().y << "," << searchedNode->GetPosition().z << ")" << endl;
                //cout << "pos (" << children[pos]->point->GetPosition().x << "," << children[pos]->point->GetPosition().y << "," << children[pos]->point->GetPosition().z << ")" << endl;
                cout << "name:" << children[pos]->point->GetName() << endl;
                if (children[pos]->point->GetName() == searchedNode->GetName()) {
                    cout << "deleted" << endl;
                    delete children[pos];
                    children[pos] = new Octree();
                    return 1;
                }
            }

            cout << "found nothing" << endl;
            return 0;
        }


    private:
        std::vector<SceneNode*> nodesInSearchedRange;

    };

}
#endif // OCTREE_H_