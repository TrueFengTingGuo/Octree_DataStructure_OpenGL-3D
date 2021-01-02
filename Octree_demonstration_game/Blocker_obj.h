#ifndef BLOCKER_H_
#define BLOCKER_H_


#include "scene_node.h"

namespace game {

    // Abstraction of an asteroid
    class Blocker : public SceneNode {

    public:
        // Create asteroid from given resources
        Blocker(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture);

        // Destructor
        ~Blocker();

        // Update geometry configuration
        void Update(float deltatime) override;

    private:

    }; // class Blocker

} // namespace game

#endif // BLOCKER_H_
