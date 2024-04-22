#pragma once
#include "includes.h"

#include "PauseLayer.h"
#include "LevelInfoLayer.h"
#include "EditorPauseLayer.h"
#include "EditLevelLayer.h"
#include "InfoLayer.h"
#include "CreatorLayer.h"
#include "EditorUI.h"
#include "CustomizeObjectLayer.h"
#include "MyCustomizeObjectLayer.h"

#include <imgui-hook.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "menu.h"
#include "state.h"
#include "utils.hpp"
#include <fstream>
#include <fcntl.h>
#include <io.h>
#include "ObjectsIds.h"

#include "imgui-hook.hpp"
//#include "HitboxNode.h"

;
std::time_t t;
SYSTEMTIME st;

float NoclipAcc = 0.f;

bool isPlayerColliding = false;
bool isPlayerDead = false;
bool isPlayerColorGot = false;

bool isPressed = false;

int deaths = 0;
bool wasDead = 0;

float deathPos = 0.f;
float deathDifference = 0.f;


int currentStartPos = 0;
bool fadedoutflag = 0;

ccColor3B playerColor1;
ccColor3B playerColor2;
ccColor3B playerColorG;
//CCLayer* PauseLayerObject;
CCLayer* PlayLayerObject;
CCArray* startPosArr;
gd::EditorUI* editUI;

std::string savedClipboard;

DWORD cocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");

void drawRect(CCDrawNode* drawer, CCRect const& rect, ccColor4F col) {
    constexpr size_t N = 4;
    CCPoint vert[N];

    vert[0] = CCPointMake(rect.getMinX(), rect.getMinY());
    vert[1] = CCPointMake(rect.getMinX(), rect.getMaxY());
    vert[2] = CCPointMake(rect.getMaxX(), rect.getMaxY());
    vert[3] = CCPointMake(rect.getMaxX(), rect.getMinY());

    auto colorA = ccc4FFromccc4B(ccc4(0, 0, 0, 0));

    drawer->drawPolygon(vert, N, colorA, 0.5, col);
}

void drawTriangleObj(CCDrawNode* drawer, gd::GameObject* ob, ccColor4F col) {

    ob->updateOrientedBox();
    if (!ob->getOrientedBox()) return;

    CCPoint triangle[3];
    auto isFlippedX = ob->getIsFlippedX();
    auto isFlippedY = ob->getIsFlippedY();
    auto point1 = ob->getOrientedBox()->getPoint1();
    auto point2 = ob->getOrientedBox()->getPoint2();
    auto point3 = ob->getOrientedBox()->getPoint3();
    auto point4 = ob->getOrientedBox()->getPoint4();

    if (!isFlippedX && !isFlippedY) {
        triangle[0] = point1;
        triangle[1] = point2;
        triangle[2] = point3;
    }
    else if (isFlippedX && !isFlippedY) {
        triangle[0] = point2;
        triangle[1] = point3;
        triangle[2] = point4;
    }
    else if (!isFlippedX && isFlippedY) {
        triangle[0] = point1;
        triangle[1] = point2;
        triangle[2] = point4;
    }
    else if (isFlippedX && isFlippedY) {
        triangle[0] = point1;
        triangle[1] = point3;
        triangle[2] = point4;
    }

    drawer->drawPolygon(triangle, 3, { 0, 0, 0, 0 }, 0.5, col);
}

void drawRectObj(CCDrawNode* drawer, gd::GameObject* ob, ccColor4F col) {
    ob->updateOrientedBox();
    if (!ob->getOrientedBox()) return;

    CCPoint vert[4];
    vert[0] = ob->getOrientedBox()->getPoint1();
    vert[1] = ob->getOrientedBox()->getPoint2();
    vert[2] = ob->getOrientedBox()->getPoint3();
    vert[3] = ob->getOrientedBox()->getPoint4();

    drawer->drawPolygon(vert, 4, { 0, 0, 0, 0 }, 0.5, col);
}

void drawCircleObj(CCDrawNode* drawer, gd::GameObject* ob, ccColor4F col) {
    ob->updateOrientedBox();
    if (!ob->getOrientedBox()) return;
    auto rad = ob->getObjectRadius();

    constexpr size_t N = 64;
    constexpr float PI = 3.14159265;

    CCPoint vert[N];
    for (size_t i = 0; i < N; ++i) {
        vert[i] = ob->getOrientedBox()->getCenterPoint() + CCPointMake(std::cos(2 * PI * i / N), std::sin(2 * PI * i / N)) * rad;
    }
    drawer->drawPolygon(vert, N, { 0, 0, 0, 0 }, 0.5, col);
}

void drawPlayerHitbox(gd::PlayerObject* player, CCDrawNode* drawNode)
{
    CCPoint pointRectangle[4];
    CCRect rectRectangle;
    CCRect rectRectangleSmall;
    pointRectangle[0] = player->getOrientedBox()->getPoint1();
    pointRectangle[1] = player->getOrientedBox()->getPoint2();
    pointRectangle[2] = player->getOrientedBox()->getPoint3();
    pointRectangle[3] = player->getOrientedBox()->getPoint4();

    auto p1x = pointRectangle[0].x - pointRectangle[1].x;
    auto p1y = pointRectangle[0].y - pointRectangle[1].y;
    auto distance1 = sqrt(p1x * p1x + p1y * p1y);

    auto p2x = pointRectangle[1].x - pointRectangle[2].x;
    auto p2y = pointRectangle[1].y - pointRectangle[2].y;
    auto distance2 = sqrt(p2x * p2x + p2y * p2y);

    auto distanceS1 = distance1 / 4;
    auto distanceS2 = distance2 / 4;

    rectRectangle.setRect(player->getPositionX() - distance1 / 2, player->getPositionY() - distance2 / 2, distance1, distance2);
    rectRectangleSmall.setRect(player->getPositionX() - distanceS1 / 2, player->getPositionY() - distanceS2 / 2, distanceS1, distanceS2);

    drawRect(drawNode, rectRectangleSmall, { 0, 0, 1, 1 });
    drawNode->drawPolygon(pointRectangle, 4, { 0, 0, 0, 0 }, 0.5, { 0.5, 0, 0, 1 });
    drawRect(drawNode, rectRectangle, { 1, 0, 0, 1 });
}

void drawObjectHitbox(gd::GameObject* obj, CCDrawNode* drawNode) {
    if (hazard.contains(obj->getObjectID()))
        drawRectObj(drawNode, obj, { 1, 0, 0, 1 });
    else if (orbsnportals.contains(obj->getObjectID()))
        drawRectObj(drawNode, obj, { 0, 1, 0, 1 });
    else if (solids.contains(obj->getObjectID()))
        drawRectObj(drawNode, obj, { 0, 0, 1, 1 });
    else if (ramps.contains(obj->getObjectID()))
        drawTriangleObj(drawNode, obj, { 0, 0, 1, 1 });
    else if (saws.contains(obj->getObjectID()))
        drawCircleObj(drawNode, obj, { 1, 0, 0, 1 });
}

class ExitAlertProtocol : public gd::FLAlertLayerProtocol {
protected:

    void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override
    {
        if (btn2)
        {
            gd::GameManager::sharedState()->getPlayLayer()->onQuit();
            gd::GameSoundManager::sharedState()->playSound("quitSound_01.ogg");
        }
    }
};
ExitAlertProtocol eaProtocol;

bool(__thiscall* PlayLayer_destroyPlayer)(gd::PlayLayer* self, gd::PlayerObject* player);
void __fastcall PlayLayer_destroyPlayer_H(gd::PlayLayer* self, void*, gd::PlayerObject* player)
{
    isPlayerDead = true;
    if (!wasDead) {
        deaths++;
    }
    else {
        deathDifference += player->getPositionX() - deathPos;
    }
    deathPos = player->getPositionX();
    wasDead = true;
    PlayLayer_destroyPlayer(self, player);
}

bool(__thiscall* PlayLayer_resetLevel)(gd::PlayLayer* self);
void __fastcall PlayLayer_resetLevel_H(gd::PlayLayer* self) {
    setting().beforeRestartCheatsCount = setting().cheatsCount;
    deaths = 0;
    wasDead = 0;
    deathDifference = 0.f;
    deathPos = 0.f;
    PlayLayer_resetLevel(self);
}

bool(__thiscall* CCKeyboardDispatcher_dispatchKeyboardMSG)(cocos2d::CCKeyboardDispatcher* self, int key, bool down);
void __fastcall CCKeyboardDispatcher_dispatchKeyboardMSG_H(CCKeyboardDispatcher* self, void* edx, int key, bool down) {
    if (down) {
        if ((key == 'R') && setting().onRetryBind) {
            auto pl = gd::GameManager::sharedState()->getPlayLayer();
            if (pl) {
                //here, should destroy pause smh
                //pl->removeMeAndCleanup();
                pl->resetLevel();
                if (layers().PauseLayerObject)
                {
                    layers().PauseLayerObject->removeMeAndCleanup();
                    layers().PauseLayerObject = nullptr;
                }
                pl->resume();
                return;
            }
        }
    }
    CCKeyboardDispatcher_dispatchKeyboardMSG(self, key, down);
}

