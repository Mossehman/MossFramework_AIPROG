#include "Application_AIPROG.h"

int main(void)
{
	Application_AIPROG app;
	app.Init("The Galactic War");
	app.Run();
	app.Exit();

	return 0;
}