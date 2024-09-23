#pragma once

#include <memory>

#include <unordered_map>
#include <string>

#include <functional>

#include "../../Log/log.h"

class NodeBase;

struct Slot {
	Slot(uint32_t n_id, std::string n_name, std::string n_type, NodeBase* n_parent, bool n_in = false, bool route = false) {
		m_id = n_id;
		name = n_name;
		isRoute = route;
		parent = n_parent;
		in = n_in;
		type = n_type;
	}
	Slot() {};

	std::string CreateData();

	std::string type;

	bool in;
	bool isRoute{ false };

	NodeBase* parent;

	std::shared_ptr<Slot> connected{nullptr};

	std::string data;

	std::string name;

	uint32_t m_id;
};



//using DataFunc = std::function<std::string(NodeBase&)>;

enum class BoolOperatorTypes : uint8_t
{
	Greater,
	Less,
	GreaterEqual,
	LessEqual,
	Equal,
	NotEqual,
	AND,
	OR,
	NOT
};

/*enum class EditableValueTypes : uint8_t {
	
};*/

class NodeBase {
public:
	NodeBase(uint32_t id, std::function<std::string(NodeBase&)> createDataFunc, std::string name) { m_id = id; CreateDataFunc = createDataFunc; m_name = name; };
	std::vector<std::shared_ptr<Slot>> input;
	std::vector<std::shared_ptr<Slot>> output;

	NodeBase() {};

	uint32_t m_id;

	virtual void Show();

	virtual std::string CreateData();

	std::function<std::string(NodeBase&)> CreateDataFunc;

	std::string m_name;
};

class VarNode : public NodeBase {
public:
	VarNode(uint32_t id, std::function<std::string(NodeBase&)> createDataFunc, std::string name) { m_id = id; CreateDataFunc = createDataFunc; m_name = name; };

	bool get{false};

	std::string CreateData() override;

	

	void Show() override;
};

class NodeViewer {
public:
	static void Init();
	

	static void Show();

	static void Interprete();

	// blueprints
	static void Branch();
	static void BoolOperator(BoolOperatorTypes type);

	

private:
	static uint32_t GetUniqueID();

	static void ShowNodeList();

	static void Start();

	static bool ValidLink(uint32_t  id0, uint32_t id1);
	static void RegisterNodeBase(NodeBase base);
	static void RegisterSlot(NodeBase* n, std::string n_name, std::string n_type, bool n_in, bool route);
};