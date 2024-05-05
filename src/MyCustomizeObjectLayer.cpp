#include "includes.h"
#include "MyCustomizeObjectLayer.h"
#include "state.h"
#include "ObjectsIds.h"
#include <GUI/CCControlExtension/CCControl.h>

void MyCustomizeObjectLayer::onColInc(CCObject *btn) {
	auto editUI = from<gd::EditorUI*>(btn, 0xFC);
	auto selectedObjects1 = editUI->getSelectedObjectsOfCCArray( );
	editUI->getLevelEditorLayer( )->createObject(142, 200.f, 200.f);
	if ( editUI->getSingleSelectedObj()!=nullptr ) {
		editUI->getSingleSelectedObj( )->setActiveColor(editUI->getSingleSelectedObj( )->getActiveColor( )+1);
	}
	else if ( selectedObjects1->count( )!=0 ) {
		int maxColId = 0;
		for ( int i = 0; i < selectedObjects1->count( ); i++ ) {
			if ( reinterpret_cast< gd::GameObject * >(selectedObjects1->objectAtIndex(i))->getActiveColor( )>maxColId )
				maxColId = reinterpret_cast< gd::GameObject * >(selectedObjects1->objectAtIndex(i))->getActiveColor( );
		}
		maxColId++;
		for ( int i = 0; i<selectedObjects1->count( ); i++ ) {
			reinterpret_cast< gd::GameObject * >(selectedObjects1->objectAtIndex(i))->setActiveColor(maxColId);
			maxColId++;
		}
	}
}

int getColorForPopup(int id) {
	switch ( id )
	{
	case 0:
	case 1:
	case 2:
	case 5:
		break;
	case 3:
		return 4;
	case 4:
		return 5;
	case 6:
		return 6;
	case 7:
		return 7;
	case 8:
		return 8;
	default:
		break;
	}
	return 9;
}

int getOffsetForSco(int id) {
	switch ( id )
	{
	case 3:
		return 0xFC;
	case 4:
		return 0x100;
	case 6:
		return 0x104;
	case 7:
		return 0x108;
	case 8:
		return 0xF8;
	default:
		break;
	}
	return 9;
}

class ColorButtonSpriteExtra : public gd::CCMenuItemSpriteExtra {
public:
	CCLabelBMFont *label;
	CCSprite *selectSprite;
	CCSprite *btnSprite;
	bool isSelected;
	bool isSpecial;
	short id;

	virtual void selected( ) override {
		this->stopActionByTag(0);
		auto actionInterval = cocos2d::CCScaleTo::create(0.3, 1.26f);
		auto action = cocos2d::CCEaseBounceOut::create(actionInterval);
		action->setTag(0);
		this->runAction(action);
	}

	virtual void unselected( ) override {
		this->stopActionByTag(0);
		float f = 0.f;
		
		//if (this->) {
		//	f = 0.4f;
		//}
		auto actionInterval = cocos2d::CCScaleTo::create(f, 1.f);
		auto action = cocos2d::CCEaseBounceOut::create(actionInterval);
		action->setTag(0);
		this->runAction(action);
	}

	static ColorButtonSpriteExtra *create(const char *lbl, CCLayer *layer, int id, bool isSpecial) {
		auto ret = new ColorButtonSpriteExtra( );
		if ( ret&&ret->init(lbl, layer, id, isSpecial) ) {
			ret->autorelease( );
		}
		else {
			delete ret;
			ret = nullptr;
		}
		return ret;
	}

	bool init(const char *lbl, CCLayer *layer, int id, bool isSpecial) {
		//sprite

		auto btnSprite = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
		auto btnSpriteSelected = CCSprite::createWithSpriteFrameName("playerSquare_001.png");
		CCMenuItemSprite::initWithNormalSprite(btnSprite, btnSpriteSelected, btnSpriteSelected, layer, menu_selector(MyCustomizeObjectLayer::onColorBtn));
		//btnSpriteSelected->setColor({ btnSprite->getColor().r, btnSprite->getColor( ).g, btnSprite->getColor( ).b });

		//label
		auto label = CCLabelBMFont::create(lbl, "bigFont.fnt");
		label->setScale(0.33f);
		label->setZOrder(1);
		//auto btn = reinterpret_cast< ColorButtonSpriteExtra * >(CCMenuItemSprite::create(btnSprite, btnSpriteSelected, layer, menu_selector(MyCustomizeObjectLayer::onColorBtn)));
		this->addChild(label);
		label->setPosition({ this->getContentSize( ).width/2, this->getContentSize( ).height/2 });

		//selectSprite
		auto selectSprite = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
		selectSprite->setZOrder(1);
		selectSprite->setTag(100);
		selectSprite->setVisible(0);
		this->addChild(selectSprite);
		selectSprite->setPosition({ this->getContentSize( ).width/2, this->getContentSize( ).height/2 });

		this->isSelected = false;
		this->label = label;
		this->selectSprite = selectSprite;
		this->btnSprite = btnSprite;
		this->id = id;
		this->isSpecial = isSpecial;

		return true;
	}

	//virtual void setColor(const ccColor3B &color) override {
	//	this->btnSprite->setColor(color);
	//}

	void select( ) {
		selectSprite->setVisible(1);
		isSelected = 1;
	}

	void deselect( ) {
		selectSprite->setVisible(0);
		isSelected = 0;
	}
};

