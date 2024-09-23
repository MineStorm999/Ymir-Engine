#include "NodeViewer.h"

#include "../../ImNodes/imnodes.h"
#include <ImGui/imgui.h>
#include <vector>

std::vector<std::shared_ptr<NodeBase>> nodes;

std::unordered_map<uint32_t, std::shared_ptr<Slot>> slots;

std::shared_ptr<NodeBase> startNode;

void NodeViewer::Init()
{
    ImNodes::CreateContext();
    Start();
}

uint32_t NodeViewer::GetUniqueID()
{
    static uint32_t id = 0;
    id++;
    return id;
}

void NodeViewer::ShowNodeList()
{
    if (ImGui::TreeNode("Code Structure")) {
        if (ImGui::Button("Branch")) {
            Branch();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Bool Operators")) {
        if (ImGui::Button("AND")) {
            BoolOperator(BoolOperatorTypes::AND);
        }
        if (ImGui::Button("OR")) {
            BoolOperator(BoolOperatorTypes::OR);
        }
        if (ImGui::Button("NOT")) {
            BoolOperator(BoolOperatorTypes::NOT);
        }
        ImGui::TreePop();
    }
}

std::function<std::string(NodeBase&)> StartFunc([](NodeBase& n) {

    std::string out = n.output[0]->CreateData();
    out.append("\n");
    return out;
    });
void NodeViewer::Start()
{
    std::shared_ptr<NodeBase> node = std::make_shared<NodeBase>(GetUniqueID(), StartFunc, "Start");

    // out
    RegisterSlot(node.get(), "", "Route", false, true);

    startNode = node;
    nodes.push_back(node);
}

std::vector<std::pair<int, int>> links;

uint32_t curSelected = 0xffffffff;

void NodeViewer::Show()
{
    if (!ImGui::Begin("node editor")) {
        return;
    }

    ImGui::BeginChild("NodeList", ImVec2(150, ImGui::GetContentRegionAvail().y));
    if (ImGui::Button("Interprete", ImVec2(145, 30))) {
        Interprete();
    }
    ShowNodeList();
    ImGui::EndChild();

    ImGui::SameLine();
    ImNodes::BeginNodeEditor();


    for (std::shared_ptr<NodeBase> n : nodes)
    {
        n->Show();
    }


    
    // elsewhere in the code...
    for (int i = 0; i < links.size(); ++i)
    {
        const std::pair<int, int> p = links[i];
        // in this case, we just use the array index of the link
        // as the unique identifier

        if (slots.find(p.first) == slots.end()) {
            links.erase(links.begin() + i);
            return;
        }
        if (slots.find(p.second) == slots.end()) {
            links.erase(links.begin() + i);
            return;
        }

        if (!slots[p.first]) {
            links.erase(links.begin() + i);
            return;
        }
        if (!slots[p.second]) {
            links.erase(links.begin() + i);
            return;
        }


        if (slots[p.first]->isRoute) {
            ImNodes::PushColorStyle(ImNodesCol_Link, IM_COL32(255, 222, 0, 255));
            ImNodes::PushColorStyle(ImNodesCol_LinkHovered, IM_COL32(255, 255, 0, 255));
            ImNodes::PushColorStyle(ImNodesCol_LinkSelected, IM_COL32(255, 255, 0, 255));
        }
        ImNodes::Link(i, p.first, p.second);
        if (slots[p.first]->isRoute) {
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
        }
    }

    

    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_TopRight);
    ImNodes::EndNodeEditor();

    

    int start_attr, end_attr;
    if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
    {
        if (ValidLink(start_attr, end_attr)) {
            links.push_back(std::make_pair(start_attr, end_attr));
            slots[start_attr]->connected = slots[end_attr];
            slots[end_attr]->connected = slots[start_attr];
        }
    }
    
    if (ImNodes::IsLinkDestroyed(&start_attr)) {
        //links.erase(start_attr);
    }

    ImGui::End();
}

