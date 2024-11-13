#include "Minimap.h"
#include "Map2D.h"

void Minimap::Init()
{
	//compoundMinimapMesh();
	//minimapSize = glm::vec2(300);
	//offset = glm::vec2(400, -200);
	//textureID = 0;
	//recalculateMapScale();

	//backgroundMesh = MeshBuilder::GenerateQuad("map BG", Color(0.188, 0.188, 0.188));
	//playerMesh = MeshBuilder::GeneratePolygon("Player", Color(1, 0.984, 0));

	//std::cout << "Map Scale: " << mapScaleX << ", " << mapScaleY << std::endl;
}

void Minimap::Update(double dt, glm::vec2 playerPos)
{
	//this->playerPos.x = (playerPos.x / Map2D::GetInstance()->GetTileSize().x) / Map2D::GetInstance()->mapSizeX[Map2D::GetInstance()->GetCurrentLevel()] - 0.5f;
	//this->playerPos.y = (playerPos.y / Map2D::GetInstance()->GetTileSize().y) / (Map2D::GetInstance()->mapSizeY[Map2D::GetInstance()->GetCurrentLevel()] + 0.5f) + 0.5f;
}

void Minimap::Render(Camera& camera)
{
	//RenderParameters::GetInstance()->modelStack.PushMatrix();
	//RenderParameters::GetInstance()->modelStack.Translate(offset.x + camera.position.x, offset.y + camera.position.y, 0);
	//RenderParameters::GetInstance()->modelStack.Rotate(0, 1, 0, 0);
	//RenderParameters::GetInstance()->modelStack.Rotate(0, 0, 1, 0);
	//RenderParameters::GetInstance()->modelStack.Scale(minimapSize.x, minimapSize.y, 1);

	//{
	//	RenderParameters::GetInstance()->modelStack.PushMatrix();
	//	RenderParameters::GetInstance()->modelStack.Translate(playerPos.x, playerPos.y, 0);
	//	RenderParameters::GetInstance()->modelStack.Rotate(0, 1, 0, 0);
	//	RenderParameters::GetInstance()->modelStack.Rotate(0, 0, 1, 0);
	//	RenderParameters::GetInstance()->modelStack.Scale(mapScaleX, mapScaleY, 1);


	//	Mtx44 MVP;
	//	MVP = RenderParameters::GetInstance()->projectionStack.Top() * RenderParameters::GetInstance()->viewStack.Top() * RenderParameters::GetInstance()->modelStack.Top();
	//	glUniformMatrix4fv(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_MVP], 1, GL_FALSE, &MVP.a[0]);


	//	if (textureID > 0)
	//	{
	//		glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_COLOR_TEXTURE_ENABLED], 1);
	//		glActiveTexture(GL_TEXTURE0);
	//		glBindTexture(GL_TEXTURE_2D, textureID);
	//		glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_COLOR_TEXTURE], 0);
	//	}
	//	else
	//	{
	//		glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_COLOR_TEXTURE_ENABLED], 0);
	//	}

	//	playerMesh->Render();

	//	if (textureID > 0)
	//	{
	//		glBindTexture(GL_TEXTURE_2D, 0);
	//	}

	//	RenderParameters::GetInstance()->modelStack.PopMatrix();
	//}

	//{
	//	RenderParameters::GetInstance()->modelStack.PushMatrix();
	//	RenderParameters::GetInstance()->modelStack.Translate(mapOffset.x, mapOffset.y, 0);
	//	RenderParameters::GetInstance()->modelStack.Rotate(0, 1, 0, 0);
	//	RenderParameters::GetInstance()->modelStack.Rotate(0, 0, 1, 0);
	//	RenderParameters::GetInstance()->modelStack.Scale(mapScaleX, mapScaleY, 1);


	//	Mtx44 MVP;
	//	MVP = RenderParameters::GetInstance()->projectionStack.Top() * RenderParameters::GetInstance()->viewStack.Top() * RenderParameters::GetInstance()->modelStack.Top();
	//	glUniformMatrix4fv(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_MVP], 1, GL_FALSE, &MVP.a[0]);


	//	if (textureID > 0)
	//	{
	//		glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_COLOR_TEXTURE_ENABLED], 1);
	//		glActiveTexture(GL_TEXTURE0);
	//		glBindTexture(GL_TEXTURE_2D, textureID);
	//		glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_COLOR_TEXTURE], 0);
	//	}
	//	else
	//	{
	//		glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_COLOR_TEXTURE_ENABLED], 0);
	//	}

	//	minimapMesh->Render();

	//	if (textureID > 0)
	//	{
	//		glBindTexture(GL_TEXTURE_2D, 0);
	//	}

	//	RenderParameters::GetInstance()->modelStack.PopMatrix();
	//}


	//RenderParameters::GetInstance()->modelStack.PushMatrix();
	//RenderParameters::GetInstance()->modelStack.Scale(1.05, 1.05, 1);
	//
	//Mtx44 MVP;
	//MVP = RenderParameters::GetInstance()->projectionStack.Top() * RenderParameters::GetInstance()->viewStack.Top() * RenderParameters::GetInstance()->modelStack.Top();
	//glUniformMatrix4fv(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_MVP], 1, GL_FALSE, &MVP.a[0]);


	//if (textureID > 0)
	//{
	//	glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_COLOR_TEXTURE_ENABLED], 1);
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, textureID);
	//	glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_COLOR_TEXTURE], 0);
	//}
	//else
	//{
	//	glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_COLOR_TEXTURE_ENABLED], 0);
	//}

	//backgroundMesh->Render();

	//if (textureID > 0)
	//{
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}

	//RenderParameters::GetInstance()->modelStack.PopMatrix();
	//RenderParameters::GetInstance()->modelStack.PopMatrix();

}

void Minimap::compoundMinimapMesh()
{
	//float tileWidth = 1.0f;  // Assume each tile is 1 unit wide
	//float tileHeight = 1.0f; // Assume each tile is 1 unit tall

	//for (int i = 0; i < Map2D::GetInstance()->mapSizeX[Map2D::GetInstance()->GetCurrentLevel()]; i++)
	//{
	//	for (int j = 0; j < Map2D::GetInstance()->mapSizeY[Map2D::GetInstance()->GetCurrentLevel()]; j++)
	//	{
	//		if (Map2D::GetInstance()->GetMapInfo(j, i, Map2D::GetInstance()->GetCurrentLevel()) != 0)
	//		{
	//			// Calculate the bottom-left corner of the tile in world coordinates
	//			float x = i * tileWidth;
	//			float y = j * tileHeight * -1;

	//			// Create vertices for the quad
	//			mapMeshData.push_back(glm::vec2(x, y));                 // Bottom-left
	//			mapMeshData.push_back(glm::vec2(x + tileWidth, y));     // Bottom-right
	//			mapMeshData.push_back(glm::vec2(x + tileWidth, y + tileHeight)); // Top-right
	//			mapMeshData.push_back(glm::vec2(x, y + tileHeight));    // Top-left
	//		}
	//	}
	//}
	//minimapMesh = MeshBuilder::compoundedTileMesh("minimap", mapMeshData);
}

void Minimap::recalculateMapScale()
{
	//mapScaleX = 1.0f / (Map2D::GetInstance()->mapSizeX[Map2D::GetInstance()->GetCurrentLevel()]);
	//mapScaleY = 1.0f / (Map2D::GetInstance()->mapSizeY[Map2D::GetInstance()->GetCurrentLevel()]);
	//mapOffset.x = -0.5f;
	//mapOffset.y = 0.5f;
}								  

