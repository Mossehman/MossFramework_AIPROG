
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include <iostream>
#include "RenderParameters.h"
#include "GameStateManager.h"

class Application
{
public:
	Application();
	~Application();
	void Init(const char* windowName, int windowWidth = 1200, int windowHeight = 800);
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);

	void CloseWindow();

protected:
	virtual void OnInit() {}
	virtual void OnRun() {}
	virtual void OnExit() {}

private:
	void UpdateInputDevices(void);
	void PostUpdateInputDevices(void);
	//Declare a window object
	StopWatch m_timer;

	bool closeApplication = false;
};

#endif