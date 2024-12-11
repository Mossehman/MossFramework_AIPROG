#pragma once
#include <FiniteState.h>

class LOLEntity;

class TurretDamagedState : public FiniteState
{
protected:
	LOLEntity* attachedEntity = nullptr;
public:
	

	virtual void OnEnter() override;
	virtual void Update(float dt) override;

};

