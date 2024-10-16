#include "InputManager.h"
#include "KeyboardController.h"

void InputManager::addToStratagemBuffer(int bufferValue)
{
	stratagemInputBuffer.push_back(bufferValue);
}

void InputManager::clearStratagemBuffer()
{
	stratagemInputBuffer.clear();
}