class EditColorSelectPopup : public CCLayer, cocos2d::extension::ColorPickerDelegate { //my class
public:
	bool P1bool;
	bool P2bool;
	bool Blendbool;
	gd::SettingsColorObject *settingsColorObject;
	ColorButtonSpriteExtra *cbse;

	static auto create(gd::SettingsColorObject *sco, ColorButtonSpriteExtra* button) {
		auto ret = new EditColorSelectPopup;
		if ( ret&&ret->init(sco, button) ) {
			ret->cocos2d::CCObject::autorelease( );
		}
		else {
			delete ret;
			ret = nullptr;
		}
		return ret;
	}
	bool init(gd::SettingsColorObject *sco, ColorButtonSpriteExtra * button)
	{
		cbse = button;
		settingsColorObject = sco;
		P1bool = false;
		P2bool = false;
		if ( sco->getPlayerMode( )==1 )
			P1bool = true;
		else if ( sco->getPlayerMode( )==2 )
			P2bool = true;
		Blendbool = sco->getBlend( );

		CCLayer::init( );
		CCLayerColor *cclcol = CCLayerColor::create(ccc4(0, 0, 0, 150));
		cclcol->setZOrder(1);

		auto director = CCDirector::sharedDirector( );

		auto touchDispatcher = director->m_pTouchDispatcher;
		touchDispatcher->incrementForcePrio( );
		touchDispatcher->incrementForcePrio( );
		registerWithTouchDispatcher( );
		setTouchEnabled(true);
		setKeypadEnabled(true);
		setMouseEnabled(true);

		this->addChild(cclcol);

		auto bgSprite = CCSprite::create("GJ_button_03.png");
		bgSprite->setScale(100.f);
		bgSprite->setOpacity(0);
		auto bgButton = gd::CCMenuItemSpriteExtra::create(bgSprite, nullptr, this, nullptr);
		auto bgMenu = CCMenu::create( );
		bgMenu->addChild(bgButton);
		bgMenu->setZOrder(0);
		bgMenu->setPosition((CCDirector::sharedDirector( )->getScreenRight( ))-25, (CCDirector::sharedDirector( )->getScreenTop( ))-25);
		this->addChild(bgMenu);

		cocos2d::extension::CCScale9Sprite *colorBtnBg = cocos2d::extension::CCScale9Sprite::create("square02_001.png");
		colorBtnBg->setContentSize({ 300.f, 300.f });
		colorBtnBg->setPosition(director->getScreenRight( )/2, director->getScreenTop( )/2);
		colorBtnBg->setOpacity(150);
		colorBtnBg->setZOrder(2);
		this->addChild(colorBtnBg);



		auto selColLbl = CCLabelBMFont::create("Select Color", "goldFont.fnt");
		selColLbl->setZOrder(3);
		selColLbl->setPosition(director->getScreenRight( )/2, director->getScreenTop( )/2+130.f);
		this->addChild(selColLbl);



		auto mainMenu = CCMenu::create( );
		mainMenu->setZOrder(3);
		auto cancelSprite = gd::ButtonSprite::create("OK", 40, 0, 2.5f, true, "goldFont.fnt", "GJ_button_01.png", 30.0);
		auto cancelButton = gd::CCMenuItemSpriteExtra::create(cancelSprite, nullptr, this, menu_selector(EditColorSelectPopup::onOk));
		cancelButton->setPosition({ 0.f, -130.f });
		mainMenu->addChild(cancelButton);
		mainMenu->setTag(5);
		this->addChild(mainMenu);

		auto copySprite = gd::ButtonSprite::create("Copy", 40, 0, 2.5f, true, "goldFont.fnt", "GJ_button_04.png", 30.0);
		auto copyButton = gd::CCMenuItemSpriteExtra::create(copySprite, nullptr, this, menu_selector(EditColorSelectPopup::onCopyColor));
		copyButton->setPosition({ -50.f+director->getScreenRight( )/2, -30.f+director->getScreenTop( )/2 });
		mainMenu->addChild(copyButton);

		auto pasteSprite = gd::ButtonSprite::create("Paste", 40, 0, 2.5f, true, "goldFont.fnt", "GJ_button_04.png", 30.0);
		auto pasteButton = gd::CCMenuItemSpriteExtra::create(pasteSprite, nullptr, this, menu_selector(EditColorSelectPopup::onPasteColor));
		pasteButton->setPosition({ -50.f+director->getScreenRight( )/2, -70.f+director->getScreenTop( )/2 });
		mainMenu->addChild(pasteButton);



		auto coloredSquareFirst = CCSprite::createWithSpriteFrameName("whiteSquare60_001.png");
		coloredSquareFirst->setPosition({ 30.f, director->getScreenTop( )-30.f });
		coloredSquareFirst->setZOrder(3);
		coloredSquareFirst->setColor(sco->getColorValue( ));
		coloredSquareFirst->setTag(1);
		this->addChild(coloredSquareFirst);

		auto coloredSquareSec = CCSprite::createWithSpriteFrameName("whiteSquare60_001.png");
		coloredSquareSec->setPosition({ 30.f, director->getScreenTop( )-60.f });
		coloredSquareSec->setZOrder(3);
		coloredSquareSec->setColor(sco->getColorValue( ));
		this->addChild(coloredSquareSec);



		auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
		auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");

		auto BlendToggler = gd::CCMenuItemToggler::create(BlendTogglerSprite(toggleOn, toggleOff), BlendTogglerSprite(toggleOff, toggleOn), this, menu_selector(EditColorSelectPopup::BlendCallback));
		BlendToggler->setPosition({ -director->getScreenRight( )/2+30.f, -50.f });
		BlendToggler->setScale(0.8f);
		mainMenu->addChild(BlendToggler);

		auto BlendLbl = CCLabelBMFont::create("Use Blending", "bigFont.fnt");
		BlendLbl->setPosition({ 50.f, director->getScreenTop( )/2-50.f });
		BlendLbl->setAnchorPoint({ 0.f, 0.5f });
		BlendLbl->setZOrder(3);
		BlendLbl->setScale(0.35f);
		this->addChild(BlendLbl);

		auto P1Toggler = gd::CCMenuItemToggler::create(toggleOff, toggleOn, this, menu_selector(EditColorSelectPopup::P1Callback));
		P1Toggler->toggle(P1bool);
		P1Toggler->setPosition({ -director->getScreenRight( )/2+30.f, -90.f });
		P1Toggler->setScale(0.8f);
		P1Toggler->setTag(1);
		mainMenu->addChild(P1Toggler);

		auto P1Lbl = CCLabelBMFont::create("Player\nColor 1", "bigFont.fnt");
		P1Lbl->setPosition({ 50.f, director->getScreenTop( )/2-90.f });
		P1Lbl->setAnchorPoint({ 0.f, 0.5f });
		P1Lbl->setZOrder(3);
		P1Lbl->setScale(0.35f);
		this->addChild(P1Lbl);

		auto P2Toggler = gd::CCMenuItemToggler::create(toggleOff, toggleOn, this, menu_selector(EditColorSelectPopup::P2Callback));
		P2Toggler->toggle(P2bool);
		P2Toggler->setPosition({ -director->getScreenRight( )/2+30.f, -130.f });
		P2Toggler->setScale(0.8f);
		P2Toggler->setTag(2);
		mainMenu->addChild(P2Toggler);

		auto P2Lbl = CCLabelBMFont::create("Player\nColor 2", "bigFont.fnt");
		P2Lbl->setPosition({ 50.f, director->getScreenTop( )/2-130.f });
		P2Lbl->setAnchorPoint({ 0.f, 0.5f });
		P2Lbl->setZOrder(3);
		P2Lbl->setScale(0.35f);
		this->addChild(P2Lbl);



		auto colorPicker = reinterpret_cast< cocos2d::extension::CCControlColourPicker*(__thiscall *)() >(GetProcAddress(GetModuleHandleA("libExtensions.dll"), "?create@CCControlColourPicker@extension@cocos2d@@SAPAV123@XZ"))();
		colorPicker->setPosition({ director->getScreenRight( )/2, director->getScreenTop( )/2 });
		colorPicker->setZOrder(3);
		colorPicker->setTag(3);
		colorPicker->setDelegate(this);
		colorPicker->setColorValue(sco->getColorValue( ));
		this->addChild(colorPicker);

		return true;
	}

