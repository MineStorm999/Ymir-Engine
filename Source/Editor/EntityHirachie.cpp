#include "EntityHirachie.h"
#include "ECS/ECSManager.h"
#include "ECS/components.h"
#include "Inspector.h"

uint32_t iter = 0;
void IterChilds(entt::entity e) {
	if (!EntityManager::GetWorld().valid(e)) {
		return;
	}
	IdentityComponent* id = EntityManager::GetWorld().try_get<IdentityComponent>(e);
	if (!id) {
		return;
	}

	ImGui::PushID(iter++);
	if (ImGui::TreeNode(id->name.c_str())) {
		if ((ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))) {
			Inspector::Select(e);
		}

		ImGui::PushID(iter++);
		if (ImGui::BeginPopupContextItem("Create Child", 1)) {
			if (ImGui::MenuItem("Create Child")) {
				EntityManager::CreateEntity(id->name + " Child", e);
			}
			if (ImGui::MenuItem("Delete")) {
				EntityManager::DeleteEntity(e);
			}
			ImGui::EndPopup();
		}
		ImGui::PopID();

		for (entt::entity child : id->childs) {
			IterChilds(child);
		}
		ImGui::TreePop();
	}
	else {
		if ((ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))) {
			Inspector::Select(e);
		}

		ImGui::PushID(iter++);
		if (ImGui::BeginPopupContextItem("Create Child", 1)) {
			if (ImGui::MenuItem("Create Child")) {
				EntityManager::CreateEntity(id->name + " Child", e);
			}
			if (ImGui::MenuItem("Delete")) {
				EntityManager::DeleteEntity(e);
			}
			ImGui::EndPopup();
		}
		ImGui::PopID();
	}
	ImGui::PopID();
	
}

void EntityHirachie::Show()
{
	if (ImGui::Begin("Hirachie")) {
		iter = 0;
		if (ImGui::BeginPopupContextItem("Create", 1)) {
			if (ImGui::MenuItem("Create Entity")) {
				EntityManager::CreateEntity("New Entity");
			}
			ImGui::EndPopup();
		}

		if (IdentityComponent* id = EntityManager::GetWorld().try_get<IdentityComponent>(EntityManager::GetRoot())){
			for (entt::entity child : id->childs)
			{
				IterChilds(child);
			}
		}
		ImGui::End();
	}
}
