#include "TestLevel.h"

void TestLevel::InitTiles()
{
	BindTexture(1, "Image/stone.tga");
	BindTexture(2, "Image/dirt.tga");
	BindTexture(3, "Image/grass.tga");
	BindPassability(1, IMPASSABLE);
}
