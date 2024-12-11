#pragma once
#include "BaseMessage.h"
#include <glm/glm.hpp>

class MsgHelp : public BaseMessage
{
public:
	glm::vec2 helpPos;
	int team = -1;

	MsgHelp(std::initializer_list<std::string> TargetID, glm::vec2 helpPos, int team, bool deleteImmediate = true);
	MsgHelp(std::vector<IMessageReciever*> specificRecievers, glm::vec2 helpPos, int team, bool deleteImmediate = true);
};