bool(__thiscall* PlayLayer_init)(gd::PlayLayer* self, gd::GJGameLevel* level);
bool __fastcall PlayLayer_init_H(gd::PlayLayer* self, void* edx, gd::GJGameLevel* level)
{
    setting().beforeRestartCheatsCount = setting().cheatsCount;
    PlayLayerObject = self;
    editUI = nullptr;
    layers().PauseLayerObject = nullptr;
    isPlayerColorGot = false;
    fadedoutflag = 0;

    if (!PlayLayer_init(self, level)) return false;

    auto playerDrawNode = CCDrawNode::create();
    playerDrawNode->setZOrder(1000);
    playerDrawNode->setTag(124);
    self->layer()->addChild(playerDrawNode);

    auto objDrawNode = CCDrawNode::create();
    objDrawNode->setZOrder(1000);
    objDrawNode->setTag(125);
    self->layer()->addChild(objDrawNode);

    auto secarr = self->getSections();
    auto objarr1 = self->getObjects();
    auto arrcount = secarr->count();


    if (startPosArr) delete startPosArr;
    auto sposarr = new CCArray;
    auto firstStartPosObj = gd::StartPosObject::create();
    sposarr->addObject(firstStartPosObj);
    startPosArr = sposarr;
    for (int i = 0; i < secarr->count(); i++)
    {
        auto objarr = reinterpret_cast<CCArray*>(secarr->objectAtIndex(i));
        for (int j = 0; j < objarr->count(); j++)
        {
            auto obj = reinterpret_cast<gd::GameObject*>(objarr->objectAtIndex(j));
            if (obj->getObjectID() == 31) {
                startPosArr->addObject(obj);
            }
        }
    }
    currentStartPos = startPosArr->count() - 1;

    if (setting().onStartPosSwitcher)
    {
        auto spswitcher = CCLabelBMFont::create("", "bigFont.fnt");
        spswitcher->setZOrder(5);
        spswitcher->setScale(0.66f);
        spswitcher->setAnchorPoint({ 0.5f, 0.5f });
        spswitcher->setString(CCString::createWithFormat("%d/%d", currentStartPos, startPosArr->count() - 1)->getCString());
        spswitcher->setPosition({ CCDirector::sharedDirector()->getScreenRight() / 2, 15.f });
        spswitcher->setTag(45712);
        if (startPosArr->count() == 1) spswitcher->setVisible(0);
        self->addChild(spswitcher);
    }


    auto size = CCDirector::sharedDirector()->getWinSize();

    if (setting().onObjHitbox)
    {
        for (int i = self->getFirstVisibleSection() + 1; i < self->getLastVisibleSection() - 1; i++)
        {
            if (i < 0) continue;
            if (i >= arrcount) break;
            auto objAtInd = secarr->objectAtIndex(i);
            auto objarr = reinterpret_cast<CCArray*>(objAtInd);

            for (int j = 0; j < objarr->count(); j++)
            {
                auto obj = reinterpret_cast<gd::GameObject*>(objarr->objectAtIndex(j));
                drawObjectHitbox(obj, objDrawNode);
            }
        }
    }

    if (setting().onPlayerHitbox) {
        if (self->player1()) drawPlayerHitbox(self->player1(), playerDrawNode);
        if (self->player2()) drawPlayerHitbox(self->player1(), playerDrawNode);
    }


    if (setting().onShowPercents)
    {
        const auto bar = gd::GameManager::sharedState()->getShowProgressBar();

        auto percentLabel = CCLabelBMFont::create("0.00%", "bigFont.fnt");
        percentLabel->setAnchorPoint({ bar ? 0.f : 0.5f, 0.5f });
        percentLabel->setScale(0.5f);
        percentLabel->setZOrder(5);
        percentLabel->setPosition({ size.width / 2.f + (bar ? 107.2f : 0.f), size.height - 8.f });
        percentLabel->setTag(4571);
        self->addChild(percentLabel);
    }
    int labelCount = 0;

    if (setting().onCheatIndicator)
    {
        labelCount++;
        auto CheatIndicatorLabel = CCLabelBMFont::create(".", "bigFont.fnt");
        CheatIndicatorLabel->setScale(1.0f);
        CheatIndicatorLabel->setZOrder(5);
        CheatIndicatorLabel->setAnchorPoint({ 0.f, 0.f });
        CheatIndicatorLabel->setPosition({ 5.f, size.height - 13.f * labelCount });
        CheatIndicatorLabel->setTag(4572);
        //CheatIndicatorLabel->setAnchorPoint({ 0.5f, 0.5f });
        ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EE9DA), &setting().CurrentNoclipByte, 1, 0);
        if (setting().NoclipByte != setting().CurrentNoclipByte && setting().onNoclipOutOfMe == false) { setting().cheatsCount++; setting().beforeRestartCheatsCount++; setting().onNoclipOutOfMe = true; }
        else if (setting().NoclipByte == setting().CurrentNoclipByte && setting().onNoclipOutOfMe == true) { setting().cheatsCount--; setting().onNoclipOutOfMe = false; }
        if (setting().cheatsCount == 0 && !setting().onSafeMode) CheatIndicatorLabel->setColor({ 0, 255, 0 });
        else if (setting().cheatsCount == 0 && setting().onSafeMode) CheatIndicatorLabel->setColor({ 255, 255, 0 });
        else CheatIndicatorLabel->setColor({ 255, 0, 0 });

        //CheatIndicatorLabel->
        self->addChild(CheatIndicatorLabel);
    }

    if (setting().onMessage)
    {
        labelCount++;
        auto messageLabel = CCLabelBMFont::create(setting().message, "bigFont.fnt");
        messageLabel->setScale(0.40f);
        messageLabel->setZOrder(5);
        messageLabel->setAnchorPoint({ 0.f, 0.f });
        messageLabel->setOpacity(128);
        messageLabel->setTag(4577);
        messageLabel->setPosition({ 5.f, size.height - 13.f * labelCount });
        self->addChild(messageLabel);
    }

    if (setting().onFPSShow)
    {
        labelCount++;
        //int fps = 1 / CCDirector::sharedDirector()->getAnimationInterval();

        float fps = ImGui::GetIO().Framerate;
        auto FPSLabel = CCLabelBMFont::create("FPS: 0", "bigFont.fnt");
        FPSLabel->setString(CCString::createWithFormat("FPS: %.f", fps)->getCString());
        FPSLabel->setScale(0.40f);
        FPSLabel->setZOrder(5);
        FPSLabel->setAnchorPoint({ 0.f, 0.f });
        FPSLabel->setOpacity(128);
        FPSLabel->setTag(4576);
        FPSLabel->setPosition({ 5.f, size.height - 13.f * labelCount });
        self->addChild(FPSLabel);
    }

    if (setting().onGlobalTime)
    {
        labelCount++;
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream s;
        s << std::put_time(&tm, "%H:%M:%S");

        auto GlobalTimeLabel = CCLabelBMFont::create(":", "bigFont.fnt");
        GlobalTimeLabel->setString(CCString::create(s.str().c_str())->getCString());
        GlobalTimeLabel->setScale(0.40f);
        GlobalTimeLabel->setZOrder(5);
        GlobalTimeLabel->setAnchorPoint({ 0.f, 0.f });
        GlobalTimeLabel->setOpacity(128);
        GlobalTimeLabel->setTag(4575);
        GlobalTimeLabel->setPosition({ 5.f, size.height - 13.f * labelCount });
        self->addChild(GlobalTimeLabel);
    }

    if (setting().onClkTimer)
    {
        labelCount++;
        auto clklbl = CCLabelBMFont::create("Time: ", "bigFont.fnt");
        clklbl->setString(CCString::createWithFormat("Time: %f", self->getClkTimer())->getCString());
        clklbl->setScale(0.40f);
        clklbl->setZOrder(5);
        clklbl->setAnchorPoint({ 0.f, 0.f });
        clklbl->setOpacity(128);
        clklbl->setTag(45714);
        clklbl->setPosition({ 6.f, size.height - 13.f * labelCount });
        self->addChild(clklbl);
    }

    if (setting().onAttemptsLabel)
    {
        labelCount++;
        auto AttemptsLabel = CCLabelBMFont::create("Attempt 1", "bigFont.fnt");
        AttemptsLabel->setScale(0.40f);
        AttemptsLabel->setZOrder(5);
        AttemptsLabel->setAnchorPoint({ 0.f, 0.f });
        AttemptsLabel->setOpacity(128);
        AttemptsLabel->setTag(4573);
        AttemptsLabel->setPosition({ 5.f, size.height - 13.f * labelCount });
        self->addChild(AttemptsLabel);
    }

    if (setting().onJumpsLabel)
    {
        labelCount++;
        auto JumpsLabel = CCLabelBMFont::create("0 Jumps", "bigFont.fnt");
        JumpsLabel->setScale(0.40f);
        JumpsLabel->setZOrder(5);
        JumpsLabel->setAnchorPoint({ 0.f, 0.f });
        JumpsLabel->setOpacity(128);
        JumpsLabel->setTag(4574);
        JumpsLabel->setPosition({ 5.f, size.height - 13.f * labelCount });
        self->addChild(JumpsLabel);
    }

    if (setting().onCollidingLabel)
    {
        labelCount++;
        auto collidinglbl = CCLabelBMFont::create("Is Colliding: 0", "bigFont.fnt");
        collidinglbl->setScale(0.40f);
        collidinglbl->setZOrder(5);
        collidinglbl->setAnchorPoint({ 0.f, 0.f });
        collidinglbl->setOpacity(128);
        collidinglbl->setTag(45711);
        collidinglbl->setPosition({ 5.f, size.height - 13.f * labelCount });
        self->addChild(collidinglbl);
    }

    if (setting().onNoclipDeaths)
    {
        labelCount++;
        auto deathslbl = CCLabelBMFont::create("0 Deaths", "bigFont.fnt");
        deathslbl->setScale(0.40f);
        deathslbl->setZOrder(5);
        deathslbl->setAnchorPoint({ 0.f, 0.f });
        deathslbl->setOpacity(128);
        deathslbl->setTag(45713);
        deathslbl->setPosition({ 5.f, size.height - 13.f * labelCount });
        self->addChild(deathslbl);
    }

    if (setting().onNoclipAcc)
    {
        labelCount++;
        auto noclipAccLabel = CCLabelBMFont::create("100.00%", "bigFont.fnt");
        noclipAccLabel->setScale(0.40f);
        noclipAccLabel->setZOrder(5);
        noclipAccLabel->setAnchorPoint({ 0.f, 0.f });
        noclipAccLabel->setOpacity(128);
        noclipAccLabel->setTag(4578);
        noclipAccLabel->setPosition({ 6.f, size.height - 13.f * labelCount });
        self->addChild(noclipAccLabel);
    }

    labelCount++;
    auto SafeModeLabel = CCLabelBMFont::create("Safe Mode", "bigFont.fnt");
    SafeModeLabel->setScale(0.40f);
    SafeModeLabel->setZOrder(5);
    SafeModeLabel->setAnchorPoint({ 0.f, 0.f });
    SafeModeLabel->setOpacity(128);
    SafeModeLabel->setPosition({ 5.f, size.height - 13.f * labelCount });
    SafeModeLabel->setTag(45710);
    if (!(setting().onSafeMode && setting().onSafeModeLabel)) SafeModeLabel->setVisible(0);
    self->addChild(SafeModeLabel);

    if (setting().onDebugLabels)
    {
        auto adresslbl = CCLabelBMFont::create("adress: ", "bigFont.fnt");
        adresslbl->setScale(0.40f);
        adresslbl->setZOrder(5);
        adresslbl->setAnchorPoint({ 1.f, 0.f });
        adresslbl->setOpacity(128);
        adresslbl->setPosition({ size.width - 10.f, size.height - 26.f });
        adresslbl->setString(CCString::createWithFormat("adress: 0x%p", self)->getCString());
        self->addChild(adresslbl);

        auto arrcountlbl = CCLabelBMFont::create(" ", "bigFont.fnt");
        arrcountlbl->setScale(0.40f);
        arrcountlbl->setZOrder(5);
        arrcountlbl->setAnchorPoint({ 1.f, 0.f });
        arrcountlbl->setOpacity(128);
        arrcountlbl->setPosition({ size.width - 10.f, size.height - 39.f });
        arrcountlbl->setString(CCString::createWithFormat("count: %d", startPosArr->count())->getCString());
        self->addChild(arrcountlbl);
    }

    /*
    const char* test1 = "ayo";
    const char* icon = "checkpoint_01_001.png";
    auto achievement = gd::AchievementBar::create(test1, test1, icon);
    */

    //checkpoint_01_001.png
    return true;
}

