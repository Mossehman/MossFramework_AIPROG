#pragma once
#include "GLFW/glfw3.h"
#include <vector>
#include "DesignPatterns/SingletonTemplate.h"
class InputManager: public SingletonTemplate<InputManager>
{
	friend SingletonTemplate<InputManager>;

public:
	void addToStratagemBuffer(int bufferValue);
	void clearStratagemBuffer();
	

	char leftKey = GLFW_KEY_A; 
	char rightKey = GLFW_KEY_D; 
	char jumpKey = GLFW_KEY_SPACE;
	char crouchKey = GLFW_KEY_C; 
	char sprintKey = GLFW_KEY_LEFT_SHIFT;
	char diveKey = GLFW_KEY_LEFT_ALT;
	char proneKey = GLFW_KEY_Z;

	char grenadeKey = GLFW_KEY_G; 
	char punchKey = GLFW_KEY_F; 
	char reloadKey = GLFW_KEY_R;

	char weaponMainKey = GLFW_KEY_1;
	char weaponSecondaryKey = GLFW_KEY_2;
	char weaponSupportKey = GLFW_KEY_3;

	char stratagemInputKey = GLFW_KEY_LEFT_CONTROL;

	char stratagemUpKey = GLFW_KEY_W; 
	char stratagemRightKey = GLFW_KEY_D; 
	char stratagemDownKey = GLFW_KEY_S; 
	char stratagemLeftKey = GLFW_KEY_A;

	char interactionKey = GLFW_KEY_E;


protected:
	std::vector<int> stratagemInputBuffer; 

};

