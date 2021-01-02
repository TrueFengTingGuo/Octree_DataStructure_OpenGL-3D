#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>

#include "scene_node.h"
using namespace std;

namespace game {

    SceneNode::SceneNode(glm::vec3 position) {
        SetPosition(position);

        if (mass_ == 0) {
            reverseMass = 0;
        }
        else {
            reverseMass = 1 / mass_;
        }
    }
    SceneNode::SceneNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture){

        // Set name of scene node
        name_ = name;

        // Set geometry
        if (geometry->GetType() == PointSet){
            mode_ = GL_POINTS;
        } else if (geometry->GetType() == Mesh){
            mode_ = GL_TRIANGLES;
        } else {
            throw(std::invalid_argument(std::string("Invalid type of geometry")));
        }

        array_buffer_ = geometry->GetArrayBuffer();
        element_array_buffer_ = geometry->GetElementArrayBuffer();
        size_ = geometry->GetSize();

        // Set material (shader program)
        if (material->GetType() != Material){
            throw(std::invalid_argument(std::string("Invalid type of material")));
        }

        material_ = material->GetResource();

        // Set texture
        if (texture){
            texture_ = texture->GetResource();
        } else {
            texture_ = 0;
        }

        // Other attributes
        scale_ = glm::vec3(1.0, 1.0, 1.0);

        //setting mass
        if (mass_ == 0) {
            reverseMass = 0;
        }
        else {
            reverseMass = 1 / mass_;
        }

        //for particle
        blending_ = false;
    }


    SceneNode::~SceneNode(){
    }


    const std::string SceneNode::GetName(void) const {

        return name_;
    }


    glm::vec3 SceneNode::GetPosition(void) const {

        return position_;
    }


    glm::quat SceneNode::GetOrientation(void) const {

        return orientation_;
    }


    glm::vec3 SceneNode::GetScale(void) const {

        return scale_;
    }

    glm::vec3 SceneNode::GetAcceleration(void)const {
        return acceleration_;
    }

    glm::vec3 SceneNode::GetVelocity(void) const
    {
        return velocity_;
    }

    glm::vec3 SceneNode::GetOrientation_acceleration_(void) const
    {
        return direcationAngularAcceleration;
    }

    glm::mat4 SceneNode::GetViewMatrix(void) const
    {
        return savedViewMatrix;
    }

    glm::vec3 SceneNode::GetDirecationAngularResult(void) const
    {
        return direcationAngularResult;
    }

    SceneNode* SceneNode::GetParent(void) const
    {
        return parentNode;
    }

    float SceneNode::GetRestitution(void) const
    {
        return restitution_;
    }

    float SceneNode::GetReverseMass(void)const { return reverseMass; }

    float SceneNode::GetObjectRadius(void) const
    {
        return baiscDetectionRadius;
    }

    std::vector<SceneNode*> SceneNode::GetChildren(void) const
    {
        return childrenNode;
    }

    bool SceneNode::GetBlending(void) const {

        return blending_;
    }

    void SceneNode::SetPosition(glm::vec3 position){

        position_ = position;
    }


    void SceneNode::SetOrientation(glm::quat orientation){

        orientation_ = orientation;
    }


    void SceneNode::SetScale(glm::vec3 scale){

        scale_ = scale;
    }

    void SceneNode::SetAcceleration(glm::vec3 newAcceleration) {
        acceleration_ = newAcceleration;
    }

    void SceneNode::SetVelocity(glm::vec3 newVelocity)
    {
        velocity_ = newVelocity;
    }

    void SceneNode::SetOrientation_acceleration_(glm::vec3 newAcceleration)
    {
        direcationAngularAcceleration = newAcceleration;
    }

    void SceneNode::SetMass(float newMass)
    {
        mass_ = newMass;
        reverseMass = 1 / mass_;
    }

    void SceneNode::SetRestitution(float newRestitution_)
    {
        restitution_ = newRestitution_;
    }

    //will add a child to node, and set parent of this child
    void SceneNode::addAChild(SceneNode* newChild)
    {
        childrenNode.push_back(newChild);
        newChild->SetParent(this);
    }

    //will add parent to a node, but will
    void SceneNode::SetParent(SceneNode* newParent)
    {
        parentNode = newParent;
    }

    void SceneNode::SetBlending(bool blending) {

        blending_ = blending;
    }


    void SceneNode::Translate(glm::vec3 trans){

        position_ += trans;
    }


    void SceneNode::Rotate(glm::quat rot){

        orientation_ *= rot;
        orientation_ = glm::normalize(orientation_);
    }


    void SceneNode::Scale(glm::vec3 scale){

        scale_ *= scale;
    }


    GLenum SceneNode::GetMode(void) const {

        return mode_;
    }


    GLuint SceneNode::GetArrayBuffer(void) const {

        return array_buffer_;
    }


    GLuint SceneNode::GetElementArrayBuffer(void) const {

        return element_array_buffer_;
    }


    GLsizei SceneNode::GetSize(void) const {

        return size_;
    }


    GLuint SceneNode::GetMaterial(void) const {

        return material_;
    }


    void SceneNode::Draw(Camera *camera){

        // Select particle blending or not
        if (blending_) {
            // Disable z-buffer
            //glDisable(GL_DEPTH_TEST);

            // Enable blending
            glEnable(GL_BLEND);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Simpler form
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);
        }
        else {
            // Enable z-buffer
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glDisable(GL_BLEND);
        }

        // Select proper material (shader program)
        glUseProgram(material_);

        // Set geometry to draw
        glBindBuffer(GL_ARRAY_BUFFER, array_buffer_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_);

        // Set globals for camera
        camera->SetupShader(material_);

        // Set world matrix and other shader input variables
        SetupShader(material_);

        // Draw geometry
        if (mode_ == GL_POINTS){
            glDrawArrays(mode_, 0, size_);
        } else {
            glDrawElements(mode_, size_, GL_UNSIGNED_INT, 0);
        }

        for (int count = 0; count < childrenNode.size(); count++) {
            childrenNode[count]->Draw(camera);
        }
    }


    void SceneNode::Update(float deltatime){
        // placeholder tumbling motion:
        orientation_ = glm::normalize(orientation_);

        //handle moving of the obj
        velocity_ += acceleration_ * deltatime;
        //std::cout << velocity_.x << "," << velocity_.y << ", " << velocity_ .z<< std::endl;
        glm::vec3 finalVelocity_ = orientation_ * velocity_ * glm::inverse(orientation_);
        float accelerationConvertion = finalVelocity_.x * finalVelocity_.x + finalVelocity_.y * finalVelocity_.y + finalVelocity_.z * finalVelocity_.z;
  
      
        if (accelerationConvertion > 0.001f) {
            //std::cout << accelerationConvertion << std::endl;
            position_ += finalVelocity_ * deltatime;
        }
        else {
            finalVelocity_ = glm::vec3(0.0f);
        }

        //handle rotation of the obj
        direcationAngularVelocity += direcationAngularAcceleration * deltatime;
        
        //give rotation a limit
        glm::vec3 speedLimit;
        for (int count = 0; count < 2; count++) {
            if (direcationAngularVelocity[count] > 0) {
                float singleDirecation = glm::abs(direcationAngularVelocity[count]);
                speedLimit[count] = 0.5f * glm::pow(singleDirecation, 1.0f / 2.0f);
            
            }
            else if (direcationAngularVelocity[count] < 0) {
            
                float singleDirecation = glm::abs(direcationAngularVelocity[count]);
                speedLimit[count] = -0.5f * glm::pow(singleDirecation, 1.0f / 2.0f);
            }
            else {
                speedLimit[count] = 0.0f;
            }
        }
     
        direcationAngularResult = speedLimit * deltatime;
        //std::cout << direcationAngularResult.x << direcationAngularResult.y << direcationAngularResult.z << std::endl;
     
        glm::quat ori_x = glm::angleAxis(direcationAngularResult.x, glm::normalize(glm::vec3(1, 0, 0)));
        glm::quat ori_y = glm::angleAxis(direcationAngularResult.y, glm::normalize(glm::vec3(0, 1, 0)));
        glm::quat ori_z = glm::angleAxis(direcationAngularResult.z, glm::normalize(glm::vec3(0, 0, 1)));
        orientation_ *= ori_x;
        orientation_ *= ori_y;
        orientation_ *= ori_z;

        //orientation_ *= orientation_acceleration_ * deltatime;
        orientation_ = glm::normalize(orientation_);

        //decrease angular velocity to recover from infinite velocity gain
        direcationAngularVelocity -= direcationAngularVelocity * 2.9f * deltatime;
        //std::cout << direcationAngularVelocity.y << std::endl;
        velocity_ -= velocity_ * 0.8f * deltatime;

        for (int count = 0; count < childrenNode.size(); count++) {
            childrenNode[count]->Update(deltatime);
        }

    }



    bool SceneNode::isCollided(SceneNode* comparedObject)
    {

        float distance = glm::abs(glm::length(comparedObject->GetPosition() - GetPosition()));
        if (distance < comparedObject->GetObjectRadius() + GetObjectRadius()) {
            //std::cout << distance << std::endl;
            return true;
        }
        return false;

    }

    void SceneNode::SetupShader(GLuint program){

        // Set attributes for shaders
        GLint vertex_att = glGetAttribLocation(program, "vertex");
        glVertexAttribPointer(vertex_att, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), 0);
        glEnableVertexAttribArray(vertex_att);

        GLint normal_att = glGetAttribLocation(program, "normal");
        glVertexAttribPointer(normal_att, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void *) (3*sizeof(GLfloat)));
        glEnableVertexAttribArray(normal_att);

        GLint color_att = glGetAttribLocation(program, "color");
        glVertexAttribPointer(color_att, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void *) (6*sizeof(GLfloat)));
        glEnableVertexAttribArray(color_att);

        GLint tex_att = glGetAttribLocation(program, "uv");
        glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void *) (9*sizeof(GLfloat)));
        glEnableVertexAttribArray(tex_att);

        // World transformation
        glm::mat4 scaling = glm::scale(glm::mat4(1.0), scale_);
        glm::mat4 rotation = glm::mat4_cast(orientation_);
        glm::mat4 translation = glm::translate(glm::mat4(1.0), position_);
        glm::mat4 transf;
        

        if (parentNode != nullptr) {

            glm::mat4 parentTansfMatrix = GetParent()->GetViewMatrix();
            glm::mat4 offset = glm::translate(glm::mat4(), GetPosition());
            glm::mat4 offsetUndo = glm::translate(glm::mat4(), -1.0f * GetPosition());

            glm::mat4 viewMatrixWithNoScaling = parentTansfMatrix * offsetUndo * offset * rotation * offsetUndo ;
            transf = viewMatrixWithNoScaling * scaling;
            savedViewMatrix = viewMatrixWithNoScaling * glm::scale(glm::mat4(1.0), glm::vec3(1.0f));
        }
        else {
            transf = translation * rotation * scaling;
            savedViewMatrix = translation * rotation;
        }

        GLint world_mat = glGetUniformLocation(program, "world_mat");
        glUniformMatrix4fv(world_mat, 1, GL_FALSE, glm::value_ptr(transf));

        // Normal matrix
        glm::mat4 normal_matrix = glm::transpose(glm::inverse(transf));
        GLint normal_mat = glGetUniformLocation(program, "normal_mat");
        glUniformMatrix4fv(normal_mat, 1, GL_FALSE, glm::value_ptr(normal_matrix));

        // Texture
        if (texture_){
            GLint tex = glGetUniformLocation(program, "texture_map");
            glUniform1i(tex, 0); // Assign the first texture to the map
            glActiveTexture(GL_TEXTURE0); 
            glBindTexture(GL_TEXTURE_2D, texture_); // First texture we bind
            // Define texture interpolation
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        // Timer
        GLint timer_var = glGetUniformLocation(program, "timer");
        double current_time = glfwGetTime();
        glUniform1f(timer_var, (float) current_time);
    }

} // namespace game;
