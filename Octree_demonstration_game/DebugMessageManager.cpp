#include "DebugMessageManager.h"


DebugMessageManager::DebugMessageManager()
{
}

void DebugMessageManager::print(string message)
{
	if (ableToPrint) {
	
		cout << message << endl;
	}
}
