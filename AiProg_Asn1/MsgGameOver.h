#pragma once

#include "BaseMessage.h"
class MsgGameOver : public BaseMessage
{
public:
	int losingTeam = -1;

	MsgGameOver(std::initializer_list<std::string> TargetID, int losingTeam, bool deleteImmediate = true);

	MsgGameOver(std::vector<IMessageReciever*> specificRecievers, int losingTeam, bool deleteImmediate = true);
};

