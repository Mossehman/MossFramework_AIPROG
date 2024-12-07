#pragma once
#include <string>
#include <vector>

class FiniteStateMachine;

class FiniteState
{
public:
	void Init(FiniteStateMachine* stateMachine);

	virtual void OnEnter() {}
	virtual void Update(float dt) = 0;
	virtual void OnExit() {}
	
	~FiniteState() {}

protected:
	FiniteStateMachine* stateMachine = nullptr;
	bool SwitchState(std::string stateName);

};

