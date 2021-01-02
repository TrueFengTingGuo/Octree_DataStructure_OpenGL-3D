#ifndef ENEMY_H_
#define ENEMY_H_

#include "gameobj.h"

using namespace game;

class Enemy : public GameObj
{
public:
	Enemy(void);
	Enemy(GLuint theshader);

	void Update(float deltatime) override;

	//getter

	//setter




private:

	float existedTime = 1000.0f;
};



#endif // PLAYER_H_