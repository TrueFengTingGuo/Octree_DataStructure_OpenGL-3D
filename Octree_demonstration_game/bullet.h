#ifndef BULLET_H_
#define BULLET_H_
#include "scene_node.h"

//global bullet number counter
extern int bulletNumber;


namespace game {

    // Abstraction of an asteroid
    class Bullet : public SceneNode {

    public:
        // Create asteroid from given resources
        Bullet(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture);

        // Destructor
        ~Bullet();

        // Update geometry configuration
        void Update(float deltatime) override;

        void Draw(Camera* camera)override;

        void targetThisNode(SceneNode* target);

    private:


    }; // class Bullet

} // namespace game

#endif //BULLET_H_