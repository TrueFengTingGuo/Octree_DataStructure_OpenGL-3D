#include "PathCalculationProcessor.h"

PathCalculationProcessor::PathCalculationProcessor()
{
}

//Take serval vec3 and turn them into smooth path
vector<glm::vec3> PathCalculationProcessor::calculateThePath(vector<glm::vec3> inputVector)
{

	vector<glm::vec3> result;
	if (inputVector.size() < 3) {
	
		return inputVector;
	}
	else {
		
		//we need at least 3 vectors to perform path caluclation process
		for (int index = 1; index < inputVector.size() - 1; index+=2) {
			//we ignore the first and last vector since we can only calculate the path using at least 3 vectors.
			//Here im using  quadratic bezier curves
			int smoothValue = 50;
			float eachConnectingPointPosition = 1.0f / smoothValue;

			cout <<"index " << index << endl;
			float t = 0.0f;
			for (int count = 0; count < smoothValue; count++) {
				//cout << "t " << t << endl;
				glm::vec3 part1 = (1.0f - t) * (1.0f - t) * (inputVector[index-1]);
				glm::vec3 part2 = (-2.0f * t * t + 2.0f *t) * (inputVector[index]);
				glm::vec3 part3 = (t * t) * (inputVector[index+1]);	
				glm::vec3 total = part1 + part2 + part3;

				t += eachConnectingPointPosition;
				result.push_back(total);
			}
	
		}
	
	}
	
	result.push_back(inputVector[inputVector.size()-1]);
	return result;

}