inline bool playerTouchesObject(CCRect* playerRect, CCRect* hitboxRect)
{
    float playerMaxX = playerRect->getMaxX();

}


bool rKeyFlag = true;
bool lKeyFlag = true;

bool(__thiscall* PlayLayer_update)(gd::PlayLayer* self, float dt);
void __fastcall PlayLayer_update_H(gd::PlayLayer* self, void*, float dt)
{
    layers().PauseLayerObject = nullptr;
    isPlayerDead = false;

    //PLAYLAYER UPDATE FUNCTION--------------------------
    PlayLayer_update(self, dt);
    if (!isPlayerDead) wasDead = false;

    auto size = CCDirector::sharedDirector()->getWinSize();
    float fps = ImGui::GetIO().Framerate;
    const auto bar = gd::GameManager::sharedState()->getShowProgressBar();
    auto audioScale = self->player1()->getAudioScale() > 1.f ? 1.f : self->player1()->getAudioScale();

    auto percentLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(4571));
    //auto CheatIndicatorLabel = self->getChildByTag(4572);
    auto AttemptsLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(4573));
    auto JumpsLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(4574));
    auto GlobalTimeLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(4575));
    auto FPSLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(4576));
    auto messageLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(4577));
    auto noclipAccLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(4578));
    auto deathLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(4579));
    auto collidinglbl = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(45711));
    auto spswitcherlbl = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(45712));
    auto deathslbl = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(45713));
    auto clklbl = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(45714));


    auto pixelSize = CCDirector::sharedDirector( )->getWinSizeInPixels( );
    SetCursorPos( pixelSize.width/2, pixelSize.height/2);

    //Start Pos Switcher
    auto secarr = self->getSections();
    auto arrcount = secarr->count();
    if (spswitcherlbl)
    {
        auto fadeout = CCSequence::create(CCDelayTime::create(2.f), CCFadeOut::create(0.5f), nullptr);
        if (!fadedoutflag)
        {
            spswitcherlbl->runAction(fadeout);
            fadedoutflag = !fadedoutflag;
        }
        if (GetAsyncKeyState(0x27) && rKeyFlag && startPosArr->count() > 1)
        {
            rKeyFlag = false;
            if (currentStartPos == startPosArr->count() - 1) currentStartPos = 0;
            else currentStartPos++;
            if (currentStartPos != 0) {
                self->setPlayerStartPosition(reinterpret_cast<gd::StartPosObject*>(startPosArr->objectAtIndex(currentStartPos))->getOrientedBox()->getCenterPoint());
                self->setStartPosObject(reinterpret_cast<gd::StartPosObject*>(startPosArr->objectAtIndex(currentStartPos)));
            }
            else {
                self->setPlayerStartPosition({ 0, 105 });
                self->setStartPosObject(nullptr);
            }
            self->resetLevel();
            spswitcherlbl->setString(CCString::createWithFormat("%d/%d", currentStartPos, startPosArr->count() - 1)->getCString());
            spswitcherlbl->stopAllActions();
            spswitcherlbl->setOpacity(255);
            spswitcherlbl->runAction(fadeout);
        }
        else if (!GetAsyncKeyState(0x27)) rKeyFlag = true;

        if (GetAsyncKeyState(0x25) && lKeyFlag && startPosArr->count() > 1)
        {
            lKeyFlag = false;
            if (currentStartPos == 0) currentStartPos = startPosArr->count() - 1;
            else currentStartPos--;
            if (currentStartPos != 0) {
                self->setPlayerStartPosition(reinterpret_cast<gd::StartPosObject*>(startPosArr->objectAtIndex(currentStartPos))->getOrientedBox()->getCenterPoint());
                self->setStartPosObject(reinterpret_cast<gd::StartPosObject*>(startPosArr->objectAtIndex(currentStartPos)));
            }
            else {
                self->setPlayerStartPosition({ 0, 105 });
                self->setStartPosObject(nullptr);
            }
            self->resetLevel();
            spswitcherlbl->setString(CCString::createWithFormat("%d/%d", currentStartPos, startPosArr->count() - 1)->getCString());
            spswitcherlbl->stopAllActions();
            spswitcherlbl->setOpacity(255);
            spswitcherlbl->runAction(fadeout);
        }
        else if (!GetAsyncKeyState(0x25)) lKeyFlag = true;
    }

    //Player Hiboxes
    auto playerDrawNode = reinterpret_cast<CCDrawNode*>(self->layer()->getChildByTag(124));
    playerDrawNode->clear();
    if (setting().onPlayerHitbox || (self->player1()->getIsDead() && setting().onHitboxesOnDeath))
    {
        if (self->player1())
        {
            drawPlayerHitbox(self->player1(), playerDrawNode);
        }
        if (self->player2())
        {
            drawPlayerHitbox(self->player2(), playerDrawNode);
        }
    }

    //Object Hitboxes
    auto objDrawNode = reinterpret_cast<CCDrawNode*>(self->layer()->getChildByTag(125));
    objDrawNode->clear();
    if (setting().onObjHitbox || (self->player1()->getIsDead() && setting().onHitboxesOnDeath))
    {
        for (int i = self->getFirstVisibleSection() + 1; i < self->getLastVisibleSection() - 1; i++)
        {
            if (i < 0) continue;
            if (i >= arrcount) break;
            auto objAtInd = secarr->objectAtIndex(i);
            auto objarr = reinterpret_cast<CCArray*>(objAtInd);

            for (int j = 0; j < objarr->count(); j++)
            {
                auto obj = reinterpret_cast<gd::GameObject*>(objarr->objectAtIndex(j));
                drawObjectHitbox(obj, objDrawNode);
            }
        }
    }


    //Labels
    if (percentLabel) {
        const auto value = self->player1()->getPositionX() / self->levelLength() * 100.f;

        percentLabel->setAnchorPoint({ bar ? 0.f : 0.5f, 0.5f });
        percentLabel->setPosition({ size.width / 2.f + (bar ? 107.2f : 0.f), size.height - 8.f });

        if (value < 100.0f) percentLabel->setString(CCString::createWithFormat("%.2f%%", value)->getCString());
        else percentLabel->setString(CCString::create("100.00%")->getCString());

    }

    if (deathslbl)
    {
        deathslbl->setString(CCString::createWithFormat("%d Deaths", deaths)->getCString());
    }

    NoclipAcc = ((self->player1()->getPositionX() - deathDifference) / self->player1()->getPositionX()) * 100;
    if (noclipAccLabel)
    {
        const auto value = self->player1()->getPositionX() / self->levelLength() * 100.f;
        if (value < 100.0f) reinterpret_cast<CCLabelBMFont*>(noclipAccLabel)->setString(CCString::createWithFormat("%.2f%%", NoclipAcc)->getCString());
    }

    if (FPSLabel)
    {
        FPSLabel->setString(CCString::createWithFormat("FPS: %.f", roundf(fps)/*CCDirector::sharedDirector()->getTotalFrames()*/)->getCString());
    }

    if (JumpsLabel)
    {
        JumpsLabel->setString(CCString::createWithFormat(self->jumpsCount() == 1 ? "%d Jump" : "%d Jumps", self->jumpsCount())->getCString());
    }

    if (GlobalTimeLabel)
    {
        t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream s;
        s << std::put_time(&tm, "%H:%M:%S");
        GlobalTimeLabel->setString(CCString::create(s.str().c_str())->getCString());
    }

    if (clklbl)
    {
        clklbl->setString(CCString::createWithFormat("Time: %f", self->getClkTimer())->getCString());
    }

    if (AttemptsLabel)
    {
        AttemptsLabel->setString(CCString::createWithFormat("Attempt %d", self->attemptsCount())->getCString());
    }

    if (messageLabel)
    {
        messageLabel->setString(CCString::create(setting().message)->getCString());
    }

    if (deathLabel)
    {
        deathLabel->setString(CCString::createWithFormat("Dead - %d", isPlayerDead)->getCString());
    }

    if (collidinglbl)
    {
        collidinglbl->setString(CCString::createWithFormat("Is Colliding: %d", isPlayerDead)->getCString());
    }

    //Player color settings
    if (isPlayerColorGot == false)
    {
        playerColor1 = self->player1()->getFirstColor();
        playerColor2 = self->player1()->getSecondColor();
        playerColorG = self->player1()->getGlowColor();
        isPlayerColorGot = true;
    }
    if (setting().onDualSameCol)
    {
        self->player2()->setColor(self->player1()->getColor());
        self->player2()->setSecondColor(self->player1()->getSecondColor());

        self->player2()->setGlowColor(self->player1()->getGlowColor());
        //self->player2()->setColor3(self->player1()->getColor3());

        self->player2()->setColorSecondVehicle(self->player1()->getColorSecondVehicle());
        //self->player2()->setColor5(self->player1()->getColor5());
        self->player2()->setColorGlowVehicle(self->player1()->getColorGlowVehicle());
        //self->player2()->setColor7(self->player1()->getColor7());

        self->player2()->setWaveTrailColor(self->player1()->getWaveTrailColor());
    }
    else
    {
        self->player2()->setColor(self->player1()->getSecondColor());
        self->player2()->setSecondColor(self->player1()->getColor());

        self->player2()->setGlowColor(self->player1()->getGlowColor());
        //self->player2()->setColor3(self->player1()->getColor3());

        self->player2()->setColorSecondVehicle(self->player1()->getColor());
        //self->player2()->setColor5(self->player1()->getColor5());
        self->player2()->setColorGlowVehicle(self->player1()->getGlowColor());
        //self->player2()->setColor7(self->player1()->getColor7());

        self->player2()->setWaveTrailColor(self->player1()->getSecondColor());
    }

    /*******************************************************************************/
    if (setting().onPrimaryPulse)
    {
        unsigned char
            RR = playerColor1.r + ((setting().PrimaryPulse[0] * 255 - playerColor1.r) * audioScale),
            RG = playerColor1.g + ((setting().PrimaryPulse[1] * 255 - playerColor1.g) * audioScale),
            RB = playerColor1.b + ((setting().PrimaryPulse[2] * 255 - playerColor1.b) * audioScale);
        self->player1()->setColor({ RR, RG, RB });
        //PRIMARY COLOR
    }
    if (setting().onSecondaryPulse)
    {
        unsigned char
            RR = playerColor2.r + ((setting().SecondaryPulse[0] * 255 - playerColor2.r) * audioScale),
            RG = playerColor2.g + ((setting().SecondaryPulse[1] * 255 - playerColor2.g) * audioScale),
            RB = playerColor2.b + ((setting().SecondaryPulse[2] * 255 - playerColor2.b) * audioScale);
        self->player1()->setSecondColor({ RR, RG, RB });
        self->player1()->setColorSecondVehicle({ RR, RG, RB });
        //SECONDARY COLOR
    }

    if (setting().onGlowPulse)
    {
        unsigned char
            RR = playerColorG.r + ((setting().GlowPulse[0] * 255 - playerColorG.r) * audioScale),
            RG = playerColorG.g + ((setting().GlowPulse[1] * 255 - playerColorG.g) * audioScale),
            RB = playerColorG.b + ((setting().GlowPulse[2] * 255 - playerColorG.b) * audioScale);
        self->player1()->setGlowColor({ RR, RG, RB });
        self->player1()->setColorGlowVehicle({ RR, RG, RB });
        //GLOW COLOR
    }

    if (setting().onWaveTrailPulse)
    {
        unsigned char
            RR = playerColor1.r + ((setting().WaveTrailPulse[0] * 255 - playerColor1.r) * audioScale),
            RG = playerColor1.g + ((setting().WaveTrailPulse[1] * 255 - playerColor1.g) * audioScale),
            RB = playerColor1.b + ((setting().WaveTrailPulse[2] * 255 - playerColor1.b) * audioScale);
        self->player1()->setWaveTrailColor({ RR, RG, RB });
        //WAVE TRAIL COLOR
    }

    /******************************************************************************/

    /*if (setting().onGlow) {
        self->player1()->getGlow()->setVisible(1);
        self->player1()->getGlowVehicle()->setVisible(1);
    }*/


    if (setting().onHidePlayer)
    {
        self->player1()->setVisible(0);
        self->player2()->setVisible(0);
    }



}

