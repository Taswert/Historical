#include "includes.h"
#include "MyCustomizeObjectLayer.h"
#include "state.h"

class ColorButtonSpriteExtra : public CCMenuItemSprite {
public:
	CCLabelBMFont *label;
	CCSprite *selectSprite;
	CCSprite *btnSprite;
	bool isSelected;
	short id;

	virtual void selected( ) {
		this->cocos2d::CCMenuItemSprite::selected( );
		std::cout<<"I am selected!"<<std::endl;
		this->stopActionByTag(0);
		auto actionInterval = cocos2d::CCScaleTo::create(0.3, 2.f);
		auto action = cocos2d::CCEaseBounceOut::create(actionInterval);
		action->setTag(0);
		this->runAction(action);
	}

	static ColorButtonSpriteExtra *create(const char* lbl, CCLayer* layer, int id) {
		//sprite
		auto btnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
		auto btnSpriteSelected = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
		//btnSpriteSelected->setColor({ btnSprite->getColor().r, btnSprite->getColor( ).g, btnSprite->getColor( ).b });

		//label
		auto label = CCLabelBMFont::create(lbl, "bigFont.fnt");
		label->setScale(0.33f);
		label->setZOrder(1);
		auto btn = reinterpret_cast< ColorButtonSpriteExtra * >(CCMenuItemSprite::create(btnSprite, btnSpriteSelected, layer, menu_selector(MyCustomizeObjectLayer::onColorBtn)));
		btn->addChild(label);
		label->setPosition({ btn->getContentSize( ).width/2, btn->getContentSize( ).height/2 });
		
		//selectSprite
		auto selectSprite = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
		selectSprite->setZOrder(1);
		selectSprite->setTag(100);
		selectSprite->setVisible(0);
		btn->addChild(selectSprite);
		selectSprite->setPosition({ btn->getContentSize( ).width/2, btn->getContentSize( ).height/2 });

		btn->isSelected = false;
		btn->label = label;
		btn->selectSprite = selectSprite;
		btn->btnSprite = btnSprite;
		btn->id = id;
		

		return btn;
	}

	void select( ) {
		selectSprite->setVisible(1);
		isSelected = 1;
	}

	void deselect( ) {
		selectSprite->setVisible(0);
		isSelected = 0;
	}
};

std::vector<ColorButtonSpriteExtra *> colorBtnVec;
ColorButtonSpriteExtra *highlightedBtn = nullptr;
CCArray* selectedObjects;
gd::GameObject* selectedObject;

void MyCustomizeObjectLayer::keyBackClicked( )
{
	selectedObjects = nullptr;
	selectedObject = nullptr;
	highlightedBtn = nullptr;
	colorBtnVec.clear( );
	this->removeMeAndCleanup( );
}

void MyCustomizeObjectLayer::onColorBtn(CCObject *btn) {
	if ( !colorBtnVec.empty( ) ) {
		if ( highlightedBtn!=nullptr )
			highlightedBtn->deselect( );
		auto darealbtn = static_cast< ColorButtonSpriteExtra *>(btn);
		darealbtn->select( );

		if ( selectedObject!=nullptr ) {
			selectedObject->setActiveColor(darealbtn->id);
			//if (darealbtn->id == 8)
			//	selectedObject
		}
		else if ( selectedObjects!=nullptr ) {
			for ( int i = 0; i<selectedObjects->count( ); i++ ) {
				auto currObj = reinterpret_cast< gd::GameObject * >(selectedObjects->objectAtIndex(i));
				if ( setting( ).anyEdit || currObj->getDefaultColor( )!=0 )
					currObj->setActiveColor(darealbtn->id);
			}
		}

		highlightedBtn = darealbtn;
	}
}

