#include "Application_AIPROG.h"

int main(void)
{
	Application_AIPROG app;
	app.Init("What are we, some kind of, League of Legends??");
	app.Run();
	app.Exit();

	return 0;
}