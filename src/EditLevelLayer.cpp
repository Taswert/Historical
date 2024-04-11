#pragma once
#include "includes.h"
#include "state.h"
#include "EditLevelLayer.h"

CCObject* objlvl;
CCArray* locallvlarr;
gd::LocalLevelManager* lvlmngr;
gd::EditLevelLayer* editlayer;

//GJ_arrow_03_001.png

void EditLevelLayer::Callback::moveUpButton(CCObject*)
{
    auto lpos = editlayer->getChildByTag(4570);
    auto l3 = editlayer->getChildByTag(4571);
    auto pos = locallvlarr->indexOfObject(objlvl);
    auto menu2 = editlayer->getChildByTag(4573);
    auto moveUpButton = menu2->getChildByTag(1111);
    auto moveDownButton = menu2->getChildByTag(1112);
    if (pos > 0)
    {
        pos--;
        locallvlarr->removeObject(objlvl, true);
        locallvlarr->insertObject(objlvl, pos);
        lvlmngr->updateLevelOrder();
        reinterpret_cast<CCLabelBMFont*>(lpos)->setString(CCString::createWithFormat("%d", pos + 1)->getCString());
        if (l3 != nullptr) reinterpret_cast<CCLabelBMFont*>(l3)->setString(CCString::createWithFormat("lvl position: %d", pos)->getCString());
        reinterpret_cast<CCNodeRGBA*>(moveDownButton)->setColor({255, 255, 255});
        if (pos == 0) reinterpret_cast<CCNodeRGBA*>(moveUpButton)->setColor({120, 120, 120}); 
    }
}

void EditLevelLayer::Callback::moveDownButton(CCObject*)
{
    auto lpos = editlayer->getChildByTag(4570);
    auto l3 = editlayer->getChildByTag(4571);
    auto pos = locallvlarr->indexOfObject(objlvl);
    auto menu2 = editlayer->getChildByTag(4573);
    auto moveUpButton = menu2->getChildByTag(1111);
    auto moveDownButton = menu2->getChildByTag(1112);
    if (pos < (locallvlarr->count() - 1))
    {
        pos++;
        locallvlarr->removeObject(objlvl, true);
        locallvlarr->insertObject(objlvl, pos);
        lvlmngr->updateLevelOrder();
        reinterpret_cast<CCLabelBMFont*>(lpos)->setString(CCString::createWithFormat("%d", pos + 1)->getCString());
        if (l3 != nullptr) reinterpret_cast<CCLabelBMFont*>(l3)->setString(CCString::createWithFormat("lvl position: %d", pos)->getCString());
        reinterpret_cast<CCNodeRGBA*>(moveUpButton)->setColor({ 255, 255, 255 });
        if (pos == locallvlarr->count() - 1) reinterpret_cast<CCNodeRGBA*>(moveDownButton)->setColor({ 120, 120, 120 });
    }
}

void EditLevelLayer::Callback::moveToTopButton(CCObject*)
{
    auto lpos = editlayer->getChildByTag(4570);
    auto l3 = editlayer->getChildByTag(4571);
    auto menu2 = editlayer->getChildByTag(4573);
    auto moveUpButton = menu2->getChildByTag(1111);
    auto moveDownButton = menu2->getChildByTag(1112);
    reinterpret_cast<CCLabelBMFont*>(lpos)->setString(CCString::createWithFormat("%d", 1)->getCString());
    if (l3 != nullptr) reinterpret_cast<CCLabelBMFont*>(l3)->setString(CCString::createWithFormat("lvl position: %d", 0)->getCString());
    reinterpret_cast<CCNodeRGBA*>(moveUpButton)->setColor({ 120, 120, 120 });
    locallvlarr->removeObject(objlvl, true);
    locallvlarr->insertObject(objlvl, 0);
    if ((locallvlarr->indexOfObject(objlvl)) < (locallvlarr->count() - 1)) reinterpret_cast<CCNodeRGBA*>(moveDownButton)->setColor({ 255, 255, 255 });
    lvlmngr->updateLevelOrder();
}

