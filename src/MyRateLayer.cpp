#include "MyRateLayer.h"


bool MyRateLayer::init()
{
    static CCLayerColor* cclcol;
    //static CCLayerColor::initWithColor(ccc4(0, 0, 0, 105));
    if (!(cclcol->initWithColor(ccc4(0, 0, 0, 105)))) return false;
    cocos2d::extension::CCScale9Sprite* bg = cocos2d::extension::CCScale9Sprite::create("GJ_square01.png");
    auto director = CCDirector::sharedDirector();
    bg->setContentSize({ 360, 180 });
    bg->setPosition((director->getScreenRight()) / 2, (director->getScreenTop()) / 2);
    bg->setZOrder(10);
    this->addChild(bg);

    return true;
}

void MyRateLayer::keyBackClicked()
{

}

MyRateLayer* MyRateLayer::create()
{
	/*auto obj = new MyRateLayer;
    if (obj->init()) obj->autorelease();
    else CC_SAFE_DELETE(obj);
	return obj;*/
    return nullptr;
}

void MyRateLayer::rateCallback(CCObject*)
{
    /*auto myRateLayer = MyRateLayer::create();*/
    /*CCDirector::sharedDirector()->getRunningScene()->addChild(myRateLayer);*/

}




