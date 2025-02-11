#pragma once
#include <GameState.h>
class GameStateWin : public GameState
{
public:
	virtual bool Init();
	virtual bool Update(double dt);
	virtual void Render();
	virtual void Destroy();

protected:
	Mesh* textMesh = MeshBuilder::GenerateText("text", 16, 16);
	Camera camera;

	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
};

