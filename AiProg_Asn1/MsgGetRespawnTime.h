#pragma once
#include "BaseMessage.h"

class MsgGetRespawnTime : public BaseMessage
{
public:
	float respawnTime = 0.0f;

	MsgGetRespawnTime(std::initializer_list<std::string> TargetID, float respawnTime, bool deleteImmediate = true);

	MsgGetRespawnTime(std::vector<IMessageReciever*> specificRecievers, float respawnTime, bool deleteImmediate = true);
};

