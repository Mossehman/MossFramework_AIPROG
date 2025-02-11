#pragma once
#include <DesignPatterns/SingletonTemplate.h>
#include <map>
#include <string>

class TurnManager : public SingletonTemplate<TurnManager>
{
	friend SingletonTemplate<TurnManager>;
public:
	std::string currentEvent = "None";
	std::map<int, std::string> turnIDs;

	void RegisterTurnID(int turnNumber, std::string listenerID);
	int turnOrder;
	int maxTurns = 1;

	void NextTurn();
};

