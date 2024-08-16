#include "Inspector.h"
#include "../ECS/ECSManager.h"
#include "Imgui/imgui.h"


SelectedType sType;

AssetBase* sAsset;
entt::entity sEntity;



void Inspector::Select(AssetBase* a)
{
	if (!a) {
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
		ShowAsset();
		break;
	default:
		ShowDefault();
		break;
	}
}

void Inspector::ShowAsset()
{
	ImGui::Begin("Inspector");
	if (!sAsset) {
		DeSelect();
		return;
	}

	if (!sAsset->vRepr) {
		DeSelect();
		return;
	}

	char name[1024] = "";
	std::string pName = sAsset->vRepr->vName;
	std::strcpy(name, pName.c_str());
	ImGui::InputText("Name", name, 1024);

	if (!std::strcmp(pName.c_str(), name)) {
		sAsset->vRepr->vName = name;
		
		sAsset->name = name;
	}


	VModel* m = dynamic_cast<VModel*>(sAsset);
	switch (sAsset->type)
	{
	case Nothing:
		break;
	case VirtualModel:
		//VModel* m = dynamic_cast<VModel*>(sAsset);
		if (!m) {
			break;
		}
		ImGui::Text("Render ID: %i", m->ID);
		ImGui::Text("Default Material ID: %i", m->DefaultMaterialID);

		ImGui::Text("Meshlet Count: %i", m->meshlets.size());

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
		}

		if (ImGui::TreeNode("Meshlets")) {
			uint32_t i = 0;
			for (MeshletDesc& mD : m->meshlets)
			{
				if (ImGui::TreeNode(std::to_string(i).c_str())) {
					ImGui::PushID(i * i + i);
					if (ImGui::TreeNode("Bounding Sphere")) {

						ImGui::Text("Position: %.3fx %.3fy %.3fz", mD.boundingSphere.x, mD.boundingSphere.y, mD.boundingSphere.z);
						ImGui::Text("Radius: %.3f", mD.boundingSphere.w);
						ImGui::TreePop();
					}

					uint32_t vTCount = mD.vertTriCount;
					uint8_t tCount = vTCount >> 8;
					uint16_t vCount = vTCount >> 16;
					
					ImGui::Text("Vertex Count: %i", (int)vCount);
					ImGui::Text("Indices Count: %i", (int)tCount);

					ImGui::PopID();
					ImGui::TreePop();
				}
				i++;
			}

			ImGui::TreePop();
		}

	case Texture:
		break;
	case Material:
		break;
	case Audio:
		break;
	default:
		break;
	}

	ImGui::Text("Asset ID: %i", sAsset->vRepr->realName);
	ImGui::Text("Asset Original Path: %i", sAsset->path);

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



	ImGui::End();
}