	virtual void colorValueChanged(ccColor3B col) override {
		//std::cout<<col.r << col.g << col.b<<std::endl;
		reinterpret_cast< CCSprite * >(getChildByTag(1))->setColor(col);
		settingsColorObject->setColorValue(col);
		//cbse->setColor(col);
	}

	CCSprite *BlendTogglerSprite(CCSprite *toggleOn, CCSprite *toggleOff)
	{
		return Blendbool ? toggleOn : toggleOff;
	}
	void BlendCallback(CCObject *) {
		Blendbool = !Blendbool;
	}
	void P1Callback(CCObject *) {
		P1bool = !P1bool;
		P2bool = false;
		reinterpret_cast< gd::CCMenuItemToggler * >(getChildByTag(5)->getChildByTag(2))->toggle(false);
	}
	void P2Callback(CCObject *) {
		P2bool = !P2bool;
		P1bool = false;
		reinterpret_cast< gd::CCMenuItemToggler * >(getChildByTag(5)->getChildByTag(1))->toggle(false);
	}


	void onOk(CCObject *) {
		keyBackClicked( );
	}
	void onCopyColor(CCObject *) {
		auto col = settingsColorObject->getColorValue( );
		gd::GameManager::sharedState( )->setClipboardedColor(col);
	}
	void onPasteColor(CCObject *) {
		auto col = gd::GameManager::sharedState( )->getClipboardedColor( );
		reinterpret_cast< CCSprite * >(getChildByTag(1))->setColor(col);
		settingsColorObject->setColorValue(col);
		reinterpret_cast< cocos2d::extension::CCControlColourPicker * >(getChildByTag(3))->setColorValue(col);
	}
	virtual void keyBackClicked( ) override {
		settingsColorObject->setBlend(Blendbool);
		if ( P1bool )
			settingsColorObject->setPlayerMode(1);
		else if ( P2bool )
			settingsColorObject->setPlayerMode(2);
		else
			settingsColorObject->setPlayerMode(0);
		cbse->btnSprite->setColor(settingsColorObject->getColorValue( ));
		cbse = nullptr;
		settingsColorObject = nullptr;
		this->removeMeAndCleanup( );
	}
};

class MultipleColorSelectPopup : public CCLayer, cocos2d::extension::ColorPickerDelegate { //my class
public:
	cocos2d::CCArray *editTriggers;
	bool TTbool;
	bool P1bool;
	bool P2bool;
	bool Blendbool;
	float fadeTime;
	bool mixedFadeTime;
	bool mixedBlend;
	bool mixedP1;
	bool mixedP2;
	bool mixedTT;
	bool mixedCol;

