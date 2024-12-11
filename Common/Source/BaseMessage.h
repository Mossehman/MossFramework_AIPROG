#pragma once
#include <string>
#include <vector>

class IMessageReciever;

class BaseMessage
{
protected:
	std::vector<std::string> TargetIDs;
	int MessageType = 0;
	std::vector<IMessageReciever*> specificRecievers;
	bool deleteImmediate;



public:
	BaseMessage(std::initializer_list<std::string> TargetID, int MessageType, bool deleteImmediate = true);
	int GetMessageType();

	BaseMessage(std::vector<IMessageReciever*> specificRecievers, int MessageType, bool deleteImmediate = true);

	virtual ~BaseMessage() {}
	std::vector<std::string> GetTargetIDs(void); 

	std::vector<IMessageReciever*> GetRecievers();

};

