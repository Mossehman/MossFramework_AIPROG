#include "MessageHub.h"

void MessageHub::AddReciever(std::string GroupID, IMessageReciever* reciever)
{
	if (!reciever) { return; }
	MessageRecievers.insert(std::pair<std::string, IMessageReciever*>(GroupID, reciever));
}

void MessageHub::SendMessage(BaseMessage* newMessage)
{
	if (!newMessage) { return; } //Message is null, do not proceed further
	cachedMessages.push_back(newMessage);
}

void MessageHub::Update()
{
	if (cachedMessages.size() <= 0) { return; } //no messages in list, we do not need to do anything

	for (BaseMessage* msg : cachedMessages)
	{
		if (!msg) { continue; }

		if (msg->GetRecievers().size() > 0)
		{
			for (auto it = MessageRecievers.begin(); it != MessageRecievers.end(); ++it)
			{
				std::string RecieverGroup = it->first;
				IMessageReciever* Reciever = it->second;

				if (!Reciever || RecieverGroup != msg->GetTargetID()) { continue; }
				Reciever->HandleMessage(msg);
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