size_t WriteCallback(char* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

class AdminCheck
{
public:
    void adminCheckCallback(CCObject*)
    {
        std::string udid = (gd::GameManager::sharedState())->getPlayerUDID();

        CURL* curl;
        CURLcode res;
        std::string readBuffer;
        std::string postfield = "deviceId=" + udid;
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "http://85.209.2.73:25568/AdminPanel/CheckAdmin");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfield);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();

        if (readBuffer == "1")
        {
            gd::FLAlertLayer::create(nullptr, "Congratulations!", "You have <cb>moderator</c> privilegies!", "Ok", nullptr, 260.f, false, 0)->show();
            setting().roleType = 1;
        }
        else if (readBuffer == "2")
        {
            gd::FLAlertLayer::create(nullptr, "Congratulations!", "You have <cp>elder moderator</c> privilegies!", "Ok", nullptr, 280.f, false, 0)->show();
            setting().roleType = 2;
        }
        else if (readBuffer == "3")
        {
            gd::FLAlertLayer::create(nullptr, "Congratulations!", "You have <cr>administrator</c> privilegies!", "Ok", nullptr, 280.f, false, 0)->show();
            setting().roleType = 3;
        }
        else if (readBuffer == "4")
        {
            gd::FLAlertLayer::create(nullptr, "Congratulations!", "You have <cy>owner</c> privilegies!", "Ok", nullptr, 260.f, false, 0)->show();
            setting().roleType = 4;
        }
        else if (readBuffer == "-1")
        {
            gd::FLAlertLayer::create(nullptr, "Admin Check", "You have no moderator privilegies.", "Ok", nullptr, 260.f, false, 0)->show();
            setting().roleType = 0;
        }
        else gd::FLAlertLayer::create(nullptr, "Something went wrong...", "You have no internet connection or servers are down.", "Ok", nullptr, 300.f, false, 0)->show();
        std::cout << "ADMIN CHECK PROTOCOL" << std::endl;
        std::cout << readBuffer << std::endl;
        //std::cout << udid << " - User device id" << std::endl << std::endl;
        readBuffer.clear();
    }
};

void adminInitCheck()
{
    std::string udid = (gd::GameManager::sharedState())->getPlayerUDID();

    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    std::string postfield = "deviceId=" + udid;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://85.209.2.73:25568/AdminPanel/CheckAdmin");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfield);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    if (readBuffer == "1") setting().roleType = 1;
    else if (readBuffer == "2") setting().roleType = 2;
    else if (readBuffer == "3") setting().roleType = 3;
    else if (readBuffer == "4") setting().roleType = 4;
    else if (readBuffer == "-1") setting().roleType = 0;
    std::cout << "ADMIN INIT CHECK PROTOCOL" << std::endl;
    //std::cout << postfield << std::endl;
    std::cout << readBuffer << std::endl;
    //std::cout << udid << " - User device id" << std::endl << std::endl;


    readBuffer.clear();
}

bool isFirstLoaded = false;

bool(__thiscall* MenuLayer_init)(CCLayer* self);
bool __fastcall MenuLayer_init_H(CCLayer* self, void*) {
    if (!MenuLayer_init(self)) return false;
    if (!isFirstLoaded)
    {
        adminInitCheck();
        isFirstLoaded = true;
    }
    auto adminCheckSprite = CCSprite::create("GJ_button_03.png");
    if (!(adminCheckSprite->initWithFile("accountBtn_pendingRequest_001.png")))
    {
        adminCheckSprite->initWithFile("GJ_button_03.png");
        auto albl = CCLabelBMFont::create("A", "bigFont.fnt");
        adminCheckSprite->addChild(albl);
        albl->setPosition({ 20, 22 });
    }
    auto adminCheckButton = gd::CCMenuItemSpriteExtra::create(adminCheckSprite, nullptr, self, menu_selector(AdminCheck::adminCheckCallback));
    auto menu = CCMenu::create();
    adminCheckButton->setScale(1.f);
    menu->addChild(adminCheckButton);
    menu->setPosition((CCDirector::sharedDirector()->getScreenRight()) - 25, (CCDirector::sharedDirector()->getScreenTop()) - 25);
    self->addChild(menu);

    return true;
}

bool(__thiscall* PauseLayer_init)(CCLayer* self);
bool __fastcall PauseLayer_init_hook(CCLayer* self)
{
    layers().PauseLayerObject = self;
    if (setting().onHidePause) self->setVisible(0);
    else self->setVisible(1);
    GetLocalTime(&st);
    bool result = PauseLayer_init(self);
    return result;
}