bool NodeViewer::ValidLink(uint32_t id0, uint32_t id1)
{
    
    if (id0 == id1)
        return false;

    if (slots.find(id0) == slots.end())
        return false;
    if (slots.find(id1) == slots.end())
        return false;

    std::shared_ptr<Slot> s0 = slots[id0];
    std::shared_ptr<Slot> s1 = slots[id1];
    if (!s0)
        return false;
    if (!s1)
        return false;

    if (s0->in == s1->in)
        return false;

    if (s0->type != s1->type) {
        if (!s0->isRoute || !s1->isRoute)
            return false;
    }



    return true;
}

void NodeViewer::RegisterNodeBase(NodeBase base)
{

}

void NodeViewer::RegisterSlot(NodeBase* n, std::string n_name, std::string n_type, bool n_in, bool route)
{
    std::shared_ptr<Slot> s = std::make_shared<Slot>(GetUniqueID(), n_name, n_type, n, n_in, route);
    slots[s->m_id] = s;

    if (n_in) {
        n->input.push_back(s);

        return;
    }
    n->output.push_back(s);
}

void NodeViewer::Interprete()
{
    if (!startNode) {
        return;
    }
    for (auto& s : slots) {
        s.second->data = "NOTCREATED";
    }

    std::string code = startNode->CreateData();
    Log::Message("Visual Scripting", "Interpreted Code:\n\n" + code);
}

// delegates:
//  branch
std::function<std::string(NodeBase&)> BranchFunc([](NodeBase& n) {

    std::string out = "\nif(" + n.input[1]->CreateData() + "){\n   " + n.output[0]->CreateData() + "\n}";
    out.append("else{\n" + n.output[1]->CreateData() + "}");
    out.append("\n");
    return out;
    });

// bool ops
std::function<std::string(NodeBase&)> GreaterThanFunc([](NodeBase& n) {

    std::string out = '(' + n.input[0]->CreateData() + ") > (" + n.input[1]->CreateData() + ')';
    n.output[0]->data = out;
    return out;
    });

std::function<std::string(NodeBase&)> LessThanFunc([](NodeBase& n) {

    std::string out = '(' + n.input[0]->CreateData() + ") < (" + n.input[1]->CreateData() + ')';
    n.output[0]->data = out;
    return out;
    });

std::function<std::string(NodeBase&)> EqualFunc([](NodeBase& n) {

    std::string out = '(' + n.input[0]->CreateData() + ") == (" + n.input[1]->CreateData() + ')';
    n.output[0]->data = out;
    return out;
    });

std::function<std::string(NodeBase&)> NotEqualFunc([](NodeBase& n) {

    std::string out = '(' + n.input[0]->CreateData() + ") != (" + n.input[1]->CreateData() + ')';
    n.output[0]->data = out;
    return out;
    });

std::function<std::string(NodeBase&)> GreaterEqualThanFunc([](NodeBase& n) {

    std::string out = '(' + n.input[0]->CreateData() + ") >= (" + n.input[1]->CreateData() + ')';
    n.output[0]->data = out;
    return out;
    });

std::function<std::string(NodeBase&)> LessEqualThanFunc([](NodeBase& n) {

    std::string out = '(' + n.input[0]->CreateData() + ") <= (" + n.input[1]->CreateData() + ')';
    n.output[0]->data = out;
    return out;
    });

std::function<std::string(NodeBase&)> ANDFunc([](NodeBase& n) {

    std::string out = '(' + n.input[0]->CreateData() + ") && (" + n.input[1]->CreateData() + ')';
    n.output[0]->data = out;
    return out;
    });

std::function<std::string(NodeBase&)> ORFunc([](NodeBase& n) {

    std::string out = '(' + n.input[0]->CreateData() + ") || (" + n.input[1]->CreateData() + ')';
    n.output[0]->data = out;
    return out;
    });

std::function<std::string(NodeBase&)> NOTFunc([](NodeBase& n) {

    std::string out = "!(" + n.input[0]->CreateData() + ')';
    n.output[0]->data = out;
    return out;
    });

//  constants


void NodeViewer::Branch()
{
    std::shared_ptr<NodeBase> node = std::make_shared<NodeBase>(GetUniqueID(), BranchFunc, "Branch");

    // in
    RegisterSlot(node.get(), "RouteIn", "Route", true, true);
    RegisterSlot(node.get(), "Input",  "bool", true, false);
    
    // out
    RegisterSlot(node.get(), "True", "Route", false, true);
    RegisterSlot(node.get(), "False", "Route", false, true);

    nodes.push_back(node);
}

