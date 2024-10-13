#include "Inspector.h"
#include "../ECS/ECSManager.h"
#include "Imgui/imgui.h"
#include "Assets/AssetManager.h"
#include "ECS/components.h"
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
	if(!ImGui::Begin("Inspector")) {
		return;
	}
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
		ImGui::Text("Current Render ID: %i", m->GetRenderID(0, sAsset));
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
	ImGui::Text("Asset Original Path: %s", AssetManager::GetAsset(sAsset)->originalPath.c_str());

	ImGui::End();
}

void Inspector::ShowEntity()
{
	if (!ImGui::Begin("Inspector")) {
		return;
	}

	ECSWorld& w = EntityManager::GetWorld();

	if (!w.valid(sEntity)) {
		ImGui::End();
		return;
	}


	bool transform = false, mesh = false, identity = false;

	static char name[1024] = "";
	
	if (IdentityComponent* id = w.try_get<IdentityComponent>(sEntity)) {
		identity = true;
		std::strcpy(name, id->name.c_str());
		ImGui::InputText("Name", name, 1024);
		id->name = name;
		ImGui::Text("GPU ID: %u", (int)id->instanceGPUID);
	}

	if (TransformComponent* t = w.try_get<TransformComponent>(sEntity)) {
		transform = true;
		if (ImGui::TreeNode("Transform")) {
			float3 b = t->localPos;
			ImGui::DragFloat3("Postion", (float*)&t->localPos);
			if (b.x != t->localPos.x || b.y != t->localPos.y || b.z != t->localPos.z) {
				w.emplace_or_replace<Dirty>(sEntity);
			}

			b = t->localRot;
			ImGui::DragFloat3("Rotation", (float*)&t->localRot);
			if (b.x != t->localRot.x || b.y != t->localRot.y || b.z != t->localRot.z) {
				w.emplace_or_replace<Dirty>(sEntity);
			}

			b = t->localScale;
			ImGui::DragFloat3("Scale", (float*)&t->localScale);
			if (b.x != t->localScale.x || b.y != t->localScale.y || b.z != t->localScale.z) {
				w.emplace_or_replace<Dirty>(sEntity);
			}

			ImGui::TreePop();
		}
	}

	if (MeshInstanceComponent* m = w.try_get<MeshInstanceComponent>(sEntity)) {
		mesh = true;
		if (ImGui::TreeNode("Mesh Instance")) {
			std::string prev = AssetManager::IsValid(m->modelID) ? AssetManager::GetAsset(m->modelID)->name : "Nothing";
			if (ImGui::BeginCombo("Mesh", prev.c_str())) {
				std::vector<AssetID> meshes = AssetManager::GetFromType(AssetType::Model);

				ImGui::PushID(INVALID_ASSET_ID);
				bool selected = INVALID_ASSET_ID == m->modelID;
				if (ImGui::Selectable("Nothing", selected)) {
					m->modelID = INVALID_ASSET_ID;
					w.emplace_or_replace<Dirty>(sEntity);
				}
				if (selected) {
					ImGui::SetItemDefaultFocus();
				}
				ImGui::PopID();
				for (AssetID id : meshes)
				{
					ImGui::PushID(id);
					bool selected = id == m->modelID;
					if (ImGui::Selectable(AssetManager::GetAsset(id)->name.c_str(), selected)) {
						m->modelID = id;
						w.emplace_or_replace<Dirty>(sEntity);
					}
					if (selected) {
						ImGui::SetItemDefaultFocus();
					}

					ImGui::PopID();
				}

				ImGui::EndCombo();
			}

			prev = AssetManager::IsValid(m->materialID) ? AssetManager::GetAsset(m->materialID)->name : "Nothing";
			if (ImGui::BeginCombo("Material", prev.c_str())) {
				std::vector<AssetID> materials = AssetManager::GetFromType(AssetType::Material);

				ImGui::PushID(INVALID_ASSET_ID);
				bool selected = INVALID_ASSET_ID == m->modelID;
				if (ImGui::Selectable("Nothing", selected)) {
					m->materialID = INVALID_ASSET_ID;
					w.emplace_or_replace<Dirty>(sEntity);
				}
				ImGui::PopID();
				for (AssetID id : materials)
				{
					ImGui::PushID(id);
					bool selected = id == m->materialID;
					if (ImGui::Selectable(AssetManager::GetAsset(id)->name.c_str(), selected)) {
						m->materialID = id;
						w.emplace_or_replace<Dirty>(sEntity);
					}
					if (selected) {
						ImGui::SetItemDefaultFocus();
					}

					ImGui::PopID();
				}

				ImGui::EndCombo();
			}

			ImGui::TreePop();
		}
	}

	if (ImGui::BeginCombo("Add Component", "")) {
		if (ImGui::Selectable("Cancel", true)) {
			
		}

		if (!transform) {
			if (ImGui::Selectable("Transform", false)) {
				w.emplace<TransformComponent>(sEntity);
			}
		}
		if (!mesh) {
			if (ImGui::Selectable("Mesh", false)) {
				w.emplace<MeshInstanceComponent>(sEntity);
			}
		}
		if (!identity) {
			if (ImGui::Selectable("Identity", false)) {
				w.emplace<IdentityComponent>(sEntity);
			}
		}
		ImGui::EndCombo();
	}

	ImGui::End();
}

void Inspector::ShowDefault()
{
	if (!ImGui::Begin("Inspector")) {
		return;
	}
	ImGui::Text("Nothing Selected...");


	ImGui::End();
}