bool(__thiscall* PlayLayer_resume)(CCLayer* self);
bool __fastcall PlayLayer_resume_H(CCLayer* self)
{
    layers().PauseLayerObject = nullptr;
    bool result = PlayLayer_resume(self);
    return result;
}

bool(__thiscall* PauseLayer_onEdit)(CCLayer* self);
void __fastcall PauseLayer_onEdit_H(CCLayer* self)
{
    layers().PauseLayerObject = nullptr;
    PlayLayerObject = nullptr;
    PauseLayer_onEdit(self);
}

bool(__thiscall* PauseLayer_onQuit)(CCObject* btn);
void __fastcall PauseLayer_onQuit_H(CCObject* btn)
{
    PlayLayerObject = nullptr;
    if (setting().onConfirmExit) gd::FLAlertLayer::create(&eaProtocol, "Confirm Exit", "Are you sure, you want to <cr>exit</c> the level?", "No", "Yes", 320.f, false, 0)->show();
    PauseLayer_onQuit(btn);
}

bool(__thiscall* PlayLayer_onQuit)(CCNode* self);
void __fastcall PlayLayer_onQuit_H(CCNode* self) {
    layers().PauseLayerObject = nullptr;
    PlayLayerObject = nullptr;
    PlayLayer_onQuit(self);
}

bool(__thiscall* GarageLayer_init)(CCLayer* self);
bool __fastcall GarageLayer_init_H(CCLayer* self) {
    auto drawer = CCDrawNode::create();
    drawer->setZOrder(10);
    drawer->drawDot({ 200, 200 }, 5.f, { 255.f, 255.f, 255.f, 255.f });

    if (!GarageLayer_init(self)) return false;
    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();
    auto scrnRight = director->getScreenRight();
    auto scrnTop = director->getScreenTop();
    auto sprite = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
    auto label = CCLabelBMFont::create("", "bigFont.fnt");

    //auto demons = gd::GJUserScore->getCompletedDemons();
    int demons = gd::GameStatsManager::sharedState()->getStat("5");

    label->setString(CCString::createWithFormat("%d", demons)->getCString());
    label->setAnchorPoint({ 1.f, 0.5f });
    label->setScale(0.5f);
    label->setPosition({ scrnRight - 58, scrnTop - 96 });
    //sprite->setAnchorPoint({ 0.f, 0.f });
    sprite->setPosition({ scrnRight - 40, scrnTop - 96 });
    sprite->setScale(1.f);

    self->addChild(sprite);
    self->addChild(label);

    self->addChild(drawer);
    return true;
}

bool(__thiscall* EndLayer_init)(CCLayer* self);
void __fastcall EndLayer_init_H(CCLayer* self) {
    /*auto label = CCLabelBMFont::create("Ayo", "bigFont.fnt");
    label->setScale(10.f);
    label->setPosition(self->getPosition());
    self->addChild(label);*/

    EndLayer_init(self);

}

bool(__thiscall* EditorUI_init)(gd::EditorUI* self, CCLayer* editor);
bool __fastcall EditorUI_init_H(gd::EditorUI *self, void *, CCLayer *editor) {
    PlayLayerObject = nullptr;
    editUI = self;
    bool result = EditorUI_init(self, editor);

    auto rotationLabel = CCLabelBMFont::create("0", "bigFont.fnt");
    rotationLabel->setScale(0.35f);
    rotationLabel->setPosition({ 0, 30.f });
    rotationLabel->setTag(1);
    self->getGJRotationControl( )->addChild(rotationLabel);

    auto editSprite = CCSprite::createWithSpriteFrameName("GJ_editObjBtn_001.png");
    editUI->getEditObjectButton( )->setVisible(0);
    editSprite->setScale(0.85f);
    auto editBtn = gd::CCMenuItemSpriteExtra::create(editSprite, nullptr, self, menu_selector(MyCustomizeObjectLayer::onEdit));
    editBtn->setPosition({ -7.865f, -87.865f });
    editBtn->setTag(4580);
    auto editMenu = from<CCMenu *>(editUI->getEditObjectButton( ), 0xAC);
    editMenu->addChild(editBtn);
    self->addChild(editMenu);

    auto duplicateSprite = CCSprite::createWithSpriteFrameName("GJ_duplicateObjectBtn_001.png");
    editUI->getDuplicateButton()->setVisible(0);
    duplicateSprite->setScale(0.85f);
    duplicateSprite->setPosition({23.46f, 24.225});
    auto duplicateBtn = gd::CCMenuItemSpriteExtra::create(duplicateSprite, nullptr, self, menu_selector(EditorUI::Callback::onDuplicate));
    duplicateBtn->setPosition({ -8.f, -44.f });
    duplicateBtn->setTag(4581);
    editMenu->addChild(duplicateBtn);

    auto colIncSprite = CCSprite::createWithSpriteFrameName("player_special_02_001.png");
    auto colIncBtn = gd::CCMenuItemSpriteExtra::create(colIncSprite, nullptr, self, menu_selector(MyCustomizeObjectLayer::onColInc));
    colIncBtn->setPosition({ -100.f, -60.f });
    if (setting().onDebugLabels)
        editMenu->addChild(colIncBtn);

    auto leftiestLayerSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    leftiestLayerSprite->setScale(0.5f);
    auto leftiestLayerButton = gd::CCMenuItemSpriteExtra::create(leftiestLayerSprite, nullptr, self, menu_selector(EditorUI::Callback::toTheFirstLayerButton));
    leftiestLayerButton->setPosition({ -42.f, 0 });
    leftiestLayerButton->setOpacity(200);

    auto rightiestLayerSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    rightiestLayerSprite->setScale(0.5f);
    rightiestLayerSprite->setFlipX(true);
    auto rightiestLayerButton = gd::CCMenuItemSpriteExtra::create(rightiestLayerSprite, nullptr, self, menu_selector(EditorUI::Callback::toTheEmptyLayerButton));
    rightiestLayerButton->setPosition({ 42.f, 0 });
    rightiestLayerButton->setOpacity(200);

    auto arrowMenu = CCMenu::create();
    arrowMenu->setPosition({ CCDirector::sharedDirector()->getScreenRight() - 50.f, CCDirector::sharedDirector()->getScreenTop() - 192.f });
    arrowMenu->addChild(rightiestLayerButton);
    arrowMenu->addChild(leftiestLayerButton);
    arrowMenu->setZOrder(10);

    self->addChild(arrowMenu);

    if (setting().onDebugLabels)
    {
        auto editorlbl = CCLabelBMFont::create("", "chatFont.fnt");
        editorlbl->setString(CCString::createWithFormat("EditorUI pointer: %p", self)->getCString());
        editorlbl->setAnchorPoint({ 1.f, 0.5f });
        editorlbl->setPosition(CCDirector::sharedDirector()->getScreenRight() - 120, 140);
        editorlbl->setScale(0.5f);
        self->addChild(editorlbl);

        auto lvllbl = CCLabelBMFont::create("", "chatFont.fnt");
        lvllbl->setString(CCString::createWithFormat("LevelEditorLayer pointer: %p", self->getLevelEditorLayer())->getCString());
        lvllbl->setAnchorPoint({ 1.f, 0.5f });
        lvllbl->setPosition(CCDirector::sharedDirector()->getScreenRight() - 120, 130);
        lvllbl->setScale(0.5f);
        lvllbl->setTag(4571);
        self->addChild(lvllbl);

        auto mngrlbl = CCLabelBMFont::create("", "chatFont.fnt");
        mngrlbl->setString(CCString::createWithFormat("GameManager pointer: %p", gd::GameManager::sharedState())->getCString());
        mngrlbl->setAnchorPoint({ 1.f, 0.5f });
        mngrlbl->setPosition(CCDirector::sharedDirector()->getScreenRight() - 120, 120);
        mngrlbl->setScale(0.5f);
        self->addChild(mngrlbl);
    }

    if (setting().onSelectedObjectLabel)
    {

        std::vector<gd::GameObject*> gameObjectVector = self->getSelectedObjects();
        auto selobjcountlbl = CCLabelBMFont::create("", "chatFont.fnt");
        selobjcountlbl->setString(CCString::createWithFormat("Objects: %d", gameObjectVector.size())->getCString());
        selobjcountlbl->setVisible(0);
        selobjcountlbl->setTag(4570);
        selobjcountlbl->setAnchorPoint({ 1.f, 0.5f });
        selobjcountlbl->setPosition(150, CCDirector::sharedDirector()->getScreenTop() - 60);
        selobjcountlbl->setScale(0.66f);
        self->addChild(selobjcountlbl);

        gd::GameObject* selectedObject = from<gd::GameObject*>(self, 0x258);
        auto selobjplbl = CCLabelBMFont::create("", "chatFont.fnt");
        selobjplbl->setString(CCString::createWithFormat("address: %p", selectedObject)->getCString());
        selobjplbl->setVisible(0);
        selobjplbl->setTag(1);
        selobjplbl->setScale(0.66f);
        auto debugMenu = CCMenu::create( );
        debugMenu->setPosition(150, CCDirector::sharedDirector( )->getScreenTop( )-70);
        debugMenu->setTag(4572);
        auto selobjpbtn = gd::CCMenuItemSpriteExtra::create(selobjplbl, nullptr, self, menu_selector(EditorUI::Callback::onDebugCopyObjAddress));
        selobjpbtn->setTag(1);
        selobjpbtn->setAnchorPoint({ 1.f, 0.5f });
        debugMenu->addChild(selobjpbtn);

        self->addChild(debugMenu);

        auto selobjidlbl = CCLabelBMFont::create("", "chatFont.fnt");
        selobjidlbl->setString(CCString::createWithFormat("ID: %d", 0)->getCString());
        selobjidlbl->setVisible(0);
        selobjidlbl->setTag(4573);
        selobjidlbl->setAnchorPoint({ 1.f, 0.5f });
        selobjidlbl->setPosition(150, CCDirector::sharedDirector()->getScreenTop() - 80);
        selobjidlbl->setScale(0.66f);
        self->addChild(selobjidlbl);

        auto selobjcol = CCLabelBMFont::create("", "chatFont.fnt");
        selobjcol->setString(CCString::createWithFormat("ColID: %d", 0)->getCString());
        selobjcol->setVisible(0);
        selobjcol->setTag(4574);
        selobjcol->setAnchorPoint({ 1.f, 0.5f });
        selobjcol->setPosition(150, CCDirector::sharedDirector()->getScreenTop() - 90);
        selobjcol->setScale(0.66f);
        self->addChild(selobjcol);

        auto selobjgroup = CCLabelBMFont::create("", "chatFont.fnt");
        selobjgroup->setString(CCString::createWithFormat("Group: %d", 0)->getCString( ));
        selobjgroup->setVisible(0);
        selobjgroup->setTag(4575);
        selobjgroup->setAnchorPoint({ 1.f, 0.5f });
        selobjgroup->setPosition(150, CCDirector::sharedDirector( )->getScreenTop( )-100);
        selobjgroup->setScale(0.66f);
        self->addChild(selobjgroup);
    }
    if (setting().onPersistentClipboard)
    {
        if (!savedClipboard.empty()) {
            self->clipboard() = savedClipboard;
            self->updateButtons();
        }
    }

    return result;
}