bool MyCustomizeObjectLayer::init(gd::LevelEditorLayer *lel, CCArray *selObjs, gd::GameObject *selObj)
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

	auto bgSprite = CCSprite::create("GJ_button_03.png");
	bgSprite->setScale(100.f);
	bgSprite->setOpacity(0);
	auto bgButtonLol = gd::CCMenuItemSpriteExtra::create(bgSprite, nullptr, this, nullptr);
	auto bgMenu = CCMenu::create( );
	bgMenu->addChild(bgButtonLol);
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


	auto levelSettingsObject = from<gd::LevelSettingsObject *>(lel, 0x190);
	int *sco = from<int *>(levelSettingsObject, 0xE8);
	auto red = from<unsigned char>(sco, 0xE8);
	auto green = from<unsigned char>(sco, 0xE9);
	auto blue = from<unsigned char>(sco, 0xEA);

	auto bgButton = ColorButtonSpriteExtra::create("BG", this, 0);
	bgButton->btnSprite->setColor({ red, green, blue });

	auto gButton = ColorButtonSpriteExtra::create("G", this, 0);
	sco = from<int *>(levelSettingsObject, 0xEC);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	gButton->btnSprite->setColor({ red, green, blue });

	auto lButton = ColorButtonSpriteExtra::create("L", this, 0);
	sco = from<int *>(levelSettingsObject, 0xF0);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	lButton->btnSprite->setColor({ red, green, blue });

	auto objButton = ColorButtonSpriteExtra::create("Obj", this, 0);
	sco = from<int *>(levelSettingsObject, 0xF4);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	objButton->btnSprite->setColor({ red, green, blue });

	auto dlButton = ColorButtonSpriteExtra::create("3DL", this, 8);
	sco = from<int *>(levelSettingsObject, 0xF8);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	dlButton->btnSprite->setColor({ red, green, blue });

	auto c1Button = ColorButtonSpriteExtra::create("1", this, 3);
	sco = from<int *>(levelSettingsObject, 0xFC);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c1Button->btnSprite->setColor({ red, green, blue });

	auto c2Button = ColorButtonSpriteExtra::create("2", this, 4);
	sco = from<int *>(levelSettingsObject, 0x100);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c2Button->btnSprite->setColor({ red, green, blue });

	auto c3Button = ColorButtonSpriteExtra::create("3", this, 6);
	sco = from<int *>(levelSettingsObject, 0x104);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c3Button->btnSprite->setColor({ red, green, blue });

	auto c4Button = ColorButtonSpriteExtra::create("4", this, 7);
	sco = from<int *>(levelSettingsObject, 0x108);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c4Button->btnSprite->setColor({ red, green, blue });

	auto defButton = ColorButtonSpriteExtra::create("Def", this, 0);
	defButton->btnSprite->setColor({ 255, 255, 255 });
	defButton->btnSprite->setOpacity(150);

	auto p1Button = ColorButtonSpriteExtra::create("P1", this, 1);
	p1Button->btnSprite->setColor(lel->getPlayer1( )->getFirstColor( ));

	auto p2Button = ColorButtonSpriteExtra::create("P2", this, 2);
	p2Button->btnSprite->setColor(lel->getPlayer1( )->getSecondColor( ));

	auto lbgButton = ColorButtonSpriteExtra::create("LBG", this, 5);
	sco = from<int *>(levelSettingsObject, 0xE8);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	lbgButton->btnSprite->setColor({ red, green, blue });
	lbgButton->btnSprite->setOpacity(150);


	/*auto bgBtnLabel = CCLabelBMFont::create("BG", "bigFont.fnt");
	bgBtnLabel->setScale(0.33f);
	bgBtnLabel->setZOrder(1);
	auto bgBtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	auto levelSettingsObject = from<gd::LevelSettingsObject *>(lel, 0x190);
	int *sco = from<int *>(levelSettingsObject, 0xE8);
	auto red = from<unsigned char>(sco, 0xE8);
	auto green = from<unsigned char>(sco, 0xE9);
	auto blue = from<unsigned char>(sco, 0xEA);
	bgBtnSprite->setColor({ red, green, blue });
	auto bgBtn = gd::CCMenuItemSpriteExtra::create(bgBtnSprite, nullptr, this, menu_selector(MyCustomizeObjectLayer::onColorBtn));
	bgBtn->addChild(bgBtnLabel);
	bgBtnLabel->setPosition({ bgBtn->getContentSize( ).width/2, bgBtn->getContentSize( ).height/2 });
	auto bgBtnSelect = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	bgBtnSelect->setZOrder(1);
	bgBtnSelect->setTag(100);
	bgBtnSelect->setVisible(0);
	bgBtn->addChild(bgBtnSelect);
	bgBtnSelect->setPosition({ bgBtn->getContentSize( ).width/2, bgBtn->getContentSize( ).height/2 });
	

	auto gBtnLabel = CCLabelBMFont::create("G", "bigFont.fnt");
	gBtnLabel->setScale(0.33f); gBtnLabel->setZOrder(1);
	auto gBtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	sco = from<int *>(levelSettingsObject, 0xEC);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	gBtnSprite->setColor({ red, green, blue });
	auto gBtn = gd::CCMenuItemSpriteExtra::create(gBtnSprite, nullptr, this, menu_selector(MyCustomizeObjectLayer::onColorBtn));
	gBtn->addChild(gBtnLabel);
	gBtnLabel->setPosition({ gBtn->getContentSize( ).width/2, gBtn->getContentSize( ).height/2 });
	auto gBtnSelect = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	gBtnSelect->setZOrder(1);
	gBtnSelect->setTag(100);
	gBtnSelect->setVisible(0);
	gBtn->addChild(gBtnSelect);
	gBtnSelect->setPosition({ gBtn->getContentSize( ).width/2, gBtn->getContentSize( ).height/2 });
	

	auto lBtnLabel = CCLabelBMFont::create("L", "bigFont.fnt");
	lBtnLabel->setScale(0.33f); lBtnLabel->setZOrder(1);
	auto lBtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	sco = from<int *>(levelSettingsObject, 0xF0);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	lBtnSprite->setColor({ red, green, blue });
	auto lBtn = gd::CCMenuItemSpriteExtra::create(lBtnSprite, nullptr, this, menu_selector(MyCustomizeObjectLayer::onColorBtn));
	lBtn->addChild(lBtnLabel);
	lBtnLabel->setPosition({ lBtn->getContentSize( ).width/2, lBtn->getContentSize( ).height/2 });
	auto lBtnSelect = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	lBtnSelect->setZOrder(1);
	lBtnSelect->setTag(100);
	lBtnSelect->setVisible(0);
	lBtn->addChild(lBtnSelect);
	lBtnSelect->setPosition({ lBtn->getContentSize( ).width/2, lBtn->getContentSize( ).height/2 });
	

	auto objBtnLabel = CCLabelBMFont::create("Obj", "bigFont.fnt");
	objBtnLabel->setScale(0.33f); objBtnLabel->setZOrder(1);
	auto objBtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	sco = from<int *>(levelSettingsObject, 0xF4);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	objBtnSprite->setColor({ red, green, blue });
	auto objBtn = gd::CCMenuItemSpriteExtra::create(objBtnSprite, nullptr, this, menu_selector(MyCustomizeObjectLayer::onColorBtn));
	objBtn->addChild(objBtnLabel);
	objBtnLabel->setPosition({ objBtn->getContentSize( ).width/2, objBtn->getContentSize( ).height/2 });
	auto objBtnSelect = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	objBtnSelect->setZOrder(1);
	objBtnSelect->setTag(100);
	objBtnSelect->setVisible(0);
	objBtn->addChild(objBtnSelect);
	objBtnSelect->setPosition({ objBtn->getContentSize( ).width/2, objBtn->getContentSize( ).height/2 });
	

	auto dlBtnLabel = CCLabelBMFont::create("3DL", "bigFont.fnt");
	dlBtnLabel->setScale(0.33f); dlBtnLabel->setZOrder(1);
	auto dlBtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	sco = from<int *>(levelSettingsObject, 0xF8);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	dlBtnSprite->setColor({ red, green, blue });
	auto dlBtn = gd::CCMenuItemSpriteExtra::create(dlBtnSprite, nullptr, this, menu_selector(MyCustomizeObjectLayer::onColorBtn));
	dlBtn->addChild(dlBtnLabel);
	dlBtnLabel->setPosition({ dlBtn->getContentSize( ).width/2, dlBtn->getContentSize( ).height/2 });
	auto dlBtnSelect = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	dlBtnSelect->setZOrder(1);
	dlBtnSelect->setTag(100);
	dlBtnSelect->setVisible(0);
	dlBtn->addChild(dlBtnSelect);
	dlBtnSelect->setPosition({ dlBtn->getContentSize( ).width/2, dlBtn->getContentSize( ).height/2 });
	

	auto c1BtnLabel = CCLabelBMFont::create("1", "bigFont.fnt");
	c1BtnLabel->setScale(0.33f); c1BtnLabel->setZOrder(1);
	auto c1BtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	sco = from<int *>(levelSettingsObject, 0xFC);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c1BtnSprite->setColor({ red, green, blue });
	auto c1Btn = gd::CCMenuItemSpriteExtra::create(c1BtnSprite, nullptr, this, menu_selector(MyCustomizeObjectLayer::onColorBtn));
	c1Btn->addChild(c1BtnLabel);
	c1BtnLabel->setPosition({ c1Btn->getContentSize( ).width/2, c1Btn->getContentSize( ).height/2 });
	auto c1BtnSelect = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	c1BtnSelect->setZOrder(1);
	c1BtnSelect->setTag(100);
	c1BtnSelect->setVisible(0);
	c1Btn->addChild(c1BtnSelect);
	c1BtnSelect->setPosition({ c1Btn->getContentSize( ).width/2, c1Btn->getContentSize( ).height/2 });
	

	auto c2BtnLabel = CCLabelBMFont::create("2", "bigFont.fnt");
	c2BtnLabel->setScale(0.33f); c2BtnLabel->setZOrder(1);
	auto c2BtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	sco = from<int *>(levelSettingsObject, 0x100);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c2BtnSprite->setColor({ red, green, blue });
	auto c2Btn = gd::CCMenuItemSpriteExtra::create(c2BtnSprite, nullptr, this, menu_selector(MyCustomizeObjectLayer::onColorBtn));
	c2Btn->addChild(c2BtnLabel);
	c2BtnLabel->setPosition({ c2Btn->getContentSize( ).width/2, c2Btn->getContentSize( ).height/2 });
	auto c2BtnSelect = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	c2BtnSelect->setZOrder(1);
	c2BtnSelect->setTag(100);
	c2BtnSelect->setVisible(0);
	c2Btn->addChild(c2BtnSelect);
	c2BtnSelect->setPosition({ c2Btn->getContentSize( ).width/2, c2Btn->getContentSize( ).height/2 });
	

	auto c3BtnLabel = CCLabelBMFont::create("3", "bigFont.fnt");
	c3BtnLabel->setScale(0.33f); c3BtnLabel->setZOrder(1);
	auto c3BtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	sco = from<int *>(levelSettingsObject, 0x104);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c3BtnSprite->setColor({ red, green, blue });
	auto c3Btn = gd::CCMenuItemSpriteExtra::create(c3BtnSprite, nullptr, this, menu_selector(MyCustomizeObjectLayer::onColorBtn));
	c3Btn->addChild(c3BtnLabel);
	c3BtnLabel->setPosition({ c3Btn->getContentSize( ).width/2, c3Btn->getContentSize( ).height/2 });
	auto c3BtnSelect = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	c3BtnSelect->setZOrder(1);
	c3BtnSelect->setTag(100);
	c3BtnSelect->setVisible(0);
	c3Btn->addChild(c3BtnSelect);
	c3BtnSelect->setPosition({ c3Btn->getContentSize( ).width/2, c3Btn->getContentSize( ).height/2 });


	auto c4BtnLabel = CCLabelBMFont::create("4", "bigFont.fnt");
	c4BtnLabel->setScale(0.33f); c4BtnLabel->setZOrder(1);
	auto c4BtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	sco = from<int *>(levelSettingsObject, 0x108);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c4BtnSprite->setColor({ red, green, blue });
	auto c4Btn = gd::CCMenuItemSpriteExtra::create(c4BtnSprite, nullptr, this, menu_selector(MyCustomizeObjectLayer::onColorBtn));
	c4Btn->addChild(c4BtnLabel);
	c4BtnLabel->setPosition({ c4Btn->getContentSize( ).width/2, c4Btn->getContentSize( ).height/2 });
	auto c4BtnSelect = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	c4BtnSelect->setZOrder(1);
	c4BtnSelect->setTag(100);
	c4BtnSelect->setVisible(0);
	c4Btn->addChild(c4BtnSelect);
	c4BtnSelect->setPosition({ c4Btn->getContentSize( ).width/2, c4Btn->getContentSize( ).height/2 });
	

	auto defBtnLabel = CCLabelBMFont::create("Def", "bigFont.fnt");
	defBtnLabel->setScale(0.33f); defBtnLabel->setZOrder(1);
	auto defBtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	defBtnSprite->setColor({ 255, 255, 255 });
	defBtnSprite->setOpacity(200);
	auto defBtn = gd::CCMenuItemSpriteExtra::create(defBtnSprite, nullptr, this, menu_selector(MyCustomizeObjectLayer::onColorBtn));
	defBtn->addChild(defBtnLabel);
	defBtnLabel->setPosition({ defBtn->getContentSize( ).width/2, defBtn->getContentSize( ).height/2 });
	auto defBtnSelect = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	defBtnSelect->setZOrder(1);
	defBtnSelect->setTag(100);
	defBtnSelect->setVisible(0);
	defBtn->addChild(defBtnSelect);
	defBtnSelect->setPosition({ defBtn->getContentSize( ).width/2, defBtn->getContentSize( ).height/2 });


	auto p1BtnLabel = CCLabelBMFont::create("P1", "bigFont.fnt");
	p1BtnLabel->setScale(0.33f); p1BtnLabel->setZOrder(1);
	auto p1BtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	p1BtnSprite->setColor(lel->getPlayer1( )->getFirstColor( ));
	auto p1Btn = gd::CCMenuItemSpriteExtra::create(p1BtnSprite, nullptr, this, menu_selector(MyCustomizeObjectLayer::onColorBtn));
	p1Btn->addChild(p1BtnLabel);
	p1BtnLabel->setPosition({ p1Btn->getContentSize( ).width/2, p1Btn->getContentSize( ).height/2 });
	auto p1BtnSelect = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	p1BtnSelect->setZOrder(1);
	p1BtnSelect->setTag(100);
	p1BtnSelect->setVisible(0);
	p1Btn->addChild(p1BtnSelect);
	p1BtnSelect->setPosition({ p1Btn->getContentSize( ).width/2, p1Btn->getContentSize( ).height/2 });


	auto p2BtnLabel = CCLabelBMFont::create("P2", "bigFont.fnt");
	p2BtnLabel->setScale(0.33f); p2BtnLabel->setZOrder(1);
	auto p2BtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	p2BtnSprite->setColor(lel->getPlayer1( )->getSecondColor( ));
	auto p2Btn = gd::CCMenuItemSpriteExtra::create(p2BtnSprite, nullptr, this, menu_selector(MyCustomizeObjectLayer::onColorBtn));
	p2Btn->addChild(p2BtnLabel);
	p2BtnLabel->setPosition({ p2Btn->getContentSize( ).width/2, p2Btn->getContentSize( ).height/2 });
	auto p2BtnSelect = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	p2BtnSelect->setZOrder(1);
	p2BtnSelect->setTag(100);
	p2BtnSelect->setVisible(0);
	p2Btn->addChild(p2BtnSelect);
	p2BtnSelect->setPosition({ p2Btn->getContentSize( ).width/2, p2Btn->getContentSize( ).height/2 });
	

	auto lbgBtnLabel = CCLabelBMFont::create("LBG", "bigFont.fnt");
	lbgBtnLabel->setScale(0.33f); lbgBtnLabel->setZOrder(1);
	auto lbgBtnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
	lbgBtnSprite->setColor({
		from<unsigned char>(from<int *>(from<gd::LevelSettingsObject *>(lel, 0x190), 232), 0xE8),
		from<unsigned char>(from<int *>(from<gd::LevelSettingsObject *>(lel, 0x190), 232), 0xE9),
		from<unsigned char>(from<int *>(from<gd::LevelSettingsObject *>(lel, 0x190), 232), 0xEA)
	});
	lbgBtnSprite->setOpacity(200);
	auto lbgBtn = gd::CCMenuItemSpriteExtra::create(lbgBtnSprite, nullptr, this, menu_selector(MyCustomizeObjectLayer::onColorBtn));
	lbgBtn->addChild(lbgBtnLabel);
	lbgBtnLabel->setPosition({ lbgBtn->getContentSize( ).width/2, lbgBtn->getContentSize( ).height/2 });
	auto lbgBtnSelect = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	lbgBtnSelect->setZOrder(1);
	lbgBtnSelect->setTag(100);
	lbgBtnSelect->setVisible(0);
	lbgBtn->addChild(lbgBtnSelect);
	lbgBtnSelect->setPosition({ lbgBtn->getContentSize( ).width/2, lbgBtn->getContentSize( ).height/2 });*/


	colorsMenu->addChild(bgButton);
	colorsMenu->addChild(gButton);
	colorsMenu->addChild(lButton);
	colorsMenu->addChild(objButton);
	colorsMenu->addChild(dlButton);
	colorsMenu->addChild(c1Button);
	colorsMenu->addChild(c2Button);
	colorsMenu->addChild(c3Button);
	colorsMenu->addChild(c4Button);
	colorsMenu->addChild(defButton);
	colorsMenu->addChild(p1Button);
	colorsMenu->addChild(p2Button);
	colorsMenu->addChild(lbgButton);
	
	

	defButton->setPosition({ -105.f, 37.5f });
	objButton->setPosition({ -67.5f, 37.5f });
	dlButton->setPosition({ -30.f, 37.5f });

	bgButton->setPosition({ -105.f, 0.f });
	gButton->setPosition({ -67.5f, 0.f });
	lButton->setPosition({ -30.f, 0.f });

	p1Button->setPosition({ -105.f, -37.5f });
	p2Button->setPosition({ -67.5f, -37.5f });
	lbgButton->setPosition({ -30.f, -37.5f });

	c1Button->setPosition({ 46.25f, 18.75f });
	c2Button->setPosition({ 83.75f, 18.75f });
	c3Button->setPosition({ 46.25f, -18.75f });
	c4Button->setPosition({ 83.75f, -18.75f });

	colorBtnVec.push_back(bgButton); colorBtnVec.push_back(gButton); colorBtnVec.push_back(lButton); colorBtnVec.push_back(objButton); colorBtnVec.push_back(dlButton);
	colorBtnVec.push_back(c1Button); colorBtnVec.push_back(c2Button); colorBtnVec.push_back(c3Button); colorBtnVec.push_back(c4Button); 
	colorBtnVec.push_back(defButton); colorBtnVec.push_back(p1Button); colorBtnVec.push_back(p2Button); colorBtnVec.push_back(lbgButton);

	this->addChild(colorsMenu);


	if ( selObj==nullptr ) {
		highlightedBtn = defButton;
		defButton->select( );
	}
	else {
		int active = selObj->getActiveColor( );
		switch ( active )
		{
		case 0:
			highlightedBtn = defButton;
			defButton->select( );
			break;
		case 1:
			highlightedBtn = p1Button;
			p1Button->select( );
			break;
		case 2:
			highlightedBtn = p2Button;
			p2Button->select( );
			break;
		case 3:
			highlightedBtn = c1Button;
			c1Button->select( );
			break;
		case 4:
			highlightedBtn = c2Button;
			c2Button->select( );
			break;
		case 5:
			highlightedBtn = lbgButton;
			lbgButton->select( );
			break;
		case 6:
			highlightedBtn = c3Button;
			c3Button->select( );
			break;
		case 7:
			highlightedBtn = c4Button;
			c4Button->select( );
			break;
		case 8:
			highlightedBtn = dlButton;
			dlButton->select( );
			break;
		default:
			break;
		}
	}

	selectedObjects = selObjs;
	selectedObject = selObj;


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

