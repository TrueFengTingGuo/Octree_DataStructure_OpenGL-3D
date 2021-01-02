#ifndef SCENE_NODE_H_
#define SCENE_NODE_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "camera.h"
#include <vector>

namespace game {

    // Class that manages one object in a scene 
    class SceneNode {

        public:

            SceneNode(glm::vec3 position);

            // Create scene node from given resources
            SceneNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL);

            // Destructor
            ~SceneNode();
            
            // Get name of node
            const std::string GetName(void) const;

            // Get node attributes
            glm::vec3 GetPosition(void) const;
            glm::quat GetOrientation(void) const;
            glm::vec3 GetScale(void) const;
            glm::vec3 GetAcceleration(void)const;
            glm::vec3 GetVelocity(void)const;
            glm::vec3 GetOrientation_acceleration_(void)const;
            glm::mat4 GetViewMatrix(void)const;
            glm::vec3 GetDirecationAngularResult(void) const;
            SceneNode* GetParent(void)const;
            float GetRestitution(void)const;
            float GetReverseMass(void)const;
            float GetObjectRadius(void)const;
            std::vector<SceneNode*> GetChildren(void)const;

            bool GetBlending(void) const;

            // Set node attributes
            void SetPosition(glm::vec3 position);
            void SetOrientation(glm::quat orientation);
            void SetScale(glm::vec3 scale);
            void SetAcceleration(glm::vec3 newAcceleration);
            void SetVelocity(glm::vec3 newVelocity);
            void SetOrientation_acceleration_(glm::vec3 newAcceleration);
            void SetMass(float newMass);
            void SetRestitution(float newRestitution_);
            void addAChild(SceneNode* newChild);
            void SetParent(SceneNode* newParent);
            void SetBlending(bool blending);

            // Perform transformations on node
            void Translate(glm::vec3 trans);
            void Rotate(glm::quat rot);
            void Scale(glm::vec3 scale);

            // Draw the node according to scene parameters in 'camera'
            // variable
            virtual void Draw(Camera *camera);

            // Update the node
            virtual void Update(float deltatime);

            //check collision
            virtual bool isCollided(SceneNode* comparedObject);

            // OpenGL variables
            GLenum GetMode(void) const;
            GLuint GetArrayBuffer(void) const;
            GLuint GetElementArrayBuffer(void) const;
            GLsizei GetSize(void) const;
            GLuint GetMaterial(void) const;

        private:
            std::string name_; // Name of the scene node
            GLuint array_buffer_; // References to geometry: vertex and array buffers
            GLuint element_array_buffer_;
            GLenum mode_; // Type of geometry
            GLsizei size_; // Number of primitives in geometry
            GLuint material_; // Reference to shader program
            GLuint texture_; // Reference to texture resource
            bool blending_; // Draw with blending or not

            //movement of object
            glm::vec3 position_; // Position of node
            glm::vec3 velocity_;
            glm::vec3 acceleration_;    

            //rotation
            glm::quat orientation_;
            glm::vec3 direcationAngularAcceleration;
            glm::vec3 direcationAngularVelocity;
            glm::vec3 direcationAngularResult;
            glm::vec4 animation_vec = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

            //view transformation
            glm::vec3 scale_; // Scale of node
            glm::mat4 savedViewMatrix;

            //collision detection
            float restitution_ = 0.01f;
            float mass_ = 15.0f;
            float reverseMass = 1/ mass_;
            float baiscDetectionRadius = 2.0f; 

            std::vector<SceneNode*> childrenNode;
            SceneNode* parentNode = nullptr;


            // Set matrices that transform the node in a shader program
            void SetupShader(GLuint program);

    }; // class SceneNode

} // namespace game

#endif // SCENE_NODE_H_
