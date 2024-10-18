#include "Application_AIPROG.h"

int main(void)
{
	Application_AIPROG app;
	app.Init("Hello World");
	app.Run();
	app.Exit();
}