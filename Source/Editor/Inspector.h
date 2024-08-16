#pragma once

enum SelectedType {
	Nothing,
	Asset,
	Entity
};

#include "../Assets/assetTypes.h"

class Inspector {
public:
	static void Select(AssetBase* a);
	static void Select(entt::entity e);
	static void DeSelect();

	static void Show();

private:
	static void ShowAsset();
	static void ShowEntity();
	static void ShowDefault();
};