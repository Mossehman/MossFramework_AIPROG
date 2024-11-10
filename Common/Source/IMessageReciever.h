#pragma once
#include "BaseMessage.h"

/// <summary>
/// Interface for all objects who can recieve and handle messages from the MessageHub
/// </summary>
class IMessageReciever {
public:
	IMessageReciever() {}
	virtual ~IMessageReciever() {}
	virtual int HandleMessage(BaseMessage* msg) = 0; //abstract class for interface implementation, made an int to allow for varying return values

};
