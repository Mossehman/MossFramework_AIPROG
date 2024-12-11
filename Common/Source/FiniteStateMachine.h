#include "FiniteState.h"
#include <map>

class EntityAI2D;


class FiniteStateMachine
{
public:
	void Update(float dt);
	void SetCurrState(FiniteState* state);
	bool SetCurrState(std::string stateName);

	void AddState(std::string name, FiniteState* state);
	void RemoveState(std::string name);
	FiniteState* GetCurrState();
	std::string GetCurrStateName();

	FiniteStateMachine(EntityAI2D* attachedEntity);
	~FiniteStateMachine();

	EntityAI2D* GetAttachedEntity();

	template <typename T>
	T* GetState(std::string name);

protected:
	std::map<std::string, FiniteState*> States = std::map<std::string, FiniteState*>();
	FiniteState* CurrentState = nullptr; //the current state we will be using to update our entity
	FiniteState* NextState = nullptr; //Allows us to set the next state without interrupting the current state's update cycle until the next frame
	EntityAI2D* attachedEntity = nullptr;
};

template<typename T>
inline T* FiniteStateMachine::GetState(std::string name) 
{
	if (States.count(name) <= 0) { return nullptr; } 
	return dynamic_cast<T>(States.at(name)); 
}