	static auto create(cocos2d::CCArray *triggers) {
		auto ret = new MultipleColorSelectPopup;
		if ( ret&&ret->init(triggers) ) {
			ret->cocos2d::CCObject::autorelease( );
		}
		else {
			delete ret;
			ret = nullptr;
		}
		return ret;
	}
	bool init(CCArray *triggers)
	{
		CCLayer::init( );
		CCLayerColor *cclcol = CCLayerColor::create(ccc4(0, 0, 0, 150));
		cclcol->setZOrder(1);

		editTriggers = triggers;
		fadeTime = reinterpret_cast< gd::GameObject * >(triggers->objectAtIndex(0))->getFadeTime( );

		auto director = CCDirector::sharedDirector( );

		auto touchDispatcher = director->m_pTouchDispatcher;
		touchDispatcher->incrementForcePrio( );
		touchDispatcher->incrementForcePrio( );
		registerWithTouchDispatcher( );
		setTouchEnabled(true);
		setKeypadEnabled(true);
		setMouseEnabled(true);

		this->addChild(cclcol);

		auto bgSprite = CCSprite::create("GJ_button_03.png");
		bgSprite->setScale(100.f);
		bgSprite->setOpacity(0);
		auto bgButton = gd::CCMenuItemSpriteExtra::create(bgSprite, nullptr, this, nullptr);
		auto bgMenu = CCMenu::create( );
		bgMenu->addChild(bgButton);
		bgMenu->setZOrder(0);
		bgMenu->setPosition((CCDirector::sharedDirector( )->getScreenRight( ))-25, (CCDirector::sharedDirector( )->getScreenTop( ))-25);
		this->addChild(bgMenu);

		cocos2d::extension::CCScale9Sprite *colorBtnBg = cocos2d::extension::CCScale9Sprite::create("square02_001.png");
		colorBtnBg->setContentSize({ 300.f, 300.f });
		colorBtnBg->setPosition(director->getScreenRight( )/2, director->getScreenTop( )/2);
		colorBtnBg->setOpacity(150);
		colorBtnBg->setZOrder(2);
		this->addChild(colorBtnBg);



		auto selColLbl = CCLabelBMFont::create("Select Color", "goldFont.fnt");
		selColLbl->setZOrder(3);
		selColLbl->setPosition(director->getScreenRight( )/2, director->getScreenTop( )/2+130.f);
		this->addChild(selColLbl);



		auto mainMenu = CCMenu::create( );
		mainMenu->setZOrder(3);
		auto cancelSprite = gd::ButtonSprite::create("OK", 40, 0, 2.5f, true, "goldFont.fnt", "GJ_button_01.png", 30.0);
		auto cancelButton = gd::CCMenuItemSpriteExtra::create(cancelSprite, nullptr, this, menu_selector(MultipleColorSelectPopup::onOk));
		cancelButton->setPosition({ 0.f, -130.f });
		mainMenu->addChild(cancelButton);
		mainMenu->setTag(5);
		this->addChild(mainMenu);

		auto copySprite = gd::ButtonSprite::create("Copy", 40, 0, 2.5f, true, "goldFont.fnt", "GJ_button_04.png", 30.0);
		auto copyButton = gd::CCMenuItemSpriteExtra::create(copySprite, nullptr, this, menu_selector(MultipleColorSelectPopup::onCopyColor));
		copyButton->setPosition({ -50.f+director->getScreenRight( )/2, -30.f+director->getScreenTop( )/2 });
		mainMenu->addChild(copyButton);

		auto pasteSprite = gd::ButtonSprite::create("Paste", 40, 0, 2.5f, true, "goldFont.fnt", "GJ_button_04.png", 30.0);
		auto pasteButton = gd::CCMenuItemSpriteExtra::create(pasteSprite, nullptr, this, menu_selector(MultipleColorSelectPopup::onPasteColor));
		pasteButton->setPosition({ -50.f+director->getScreenRight( )/2, -70.f+director->getScreenTop( )/2 });
		mainMenu->addChild(pasteButton);



		mixedFadeTime = false;
		mixedBlend = false;
		mixedP1 = false;
		mixedP2 = false;
		mixedTT = false;
		mixedCol = false;
		auto fadeTimeLbl = CCLabelBMFont::create("FadeTime: Mixed", "goldFont.fnt");
		auto firstFadeTime = reinterpret_cast< gd::GameObject * >(triggers->objectAtIndex(0))->getFadeTime( );
		auto firstBlendbool = reinterpret_cast< gd::GameObject * >(triggers->objectAtIndex(0))->getBlending( );
		auto firstP1bool = reinterpret_cast< gd::GameObject * >(triggers->objectAtIndex(0))->getP1color( );
		auto firstP2bool = reinterpret_cast< gd::GameObject * >(triggers->objectAtIndex(0))->getP2color( );
		auto firstTTbool = reinterpret_cast< gd::GameObject * >(triggers->objectAtIndex(0))->getTouchTriggered( );
		auto firstCol = reinterpret_cast< gd::GameObject * >(triggers->objectAtIndex(0))->getTriggerColor( );
		Blendbool = firstBlendbool;
		P1bool = firstP1bool;
		P2bool = firstP2bool;
		TTbool = firstTTbool;

		for ( int i = 1; i<triggers->count( ); i++ ) {
			auto singleObj = reinterpret_cast< gd::GameObject * >(triggers->objectAtIndex(i));
			if ( firstFadeTime!=singleObj->getFadeTime( ) )
				mixedFadeTime = true;
			if ( firstBlendbool!=singleObj->getBlending( ) ) {
				mixedBlend = true; Blendbool = false;
			}
			if ( firstP1bool!=singleObj->getP1color( ) ) {
				mixedP1 = true; P1bool = false;
			}
			if ( firstP2bool!=singleObj->getP2color( ) ) {
				mixedP2 = true; P2bool = false;
			}
			if ( firstTTbool!=singleObj->getTouchTriggered( ) ) {
				mixedTT = true; TTbool = false;
			}
			if ( firstCol.r!=singleObj->getTriggerColor( ).r||
				 firstCol.g!=singleObj->getTriggerColor( ).g||
				 firstCol.b!=singleObj->getTriggerColor( ).b ) {
				mixedCol = true;
			}
		}



		fadeTimeLbl->setZOrder(3);
		fadeTimeLbl->setTag(2);
		fadeTimeLbl->setPosition(director->getScreenRight( )/2, director->getScreenTop( )/2-71.f);
		this->addChild(fadeTimeLbl);

		auto fadeTimeSlider = gd::Slider::create(this, menu_selector(MultipleColorSelectPopup::fadeTimeSliderChanged), 1.f);
		fadeTimeSlider->setPosition(director->getScreenRight( )/2, director->getScreenTop( )/2-96.f);
		fadeTimeSlider->setZOrder(3);
		if ( !mixedFadeTime ) {
			fadeTimeLbl->setString(CCString::createWithFormat("FadeTime: %.2f%", reinterpret_cast< gd::GameObject * >(triggers->objectAtIndex(0))->getFadeTime( ))->getCString( ));
			fadeTimeSlider->setValue(reinterpret_cast< gd::GameObject * >(triggers->objectAtIndex(0))->getFadeTime( )*20.f-100.f);
		}
		fadeTimeSlider->updateBar( );
		this->addChild(fadeTimeSlider);



		auto coloredSquareFirst = CCSprite::createWithSpriteFrameName("whiteSquare60_001.png");
		coloredSquareFirst->setPosition({ 30.f, director->getScreenTop( )-30.f });
		coloredSquareFirst->setZOrder(3);
		if (!mixedCol)
			coloredSquareFirst->setColor(reinterpret_cast< gd::GameObject * >(triggers->objectAtIndex(0))->getTriggerColor( ));
		coloredSquareFirst->setTag(1);
		this->addChild(coloredSquareFirst);

		auto coloredSquareSec = CCSprite::createWithSpriteFrameName("whiteSquare60_001.png");
		coloredSquareSec->setPosition({ 30.f, director->getScreenTop( )-60.f });
		coloredSquareSec->setZOrder(3);
		if (!mixedCol)
			coloredSquareSec->setColor(reinterpret_cast< gd::GameObject * >(triggers->objectAtIndex(0))->getTriggerColor( ));
		this->addChild(coloredSquareSec);



		auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
		auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");

		auto touchTriggeredToggler = gd::CCMenuItemToggler::create(TTTogglerSprite(toggleOn, toggleOff), TTTogglerSprite(toggleOff, toggleOn), this, menu_selector(MultipleColorSelectPopup::TTCallback));
		touchTriggeredToggler->setPosition({ 64.f, -130.f });
		touchTriggeredToggler->setScale(0.8f);
		mainMenu->addChild(touchTriggeredToggler);

		auto touchTriggeredLbl = CCLabelBMFont::create("Touch\nTriggered", "bigFont.fnt");
		touchTriggeredLbl->setPosition({ director->getScreenRight() / 2 + 84.f, director->getScreenTop() / 2 - 130.f});
		touchTriggeredLbl->setAnchorPoint({ 0.f, 0.5f });
		touchTriggeredLbl->setZOrder(3);
		touchTriggeredLbl->setScale(0.35f);
		this->addChild(touchTriggeredLbl);

		auto BlendToggler = gd::CCMenuItemToggler::create(BlendTogglerSprite(toggleOn, toggleOff), BlendTogglerSprite(toggleOff, toggleOn), this, menu_selector(MultipleColorSelectPopup::BlendCallback));
		BlendToggler->setPosition({ -director->getScreenRight() / 2 + 30.f, -50.f });
		BlendToggler->setScale(0.8f);
		mainMenu->addChild(BlendToggler);

		auto BlendLbl = CCLabelBMFont::create("Use Blending", "bigFont.fnt");
		BlendLbl->setPosition({ 50.f, director->getScreenTop( )/2-50.f });
		BlendLbl->setAnchorPoint({ 0.f, 0.5f });
		BlendLbl->setZOrder(3);
		BlendLbl->setScale(0.35f);
		this->addChild(BlendLbl);

		auto P1Toggler = gd::CCMenuItemToggler::create(toggleOff, toggleOn, this, menu_selector(MultipleColorSelectPopup::P1Callback));
		P1Toggler->toggle(P1bool);
		P1Toggler->setPosition({ -director->getScreenRight( )/2+30.f, -90.f });
		P1Toggler->setScale(0.8f);
		P1Toggler->setTag(1);
		mainMenu->addChild(P1Toggler);

		auto P1Lbl = CCLabelBMFont::create("Player\nColor 1", "bigFont.fnt");
		P1Lbl->setPosition({ 50.f, director->getScreenTop( )/2-90.f });
		P1Lbl->setAnchorPoint({ 0.f, 0.5f });
		P1Lbl->setZOrder(3);
		P1Lbl->setScale(0.35f);
		this->addChild(P1Lbl);

		auto P2Toggler = gd::CCMenuItemToggler::create(toggleOff, toggleOn, this, menu_selector(MultipleColorSelectPopup::P2Callback));
		P2Toggler->toggle(P2bool);
		P2Toggler->setPosition({ -director->getScreenRight( )/2+30.f, -130.f });
		P2Toggler->setScale(0.8f);
		P2Toggler->setTag(2);
		mainMenu->addChild(P2Toggler);

		auto P2Lbl = CCLabelBMFont::create("Player\nColor 2", "bigFont.fnt");
		P2Lbl->setPosition({ 50.f, director->getScreenTop( )/2-130.f });
		P2Lbl->setAnchorPoint({ 0.f, 0.5f });
		P2Lbl->setZOrder(3);
		P2Lbl->setScale(0.35f);
		this->addChild(P2Lbl);



		auto colorPicker = reinterpret_cast< cocos2d::extension::CCControlColourPicker*(__thiscall *)() >(GetProcAddress(GetModuleHandleA("libExtensions.dll"), "?create@CCControlColourPicker@extension@cocos2d@@SAPAV123@XZ"))();
		colorPicker->setPosition({ director->getScreenRight( )/2, director->getScreenTop( )/2 + 30.f });
		colorPicker->setZOrder(3);
		colorPicker->setTag(3);
		colorPicker->setDelegate(this);
		if (!mixedCol )
			colorPicker->setColorValue(reinterpret_cast< gd::GameObject * >(triggers->objectAtIndex(0))->getTriggerColor( ));
		this->addChild(colorPicker);

		return true;
	}

