#include "ToolBarWindowManager.h"

#include <unordered_map>

#include "../Assets/AssetManager.h"
#include "Inspector.h"
#include "VisualScripting/StructEditor.h"
#include "../Log/Log.h"

#include "Imgui/imgui.h"

bool ShowInspector{true};
bool ShowAssetManager{ true };
bool ShowLog{ true };
bool ShowStructEditor{ true };

void WindowManager::Show()
{
	ImGui::BeginMainMenuBar();

	if(ImGui::BeginMenu("File")){
		if (ImGui::MenuItem("Save")) {
			//Log::Error("Window Manager", "Not Supported");
			StructEditor::Save();
		}
		if (ImGui::MenuItem("Close")) {
			exit(EXIT_SUCCESS);
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Windows")) {

		ImGui::MenuItem("AssetManager", "", &ShowAssetManager);
		ImGui::MenuItem("Inspector", "", &ShowInspector);
		ImGui::MenuItem("Log", "", &ShowLog);
		ImGui::MenuItem("Component Editor", "", &ShowStructEditor);

		ImGui::EndMenu();
	}
	
	

	ImGui::EndMainMenuBar();
	
	if (ShowAssetManager) {
		AssetManager::Show();
	}
	if (ShowInspector) {
		Inspector::Show();
	}
	if (ShowLog) {
		Log::Show();
	}
	if (ShowStructEditor) {
		StructEditor::Show();
	}
}

void WindowManager::Init()
{
	Log::Init();
	AssetManager::Init();
	StructEditor::Init("F:/GameEngines/NRISamples/_SaveFiles/Component_Save.ymir");
}
