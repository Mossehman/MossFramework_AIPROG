#include "InventoryManager.h"
#include "MouseController.h"
#include "KeyboardController.h"

GunBase*& InventoryManager::getPrimaryGun(void)
{
    return primaryGun;
}

void InventoryManager::setPrimaryGun(GunBase* newGun)
{
    this->primaryGun = newGun;
}

void InventoryManager::Update(double dt)
{
    if (CMouseController::GetInstance()->IsButtonDown(0))
    {
        primaryGun->Shoot();
    }

    if (CKeyboardController::GetInstance()->IsKeyPressed('R'))
    {
        primaryGun->Reload();
    }

    primaryGun->Update(dt, positionReference, entities);
}

void InventoryManager::Render()
{
    for (int i = 0; i < primaryGun->bullets.size(); i++)
    {
        if (primaryGun->bullets[i] == nullptr) { continue; }
        primaryGun->bullets[i]->Render();
    }
}