MyCustomizeObjectLayer* MyCustomizeObjectLayer::create(gd::LevelEditorLayer* lel, CCArray* selObjs, gd::GameObject* selObj)
{
	MyCustomizeObjectLayer *layer = new MyCustomizeObjectLayer;
	if ( layer->init(lel, selObjs, selObj) ) layer->autorelease( );
	else CC_SAFE_DELETE(layer);
	return layer;
}

void MyCustomizeObjectLayer::onEdit(CCObject *btn) {
	auto editUI = from<gd::EditorUI *>(btn, 0xFC);

	auto director = CCDirector::sharedDirector( );
	auto myLayer = editUI->getLevelEditorLayer( );
	auto myCustLayer = MyCustomizeObjectLayer::create(myLayer, editUI->getSelectedObjectsOfCCArray( ), editUI->getSingleSelectedObj( ));
	myCustLayer->setZOrder(100000);
	
	//auto myLayer = static_cast< CCLayer * >(static_cast< CCNode * >(btn)->getUserObject( ));

	myLayer->addChild(myCustLayer);
}

void MyCustomizeObjectLayer::okButtonCallback(CCObject *btn) {
	selectedObjects = nullptr;
	selectedObject = nullptr;
	highlightedBtn = nullptr;
	colorBtnVec.clear( );
	this->removeMeAndCleanup( );
}