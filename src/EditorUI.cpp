#pragma once
#include "includes.h"
#include "EditorUI.h"
#include "MyCustomizeObjectLayer.h"
#include "ObjectsIds.h"
#include "utils.hpp"


void EditorUI::Callback::toTheFirstLayerButton(CCObject* obj) {
    from<int>(from<gd::EditorUI*>(obj, 0xFC)->getLevelEditorLayer(), 0x12C) = -1;
    from<CCLabelBMFont*>(from<gd::EditorUI*>(obj, 0xFC), 0x20C)->setString("All");
}

void EditorUI::Callback::toTheEmptyLayerButton(CCObject* obj) {
    //std::cout << "obj = " << obj << std::endl;
    //std::cout << "0xfc = " << from<gd::EditorUI*>(obj, 0xFC) << std::endl;

    auto leveleditor = from<gd::EditorUI*>(obj, 0xFC)->getLevelEditorLayer();
    bool foundClear = false;
    bool nextCycle = false;
    int currentLayer = 0;

    while (!foundClear && !nextCycle)
    {
        from<int>(leveleditor, 0x12C) = currentLayer;
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

        if ( leveleditor->getLayerGroup( )==-1 ) 
            nextCycle = true;
    }
    from<int>(from<gd::EditorUI*>(obj, 0xFC)->getLevelEditorLayer(), 0x12C) = currentLayer;
    from<CCLabelBMFont*>(from<gd::EditorUI*>(obj, 0xFC), 0x20C)->setString(std::to_string(currentLayer).c_str());
    //from<CCLabelBMFont*>(obj, 0x20C)->setString("All");
}

CCObject* createWithArray(CCArray *arr, int UndoCommand) {
    return reinterpret_cast< CCObject*(__fastcall *)(CCArray*, int) >(gd::base+0x91ed0)(arr, UndoCommand);
}

void EditorUI::Callback::onDuplicate(CCObject *obj) {
    auto editUI = from<gd::EditorUI *>(obj, 0xFC);
    auto lel = editUI->getLevelEditorLayer( );
    auto preDuplicated = editUI->getSelectedObjectsOfCCArray( );
    auto postDuplicated = CCArray::create( );
    for ( int i = 0; i<preDuplicated->count(); i++ ) {
        auto object = reinterpret_cast< gd::GameObject * >(preDuplicated->objectAtIndex(i));
        gd::GameObject* newObj = lel->createObject(object->getObjectID(), object->getPositionX(), object->getPositionY());
        lel->getUndoList( )->removeLastObject();
        newObj->setGroup(object->getGroup( ));
        newObj->setActiveColor(object->getActiveColor( ));
        newObj->setRotation(object->getRotation( ));
        newObj->setIsPreviewEnabled(object->getIsPreviewEnabled( ));

        newObj->setTriggerColor(object->getTriggerColor( ));
        newObj->setP1color(object->getP1color( ));
        newObj->setP2color(object->getP2color( ));
        newObj->setBlending(object->getBlending( ));
        newObj->setTouchTriggered(object->getTouchTriggered( ));
        newObj->setTintGround(object->getTintGround( ));

        postDuplicated->addObject(newObj);
        auto dgl = lel->getDrawGridLayer( );
        if ( newObj->getIsPreviewEnabled() && gamemodePortals.contains(newObj->getObjectID( )) ) {
            dgl->getGuides( )->addObject(newObj);
        }
        if ( newObj->getIsPreviewEnabled( ) && speedPortals.contains(newObj->getObjectID( )) ) {
            dgl->getSpeedObjects()->addObject(newObj);
        }
        if ( object->getObjectID( )==31 ) {
            reinterpret_cast< gd::StartPosObject * >(newObj)->setLevelSettingsObject(reinterpret_cast< gd::StartPosObject * >(object)->getLevelSettingsObject());
        }
    }
    auto undoObj = createWithArray(postDuplicated, 3);
    lel->getUndoList( )->addObject(undoObj);
    editUI->deselectAll( );
    editUI->selectObjects(postDuplicated);
    editUI->updateButtons( );

    for ( int i = 0; i<postDuplicated->count( ); i++ ) {
        reinterpret_cast< gd::GameObject * >(postDuplicated->objectAtIndex(i))->setColor({ 0, 255, 255 });
        if ( reinterpret_cast< gd::GameObject * >(postDuplicated->objectAtIndex(i))->getChildSprite( ) )
            reinterpret_cast< gd::GameObject * >(postDuplicated->objectAtIndex(i))->getChildSprite( )->setColor({ 0, 255, 255 });
    }
}

void EditorUI::Callback::onDebugCopyObjAddress(CCObject *obj) {
    auto editUI = from<gd::EditorUI *>(obj, 0xFC);
    std::stringstream stream;
    stream<<std::uppercase<<std::hex<<reinterpret_cast< uintptr_t >(editUI->getSingleSelectedObj( ));
    set_clipboard_text(stream.str());
}