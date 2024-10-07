#include "ToolBarWindowManager.h"

#include <unordered_map>

#include "../Assets/AssetManager.h"
#include "Inspector.h"

#ifdef EXPERIMENTAL
#include "VisualScripting/StructEditor.h"
#include "VisualScripting/NodeViewer.h"
#endif

#include "../Log/Log.h"

#include "Imgui/imgui.h"

bool ShowInspector{true};
bool ShowAssetManager{ true };
bool ShowLog{ true };
bool ShowStructEditor{ true };
bool ShowNodeGraph{ true };

void WindowManager::Show()
{
	ImGui::BeginMainMenuBar();

	if(ImGui::BeginMenu("File")){
		if (ImGui::MenuItem("Save")) {
			//Log::Error("Window Manager", "Not Supported");
#ifdef EXPERIMENTAL
			StructEditor::Save();
#endif
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

#ifdef EXPERIMENTAL
		ImGui::MenuItem("Component Editor", "", &ShowStructEditor);
		ImGui::MenuItem("Node Graph Editor", "", &ShowNodeGraph);
#endif // EXPERIMENTAL

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
#ifdef EXPERIMENTAL
	if (ShowStructEditor) {
		StructEditor::Show();
	}
	if (ShowNodeGraph) {
		NodeViewer::Show();
	}
#endif
}

void WindowManager::Init()
{
	//Log::Init();
	//AssetManager::Init();

#ifdef EXPERIMENTAL
	NodeViewer::Init();
	StructEditor::Init("F:/GameEngines/NRISamples/_SaveFiles/Component_Save.ysave");
#endif
}