void NodeViewer::BoolOperator(BoolOperatorTypes type)
{
    std::shared_ptr<NodeBase> node;
    switch (type)
    {
    case BoolOperatorTypes::Greater:
        node = std::make_shared<NodeBase>(GetUniqueID(), GreaterThanFunc, ">");
        break;
    case BoolOperatorTypes::Less:
        node = std::make_shared<NodeBase>(GetUniqueID(), LessThanFunc, "<");
        break;
    case BoolOperatorTypes::GreaterEqual:
        node = std::make_shared<NodeBase>(GetUniqueID(), GreaterEqualThanFunc, ">=");
        break;
    case BoolOperatorTypes::LessEqual:
        node = std::make_shared<NodeBase>(GetUniqueID(), LessEqualThanFunc, "<=");
        break;
    case BoolOperatorTypes::Equal:
        node = std::make_shared<NodeBase>(GetUniqueID(), EqualFunc, "==");
        break;
    case BoolOperatorTypes::NotEqual:
        node = std::make_shared<NodeBase>(GetUniqueID(), NotEqualFunc, "!=");
        break;
    case BoolOperatorTypes::AND:
        node = std::make_shared<NodeBase>(GetUniqueID(), ANDFunc, "AND");
        break;
    case BoolOperatorTypes::OR:
        node = std::make_shared<NodeBase>(GetUniqueID(), ORFunc, "OR");
        break;
    case BoolOperatorTypes::NOT:
        node = std::make_shared<NodeBase>(GetUniqueID(), NOTFunc, "NOT");
        break;
    default:
        break;
    }
    

    // in
    RegisterSlot(node.get(), "Left", "bool", true, false);
    RegisterSlot(node.get(), "Right", "bool", true, false);

    // out
    RegisterSlot(node.get(), "Out", "bool", false, false);

    nodes.push_back(node);
}

void NodeBase::Show()
{
    ImNodes::BeginNode(m_id);

    ImNodes::BeginNodeTitleBar();
    ImGui::Text(m_name.c_str());
    ImNodes::EndNodeTitleBar();

    if (input.size() > 0) {
        if (input[0]->isRoute) {
            ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(255, 200, 0, 255));
            ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(255, 255, 0, 255));
        }
        ImNodes::BeginInputAttribute(input[0]->m_id);
        ImGui::Text(input[0]->name.c_str());
        ImNodes::EndInputAttribute();
        if (input[0]->isRoute) {
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
        }
    }

    for (auto& out : output)
    {
        if (out->isRoute) {
            ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(255, 200, 0, 255));
            ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(255, 255, 0, 255));
        }
        ImNodes::BeginOutputAttribute(out->m_id);
        ImGui::Text(out->name.c_str());
        ImNodes::EndOutputAttribute();
        if (out->isRoute) {
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
        }
    }

    bool first = true;
    for (auto& in : input)
    {
        if (first) {
            first = false;
            continue;
        }
        
        if (in->isRoute) {
            ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(255, 200, 0, 255));
            ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(255, 255, 0, 255));
        }
        
        ImNodes::BeginInputAttribute(in->m_id);
        ImGui::Text(in->name.c_str());
        ImNodes::EndInputAttribute();
        if (in->isRoute) {
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
        }
    }
    ImNodes::EndNode();
}

std::string NodeBase::CreateData()
{
    if (CreateDataFunc) {
        return CreateDataFunc(*this);
    }
    else {
        Log::Error("Node " + m_name, "No Data Creation Function");
        return "";
    }
}

std::string Slot::CreateData()
{
    if (in) {
        if (isRoute) {
            if (!parent) {
                return "";
            }
            return parent->CreateData();
        }

        if (!connected) {
            return "";
        }
        return connected->CreateData();
    }

    if (isRoute) {
        if (!connected) {
            return "\n   return;\n";
        }
        return connected->CreateData();
    }

    if (!parent) {
        return "";
    }
    if (data == "NOTCREATED") {
        parent->CreateData();
    }
    return data;
}

std::string VarNode::CreateData()
{
    if (get) {
        return m_name;
    }

    output[1]->data = m_name;
    return "type" + ' ' + m_name + " = ";
}

void VarNode::Show()
{

}
