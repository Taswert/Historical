#include <cocos2d.h>
#include <imgui-hook.hpp>
#include <filesystem>
#include "utils.hpp"
//#include "includes.h"
//#include "base_nodes/CCNode.h"
//#include "gd.h"
using namespace cocos2d;

static CCNode* selected_node = nullptr;
static bool reached_selected_node;

auto name_for_node(const CCNode* node) {
    return typeid(*node).name() + 6;
    // return "hon hon hon";
}

void render_node_tree(CCNode* node, const size_t index) {
    if (!node) return;
    auto flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
    if (selected_node == node) {
        flags |= ImGuiTreeNodeFlags_Selected;
        reached_selected_node = true;
    }
    if ( node->getChildrenCount()==0 )
        flags |= ImGuiTreeNodeFlags_Leaf;
    const bool open = ImGui::TreeNodeEx(node, flags, "[%d] %s", index, name_for_node(node));
    if (ImGui::IsItemClicked()) {
        selected_node = node;
        reached_selected_node = true;
    }
    if (open) {
        const auto children = node->getChildren();
        // for some reason its nullptr when empty lol
        if (children) {
            for (size_t i = 0; i < children->count(); ++i) {
                render_node_tree(static_cast<CCNode*>(children->objectAtIndex(i)), i);
            }
        }
        ImGui::TreePop();
    }
}

std::string get_module_name(HMODULE mod) {
    char buffer[MAX_PATH];
    if (!mod || !GetModuleFileNameA(mod, buffer, MAX_PATH))
        return "Unknown";
    return std::filesystem::path(buffer).filename().string();
}

auto format_addr(void* addr) {
    HMODULE mod;

    if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<char*>(addr), &mod))
        mod = NULL;

    return ::format("{}.{}", get_module_name(mod),
        reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(addr) - reinterpret_cast<uintptr_t>(mod)));
}


void render_node_properties(CCNode* node) {
    ImGui::Text("Addr: %p", node);
    ImGui::SameLine( );
    if ( ImGui::Button("Copy##copyaddr") ) {
        std::stringstream stream;
        stream<<std::uppercase<<std::hex<<reinterpret_cast< uintptr_t >(node);
        set_clipboard_text(stream.str( ));
    }
    ImGui::Text("Tag: %d", node->getTag());
    {
        auto value = node->getPosition();
        ImGui::DragFloat2("Position", (float*)&value);
        if (value != node->getPosition()) node->setPosition(value);
    }
    if (ImGui::TreeNode("Advanced Position PRO")) {
        if (node->getParent()) {
            const auto pos = node->getParent()->convertToWorldSpace(node->getPosition());
            const auto win_size = CCDirector::sharedDirector()->getWinSize();
            ImGui::Text("Window size: %.2fx%.2f", win_size.width, win_size.height);
            ImGui::Text("World space: %.2f, %.2f", pos.x, pos.y);
        }
        ImGui::TreePop();
    }
    {
        auto value = node->getScale();
        ImGui::DragFloat("Scale", &value, 0.05f);
        if (value != node->getScale()) node->setScale(value);
    }
    {
        auto value = node->getRotation();
        ImGui::DragFloat("Rotation", &value, 0.05f);
        if ( value!=node->getRotation() ) node->setRotation(value);
    }
    if (auto item = dynamic_cast<CCMenuItemSprite*>(node)) {
        ImGui::Text("CCMenuItem selector: %s", format_addr(union_cast<void*>(item->getSelector())).c_str());
    }
    {
        auto value = node->isVisible();
        ImGui::Checkbox("Visible", &value);
        if (value != node->isVisible()) node->setVisible(value);
    }
    {
        auto value = node->getZOrder();
        ImGui::InputInt("Z Order", &value);
        if (value != node->getZOrder()) node->setZOrder(value);
    }

    auto rgba = dynamic_cast< CCRGBAProtocol * >(node);
    if ( rgba ) {
        const auto color = rgba->getColor( );
        float colors[4] = { color.r / 255.f, color.g / 255.f, color.b / 255.f, rgba->getOpacity() / 255.f};
        if ( ImGui::ColorEdit4("Color", colors) ) {
            rgba->setColor(ccc3(colors[0]*255.f, colors[1]*255.f, colors[2]*255.f));
            rgba->setOpacity(static_cast< GLubyte >(colors[3] * 255.f));
        }
    }
}

void render_explorer_window(bool& open) {
    ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Cocos explorer", &open)) {
        reached_selected_node = false;
        auto node = CCDirector::sharedDirector()->getRunningScene();

        const auto avail = ImGui::GetContentRegionAvail();

        ImGui::BeginChild("explorer.tree", ImVec2(avail.x * 0.5f, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        render_node_tree(node, 0);
        ImGui::EndChild();

        if (!reached_selected_node)
            selected_node = nullptr;

        ImGui::SameLine();

        ImGui::BeginChild("explorer.options");

        if (selected_node)
            render_node_properties(selected_node);
        else
            ImGui::Text("Select a node to edit its properties :-)");
        ImGui::EndChild();
    }
    ImGui::End();
}