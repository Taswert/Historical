#include "includes.h"
#include "EditorUI.h"
#include "MyCustomizeObjectLayer.h"

void EditorUI::Callback::toTheFirstLayerButton(CCObject* obj) {
    //int currentLayer = from<int>(from<void*>(obj, 0x22C), 0x1F4);
    //std::string currentLayerString = from<std::string>(from<void*>(obj, 0x20C), 0x100);
    //from<std::string>(from<void*>(obj, 0x20C), 0x100) = "All";
    //from<int>(from<void*>(obj, 0x22C), 0x1F4) = 2;
    //std::cout << "obj = " << obj << std::endl;
    //std::cout << "0xfc = " << from<gd::EditorUI*>(obj, 0xFC) << std::endl;

    from<int>(from<gd::EditorUI*>(obj, 0xFC)->getLevelEditorLayer(), 0x12C) = -1;
    from<CCLabelBMFont*>(from<gd::EditorUI*>(obj, 0xFC), 0x20C)->setString("All");
    //from<CCLabelBMFont*>(obj, 0x20C)->setString("All");
}

void EditorUI::Callback::toTheEmptyLayerButton(CCObject* obj) {
    //std::cout << "obj = " << obj << std::endl;
    //std::cout << "0xfc = " << from<gd::EditorUI*>(obj, 0xFC) << std::endl;

    auto leveleditor = from<gd::EditorUI*>(obj, 0xFC)->getLevelEditorLayer();
    bool foundClear = false;
    int currentLayer = 0;

    while (!foundClear)
    {
        from<int>(from<gd::EditorUI*>(obj, 0xFC)->getLevelEditorLayer(), 0x12C) = currentLayer;
        auto objs = CCArray::create();
        for (int i = 0; i < (leveleditor->getAllObjects()->count()); i++)
        {
            objs->addObjectsFromArray(reinterpret_cast<CCArray*>(leveleditor->getAllObjects()->objectAtIndex(i)));
        }

        auto objs2 = CCArray::create();
        for (int i = 0; i < (objs->count()); i++)
        {
            if (reinterpret_cast<gd::GameObject*>(objs->objectAtIndex(i))->getGroup() == leveleditor->getLayerGroup() || leveleditor->getLayerGroup() == -1)
                objs2->addObject(objs->objectAtIndex(i));
        }

        if (objs2->count() > 0) currentLayer++;
        else foundClear = true;
    }
    from<int>(from<gd::EditorUI*>(obj, 0xFC)->getLevelEditorLayer(), 0x12C) = currentLayer;
    from<CCLabelBMFont*>(from<gd::EditorUI*>(obj, 0xFC), 0x20C)->setString(std::to_string(currentLayer).c_str());
    //from<CCLabelBMFont*>(obj, 0x20C)->setString("All");
}