	virtual void colorValueChanged(ccColor3B col) override {
		//std::cout<<col.r << col.g << col.b<<std::endl;
		reinterpret_cast< CCSprite * >(getChildByTag(1))->setColor(col);
		reinterpret_cast< gd::GameObject * >(editTriggers->objectAtIndex(0))->setTriggerColor(col);
		mixedCol = false;
	}

	void fadeTimeSliderChanged(CCObject * sliderThumb) {
		auto castedSliderThumb = reinterpret_cast< gd::SliderThumb * >(sliderThumb);
		float value = (castedSliderThumb->getValue( )+100.f)/20;
		auto fadeTimeLbl = reinterpret_cast< CCLabelBMFont * >(this->getChildByTag(2));
		fadeTimeLbl->setString(CCString::createWithFormat("FadeTime: %.2f%", value)->getCString( ));
		mixedFadeTime = false;
		fadeTime = (castedSliderThumb->getValue( ) + 100.f )/ 20;
	}

	CCSprite* TTTogglerSprite(CCSprite *toggleOn, CCSprite *toggleOff)
	{
		return TTbool ? toggleOn : toggleOff;
	}
	void TTCallback(CCObject*) {
		TTbool = !TTbool;
		mixedTT = false;
	}

	CCSprite *BlendTogglerSprite(CCSprite *toggleOn, CCSprite *toggleOff)
	{
		return Blendbool ? toggleOn : toggleOff;
	}
	void BlendCallback(CCObject *) {
		Blendbool = !Blendbool;
		mixedBlend = false;
	}

