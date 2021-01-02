#ifndef DMM_H_
#define DMM_H_

#include <iostream> 
#include <string>
using namespace std;
class DebugMessageManager{


public:
	DebugMessageManager();
	void print(string message);
	bool ableToPrint = true;
private:
};

#endif