#ifndef PLAYER_H_
#define PLAYER_H_

#include "scene_node.h"

using namespace game;

class Player : public SceneNode
{
public:
	Player(glm::vec3 position);
	Player(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture = NULL);
	void setthisTexture(GLuint w, char* fname);
	void setallTexture(void);
	// Destructor
	~Player();



	void Update(float deltatime) override;

	void Draw(Camera* camera) override;

	//getter
	float GetBalanceAngle(void);
	//setter
	void SetBalanceAngle(float);
	void AddScore(int);

	//camera controll
	glm::vec3 cameraFollowingPosition();
	glm::quat cameraFollowingRotation();

	//fire
	bool fire();

private:

	GLuint tex[35];
	float balanceAngle = 0.0f;

	//fire 
	float fireCooldown = 0.0f;
	float initFireCooldown = 10.0f;

	//hp
	float init = 50.0f;
	float hp = 50.0f;

	//shield
	int shield = 0;

	//score
	int score = 0;
};



#endif // PLAYER_H_