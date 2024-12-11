#include "MsgHelp.h"
#include "MessageTypes.h"

MsgHelp::MsgHelp(std::initializer_list<std::string> TargetID, glm::vec2 helpPos, int team, bool deleteImmediate) : BaseMessage(TargetID, GET_HELP, deleteImmediate)
{
	this->helpPos = helpPos;
	this->team = team;
}

MsgHelp::MsgHelp(std::vector<IMessageReciever*> specificRecievers, glm::vec2 helpPos, int team, bool deleteImmediate) : BaseMessage(specificRecievers, GET_HELP, deleteImmediate)
{
	this->helpPos = helpPos;
	this->team = team;
}
