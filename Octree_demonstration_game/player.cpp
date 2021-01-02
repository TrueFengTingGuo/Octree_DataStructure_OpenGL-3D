#include "player.h"
#include "camera.h"
#include <string>
#include <iostream>
#include <SOIL\SOIL.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
using namespace std;


Player::Player(glm::vec3 position) : SceneNode(position) {}
Player::Player(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture) : SceneNode(name, geometry, material, texture) {

	SetScale(glm::vec3(0.1f));
}



void Player::setthisTexture(GLuint w, char* fname) {
	glBindTexture(GL_TEXTURE_2D, w);

	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	// Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Player::setallTexture(void) {

	setthisTexture(tex[0], "number_png/0-Number-PNG.png");
	setthisTexture(tex[1], "number_png/1-Number-PNG.png");
	setthisTexture(tex[2], "number_png/2-Number-PNG.png");
	setthisTexture(tex[3], "number_png/3-Number-PNG.png");
	setthisTexture(tex[4], "number_png/4-Number-PNG.png");
	setthisTexture(tex[5], "number_png/5-Number-PNG.png");
	setthisTexture(tex[6], "number_png/6-Number-PNG.png");
	setthisTexture(tex[7], "number_png/7-Number-PNG.png");
	setthisTexture(tex[8], "number_png/8-Number-PNG.png");
	setthisTexture(tex[9], "number_png/9-Number-PNG.png");
	glBindTexture(GL_TEXTURE_2D, tex[0]);
}



void Player::Update(float deltatime) {
	//addAcceleration(glm::vec3(0.0f, 0.0f, -0.01f));
	if (glm::abs(balanceAngle) > 0.0001f) {
		if (balanceAngle > 0) {
			balanceAngle -= 0.91 * deltatime;
		}
		else {
			balanceAngle += 0.91 * deltatime;
		}
		
	}
	else {
		balanceAngle = 0.0f;
	}

	if (fireCooldown > 0) {
		fireCooldown -= 1.0f;
	}
	else {
		fireCooldown = 0.0f;
	}
	
	SceneNode::Update(deltatime);
	
}


void Player::Draw(Camera* camera) {

	int tempCurrency = 20000000;
	vector<int> intVec;

	while (tempCurrency > 0) {
		int digit = tempCurrency % 10;
		tempCurrency = tempCurrency / 10;
		intVec.push_back(digit);
	}

	int intVecSize = intVec.size();
	for (int count = 0; count < intVecSize; count++) {

		// Bind the entities texture
		glBindTexture(GL_TEXTURE_2D, tex[intVec.back()]);

		intVec.pop_back();//take out one digit

		// Setup the transformation matrix for the shader
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), GetPosition() + glm::vec3(count * 0.5f, 0.0f, 0.0f));
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(4.5f, 4.5f, 4.5f));


		// Set the transformation matrix in the shader
		glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		//transformationMatrix = rotationMatrix * translationMatrix  * scaleMatrix;
		//shader.setUniformMat4("transformationMatrix", transformationMatrix);

		GLint world_mat = glGetUniformLocation(GetMaterial(), "world_mat");
		glUniformMatrix4fv(world_mat, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

		// Draw the entity
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	}

	SceneNode::Draw(camera);
}
float Player::GetBalanceAngle(void)
{
	return balanceAngle;
}

void Player::SetBalanceAngle(float number)
{
	balanceAngle = number;
}

void Player::AddScore(int newAddScore)
{
	score += newAddScore;
}

glm::vec3 Player::cameraFollowingPosition() {

	return GetPosition();

}

glm::quat Player::cameraFollowingRotation() {

	return GetOrientation();

}

bool Player::fire()
{
	if (fireCooldown == 0) {

		fireCooldown = initFireCooldown;
		return true;
	}
	return false;
}
