#pragma once
#include "BaseMessage.h"
#include "MessageTypes.h"

class LOLEntity;

enum TeamSearchParam {
	CURR_TEAM,
	OPPOSING_TEAMS,
	ALL,
	REPLY,
	NUM_SEARCH_PARAMS
};

class MsgGetUnits : public BaseMessage
{
public:
	LOLEntity* senderEntity;
	TeamSearchParam teamSearch = NUM_SEARCH_PARAMS;
	int teamToSearch = -1;

	MsgGetUnits(std::initializer_list<std::string> TargetID, TeamSearchParam unitSearchParam, LOLEntity* me, int teamToSearch = -1, bool deleteImmediate = true);

	MsgGetUnits(std::vector<IMessageReciever*> specificRecievers, TeamSearchParam unitSearchParam, LOLEntity* me, int teamToSearch = -1, bool deleteImmediate = true);
};