//bool(__thiscall* CustomizeObjectLayer_init)(CustomizeObjectLayer* self, gd::GameObject* obj, CCArray* objs);
//bool __fastcall CustomizeObjectLayer_init_H(CustomizeObjectLayer* self, void* edx, gd::GameObject* obj, CCArray* objs) {
//    if (!CustomizeObjectLayer_init(self, obj, objs)) return false;
//
//    auto sprite = gd::ButtonSprite::create("3DL", 250, 0, 0.4, false, "bigFont.fnt", "GJ_button_04.png", 25.0);
//    auto button = gd::CCMenuItemSpriteExtra::create(sprite, nullptr, self, union_cast<SEL_MenuHandler>(gd::base + 0x2e390));
//    button->setTag(8);
//    button->setPosition(100, 0);
//    from<CCArray*>(self, 0x1c4)->addObject(sprite);
//    self->getMenu()->addChild(button);
//
//    auto colorBtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
//    auto colorBtn = gd::CCMenuItemSpriteExtra::create(colorBtnSprite, nullptr, self, nullptr);
//    auto selectSprite = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
//
//    colorBtn->setPosition({ -50.f, 0.f });
//    selectSprite->setPosition({ -50.f, 0.f });
//
//    self->getMenu()->addChild(colorBtn);
//    self->getMenu()->addChild(selectSprite);
//
//    if (obj && obj->getColorMode() == 8) {
//        self->hightlightSelected(sprite);
//    }
//
//    return true;
//}

//bool(__thiscall* EditorUI_selectObject)(gd::EditorUI* self, gd::GameObject* obj);
//void __fastcall EditorUI_selectObject_H(gd::EditorUI* self, void* edx, gd::GameObject* obj)
//{
//    EditorUI_selectObject(self, obj);
//}
//
//bool(__thiscall* EditorUI_selectObjects)(gd::EditorUI* self, CCArray* obj);
//void __fastcall EditorUI_selectObjects_H(gd::EditorUI* self, void* edx, CCArray* obj)
//{
//    EditorUI_selectObjects(self, obj);
//}

bool(__thiscall* EditorUI_dtor)(gd::EditorUI* self);
bool __fastcall EditorUI_dtor_H(gd::EditorUI* self) {
    //editUI = self;
    editUI = nullptr;
    savedClipboard = self->clipboard();
    EditorUI_dtor(self);
}

bool(__thiscall* EditorPauseLayer_onExitEditor)(CCObject* self);
void __fastcall EditorPauseLayer_onExitEditor_H(CCObject* self) {
    editUI = nullptr;
    EditorPauseLayer_onExitEditor(self);
}

bool(__thiscall* EditorPauseLayer_saveLevel)(CCLayer* self);
void __fastcall EditorPauseLayer_saveLevel_H(CCLayer* self) {
    editUI = nullptr;
    EditorPauseLayer_saveLevel(self);
}



//bool(__thiscall* MenuLayer_init)(CCLayer* self);
//bool __fastcall MenuLayer_init_H(CCLayer* self) {
//    
//    bool result = MenuLayer_init(self);
//    auto label = CCLabelBMFont::create("Goodbye World!", "bigFont.fnt");
//    label->setPosition({100, 100});
//    label->setScale(1.5f);
//    self->addChild(label);
//    return result;
//}

bool(__thiscall* Scheduler_update)(CCScheduler* self, float idk);
void __fastcall Scheduler_update_H(CCScheduler* self, void* edx, float idk) {

    /*if (PauseLayerObject && setting().onTimeStop)
    {
        BOOL bRet = SetLocalTime(&st);
    }*/
    //cocos2d::extension::RGBA
    if (editUI)
    {
        //auto objArr = editUI->getAllObjects( );
        //for ( int i = 0; i<objArr->count( ); i++ ) {
        //    auto iObj = reinterpret_cast< gd::GameObject * >(objArr->objectAtIndex(i));
        //    if ( saws.contains(iObj->getObjectID( )) ) {
        //        iObj->setRotation(iObj->getRotation( )+1.f);
        //    }
        //}

        if (setting().onHideEditorUI) editUI->setVisible(0);
        else editUI->setVisible(1);
        auto selobjcountlbl = editUI->getChildByTag(4570);
        auto lvllbl = editUI->getChildByTag(4571);
        auto debugMenu = editUI->getChildByTag(4572);
        auto selobjpbtn = debugMenu->getChildByTag(1);
        auto selobjplbl = selobjpbtn->getChildByTag(1);
        auto selobjidlbl = editUI->getChildByTag(4573);
        auto selobjcollbl = editUI->getChildByTag(4574);
        auto selobjgroup = editUI->getChildByTag(4575);

        auto editMenu = from<CCMenu *>(editUI->getEditObjectButton( ), 0xAC);
        auto editBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra *>(editMenu->getChildByTag(4580));
        auto dupclicateBtn = reinterpret_cast< gd::CCMenuItemSpriteExtra * >(editMenu->getChildByTag(4581));

        if ( editBtn ) {
            if ((from<bool>(editUI->getEditObjectButton( ), 0x11c)||setting( ).anyEdit||editUI->isEveryObjectIsTrigger( ))
                &&editUI->getSelectedObjectsOfCCArray( )->count( )>0 ) {
                editBtn->setOpacity(255);
                editBtn->setColor({ 255, 255, 255 });
                editBtn->setEnabled(true);
            }
            else {
                editBtn->setOpacity(175);
                editBtn->setColor({166, 166, 166});
                editBtn->setEnabled(false);
            }
        }

        if ( dupclicateBtn ) {
            if (editUI->getSelectedObjectsOfCCArray( )->count( )==0) {
                dupclicateBtn->setOpacity(175);
                dupclicateBtn->setColor({ 166, 166, 166 });
                dupclicateBtn->setEnabled(false);
            }
            else {
                dupclicateBtn->setOpacity(255);
                dupclicateBtn->setColor({ 255, 255, 255 });
                dupclicateBtn->setEnabled(true);
            }
        }
        if ( editUI->getSingleSelectedObj( ) ) {
            if ( editUI->getSingleSelectedObj( )->getRotation( )<0 )
                editUI->getSingleSelectedObj( )->setRotation(editUI->getSingleSelectedObj( )->getRotation( )+360);
            if ( editUI->getSingleSelectedObj( )->getRotation( )>360 )
                editUI->getSingleSelectedObj( )->setRotation(editUI->getSingleSelectedObj( )->getRotation( )-360);
            reinterpret_cast< CCLabelBMFont * >(editUI->getGJRotationControl( )->getChildByTag(1))->setVisible(true);
            reinterpret_cast< CCLabelBMFont * >(editUI->getGJRotationControl( )->getChildByTag(1))->setString(CCString::createWithFormat("%.0f%", editUI->getSingleSelectedObj( )->getRotation( ))->getCString( ));
        }
        else reinterpret_cast< CCLabelBMFont * >(editUI->getGJRotationControl( )->getChildByTag(1))->setVisible(false);


        if ( selobjgroup ) {
            if ( editUI->getSingleSelectedObj( )==0 ) selobjgroup->setVisible(0);
            else
            {
                reinterpret_cast< CCLabelBMFont * >(selobjgroup)->setString(CCString::createWithFormat("Group: %d", editUI->getSingleSelectedObj( )->getGroup())->getCString( ));
                selobjgroup->setVisible(1);
            }
        }
        if (selobjcollbl) {
            if (editUI->getSingleSelectedObj() == 0) selobjcollbl->setVisible(0);
            else
            {
                reinterpret_cast<CCLabelBMFont*>(selobjcollbl)->setString(CCString::createWithFormat("ColID: %d", editUI->getSingleSelectedObj()->getActiveColor())->getCString());
                selobjcollbl->setVisible(1);
            }
        }
        if (selobjidlbl)
        {
            if (editUI->getSingleSelectedObj() == 0) selobjidlbl->setVisible(0);
            else
            {
                reinterpret_cast<CCLabelBMFont*>(selobjidlbl)->setString(CCString::createWithFormat("ID: %d", editUI->getSingleSelectedObj()->getObjectID())->getCString());
                selobjidlbl->setVisible(1);
            }
        }
        if (selobjcountlbl)
        {
            reinterpret_cast<CCLabelBMFont*>(selobjcountlbl)->setString(CCString::createWithFormat("Objects: %d", (editUI->getSelectedObjectsOfCCArray()->count()))->getCString());
            if (editUI->getSelectedObjectsOfCCArray()->count() == 0) selobjcountlbl->setVisible(0);
            else selobjcountlbl->setVisible(1);
        }
        if (lvllbl)
        {
            reinterpret_cast<CCLabelBMFont*>(lvllbl)->setString(CCString::createWithFormat("LevelEditorLayer pointer: %p", editUI->getLevelEditorLayer())->getCString());
        }
        if (selobjplbl)
        {
            if (editUI->getSingleSelectedObj() == 0) selobjplbl->setVisible(0);
            else {
                reinterpret_cast<CCLabelBMFont*>(selobjplbl)->setString(CCString::createWithFormat("address: %p", editUI->getSingleSelectedObj())->getCString());
                selobjplbl->setVisible(1);
            }
        }
    }

    if (PlayLayerObject)
    {
        auto SafeModeLabel = reinterpret_cast<CCLabelBMFont*>(PlayLayerObject->getChildByTag(45710));
        auto CheatIndicatorLabel = reinterpret_cast<CCLabelBMFont*>(PlayLayerObject->getChildByTag(4572));

        if (SafeModeLabel)
        {
            if (setting().onSafeMode && setting().onSafeModeLabel) SafeModeLabel->setVisible(1);
            else SafeModeLabel->setVisible(0);
        }

        if (CheatIndicatorLabel)
        {
            ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EE9DA), &setting().CurrentNoclipByte, 1, 0);
            CheatIndicatorLabel->setColor({ 0, 255, 0 });

            if (setting().NoclipByte != setting().CurrentNoclipByte && setting().onNoclipOutOfMe == false) { setting().cheatsCount++; setting().beforeRestartCheatsCount++; setting().onNoclipOutOfMe = true; }
            else if (setting().NoclipByte == setting().CurrentNoclipByte && setting().onNoclipOutOfMe == true) { setting().cheatsCount--; setting().onNoclipOutOfMe = false; }

            //no cheats, no before restart cheats, no safe mode
            if (setting().cheatsCount == 0 &&
                setting().beforeRestartCheatsCount == 0 &&
                setting().NoclipByte == setting().CurrentNoclipByte &&
                !setting().onSafeMode)
                CheatIndicatorLabel->setColor({ 0, 255, 0 });

            //no cheats, no before restart cheats, safe mode
            else if (setting().cheatsCount == 0 &&
                setting().beforeRestartCheatsCount == 0 &&
                setting().NoclipByte == setting().CurrentNoclipByte &&
                setting().onSafeMode)
                CheatIndicatorLabel->setColor({ 255, 255, 0 });

            //no cheats
            else if (setting().cheatsCount == 0 &&
                setting().NoclipByte == setting().CurrentNoclipByte)
                CheatIndicatorLabel->setColor({ 255, 128, 0 });

            else CheatIndicatorLabel->setColor({ 255, 0, 0 });
        }
    }

    Scheduler_update(self, idk);
}

