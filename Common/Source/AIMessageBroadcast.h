#pragma once
#include "DesignPatterns/SingletonTemplate.h"
#include <vector>
#include <string>

//Message broadcast hub for caching the different AI messages with ease
class AIMessageBroadcast : public SingletonTemplate<AIMessageBroadcast>
{
public:
	

protected:

	AIMessageBroadcast() {}
	~AIMessageBroadcast() {}
};

struct AIMessageData {
private:

	int SenderID;
	int Priority;
	std::string MessageData;

public:
	AIMessageData(int SenderID, int Priority, std::string MessageData)
	{
		this->SenderID = SenderID;
		this->Priority = Priority;
		this->MessageData = MessageData;
	}

	int GetSenderID(void) const {
		return this->SenderID;
	}

	int GetPriority(void) const {
		return this->Priority;
	}

	std::string GetMessageData(void) const {
		return this->MessageData;
	}
};

