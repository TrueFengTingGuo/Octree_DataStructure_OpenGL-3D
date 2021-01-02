#ifndef GUN_H_
#define GUN_H_
#include "scene_node.h"



namespace game {

    // Abstraction of an asteroid
    class Gun : public SceneNode {

    public:
        // Create asteroid from given resources
        Gun(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture);

        // Destructor
        ~Gun();

        // Update geometry configuration
        void Update(float deltatime) override;
        bool Fire();
    private:
        bool canBeFire;
        float currentCD = 0.0f;
        float totalCD = 2.0f;

    }; // class Gun

} // namespace game

#endif //GUN_H_