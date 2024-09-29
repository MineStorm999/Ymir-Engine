#include "Inspector.h"
#include "../ECS/ECSManager.h"
#include "Imgui/imgui.h"
#include "Assets/AssetManager.h"

SelectedType sType;

AssetID sAsset;
entt::entity sEntity;



void Inspector::Select(AssetID a)
{
	if (!AssetManager::IsValid(a)) {
		return;
	}
	sAsset = a;
	sType = SelectedType::Asset;
}

void Inspector::Select(entt::entity e)
{
	if (!EntityManager::GetWorld().valid(e)) {
		return;
	}
	sType = SelectedType::Entity;
	sEntity = e;
}

void Inspector::DeSelect()
{
	sType = SelectedType::Nothing;
}

void Inspector::Show()
{
	switch (sType)
	{
	case Nothing:
		ShowDefault();
		break;
	case Asset:
		ShowAsset();
		break;
	case Entity:
		ShowEntity();
		break;
	default:
		ShowDefault();
		break;
	}
}

void Inspector::ShowAsset()
{
	ImGui::Begin("Inspector");
	if (!AssetManager::IsValid(sAsset)) {
		DeSelect();
		return;
	}

	char name[1024] = "";
	std::string pName = AssetManager::GetAsset(sAsset)->name;
	std::strcpy(name, pName.c_str());
	ImGui::InputText("Name", name, 1024);

	if (std::strcmp(pName.c_str(), name) != 0) {
		//sAsset->vRepr->vName = name;
		
		//sAsset->name = name;
	}


	AModel* m = dynamic_cast<AModel*>(AssetManager::GetAsset(sAsset));
	switch (AssetManager::GetAsset(sAsset)->type)
	{
	case AssetType::None:
		break;
	case AssetType::Model:
		//VModel* m = dynamic_cast<VModel*>(sAsset);
		if (!m) {
			break;
		}
		ImGui::Text("Current Render ID: %i", m->GetRenderID(0));
		if (ImGui::Button("Default Material")) {
			Select(m->DefaultMaterialID);
		}
		ImGui::Text("Trieangles: %i", m->indexCount);
		ImGui::Text("Vertices: %i", m->vertCount);

		if (ImGui::TreeNode("LODs")) {
			for (uint32_t i = 0; i < m->lods.size(); i++)
			{

				ImGui::Text("Triangle Count: %i", m->lods[i].lenght);
				float min = i > 0 ? m->lods[i - 1].distance : 0;
				float max = i + 1 < m->lods.size() ? m->lods[i + 1].distance : 0;

				ImGui::SliderFloat("Distance", &m->lods[i].distance, min, max);
			}

			ImGui::TreePop();
		}

		/*
		if (ImGui::TreeNode("Defaults Transforms")) {

			uint32_t i = 0;
			for (Transform& t : m->defaultTransforms)
			{

				if (ImGui::TreeNode(std::to_string(i).c_str())) {
					
					ImGui::Text("Local Position: %.3fx %.3fy %.3fz", t.localPos.x, t.localPos.y, t.localPos.z);
					ImGui::Text("Local Rotation: %.3fY %.3fP %.3fR", t.localRot.x, t.localRot.y, t.localRot.z);
					ImGui::Text("Local Scale: %.3fx %.3fy %.3fz", t.localScale.x, t.localScale.y, t.localScale.z);

					ImGui::TreePop();
				}
				i++;
			}

			ImGui::TreePop();
		}*/

	case AssetType::Texture:
		break;
	case AssetType::Material:
		break;
	case AssetType::Audio:
		break;
	default:
		break;
	}

	ImGui::Text("Asset ID: %i", sAsset);
	ImGui::Text("Asset Original Path: %i", AssetManager::GetAsset(sAsset)->originalPath);

	ImGui::End();
}

void Inspector::ShowEntity()
{
	ImGui::Begin("Inspector");



	ImGui::End();
}

void Inspector::ShowDefault()
{
	ImGui::Begin("Inspector");
	ImGui::Text("Nothing Selected...");


	ImGui::End();
}
