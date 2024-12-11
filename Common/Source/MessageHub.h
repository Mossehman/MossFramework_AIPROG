#pragma once
#include "DesignPatterns/SingletonTemplate.h"
#include "IMessageReciever.h"
#include "BaseMessage.h"


#include <vector>
#include <string>
#include <map>

class MessageHub : public SingletonTemplate<MessageHub>
{
public:

	void AddReciever(std::string GroupID, IMessageReciever* reciever);

	/// <summary>
	/// Adds a message to the messageCache
	/// </summary>
	/// <param name="newMessage"> - The message we are trying to send</param>
	void SendMsg(BaseMessage* newMessage);

	/// <summary>
	/// All messages will be read at the end of each frame via 1 for-loop
	/// </summary>
	void Update();



private:
	std::multimap<std::string, IMessageReciever*> MessageRecievers;
	std::vector<BaseMessage*> cachedMessages;

	std::vector<BaseMessage*> queuedMessages;

	
	

};

