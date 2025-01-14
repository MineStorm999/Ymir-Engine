#include "Inspector.h"
#include "../ECS/ECSManager.h"
#include "Imgui/imgui.h"
#include "Assets/AssetManager.h"
#include "ECS/components.h"
#include "World/SceneManager.h"

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
	if (!EntityManager::IsValid(e)) {
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

	if (AssetManager::GetAsset(sAsset)->type == AssetType::Model) {
		AModel* m = dynamic_cast<AModel*>(AssetManager::GetAsset(sAsset));
		if (!m) {
			return;
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
	}
	else if (AssetManager::GetAsset(sAsset)->type == AssetType::Material) {
		AMaterial* mat = dynamic_cast<AMaterial*>(AssetManager::GetAsset(sAsset));
		if (!mat) {
			return;
		}
		ImGui::Text("Current Render ID: %i", SceneManager::GetRenderScene()->GetRenderID(sAsset));
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

	if (!EntityManager::IsValid(sEntity)) {
		ImGui::End();
		return;
	}


	bool transform = false, mesh = false, identity = false;

	static char name[1024] = "";
	
	if (EntityManager::HasComponent<IdentityComponent>(sEntity)) {
		IdentityComponent& id = EntityManager::GetComponent<IdentityComponent>(sEntity);
		identity = true;
		std::strcpy(name, id.name.c_str());
		ImGui::InputText("Name", name, 1024);
		id.name = name;
		ImGui::Text("GPU ID: %u", (int)id.instanceGPUID);
	}
	static bool lock = false;
	if (EntityManager::HasComponent<TransformComponent>(sEntity)) {
		TransformComponent& t = EntityManager::GetComponent<TransformComponent>(sEntity);
		transform = true;
		if (ImGui::TreeNode("Transform")) {
			float3 b = t.localPos;
			
			ImGui::DragFloat3("Postion", (float*)&t.localPos, 0.1f);
			if (b.x != t.localPos.x || b.y != t.localPos.y || b.z != t.localPos.z) {
				EntityManager::SetDirty(sEntity, DirtyFlags::Transform);
			}

			b = t.localRot;
			
			ImGui::DragFloat3("Rotation", (float*)&t.localRot);
			if (b.x != t.localRot.x || b.y != t.localRot.y || b.z != t.localRot.z) {
				EntityManager::SetDirty(sEntity, DirtyFlags::Transform);
			}

			//ImGui::Checkbox("Lock", &lock);
			b = t.localScale;
			ImGui::DragFloat3("Scale", (float*)&t.localScale, 0.2f);
			if (b.x != t.localScale.x || b.y != t.localScale.y || b.z != t.localScale.z) {
				EntityManager::SetDirty(sEntity, DirtyFlags::Transform);
			}

			ImGui::TreePop();
		}
	}

	if (EntityManager::HasComponent<MeshInstanceComponent>(sEntity)) {
		MeshInstanceComponent& m = EntityManager::GetComponent<MeshInstanceComponent>(sEntity);
		RenderScene* rs = SceneManager::GetRenderScene();
		mesh = true;
		if (ImGui::TreeNode("Mesh Instance")) {
			std::string prev = AssetManager::IsValid(m.modelID) ? AssetManager::GetAsset(m.modelID)->name : "Nothing";
			if (ImGui::BeginCombo("Mesh", prev.c_str())) {
				std::vector<AssetID> meshes = AssetManager::GetFromType(AssetType::Model);

				ImGui::PushID(INVALID_ASSET_ID);
				bool selected = INVALID_ASSET_ID == m.modelID;
				if (ImGui::Selectable("Nothing", selected)) {
					m.modelID = INVALID_ASSET_ID;
					EntityManager::SetDirty(sEntity);
				}
				if (selected) {
					ImGui::SetItemDefaultFocus();
				}
				ImGui::PopID();
				for (AssetID id : meshes)
				{
					ImGui::PushID(id);
					selected = id == m.modelID;
					if (ImGui::Selectable(AssetManager::GetAsset(id)->name.c_str(), selected)) {
						m.modelID = id;
						if (rs) {
							rs->Add(id);
						}
						EntityManager::SetDirty(sEntity, DirtyFlags::Transform);
					}
					if (selected) {
						ImGui::SetItemDefaultFocus();
					}

					ImGui::PopID();
				}

				ImGui::EndCombo();
			}

			prev = AssetManager::IsValid(m.materialID) ? AssetManager::GetAsset(m.materialID)->name : "Nothing";
			if (ImGui::BeginCombo("Material", prev.c_str())) {
				std::vector<AssetID> materials = AssetManager::GetFromType(AssetType::Material);

				ImGui::PushID(INVALID_ASSET_ID);
				bool selected = INVALID_ASSET_ID == m.modelID;
				if (ImGui::Selectable("Nothing", selected)) {
					m.materialID = INVALID_ASSET_ID;
					EntityManager::SetDirty(sEntity, DirtyFlags::Transform);
				}
				ImGui::PopID();
				uint32_t i = 0;
				for (AssetID id : materials)
				{
					AMaterial* mat = dynamic_cast<AMaterial*>(AssetManager::GetAsset(id));
					i++;
					ImGui::PushID(id);
					selected = id == m.materialID;
					//Log::Message("Inspector", "Material Option" + std::to_string(i) + mat->name);
					if (ImGui::Selectable(mat->name.c_str(), selected)) {
						m.materialID = id;
						if (rs) {
							rs->Add(id);
						}
						EntityManager::SetDirty(sEntity, DirtyFlags::Transform);
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
				EntityManager::AddComponent<TransformComponent>(sEntity);
			}
		}
		if (!mesh) {
			if (ImGui::Selectable("Mesh", false)) {
				EntityManager::AddComponent<MeshInstanceComponent>(sEntity);
			}
		}
		if (!identity) {
			if (ImGui::Selectable("Identity", false)) {
				EntityManager::AddComponent<IdentityComponent>(sEntity);
			}
		}
		if (!identity) {
			if (ImGui::Selectable("Identity", false)) {
				EntityManager::AddComponent<IdentityComponent>(sEntity);
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
