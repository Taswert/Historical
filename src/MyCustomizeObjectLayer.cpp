#include "includes.h"
#include "MyCustomizeObjectLayer.h"

void MyCustomizeObjectLayer::keyBackClicked( )
{
	this->removeMeAndCleanup( );
}

bool MyCustomizeObjectLayer::init(gd::LevelEditorLayer *lel)
{

	CCLayer::init( );
	CCLayerColor *cclcol = CCLayerColor::create(ccc4(0, 0, 0, 0));
	cclcol->setZOrder(1);
	cclcol->setScale(10.f);
	this->addChild(cclcol);
	auto actionCol = CCFadeTo::create(0.1f, 75);
	cclcol->runAction(actionCol);

	auto touchDispatcher = CCDirector::sharedDirector( )->m_pTouchDispatcher;
	touchDispatcher->incrementForcePrio( );
	touchDispatcher->incrementForcePrio( );
	registerWithTouchDispatcher( );
	setTouchEnabled(true);
	setKeypadEnabled(true);
	setMouseEnabled(true);

	auto bgSprite = CCSprite::create("GJ_button_03.png"); //Background button. Ik this is so bad, but i can't do anything better than that
	bgSprite->setScale(100.f);
	bgSprite->setOpacity(0);
	auto bgButton = gd::CCMenuItemSpriteExtra::create(bgSprite, nullptr, this, nullptr);
	auto bgMenu = CCMenu::create( );
	bgMenu->addChild(bgButton);
	bgMenu->setZOrder(0);
	bgMenu->setPosition((CCDirector::sharedDirector( )->getScreenRight( ))-25, (CCDirector::sharedDirector( )->getScreenTop( ))-25);
	this->addChild(bgMenu);

	auto mainLabel = CCLabelBMFont::create("Use Color", "bigFont.fnt");
	mainLabel->setPosition((CCDirector::sharedDirector( )->getScreenRight( ))/2, (CCDirector::sharedDirector( )->getScreenTop( ))/2+90);
	mainLabel->setZOrder(3);
	this->addChild(mainLabel);


	auto colorsMenu = CCMenu::create( );
	colorsMenu->setPosition(CCDirector::sharedDirector( )->getScreenRight( )/2, CCDirector::sharedDirector( )->getScreenTop( )/2);
	colorsMenu->setZOrder(5);


	
	auto bgBtnLabel = CCLabelBMFont::create("BG", "bigFont.fnt");
	bgBtnLabel->setScale(0.33f);
	bgBtnLabel->setZOrder(1);
	auto bgBtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	auto levelSettingsObject = from<gd::LevelSettingsObject *>(lel, 0x190);
	int *sco = from<int *>(levelSettingsObject, 0xE8);
	auto red = from<unsigned char>(sco, 0xE8);
	auto green = from<unsigned char>(sco, 0xE9);
	auto blue = from<unsigned char>(sco, 0xEA);
	bgBtnSprite->setColor({ red, green, blue });
	auto bgBtn = gd::CCMenuItemSpriteExtra::create(bgBtnSprite, nullptr, this, nullptr);
	bgBtn->addChild(bgBtnLabel);
	bgBtnLabel->setPosition({ bgBtn->getContentSize( ).width/2, bgBtn->getContentSize( ).height/2 });
	

	auto gBtnLabel = CCLabelBMFont::create("G", "bigFont.fnt");
	gBtnLabel->setScale(0.33f); gBtnLabel->setZOrder(1);
	auto gBtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	sco = from<int *>(levelSettingsObject, 0xEC);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	gBtnSprite->setColor({ red, green, blue });
	auto gBtn = gd::CCMenuItemSpriteExtra::create(gBtnSprite, nullptr, this, nullptr);
	gBtn->addChild(gBtnLabel);
	gBtnLabel->setPosition({ bgBtn->getContentSize( ).width/2, bgBtn->getContentSize( ).height/2 });
	

	auto lBtnLabel = CCLabelBMFont::create("L", "bigFont.fnt");
	lBtnLabel->setScale(0.33f); lBtnLabel->setZOrder(1);
	auto lBtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	sco = from<int *>(levelSettingsObject, 0xF0);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	lBtnSprite->setColor({ red, green, blue });
	auto lBtn = gd::CCMenuItemSpriteExtra::create(lBtnSprite, nullptr, this, nullptr);
	lBtn->addChild(lBtnLabel);
	lBtnLabel->setPosition({ bgBtn->getContentSize( ).width/2, bgBtn->getContentSize( ).height/2 });
	

	auto objBtnLabel = CCLabelBMFont::create("Obj", "bigFont.fnt");
	objBtnLabel->setScale(0.33f); objBtnLabel->setZOrder(1);
	auto objBtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	sco = from<int *>(levelSettingsObject, 0xF4);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	objBtnSprite->setColor({ red, green, blue });
	auto objBtn = gd::CCMenuItemSpriteExtra::create(objBtnSprite, nullptr, this, nullptr);
	objBtn->addChild(objBtnLabel);
	objBtnLabel->setPosition({ bgBtn->getContentSize( ).width/2, bgBtn->getContentSize( ).height/2 });
	

	auto dlBtnLabel = CCLabelBMFont::create("3DL", "bigFont.fnt");
	dlBtnLabel->setScale(0.33f); dlBtnLabel->setZOrder(1);
	auto dlBtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	sco = from<int *>(levelSettingsObject, 0xF8);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	dlBtnSprite->setColor({ red, green, blue });
	auto dlBtn = gd::CCMenuItemSpriteExtra::create(dlBtnSprite, nullptr, this, nullptr);
	dlBtn->addChild(dlBtnLabel);
	dlBtnLabel->setPosition({ bgBtn->getContentSize( ).width/2, bgBtn->getContentSize( ).height/2 });
	

	auto c1BtnLabel = CCLabelBMFont::create("1", "bigFont.fnt");
	c1BtnLabel->setScale(0.33f); c1BtnLabel->setZOrder(1);
	auto c1BtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	sco = from<int *>(levelSettingsObject, 0xFC);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c1BtnSprite->setColor({ red, green, blue });
	auto c1Btn = gd::CCMenuItemSpriteExtra::create(c1BtnSprite, nullptr, this, nullptr);
	c1Btn->addChild(c1BtnLabel);
	c1BtnLabel->setPosition({ bgBtn->getContentSize( ).width/2, bgBtn->getContentSize( ).height/2 });
	

	auto c2BtnLabel = CCLabelBMFont::create("2", "bigFont.fnt");
	c2BtnLabel->setScale(0.33f); c2BtnLabel->setZOrder(1);
	auto c2BtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	sco = from<int *>(levelSettingsObject, 0x100);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c2BtnSprite->setColor({ red, green, blue });
	auto c2Btn = gd::CCMenuItemSpriteExtra::create(c2BtnSprite, nullptr, this, nullptr);
	c2Btn->addChild(c2BtnLabel);
	c2BtnLabel->setPosition({ bgBtn->getContentSize( ).width/2, bgBtn->getContentSize( ).height/2 });
	

	auto c3BtnLabel = CCLabelBMFont::create("3", "bigFont.fnt");
	c3BtnLabel->setScale(0.33f); c3BtnLabel->setZOrder(1);
	auto c3BtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	sco = from<int *>(levelSettingsObject, 0x104);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c3BtnSprite->setColor({ red, green, blue });
	auto c3Btn = gd::CCMenuItemSpriteExtra::create(c3BtnSprite, nullptr, this, nullptr);
	c3Btn->addChild(c3BtnLabel);
	c3BtnLabel->setPosition({ bgBtn->getContentSize( ).width/2, bgBtn->getContentSize( ).height/2 });


	auto c4BtnLabel = CCLabelBMFont::create("4", "bigFont.fnt");
	c4BtnLabel->setScale(0.33f); c4BtnLabel->setZOrder(1);
	auto c4BtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	sco = from<int *>(levelSettingsObject, 0x108);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c4BtnSprite->setColor({ red, green, blue });
	auto c4Btn = gd::CCMenuItemSpriteExtra::create(c4BtnSprite, nullptr, this, nullptr);
	c4Btn->addChild(c4BtnLabel);
	c4BtnLabel->setPosition({ bgBtn->getContentSize( ).width/2, bgBtn->getContentSize( ).height/2 });
	

	auto defBtnLabel = CCLabelBMFont::create("Def", "bigFont.fnt");
	defBtnLabel->setScale(0.33f); defBtnLabel->setZOrder(1);
	auto defBtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	defBtnSprite->setColor({ 255, 255, 255 });
	defBtnSprite->setOpacity(200);
	auto defBtn = gd::CCMenuItemSpriteExtra::create(defBtnSprite, nullptr, this, nullptr);
	defBtn->addChild(defBtnLabel);
	defBtnLabel->setPosition({ bgBtn->getContentSize( ).width/2, bgBtn->getContentSize( ).height/2 });


	auto p1BtnLabel = CCLabelBMFont::create("P1", "bigFont.fnt");
	p1BtnLabel->setScale(0.33f); p1BtnLabel->setZOrder(1);
	auto p1BtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	p1BtnSprite->setColor(lel->getPlayer1( )->getFirstColor( ));
	auto p1Btn = gd::CCMenuItemSpriteExtra::create(p1BtnSprite, nullptr, this, nullptr);
	p1Btn->addChild(p1BtnLabel);
	p1BtnLabel->setPosition({ bgBtn->getContentSize( ).width/2, bgBtn->getContentSize( ).height/2 });


	auto p2BtnLabel = CCLabelBMFont::create("P2", "bigFont.fnt");
	p2BtnLabel->setScale(0.33f); p2BtnLabel->setZOrder(1);
	auto p2BtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	p2BtnSprite->setColor(lel->getPlayer1( )->getSecondColor( ));
	auto p2Btn = gd::CCMenuItemSpriteExtra::create(p2BtnSprite, nullptr, this, nullptr);
	p2Btn->addChild(p2BtnLabel);
	p2BtnLabel->setPosition({ bgBtn->getContentSize( ).width/2, bgBtn->getContentSize( ).height/2 });
	

	auto lbgBtnLabel = CCLabelBMFont::create("LBG", "bigFont.fnt");
	lbgBtnLabel->setScale(0.33f); lbgBtnLabel->setZOrder(1);
	auto lbgBtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	lbgBtnSprite->setColor({
		from<unsigned char>(from<int *>(from<gd::LevelSettingsObject *>(lel, 0x190), 232), 0xE8),
		from<unsigned char>(from<int *>(from<gd::LevelSettingsObject *>(lel, 0x190), 232), 0xE9),
		from<unsigned char>(from<int *>(from<gd::LevelSettingsObject *>(lel, 0x190), 232), 0xEA)
	});
	lbgBtnSprite->setOpacity(200);
	auto lbgBtn = gd::CCMenuItemSpriteExtra::create(lbgBtnSprite, nullptr, this, nullptr);
	lbgBtn->addChild(lbgBtnLabel);
	lbgBtnLabel->setPosition({ bgBtn->getContentSize( ).width/2, bgBtn->getContentSize( ).height/2 });


	colorsMenu->addChild(bgBtn);
	colorsMenu->addChild(gBtn);
	colorsMenu->addChild(lBtn);
	colorsMenu->addChild(objBtn);
	colorsMenu->addChild(dlBtn);
	colorsMenu->addChild(c1Btn);
	colorsMenu->addChild(c2Btn);
	colorsMenu->addChild(c3Btn);
	colorsMenu->addChild(c4Btn);
	colorsMenu->addChild(defBtn);
	colorsMenu->addChild(p1Btn);
	colorsMenu->addChild(p2Btn);
	colorsMenu->addChild(lbgBtn);
	
	

	defBtn->setPosition({ -105.f, 37.5f });
	objBtn->setPosition({ -67.5f, 37.5f });
	dlBtn->setPosition({ -30.f, 37.5f });

	bgBtn->setPosition({ -105.f, 0.f });
	gBtn->setPosition({ -67.5f, 0.f });
	lBtn->setPosition({ -30.f, 0.f });

	p1Btn->setPosition({ -105.f, -37.5f });
	p2Btn->setPosition({ -67.5f, -37.5f });
	lbgBtn->setPosition({ -30.f, -37.5f });

	c1Btn->setPosition({ 46.25f, 18.75f });
	c2Btn->setPosition({ 83.75f, 18.75f });
	c3Btn->setPosition({ 46.25f, -18.75f });
	c4Btn->setPosition({ 83.75f, -18.75f });



	auto selectSprite = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	selectSprite->setZOrder(1);
	selectSprite->setPosition({ -105.f, 37.5f });

	colorsMenu->addChild(selectSprite);
	this->addChild(colorsMenu);



	auto cancelSprite = gd::ButtonSprite::create("OK", 40, 0, 2.5f, true, "goldFont.fnt", "GJ_button_01.png", 30.0);
	auto cancelButton = gd::CCMenuItemSpriteExtra::create(cancelSprite, nullptr, this, menu_selector(MyCustomizeObjectLayer::okButtonCallback));
	cancelButton->setPosition(0, 0);

	auto bottomMenu = CCMenu::create( );
	bottomMenu->setPosition({ (CCDirector::sharedDirector()->getScreenRight())/2, (CCDirector::sharedDirector( )->getScreenTop())/2-85});
	bottomMenu->setZOrder(3);
	bottomMenu->addChild(cancelButton);
	this->addChild(bottomMenu);

	cocos2d::extension::CCScale9Sprite *bg = cocos2d::extension::CCScale9Sprite::create("GJ_square01.png");
	auto director = CCDirector::sharedDirector( );
	bg->setContentSize({ 320, 220 });
	bg->setPosition((director->getScreenRight( ))/2, (director->getScreenTop( ))/2);
	bg->setZOrder(2);
	this->addChild(bg);
	auto appearAction = CCEaseBackOut::create(CCScaleTo::create(.25f, 1.f));

	cocos2d::extension::CCScale9Sprite *colorBtnBg = cocos2d::extension::CCScale9Sprite::create("square02_001.png");
	colorBtnBg->setContentSize({ 260, 120 });
	colorBtnBg->setPosition((director->getScreenRight( ))/2, (director->getScreenTop( ))/2);
	colorBtnBg->setOpacity(100);
	colorBtnBg->setZOrder(3);
	this->addChild(colorBtnBg);

	auto gravityLine = CCSprite::create("gravityLine_001.png");
	gravityLine->setPosition({ (director->getScreenRight( ))/2, (director->getScreenTop( ))/2 });
	gravityLine->setScale(5.5f);
	gravityLine->setZOrder(4);
	this->addChild(gravityLine);

	this->setScale(0.1f);
	this->runAction(appearAction);

	return true;
}

MyCustomizeObjectLayer* MyCustomizeObjectLayer::create(gd::LevelEditorLayer* lel)
{
	MyCustomizeObjectLayer *layer = new MyCustomizeObjectLayer;
	if ( layer->init(lel) ) layer->autorelease( );
	else CC_SAFE_DELETE(layer);
	return layer;
}

void MyCustomizeObjectLayer::onPashalko(CCObject *btn) {
	auto director = CCDirector::sharedDirector( );
	auto myLayer = from<gd::EditorUI *>(btn, 0xFC)->getLevelEditorLayer( );
	auto myCustLayer = MyCustomizeObjectLayer::create(myLayer);
	myCustLayer->setZOrder(100000);
	
	//auto myLayer = static_cast< CCLayer * >(static_cast< CCNode * >(btn)->getUserObject( ));

	myLayer->addChild(myCustLayer);
}

void MyCustomizeObjectLayer::okButtonCallback(CCObject *btn) {
	this->removeMeAndCleanup( );
}