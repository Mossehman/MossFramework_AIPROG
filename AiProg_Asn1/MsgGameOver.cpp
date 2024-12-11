#include "MsgGameOver.h"
#include "MessageTypes.h"

MsgGameOver::MsgGameOver(std::initializer_list<std::string> TargetID, int losingTeam, bool deleteImmediate) : BaseMessage(TargetID, GAME_OVER, deleteImmediate)
{
	this->losingTeam = losingTeam;
}

MsgGameOver::MsgGameOver(std::vector<IMessageReciever*> specificRecievers, int losingTeam, bool deleteImmediate) : BaseMessage(specificRecievers, GAME_OVER, deleteImmediate)
{
	this->losingTeam = losingTeam;
}
