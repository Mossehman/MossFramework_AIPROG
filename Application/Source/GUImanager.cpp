#include "GUImanager.h"
#include "RenderParameters.h"
#include "KeyboardController.h"
#include <iostream>

void GUImanager::Init()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(RenderParameters::GetInstance()->getCurrentWindow(), true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Define the window flags
	window_flags = 0; 
	window_flags |= ImGuiWindowFlags_NoBackground; 
	window_flags |= ImGuiWindowFlags_NoTitleBar; 
	window_flags |= ImGuiWindowFlags_NoMove; 
	window_flags |= ImGuiWindowFlags_NoResize; 
	window_flags |= ImGuiWindowFlags_NoCollapse; 

	toDialogue = false;
	toShowStratagems = true;
	toShowWarning = true;

	pushToDialogueQueue("(Press Q to continue) Hello, Aaron here, honestly I wanted to do more with this, but alas, poor time management it seems");
	pushToDialogueQueue("You can also queue multiple dialogues, which is pretty neat");
	pushToDialogueQueue("Anyways sorry for the crap submission, more of a tech demo if anything");

	startDialogue();
}

void GUImanager::Update(double dt)
{
	const float windowScaleX = RenderParameters::GetInstance()->getWindowSizeX() / 1200;
	const float windowScaleY = RenderParameters::GetInstance()->getWindowSizeY() / 800;

	ImGui_ImplOpenGL3_NewFrame(); 
	ImGui_ImplGlfw_NewFrame(); 
	ImGui::NewFrame(); 

	if (windowScaleX <= 0 || windowScaleY <= 0) { return; }

	ImGui::Begin("Invisible window", NULL, window_flags);
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2(1200, 800));
	ImGui::SetWindowFontScale(1.5f * windowScaleY);

	if (toDialogue)
	{

		ImGuiWindowFlags dialogueWindowFlags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.188f, 0.188f, 0.188f, 1.0f)); // Red background// Set border color to yellow
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // Yellow border
		// Set border size
		ImGui::Begin("Dialogue", NULL, dialogueWindowFlags);
		ImGui::SetWindowPos(ImVec2(RenderParameters::GetInstance()->getWindowSizeX() * 0.03f,
			RenderParameters::GetInstance()->getWindowSizeY() * 0.7f));
		ImGui::SetWindowSize(ImVec2(1128.f * windowScaleX, 200.f * windowScaleY));
		ImGui::SetWindowFontScale(1.5f * windowScaleY);
		//ImGui::SameLine();
		{
			ImGuiWindowFlags nameWindowFlags = 
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoScrollbar;
			ImGui::Begin("Dialogue Name", NULL, nameWindowFlags);
			ImGui::SetWindowPos(ImVec2(RenderParameters::GetInstance()->getWindowSizeX() * 0.03f,
				RenderParameters::GetInstance()->getWindowSizeY() * 0.7f));
			ImGui::SetWindowSize(ImVec2(1128.f * windowScaleX, 40.f * windowScaleY));
			ImGui::SetWindowFontScale(2.0f * windowScaleY);
			ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "NPC name");
			ImGui::End();

			ImGuiWindowFlags textWindowFlags =
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoScrollbar;
			ImGui::Begin("Dialogue Text", NULL, textWindowFlags);
			ImGui::SetWindowPos(ImVec2(RenderParameters::GetInstance()->getWindowSizeX() * 0.03f,
				RenderParameters::GetInstance()->getWindowSizeY() * 0.7f + 40.f * windowScaleY));
			ImGui::SetWindowSize(ImVec2(1128.f * windowScaleX, 160.f * windowScaleY));
			ImGui::PushTextWrapPos(0.0f);
			ImGui::SetWindowFontScale(1.5f * windowScaleY);
			textScroll(dt);
			const char* dialogue = currentDialogue.c_str();
			ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), dialogue);
			ImGui::End();

		}


		ImGui::End();
		ImGui::PopStyleColor(2); // Pop the dialogue window background color
	}
	//if (toShowStratagems)
	//{
	//	ImGuiWindowFlags stratagemTitleFlags = ImGuiWindowFlags_AlwaysAutoResize |
	//		ImGuiWindowFlags_NoTitleBar |
	//		ImGuiWindowFlags_NoMove |
	//		ImGuiWindowFlags_NoResize |
	//		ImGuiWindowFlags_NoCollapse |
	//		ImGuiWindowFlags_NoScrollbar;
	//	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.188f, 0.188f, 0.188f, 0.5f));
	//	// Set border size
	//	ImGui::Begin("Stratagem Titles", NULL, stratagemTitleFlags);
	//	ImGui::SetWindowPos(ImVec2(RenderParameters::GetInstance()->getWindowSizeX() * 0.01f,
	//		RenderParameters::GetInstance()->getWindowSizeY() * 0.01f));
	//	//ImGui::SetWindowSize(ImVec2(1128.f * windowScaleX, 200.f * windowScaleY));
	//	ImGui::SetWindowFontScale(2.0f * windowScaleY);
	//	ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "Stratagems");
	//	ImGui::End();
	//	ImGui::PopStyleColor(); // Pop the dialogue window background color


	//	ImGuiWindowFlags stratagemWindowFlags = ImGuiWindowFlags_AlwaysAutoResize | 
	//		ImGuiWindowFlags_NoTitleBar | 
	//		ImGuiWindowFlags_NoMove | 
	//		ImGuiWindowFlags_NoResize |  
	//		ImGuiWindowFlags_NoCollapse | 
	//		ImGuiWindowFlags_NoScrollbar; 
	//	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.188f, 0.188f, 0.188f, 0.5f)); 
	//	// Set border size
	//	ImGui::Begin("Stratagem Window", NULL, stratagemTitleFlags); 
	//	ImGui::SetWindowPos(ImVec2(RenderParameters::GetInstance()->getWindowSizeX() * 0.01f, 
	//		RenderParameters::GetInstance()->getWindowSizeY() * 0.07f)); 
	//	ImGui::End(); 
	//	ImGui::PopStyleColor(); // Pop the dialogue window background color 
	//}
	if (toShowWarning)
	{
		ImGuiWindowFlags warningFlags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar;

		// Set a semi-transparent red background for the warning
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.f, 0.f, 0.f, 0.5f));

		ImGui::Begin("Warnings", NULL, warningFlags);

		// Set the position and size of the window
		ImGui::SetWindowPos(ImVec2(RenderParameters::GetInstance()->getWindowSizeX() * 0.2f,
			RenderParameters::GetInstance()->getWindowSizeY() * 0.01f));
		ImGui::SetWindowSize(ImVec2(720.f * windowScaleX, 60.f * windowScaleY));

		// Set the font scale
		ImGui::SetWindowFontScale(2.0f * windowScaleY);

		// Convert the unsigned int to std::string
		std::string healthStr = std::to_string(health);

		// Calculate text size
		ImVec2 textSize = ImGui::CalcTextSize(healthStr.c_str());

		// Calculate the center position for the text
		float windowWidth = ImGui::GetWindowSize().x;
		float textIndentation = (windowWidth - textSize.x) / 2.0f;

		// Ensure indentation is not negative
		if (textIndentation > 0.0f)
			ImGui::SetCursorPosX(textIndentation);

		// Draw the centered text
		ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "%s", healthStr.c_str());

		ImGui::End();

		// Pop the window background color
		ImGui::PopStyleColor();
	}


	ImGui::End();
}

