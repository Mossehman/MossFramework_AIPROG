#include "BaseMessage.h"
#include "IMessageReciever.h"

int BaseMessage::GetMessageType(void)
{
	return MessageType;
}
 
BaseMessage::BaseMessage(std::initializer_list<std::string> TargetID, int MessageType, bool deleteImmediate) : TargetIDs(TargetID)
{
	this->MessageType = MessageType;
	this->deleteImmediate = deleteImmediate;
}

BaseMessage::BaseMessage(std::vector<IMessageReciever*> specificRecievers, int MessageType, bool deleteImmediate) : specificRecievers(specificRecievers)
{
	this->MessageType = MessageType;
	this->deleteImmediate = deleteImmediate;
}

std::vector<IMessageReciever*> BaseMessage::GetRecievers()
{
	return this->specificRecievers;
}

std::vector<std::string> BaseMessage::GetTargetIDs(void)
{
	return this->TargetIDs;
}