bool __fastcall EditLevelLayer::EditLevelLayer_init_H(gd::EditLevelLayer* self, void*, gd::GJGameLevel* gjlvl)
{
    if (!EditLevelLayer::EditLevelLayer_init(self, gjlvl)) return false;

    editlayer = self;
    
    lvlmngr = gd::LocalLevelManager::sharedState();
    locallvlarr = lvlmngr->getLocalevels();
    objlvl = (CCObject*)gjlvl;

    auto pos = locallvlarr->indexOfObject(objlvl);
    auto director = CCDirector::sharedDirector();
    auto scrnRight = director->getScreenRight();
    auto menu = CCMenu::create();
    auto menu2 = CCMenu::create();
    auto moveToTopSprite = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
    auto moveToTopButton = gd::CCMenuItemSpriteExtra::create(moveToTopSprite, nullptr, self, menu_selector(EditLevelLayer::Callback::moveToTopButton));

    auto arrowUpSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    arrowUpSprite->setFlipY(1);
    auto arrowDownSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    auto moveUpButton = gd::CCMenuItemSpriteExtra::create(arrowUpSprite, nullptr, self, menu_selector(EditLevelLayer::Callback::moveUpButton));
    auto moveDownButton = gd::CCMenuItemSpriteExtra::create(arrowDownSprite, nullptr, self, menu_selector(EditLevelLayer::Callback::moveDownButton));
    moveUpButton->setTag(1111);
    moveDownButton->setTag(1112);

    if (pos == 0) reinterpret_cast<CCNodeRGBA*>(moveUpButton)->setColor({ 120, 120, 120 });
    if (pos == locallvlarr->count() - 1) reinterpret_cast<CCNodeRGBA*>(moveDownButton)->setColor({ 120, 120, 120 });

    menu->setPosition({ scrnRight-30, 120 });
    menu->addChild(moveToTopButton);

    menu2->setPosition(75, 265);
    menu2->addChild(moveUpButton);
    menu2->addChild(moveDownButton);
    menu2->setTag(4573);

    /*moveToTopButton->setPosition(scrnRight - 30, 120);*/
    moveUpButton->setPosition(0, 30);
    moveUpButton->setRotation(90);
    /*moveUpButton->setOpacity(120);*/
    /*moveUpButton-> FLIP IDK*/
    moveDownButton->setPosition(0, -35);
    moveDownButton->setRotation(-90);

    self->addChild(menu);
    self->addChild(menu2);

    if (setting().onDebugLabels)
    {
        auto l1 = CCLabelBMFont::create("", "chatFont.fnt");
        l1->setString(CCString::createWithFormat("ell address: %p", self)->getCString());
        l1->setAnchorPoint({ 1.f, 0.5f });
        l1->setPosition(100, 100);
        l1->setScale(0.5f);
        self->addChild(l1);

        auto l2 = CCLabelBMFont::create("", "chatFont.fnt");
        l2->setString(CCString::createWithFormat("mng address: %p", lvlmngr)->getCString());
        l2->setAnchorPoint({ 1.f, 0.5f });
        l2->setPosition(100, 90);
        l2->setScale(0.5f);
        self->addChild(l2);

        auto l3 = CCLabelBMFont::create("", "chatFont.fnt");
        l3->setString(CCString::createWithFormat("lvl position: %d", locallvlarr->indexOfObject(objlvl))->getCString());
        l3->setAnchorPoint({ 1.f, 0.5f });
        l3->setPosition(100, 80);
        l3->setScale(0.5f);
        l3->setTag(4571);
        self->addChild(l3);
    }

    auto lpos = CCLabelBMFont::create("", "bigFont.fnt");
    lpos->setString(CCString::createWithFormat("%d", locallvlarr->indexOfObject(objlvl)+1)->getCString());
    lpos->setAnchorPoint({ 0.5f, 0.5f });
    lpos->setPosition(75, 265);
    lpos->setScale(1.f);
    lpos->setTag(4570);
    self->addChild(lpos);

    return true;
}

/*------
TAGS:
--------
lpos = 4570 (position label)
l3 = 4571 (position debug label)
menu2 = 4573
    moveUpButton = 1111
    moveDownButton = 1112
...
------*/