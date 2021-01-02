#ifndef PCP_H_
#define PCP_H_

#include <iostream> 
#include <vector> 
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

using std::vector;
using namespace std;

class PathCalculationProcessor{

	public:

		PathCalculationProcessor(void);

		vector<glm::vec3> calculateThePath(vector<glm::vec3> inputVector);
		

	private:
		

};

#endif