	void P1Callback(CCObject *) {
		P1bool = !P1bool;
		P2bool = false;
		reinterpret_cast< gd::CCMenuItemToggler * >(getChildByTag(5)->getChildByTag(2))->toggle(false);
		mixedP1 = false;
	}

	void P2Callback(CCObject *) {
		P2bool = !P2bool;
		P1bool = false;
		reinterpret_cast< gd::CCMenuItemToggler * >(getChildByTag(5)->getChildByTag(1))->toggle(false);
		mixedP2 = false;
	}


	void onOk(CCObject *) {
		keyBackClicked();
	}
	void onCopyColor(CCObject *) {
		auto col = reinterpret_cast< gd::GameObject * >(editTriggers->objectAtIndex(0))->getTriggerColor( );
		gd::GameManager::sharedState()->setClipboardedColor(col);
	}
	void onPasteColor(CCObject *) {
		auto col = gd::GameManager::sharedState( )->getClipboardedColor();
		reinterpret_cast< CCSprite * >(getChildByTag(1))->setColor(col);
		reinterpret_cast< gd::GameObject * >(editTriggers->objectAtIndex(0))->setTriggerColor(col);
		reinterpret_cast< cocos2d::extension::CCControlColourPicker * >(getChildByTag(3))->setColorValue(col);
		mixedCol = false;
	}
	virtual void keyBackClicked( ) override {
		//exit(0);
		auto arr = editTriggers;
		auto firstObject = reinterpret_cast< gd::GameObject * >(arr->objectAtIndex(0));
		if (!mixedFadeTime)
			firstObject->setFadeTime(fadeTime);
		if ( !mixedBlend )
			firstObject->setBlending(Blendbool);
		if ( !mixedP1 )
			firstObject->setP1color(P1bool);
		if ( !mixedP2 )
			firstObject->setP2color(P2bool);
		if ( !mixedTT )
			firstObject->setTouchTriggered(TTbool);
		for( int i = 1; i<arr->count( ); i++ ) {
			auto singleObj = reinterpret_cast< gd::GameObject * >(arr->objectAtIndex(i));
			if (!mixedCol )
				singleObj->setTriggerColor(firstObject->getTriggerColor( ));
			if (!mixedBlend )
				singleObj->setBlending(Blendbool);
			if (!mixedP1 )
				singleObj->setP1color(P1bool);
			if (!mixedP2 )
				singleObj->setP2color(P2bool);
			if (!mixedTT)
				singleObj->setTouchTriggered(TTbool);
			if ( !mixedFadeTime )
				singleObj->setFadeTime(fadeTime);
		}
		//editTriggers = nullptr;
		this->removeMeAndCleanup( );
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

//void showPicker(CCLayer *layer, int colorId, int *settingsColorObject) {
//	return reinterpret_cast< void(__thiscall *)
//		(CCLayer *, int, int *) >
//		(gd::base+0x993a0)
//		(layer, colorId, settingsColorObject);
//}

void MyCustomizeObjectLayer::onColorBtn(CCObject *btn) {
	if ( !colorBtnVec.empty( ) ) {
		auto darealbtn = static_cast< ColorButtonSpriteExtra * >(btn);
		
		if ( highlightedBtn==darealbtn) {
			auto lel = from<gd::LevelEditorLayer *>(this, 0xAC);
			auto sco = from<gd::SettingsColorObject *>(lel->getLevelSettingsObject( ), getOffsetForSco(darealbtn->id));
			if ( !darealbtn->isSpecial ) {
				//auto csp = gd::ColorSelectPopup::create(nullptr, getColorForPopup(darealbtn->id), 0, 0);
				//showPicker(lel, getColorForPopup(darealbtn->id), from<int *>(lel->getLevelSettingsObject( ), 0xF8));
				auto edsp = EditColorSelectPopup::create(sco, darealbtn);
				CCDirector::sharedDirector( )->getRunningScene( )->addChild(edsp);
			}
		}
		else {
			if ( highlightedBtn!=nullptr ) 
				highlightedBtn->deselect( );
			darealbtn->select( );
			

			if ( selectedObject!=nullptr ) {
				selectedObject->setActiveColor(darealbtn->id);
				//if (darealbtn->id == 8)
				//	selectedObject
			}
			else if ( selectedObjects!=nullptr ) {
				for ( int i = 0; i<selectedObjects->count( ); i++ ) {
					auto currObj = reinterpret_cast< gd::GameObject * >(selectedObjects->objectAtIndex(i));
					if ( setting( ).anyEdit||currObj->getDefaultColor( )!=0 )
						currObj->setActiveColor(darealbtn->id);
				}
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
	gd::SettingsColorObject *sco = from<gd::SettingsColorObject *>(levelSettingsObject, 0xE8);
	auto red = from<unsigned char>(sco, 0xE8);
	auto green = from<unsigned char>(sco, 0xE9);
	auto blue = from<unsigned char>(sco, 0xEA);

	auto wButton = ColorButtonSpriteExtra::create("White", this, 9, true);
	wButton->btnSprite->setColor({ 255, 255, 255 });

	auto dlButton = ColorButtonSpriteExtra::create("3DL", this, 8, false);
	sco = from<gd::SettingsColorObject *>(levelSettingsObject, 0xF8);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	dlButton->btnSprite->setColor({ red, green, blue });

	auto c1Button = ColorButtonSpriteExtra::create("1", this, 3, false);
	sco = from<gd::SettingsColorObject *>(levelSettingsObject, 0xFC);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c1Button->btnSprite->setColor({ red, green, blue });

	auto c2Button = ColorButtonSpriteExtra::create("2", this, 4, false);
	sco = from<gd::SettingsColorObject *>(levelSettingsObject, 0x100);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c2Button->btnSprite->setColor({ red, green, blue });

	auto c3Button = ColorButtonSpriteExtra::create("3", this, 6, false);
	sco = from<gd::SettingsColorObject*>(levelSettingsObject, 0x104);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c3Button->btnSprite->setColor({ red, green, blue });

	auto c4Button = ColorButtonSpriteExtra::create("4", this, 7, false);
	sco = from<gd::SettingsColorObject *>(levelSettingsObject, 0x108);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c4Button->btnSprite->setColor({ red, green, blue });

	auto defButton = ColorButtonSpriteExtra::create("Def", this, 0, true);
	defButton->btnSprite->setColor({ 255, 255, 255 });
	defButton->btnSprite->setOpacity(150);

	auto p1Button = ColorButtonSpriteExtra::create("P1", this, 1, true);
	p1Button->btnSprite->setColor(lel->getPlayer1( )->getFirstColor( ));

	auto p2Button = ColorButtonSpriteExtra::create("P2", this, 2, true);
	p2Button->btnSprite->setColor(lel->getPlayer1( )->getSecondColor( ));

	auto lbgButton = ColorButtonSpriteExtra::create("LBG", this, 5, true);
	sco = from<gd::SettingsColorObject *>(levelSettingsObject, 0xE8);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	lbgButton->btnSprite->setColor({ red, green, blue });
	lbgButton->btnSprite->setOpacity(150);

	colorsMenu->addChild(wButton);
	colorsMenu->addChild(dlButton);
	colorsMenu->addChild(c1Button);
	colorsMenu->addChild(c2Button);
	colorsMenu->addChild(c3Button);
	colorsMenu->addChild(c4Button);
	colorsMenu->addChild(defButton);
	colorsMenu->addChild(p1Button);
	colorsMenu->addChild(p2Button);
	colorsMenu->addChild(lbgButton);
	
	//37.5f

	defButton->setPosition({ -75.f, 30.f });
	p1Button->setPosition({ -37.5f, 30.f });
	p2Button->setPosition({ 0, 30.f });
	lbgButton->setPosition({ 37.5f, 30.f });
	wButton->setPosition({ 75.f, 30.f });

	c1Button->setPosition({ -75.f, -30.f });
	c2Button->setPosition({ -37.5f, -30.f });
	c3Button->setPosition({ 0.f, -30.f });
	c4Button->setPosition({ 37.5f, -30.f });
	dlButton->setPosition({ 75.f, -30.f });

	//objButton->setPosition({ -67.5f, 37.5f });
	//bgButton->setPosition({ -105.f, 0.f });
	//gButton->setPosition({ -67.5f, 0.f });
	//lButton->setPosition({ -30.f, 0.f });

	
	//colorBtnVec.push_back(bgButton); colorBtnVec.push_back(gButton); colorBtnVec.push_back(lButton); colorBtnVec.push_back(objButton); 
	colorBtnVec.push_back(defButton); colorBtnVec.push_back(p1Button); colorBtnVec.push_back(p2Button); colorBtnVec.push_back(lbgButton); colorBtnVec.push_back(wButton);
	colorBtnVec.push_back(dlButton); colorBtnVec.push_back(c1Button); colorBtnVec.push_back(c2Button); colorBtnVec.push_back(c3Button); colorBtnVec.push_back(c4Button);
	
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
		case 9:
			highlightedBtn = wButton;
			wButton->select( );
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
	bg->setContentSize({ 280, 220 });
	bg->setPosition((director->getScreenRight( ))/2, (director->getScreenTop( ))/2);
	bg->setZOrder(2);
	this->addChild(bg);
	auto appearAction = CCEaseElasticOut::create(CCScaleTo::create(.5f, 1.f), 0.6f);

	cocos2d::extension::CCScale9Sprite *colorBtnBg = cocos2d::extension::CCScale9Sprite::create("square02_001.png");
	colorBtnBg->setContentSize({ 220, 120 });
	colorBtnBg->setPosition((director->getScreenRight( ))/2, (director->getScreenTop( ))/2);
	colorBtnBg->setOpacity(100);
	colorBtnBg->setZOrder(3);
	this->addChild(colorBtnBg);

	auto gravityLine = CCSprite::create("gravityLine_001.png");
	gravityLine->setPosition({ (director->getScreenRight( ))/2, (director->getScreenTop( ))/2 });
	gravityLine->setScale(8.f);
	gravityLine->setScaleX(4.f);
	gravityLine->setZOrder(4);
	gravityLine->setRotation(90.f);
	this->addChild(gravityLine);

	auto dot = CCSprite::createWithSpriteFrameName("d_link_b_01_color_001.png");
	dot->setPosition({ (director->getScreenRight( ))/2, (director->getScreenTop( ))/2 });
	dot->setScale(0.5f);
	dot->setZOrder(4);
	this->addChild(dot);
	

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
	if ( editUI->getSingleSelectedObj( ) ) {
		auto singleObj = editUI->getSingleSelectedObj( );
		if ( triggers.contains(singleObj->getObjectID( )) ) {
			auto csp = gd::ColorSelectPopup::create(editUI->getSingleSelectedObj( ), 0, 0, 0);
			csp->show( );
		}
		else if ( singleObj->getObjectID( )==31 ) {
			auto lsl = gd::LevelSettingsLayer::create(reinterpret_cast< gd::StartPosObject * >(singleObj)->getLevelSettingsObject( ));
			lsl->show( );
		}
		else {
			auto myCustLayer = MyCustomizeObjectLayer::create(myLayer, editUI->getSelectedObjectsOfCCArray( ), editUI->getSingleSelectedObj( ));
			myCustLayer->setZOrder(100000);

			//auto myLayer = static_cast< CCLayer * >(static_cast< CCNode * >(btn)->getUserObject( ));

			myLayer->addChild(myCustLayer);
		}
	}
	else {
		auto objArr = editUI->getSelectedObjectsOfCCArray( );
		if ( editUI->isEveryObjectIsTrigger() ) {
			auto mcsp = MultipleColorSelectPopup::create(objArr);
			mcsp->setZOrder(editUI->getZOrder( )+1);
			CCDirector::sharedDirector( )->getRunningScene( )->addChild(mcsp);
			//editUI->addChild(mcsp);
			//mcsp->show( );
			//auto csp = gd::ColorSelectPopup::create(reinterpret_cast< gd::GameObject * >(objArr->objectAtIndex(0)), 0, 0, 0);
			//csp->show( );
		}
		else {
			auto myCustLayer = MyCustomizeObjectLayer::create(myLayer, objArr, editUI->getSingleSelectedObj( ));
			myCustLayer->setZOrder(100000);

			//auto myLayer = static_cast< CCLayer * >(static_cast< CCNode * >(btn)->getUserObject( ));

			myLayer->addChild(myCustLayer);
		}
	}
}

void MyCustomizeObjectLayer::okButtonCallback(CCObject *btn) {
	selectedObjects = nullptr;
	selectedObject = nullptr;
	highlightedBtn = nullptr;
	colorBtnVec.clear( );
	this->removeMeAndCleanup( );
}