#pragma once
#include <string>
class EntityAI2D;

class FiniteState
{
public:
	void Init(EntityAI2D* entity);
	std::string GetName(void);

	virtual void OnEnter() {}
	virtual void Update() {}
	virtual void OnExit() {}

	void SwitchState(std::string stateName);

	FiniteState(std::string stateName);
	~FiniteState() {}

protected:

private:
	//kept private as we do not want to modify the logic when creating derived classes
	std::string stateName;
	EntityAI2D* entity = nullptr;
};

