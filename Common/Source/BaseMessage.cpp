#include "BaseMessage.h"
#include "IMessageReciever.h"

BaseMessage::BaseMessage(std::string, int MessageType, bool deleteImmediate)
{
	this->TargetID = TargetID;
	this->MessageType = MessageType;
	this->deleteImmediate = deleteImmediate;
}

BaseMessage::BaseMessage(std::vector<IMessageReciever*> specificRecievers, int MessageType, bool deleteImmediate)
{
	this->specificRecievers = specificRecievers;
	this->MessageType = MessageType;
	this->deleteImmediate = deleteImmediate;
}

std::vector<IMessageReciever*>& BaseMessage::GetRecievers()
{
	return specificRecievers;
}

std::string BaseMessage::GetTargetID(void)
{
	return this->TargetID;
}
