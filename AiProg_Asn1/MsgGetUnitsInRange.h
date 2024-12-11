#pragma once
#include "MsgGetUnits.h"
#include <glm/glm.hpp>

class MsgGetUnitsInRange : public MsgGetUnits
{
public:
	float viewRadius = 0.0f;
	glm::vec2 radiusOrigin = glm::vec2();

	MsgGetUnitsInRange(std::initializer_list<std::string> TargetID, TeamSearchParam unitSearchParam, LOLEntity* me, float radius, bool deleteImmediate = true);
	MsgGetUnitsInRange(std::vector<IMessageReciever*> specificRecievers, TeamSearchParam unitSearchParam, LOLEntity* me, float radius, bool deleteImmediate = true);

	MsgGetUnitsInRange(std::initializer_list<std::string> TargetID, TeamSearchParam unitSearchParam, LOLEntity* me, float radius, glm::vec2 radiusOrigin, bool deleteImmediate = true);
	MsgGetUnitsInRange(std::vector<IMessageReciever*> specificRecievers, TeamSearchParam unitSearchParam, LOLEntity* me, float radius, glm::vec2 radiusOrigin, bool deleteImmediate = true);
};

