#include "MsgGetUnitsInRange.h"
#include "LOLEntity.h"

MsgGetUnitsInRange::MsgGetUnitsInRange(std::initializer_list<std::string> TargetID, TeamSearchParam unitSearchParam, LOLEntity* me, float radius, bool deleteImmediate) : MsgGetUnits(TargetID, unitSearchParam, me, deleteImmediate)
{
	this->MessageType = GET_UNITS_IN_RANGE;
	this->viewRadius = radius;
	this->radiusOrigin = me->getPosition();
}

MsgGetUnitsInRange::MsgGetUnitsInRange(std::vector<IMessageReciever*> specificRecievers, TeamSearchParam unitSearchParam, LOLEntity* me, float radius, bool deleteImmediate) : MsgGetUnits(specificRecievers, unitSearchParam, me, deleteImmediate)
{
	this->MessageType = GET_UNITS_IN_RANGE;
	this->viewRadius = radius;
	this->radiusOrigin = me->getPosition();
}

MsgGetUnitsInRange::MsgGetUnitsInRange(std::initializer_list<std::string> TargetID, TeamSearchParam unitSearchParam, LOLEntity* me, float radius, glm::vec2 radiusOrigin, bool deleteImmediate) : MsgGetUnits(TargetID, unitSearchParam, me, deleteImmediate)
{
	this->MessageType = GET_UNITS_IN_RANGE;
	this->viewRadius = radius;
	this->radiusOrigin = radiusOrigin;
}

MsgGetUnitsInRange::MsgGetUnitsInRange(std::vector<IMessageReciever*> specificRecievers, TeamSearchParam unitSearchParam, LOLEntity* me, float radius, glm::vec2 radiusOrigin, bool deleteImmediate) : MsgGetUnits(specificRecievers, unitSearchParam, me, deleteImmediate)
{
	this->MessageType = GET_UNITS_IN_RANGE; 
	this->viewRadius = radius;
	this->radiusOrigin = radiusOrigin;
}
