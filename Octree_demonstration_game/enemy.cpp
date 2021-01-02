#include "enemy.h"

#include <string>
#include <iostream>
using namespace std;


Enemy::Enemy(void) : GameObj() {}
Enemy::Enemy(GLuint theshader) : GameObj(theshader) {


}

void Enemy::Update(float deltatime) {

	//delete this object when this time runs down
	existedTime -= 1.0f;
	if (existedTime < 0) {
		canBeDestroied = true;
	}
	//handle rotation of the obj
	GameObj::Update(float deltatime);
}