//auto save hack state
bool(__thiscall* CCDirector_end)(CCDirector* self);
void __fastcall CCDirector_end_H(CCDirector* self, void* edx) {
    if (setting().onAutoSave)
    {
        auto file = fopen("Resources/hacksave.dat", "wb");
        if (file) {
            fwrite(&setting(), sizeof(setting()), 1, file);
            fclose(file);
        }
    }
    CCDirector_end(self);
}

//bool(__thiscall* CCDirector_init)(CCDirector* self);
//void __fastcall CCDirector_init_H(CCDirector* self, void* edx) {
//    adminInitCheck();
//    CCDirector_end(self);
//}


void idk() {
    return reinterpret_cast<void(__fastcall*)()>(gd::base + 0x47100)();
}


void objectAdd(int id)
{
    __asm {
        push ecx
        push 0x4
        push id
        mov ecx, ebx
        call idk
        push eax
        mov ecx, esi
        call edi
    }
}


bool(__thiscall* ObjectToolboxAdd_RampTab)();
void __fastcall ObjectToolboxAdd_RampTab_H() {
    objectAdd(0x173);
    objectAdd(0x174);
    objectAdd(0x175);
    objectAdd(0x176);
    ObjectToolboxAdd_RampTab();
}

bool(__thiscall* ObjectToolboxAdd_SpikesTab)();
void __fastcall ObjectToolboxAdd_SpikesTab_H() {
    objectAdd(0x1A5);
    objectAdd(0x1A6);
    ObjectToolboxAdd_SpikesTab();
}

bool(__thiscall* ObjectToolboxAdd_DecoSpikesTab)();
void __fastcall ObjectToolboxAdd_DecoSpikesTab_H() {
    objectAdd(0x1A3);
    objectAdd(0x1A4);
    ObjectToolboxAdd_DecoSpikesTab();
}

bool(__thiscall* ObjectToolboxAdd_ChainTab)();
void __fastcall ObjectToolboxAdd_ChainTab_H() {
    objectAdd(0x196);
    objectAdd(0x197);
    objectAdd(0x198);
    objectAdd(0x2D5);
    ObjectToolboxAdd_ChainTab();
}

bool(__thiscall* ObjectToolboxAdd_TriggerTab)();
void __fastcall ObjectToolboxAdd_TriggerTab_H() {
    objectAdd(0x37);
    ObjectToolboxAdd_TriggerTab();
}

int*(__thiscall *EditorColorSetter)(gd::GameObject *obj, ccColor4B col );
void __fastcall EditorColorSetter_H(gd::GameObject* obj, void* edx, ccColor4B col) {
    int* a = EditorColorSetter(obj, col);

    switch ( obj->getActiveColor( ) ) {
    case 0:
        *a = 0x0AFFFFFF;
        break;
    case 1: //p1 -
        *a = 0x0AFF96AF;
        break;
    case 2: //p2 -
        *a = 0x0A9696FF;
        break;
    case 3: //c1 -
        *a = 0x0AFF96FF;
        break;
    case 4: //c2 -
        *a = 0x0A96FFFF;
        break;
    case 5: //lbg -
        *a = 0x0AFFAF4B;
        break;
    case 6: //c3 -
        *a = 0x0AFFFF96;
        break;
    case 7: //c4 -
        *a = 0x0A96FF96;
        break;
    case 8: //3dl -
        *a = 0x0A00FFFF;
        break;
    case 9: //3dl -
        *a = 0x0AA6A6A6;
        break;
    default:
        *a = 0x0A0000FF;
    }
    if ( obj->isInvisible() ) {
        *a = 0x0A007FFF;
    }
    
    //*a = 0x0A0000D1;
}

bool(__thiscall *ColorSelectPopup_init)(gd::FLAlertLayer *self, CCObject* obj, int i1, int i2, int i3);
bool __fastcall ColorSelectPopup_init_H(gd::FLAlertLayer *self, void* edx, CCObject* obj, int i1, int i2, int i3) {
    if ( !ColorSelectPopup_init(self, obj, i1, i2, i3) ) return false;

    if ( setting( ).onDebugLabels )
    {
        auto lbl = CCLabelBMFont::create("", "chatFont.fnt");
        lbl->setString(CCString::createWithFormat("CSP pointer: %p", self)->getCString( ));
        lbl->setAnchorPoint({ 1.f, 0.5f });
        lbl->setPosition(CCDirector::sharedDirector( )->getScreenRight( )-120, 140);
        lbl->setScale(0.5f);
        lbl->setZOrder(1000);
        self->getLayer()->addChild(lbl);
    }

    return true;
}

//bool(__thiscall *EditorUI_onDuplicate)(CCObject* obj);
//void __fastcall EditorUI_onDuplicate_H(CCObject* obj) {
//    auto eui = reinterpret_cast< gd::EditorUI * >(obj);
//    auto preDuplicateObjs = eui->getSelectedObjectsOfCCArray( );
//    EditorUI_onDuplicate(obj);
//    std::cout<<eui<<std::endl;
    //auto postDuplicateObjs = eui->getSelectedObjectsOfCCArray( );
    //
    //for ( int i = 0; i<preDuplicateObjs->count( ); i++ ) {
        //int groupId = reinterpret_cast< gd::GameObject * >(postDuplicateObjs->objectAtIndex(i))->getGroup( );
        //reinterpret_cast< gd::GameObject * >(preDuplicateObjs->objectAtIndex(i))->setGroup(groupId);
    //}
//}


