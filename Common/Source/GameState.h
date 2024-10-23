#pragma once
#include <string>

class GameState
{
protected:
	std::string gameStateName;

public:

	GameState(std::string gameStateName);
	GameState() {}
	~GameState() {}

	virtual bool Init() = 0;
	virtual bool Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Destroy() = 0;

	void SetName(std::string newName);
	std::string GetName(void) const;
};