void GUImanager::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUImanager::textScroll(double dt)
{
	if (currentDialogueIndex >= dialogueQueue.size())
	{
		toDialogue = false;
		dialogueQueue.clear();
		return;
	}
	currentTextInterval += dt;

	if (CKeyboardController::GetInstance()->IsKeyUp('Q'))
	{
		hasTextInteracted = false;
	}

	while (this->currentTextIndex < dialogueQueue[currentDialogueIndex].size() && currentTextInterval >= textScrollInterval)
	{
		currentDialogue += dialogueQueue[currentDialogueIndex][currentTextIndex];
		currentTextInterval = 0;
		currentTextIndex++;

		if (CKeyboardController::GetInstance()->IsKeyDown('Q') && !hasTextInteracted)
		{
			textScrollInterval = 0;
			hasTextInteracted = true;
		}
	}

	if (CKeyboardController::GetInstance()->IsKeyDown('Q') && !hasTextInteracted && this->currentTextIndex >= dialogueQueue[currentDialogueIndex].size())
	{
		currentDialogueIndex++;
		currentDialogue = "";
		currentTextIndex = 0;
		currentTextInterval = 0;
		textScrollInterval = textScrollIntervalBase;
	}

}

void GUImanager::pushToDialogueQueue(std::string dialogueLine)
{
	this->dialogueQueue.push_back(dialogueLine);
}

void GUImanager::startDialogue()
{
	if (dialogueQueue.empty())
	{
		return;
	}

	toDialogue = true;
}
