#pragma once

#include <GL/glew.h> // Include GLEW before everything else

#include <GLFW/glfw3.h> // Include GLFW after GLEW

#ifndef IMGUI_ACTIVE

// Include ImGui headers
#include "imGUI/imgui.h"
#include "imGUI/backends/imgui_impl_glfw.h"
#include "imGUI/backends/imgui_impl_opengl3.h"

#define IMGUI_ACTIVE
#endif

#include "DesignPatterns/SingletonTemplate.h"
#include <vector>
#include <string>

class GUImanager: public SingletonTemplate<GUImanager>
{
	friend SingletonTemplate<GUImanager>;
public:
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Destroy() {}

	void textScroll(double dt);

	void pushToDialogueQueue(std::string dialogueLine);
	void startDialogue();

	unsigned int health = 100;

protected:

	char* NPCname;
	std::vector<std::string> dialogueQueue;
	unsigned int currentDialogueIndex = 0;
	unsigned int currentTextIndex = 0;
	float textScrollInterval = 0.2f;
	float textScrollIntervalBase = 0.2f;
	float currentTextInterval = 0.f;
	std::string currentDialogue = "";
	bool toDialogue;
	bool hasTextInteracted = false;

	bool toShowStratagems = false;

	bool toShowWarning;

	ImGuiWindowFlags window_flags; 
};

