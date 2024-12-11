#include "MsgGetUnits.h"
#include "LOLEntity.h"

MsgGetUnits::MsgGetUnits(std::initializer_list<std::string> TargetID, TeamSearchParam unitSearchParam, LOLEntity* me, int teamToSearch, bool deleteImmediate) : BaseMessage(TargetID, GET_UNITS, deleteImmediate)
{
	this->teamSearch = unitSearchParam;
	this->senderEntity = me;
	this->teamToSearch = teamToSearch;
}

MsgGetUnits::MsgGetUnits(std::vector<IMessageReciever*> specificRecievers, TeamSearchParam unitSearchParam, LOLEntity* me, int teamToSearch, bool deleteImmediate) : BaseMessage(specificRecievers, GET_UNITS, deleteImmediate)
{
	this->teamSearch = unitSearchParam; 
	this->senderEntity = me; 
	this->teamToSearch = teamToSearch;

	//std::cout << this->specificRecievers.size() << std::endl;
}
