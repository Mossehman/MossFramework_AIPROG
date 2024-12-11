#include "TestEntity.h"

void TestEntity::MoveAlongPath(float dt)
{

	if (currWaypointIndex < 0) { return; }

	if (this->position.x > pathWaypoints[currWaypointIndex].x)
	{

	}
	else if (this->position.x < pathWaypoints[currWaypointIndex].x)
	{

	}

	if (this->position.y > pathWaypoints[currWaypointIndex].y)
	{

	}
	else if (this->position.y < pathWaypoints[currWaypointIndex].y)
	{

	}

}

int TestEntity::HandleMessage(BaseMessage* message)
{
	std::cout << "I have recieved the message" << std::endl;
	return 1;
}