DWORD WINAPI thread_func(void* hModule) {
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x3A21B), "\xB8\x01\x00\x00\x00\x90\x90", 7, NULL);

    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bfab), "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 11, NULL); //LevelInfoLayer init (deleting info icon)

    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x8A3DB), "\x83\xF8\x61\x0F\x84\xAB\x01\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90\x90", 18, NULL); //HallOfShame onBack patching
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x324000), "\x48\x61\x6c\x6c\x20\x6f\x66\x20\x46\x61\x6d\x65\x00", 13, NULL); //Hall of Shame string
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x89D15), "\x75\x0A\xBE\x1C\x3E\x52\x00\xE9\x81\x01\x00\x00\x83\xF8\x62\x75\x0A\xBE\x2C\x3E\x52\x00\xE9\x73\x01\x00\x00\x83\xF8\x61\x75\x0A\xBE\x00\x40\x72\x00\xE9\x64\x01\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 55, NULL);
    //also Hall of Shame string (but updating)
    //75 0A BE 1C 3E 52 00 E9 81 01 00 00 83 F8 62 75 0A BE 2C 3E 52 00 E9 73 01 00 00 83 F8 61 75 0A BE 00 40 72 00 E9 64 01 00 00 90 90 90 90 90 90 90 90 90 90 90 90 90

    ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EE9DA), &setting().NoclipByte, 1, 0);
    if (MH_Initialize() != MH_OK)
    {
        FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
    }

    MH_CreateHook(
        reinterpret_cast< void * >(gd::base+0x29b60),
        reinterpret_cast< void * >(&ColorSelectPopup_init_H),
        reinterpret_cast< void ** >(&ColorSelectPopup_init));
    //0xd6040 PlayBTN
    //0xd61b0 EditBTN
    //0xd64c0 MenuBTN!

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xee990),
        reinterpret_cast<void*>(&PlayLayer_destroyPlayer_H),
        reinterpret_cast<void**>(&PlayLayer_destroyPlayer));


    AllocConsole();
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);

    /*MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x44864),
        reinterpret_cast<void*>(&GraphicsSettingsApply_H),
        reinterpret_cast<void**>(&GraphicsSettingsApply));*/
        //gd::base + 0x48a3cc
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x2cd53),
        CreatorLayer::CreatorLayer_menu_H,
        reinterpret_cast<void**>(&CreatorLayer::CreatorLayer_menu));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x2cc30),
        CreatorLayer::CreatorLayer_init_H,
        reinterpret_cast<void**>(&CreatorLayer::CreatorLayer_init));
    MH_CreateHook(
        reinterpret_cast< void * >(gd::base + 0x74f60),
        reinterpret_cast< void * >(&EditorColorSetter_H),
        reinterpret_cast< void ** >(&EditorColorSetter));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x44864),
        reinterpret_cast<void*>(&ObjectToolboxAdd_RampTab_H),
        reinterpret_cast<void**>(&ObjectToolboxAdd_RampTab));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x44e99),
        reinterpret_cast<void*>(&ObjectToolboxAdd_SpikesTab_H),
        reinterpret_cast<void**>(&ObjectToolboxAdd_SpikesTab));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x45e4b),
        reinterpret_cast<void*>(&ObjectToolboxAdd_DecoSpikesTab_H),
        reinterpret_cast<void**>(&ObjectToolboxAdd_DecoSpikesTab));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x463d6),
        reinterpret_cast<void*>(&ObjectToolboxAdd_ChainTab_H),
        reinterpret_cast<void**>(&ObjectToolboxAdd_ChainTab));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x46c37),
        reinterpret_cast<void*>(&ObjectToolboxAdd_TriggerTab_H),
        reinterpret_cast<void**>(&ObjectToolboxAdd_TriggerTab));


    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x7bd90),
        reinterpret_cast<void*>(&GarageLayer_init_H),
        reinterpret_cast<void**>(&GarageLayer_init));


    auto cocos = GetModuleHandleA("libcocos2d.dll");

    MH_CreateHook(
        reinterpret_cast<void*>(GetProcAddress(cocos, "?dispatchKeyboardMSG@CCKeyboardDispatcher@cocos2d@@QAE_NW4enumKeyCodes@2@_N@Z")),
        reinterpret_cast<void**>(&CCKeyboardDispatcher_dispatchKeyboardMSG_H),
        reinterpret_cast<void**>(&CCKeyboardDispatcher_dispatchKeyboardMSG));

    MH_CreateHook(
        reinterpret_cast<void*>(GetProcAddress(cocos, "?end@CCDirector@cocos2d@@QAEXXZ")),
        reinterpret_cast<void**>(&CCDirector_end_H),
        reinterpret_cast<void**>(&CCDirector_end));

    //MH_CreateHook(
    //    reinterpret_cast<void*>(GetProcAddress(cocos, "?init@CCDirector@cocos2d@@UAE_NXZ")),
    //    reinterpret_cast<void**>(&CCDirector_init_H),
    //    reinterpret_cast<void**>(&CCDirector_init));


    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd4510),
        reinterpret_cast<void**>(&PauseLayer_init_hook),
        reinterpret_cast<void**>(&PauseLayer_init));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd61b0),
        reinterpret_cast<void**>(&PauseLayer_onEdit_H),
        reinterpret_cast<void**>(&PauseLayer_onEdit));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd64c0),
        reinterpret_cast<void*>(&PauseLayer_onQuit_H),
        reinterpret_cast<void**>(&PauseLayer_onQuit));


    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf03f0),
        reinterpret_cast<void**>(&PlayLayer_resetLevel_H),
        reinterpret_cast<void**>(&PlayLayer_resetLevel));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xE1b10),
        reinterpret_cast<void**>(&PlayLayer_init_H),
        reinterpret_cast<void**>(&PlayLayer_init));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xe78a0),
        reinterpret_cast<void**>(&PlayLayer_update_H),
        reinterpret_cast<void**>(&PlayLayer_update));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf1e10),
        reinterpret_cast<void**>(&PlayLayer_resume_H),
        reinterpret_cast<void**>(&PlayLayer_resume));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf1fe0),
        reinterpret_cast<void*>(&PlayLayer_onQuit_H),
        reinterpret_cast<void**>(&PlayLayer_onQuit));

    //MH_CreateHook(
    //    reinterpret_cast< void * >(gd::base+0x48d40),
    //    reinterpret_cast< void * >(&EditorUI_onDuplicate_H),
    //    reinterpret_cast< void ** >(&EditorUI_onDuplicate));


    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3f1e0),
        reinterpret_cast<void**>(&EditorPauseLayer_onExitEditor_H),
        reinterpret_cast<void**>(&EditorPauseLayer_onExitEditor));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3ed20),
        reinterpret_cast<void**>(&EditorPauseLayer_saveLevel_H),
        reinterpret_cast<void**>(&EditorPauseLayer_saveLevel));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3e150),
        EditorPauseLayer::EditorPauseLayer_init_hook,
        reinterpret_cast<void**>(&EditorPauseLayer::EditorPauseLayer_init));

    /*
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd4510),
        PauseLayer::hook,
        reinterpret_cast<void**>(&PauseLayer::init));
    */


    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xae7a0), //0x1907b0 (original address) 0xae7a0
        reinterpret_cast<void*>(&MenuLayer_init_H),
        reinterpret_cast<void**>(&MenuLayer_init)); // note the &, this gets the address of the variable

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3fc00),
        reinterpret_cast<void**>(&EditorUI_init_H),
        reinterpret_cast<void**>(&EditorUI_init));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3f9d0),
        reinterpret_cast<void**>(&EditorUI_dtor_H),
        reinterpret_cast<void**>(&EditorUI_dtor));
    //MH_CreateHook(
    //    reinterpret_cast< void * >(gd::base + 0x2d950),
    //    CustomizeObjectLayer::create_H,
    //    reinterpret_cast< void ** >(&CustomizeObjectLayer::create));

    //MH_CreateHook(
    //    reinterpret_cast<void*>(gd::base + 0x2da00),
    //    CustomizeObjectLayer::CustomizeObjectLayer_init_H,
    //    reinterpret_cast<void**>(&CustomizeObjectLayer::CustomizeObjectLayer_init));

    //MH_CreateHook(
    //    reinterpret_cast<void*>(gd::base + 0x47df0),
    //    reinterpret_cast<void**>(&EditorUI_selectObject_H),
    //    reinterpret_cast<void**>(&EditorUI_selectObject));
    //MH_CreateHook(
    //    reinterpret_cast<void*>(gd::base + 0x47e80),
    //    reinterpret_cast<void**>(&EditorUI_selectObjects_H),
    //    reinterpret_cast<void**>(&EditorUI_selectObjects));


    //MH_CreateHook(
    //    reinterpret_cast<void*>(gd::base + 0x3fc00),
    //    EditorPauseLayer::EditorUILayer_init_H,
    //    reinterpret_cast<void**>(&EditorPauseLayer::EditorUILayer_init));
    //MH_CreateHook(
    //    reinterpret_cast<void*>(gd::base + 0x3f9d0),
    //    EditorPauseLayer::EditorUILayer_dtor_H,
    //    reinterpret_cast<void**>(&EditorPauseLayer::EditorUILayer_dtor));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3b320),
        EditLevelLayer::EditLevelLayer_init_H,
        reinterpret_cast<void**>(&EditLevelLayer::EditLevelLayer_init));
    //base + 0x9e2b0


    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x835f0),
        InfoLayer::InfoLayer_init_hook,
        reinterpret_cast<void**>(&InfoLayer::InfoLayer_init));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x9b160),
        LevelInfoLayer::LevelInfoLayer_init_hook,
        reinterpret_cast<void**>(&LevelInfoLayer::LevelInfoLayer_init));

    //MH_CreateHook(
    //    reinterpret_cast<void*>(gd::base + 0x9e2b0),
    //    LevelInfoLayer::LevelInfoLayer_onLevelInfo_hook,
    //    reinterpret_cast<void**>(&LevelInfoLayer::LevelInfoLayer_onLevelInfo));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x4ff10),
        reinterpret_cast<void**>(&EndLayer_init_H),
        reinterpret_cast<void**>(&EndLayer_init));
    MH_CreateHook(
        reinterpret_cast<void*>(cocosbase + 0xffcc0),
        reinterpret_cast<void**>(&Scheduler_update_H),
        reinterpret_cast<void**>(&Scheduler_update));

    //0x47f520

    //GJGameLayerInit gd::base + 0x9b160

    setup_imgui_menu();

    MH_EnableHook(MH_ALL_HOOKS);
}

/*
This function runs whenever we inject our dll, this is present
in pretty much every mod, and is just winapi boilerplate
*/
BOOL APIENTRY DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        auto h = CreateThread(0, 0, thread_func, handle, 0, 0);
        if (h)
            CloseHandle(h);
        else
            return FALSE;
    }
    return TRUE;
}