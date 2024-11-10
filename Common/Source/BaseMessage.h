#pragma once
#include <string>
#include <vector>

class IMessageReciever;

class BaseMessage
{
protected:
	std::string TargetID;
	int MessageType = 0;
	std::vector<IMessageReciever*> specificRecievers;
	bool deleteImmediate;



public:
	BaseMessage(std::string TargetID, int MessageType, bool deleteImmediate = true);

	BaseMessage(std::vector<IMessageReciever*> specificRecievers, int MessageType, bool deleteImmediate = true);
	std::string GetTargetID(void);

	std::vector<IMessageReciever*>& GetRecievers();

};

