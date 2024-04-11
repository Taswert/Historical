#include "includes.h"
#include "PlayLayer.h"
#include "state.h"

bool __fastcall PlayLayer::init_H(gd::PlayLayer* self, void* edx, gd::GJGameLevel* level)
{
    if (!PlayLayer::init(self, level)) return false;
    auto gm = gd::GameManager::sharedState();
    auto size = CCDirector::sharedDirector()->getWinSize();
    if (setting().onShowPercents)
    {
        const auto bar = gm->getShowProgressBar();

        auto percentsLabel = CCLabelBMFont::create("0.00%", "bigFont.fnt");
        //percentsLabel->autorelease();
        //percentsLabel->setAnchorPoint({ bar ? 0.f : 0.5f, 0.5f });
        percentsLabel->setScale(0.5f);
        percentsLabel->setZOrder(5);
        percentsLabel->setPosition({ size.width / 2.f + (bar ? 107.2f : 0.f), size.height - 8.f });
        percentsLabel->setTag(4571);
        self->addChild(percentsLabel);
    }
    ///if (setting().onCheatIndicator)
    //{
        auto CheatIndicatorLabel = CCLabelBMFont::create("Cheats ON", "bigFont.fnt");

        CheatIndicatorLabel->setScale(1.f);
        CheatIndicatorLabel->setZOrder(6);
        CheatIndicatorLabel->setPosition({ size.width / 2.f, size.height - 8.f });
        CheatIndicatorLabel->setAnchorPoint({0.5f, 0.5f});
        //CheatIndicatorSprite->setColor({255, 0, 0});
        self->addChild(CheatIndicatorLabel);
    //}
    return true;
}

void __fastcall PlayLayer::update_H(gd::PlayLayer* self, void*, float dt)
{

    PlayLayer::update(self, dt);
    auto percentsLabel = self->getChildByTag(4571);
    if (percentsLabel) {
        const auto value = self->player1()->getPositionX() / self->levelLength() * 100.f;
        if (value < 100.0f) {
            reinterpret_cast<CCLabelBMFont*>(percentsLabel)->setString(CCString::createWithFormat("%.2f%%", value)->getCString());
        }
        else
        {
            reinterpret_cast<CCLabelBMFont*>(percentsLabel)->setString(CCString::create("100.00%")->getCString());
        }
    }
}