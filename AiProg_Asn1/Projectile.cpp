#include "Projectile.h"
#include "Map2D.h"
#include "IDamageable.h"
#include "NodePassabilityAI.h"

void Projectile::Update(double dt)
{
	toRender = IsActive;
	if (!IsActive) { return; }

	if (Direction.x == 0 && Direction.y == 0) { 
		Collided = true; 
		return; 
	}

	position += Direction * (float)(MoveSpeed * dt);

	glm::ivec2 tilePos = Map2D::GetInstance()->PosToTilePos(position);
	if (Map2D::GetInstance()->GetLevel()->GetTilemap()[tilePos.y][tilePos.x]->Passability == IMPASSABLE) { 
		Collided = true; 

 		return; 
	}

	for (Entity2D* entityToCheck : EntitiesToCollideWith)
	{
		if (!checkEntityCollision(entityToCheck)) { continue; }
		//std::cout << "collision with entity: " << position.x << ", " << position.y << std::endl;
		IDamageable* damageableEntity = dynamic_cast<IDamageable*>(entityToCheck);

		if (!damageableEntity) { continue; }
		damageableEntity->Damage(Damage);
		Collided = true;
		return;
	}
}
