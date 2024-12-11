#include "MessageHub.h"
#include <iostream>

void MessageHub::AddReciever(std::string GroupID, IMessageReciever* reciever)
{
	if (!reciever) { return; }
	MessageRecievers.insert(std::pair<std::string, IMessageReciever*>(GroupID, reciever));
}

void MessageHub::SendMsg(BaseMessage* newMessage)
{
	if (!newMessage) { return; } //Message is null, do not proceed further
	queuedMessages.push_back(newMessage);
}

void MessageHub::Update()
{
	for (BaseMessage* msg : queuedMessages)
	{
		cachedMessages.push_back(msg);
	}
	queuedMessages.clear();

	if (cachedMessages.empty()) { return; } //no messages in list, we do not need to do anything

	for (BaseMessage* msg : cachedMessages)
	{
		if (!msg) { continue; }
		if (msg->GetRecievers().empty())
		{
			for (auto it = MessageRecievers.begin(); it != MessageRecievers.end(); ++it)
			{
				std::string RecieverGroup = it->first;
				IMessageReciever* Reciever = it->second;

				for (std::string name : msg->GetTargetIDs())
				{
					if (!Reciever || RecieverGroup != name) { continue; }
					Reciever->HandleMessage(msg);
				}
			}
		}
		else
		{
			for (IMessageReciever* Reciever : msg->GetRecievers())
			{
				if (!Reciever) { continue; }
				Reciever->HandleMessage(msg);
			}
		}

		delete msg; //delete the message after all our gameObjects have read it
	}

	//clear the message list each frame
	cachedMessages.clear();
}
