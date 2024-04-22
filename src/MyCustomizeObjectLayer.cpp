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

class ColorButtonSpriteExtra : public gd::CCMenuItemSpriteExtra {
public:
	CCLabelBMFont *label;
	CCSprite *selectSprite;
	CCSprite *btnSprite;
	bool isSelected;
	bool isSpecial;
	short id;

	virtual void selected() override {
		this->stopActionByTag(0);
		auto actionInterval = cocos2d::CCScaleTo::create(0.3, 1.26f);
		auto action = cocos2d::CCEaseBounceOut::create(actionInterval);
		action->setTag(0);
		this->runAction(action);
	}

	virtual void unselected( ) override {
		this->stopActionByTag(0);
		auto actionInterval = cocos2d::CCScaleTo::create(0.0, 1.f);
		auto action = cocos2d::CCEaseBounceOut::create(actionInterval);
		action->setTag(0);
		this->runAction(action);
	}

	static ColorButtonSpriteExtra *create(const char* lbl, CCLayer* layer, int id, bool isSpecial) {
		auto ret = new ColorButtonSpriteExtra( );
		if ( ret&&ret->init(lbl, layer, id, isSpecial)) {
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
		selColLbl->setPosition(director->getScreenRight( )/2, director->getScreenTop( )/2 + 130.f);
		this->addChild(selColLbl);
		


		auto mainMenu = CCMenu::create( );
		mainMenu->setZOrder(3);
		auto cancelSprite = gd::ButtonSprite::create("OK", 40, 0, 2.5f, true, "goldFont.fnt", "GJ_button_01.png", 30.0);
		auto cancelButton = gd::CCMenuItemSpriteExtra::create(cancelSprite, nullptr, this, menu_selector(MultipleColorSelectPopup::onOk));
		cancelButton->setPosition({ 0.f, -130.f });
		mainMenu->addChild(cancelButton);
		this->addChild(mainMenu);

		auto copySprite = gd::ButtonSprite::create("Copy", 40, 0, 2.5f, true, "goldFont.fnt", "GJ_button_04.png", 30.0);
		auto copyButton = gd::CCMenuItemSpriteExtra::create(copySprite, nullptr, this, menu_selector(MultipleColorSelectPopup::onCopyColor));
		copyButton->setPosition({ -50.f+director->getScreenRight( )/2, -30.f+director->getScreenTop( )/2 });
		mainMenu->addChild(copyButton);

		auto pasteSprite = gd::ButtonSprite::create("Paste", 40, 0, 2.5f, true, "goldFont.fnt", "GJ_button_04.png", 30.0);
		auto pasteButton = gd::CCMenuItemSpriteExtra::create(pasteSprite, nullptr, this, menu_selector(MultipleColorSelectPopup::onPasteColor));
		pasteButton->setPosition({ -50.f + director->getScreenRight() / 2, -70.f+director->getScreenTop( )/2 });
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
			if ( firstCol.r!=singleObj->getTriggerColor( ).r || 
				 firstCol.g!=singleObj->getTriggerColor( ).g  ||
				 firstCol.b!=singleObj->getTriggerColor( ).b) {
				mixedCol = true;
			}
		}



		fadeTimeLbl->setZOrder(3);
		fadeTimeLbl->setTag(2);
		fadeTimeLbl->setPosition(director->getScreenRight( )/2, director->getScreenTop( )/2-71.f);
		this->addChild(fadeTimeLbl);

		//auto colorPicker = cocos2d::extension::CCControlColourPicker::create( );
		auto fadeTimeSlider = gd::Slider::create(this, menu_selector(MultipleColorSelectPopup::fadeTimeSliderChanged), 1.f);
		fadeTimeSlider->setPosition(director->getScreenRight( )/2, director->getScreenTop( )/2-96.f);
		//SliderThumb *sfxSliderThumb = from<SliderThumb *>(from<int *>(sfxSlider, 0x13c), 0x160);
		//sfxSliderThumb->setValue(from<float>(fmod, 0x16c));
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

		//auto hexLabelFirst = CCLabelBMFont::create("HEXHEX", "bigFont.fnt");
		//hexLabelFirst->setScale(0.35f);
		//hexLabelFirst->setZOrder(3);
		//hexLabelFirst->setTag(4);
		//hexLabelFirst->setAnchorPoint({ 0.f, 0.5f });
		//hexLabelFirst->setPosition({ 50.f, director->getScreenTop( )-30.f });
		//this->addChild(hexLabelFirst);

		auto coloredSquareSec = CCSprite::createWithSpriteFrameName("whiteSquare60_001.png");
		coloredSquareSec->setPosition({ 30.f, director->getScreenTop( )-60.f });
		coloredSquareSec->setZOrder(3);
		if (!mixedCol)
			coloredSquareSec->setColor(reinterpret_cast< gd::GameObject * >(triggers->objectAtIndex(0))->getTriggerColor( ));
		this->addChild(coloredSquareSec);

		//auto hexLabelSec = CCLabelBMFont::create("HEXHEX", "bigFont.fnt");
		//hexLabelSec->setScale(0.35f);
		//hexLabelSec->setZOrder(3);
		//hexLabelSec->setTag(5);
		//hexLabelSec->setAnchorPoint({ 0.f, 0.5f });
		//hexLabelSec->setPosition({ 50.f, director->getScreenTop( )-60.f });
		//this->addChild(hexLabelSec);



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

		auto P1Toggler = gd::CCMenuItemToggler::create(P1TogglerSprite(toggleOn, toggleOff), P1TogglerSprite(toggleOff, toggleOn), this, menu_selector(MultipleColorSelectPopup::P1Callback));
		P1Toggler->setPosition({ -director->getScreenRight( )/2+30.f, -90.f });
		P1Toggler->setScale(0.8f);
		mainMenu->addChild(P1Toggler);

		auto P1Lbl = CCLabelBMFont::create("Player\nColor 1", "bigFont.fnt");
		P1Lbl->setPosition({ 50.f, director->getScreenTop( )/2-90.f });
		P1Lbl->setAnchorPoint({ 0.f, 0.5f });
		P1Lbl->setZOrder(3);
		P1Lbl->setScale(0.35f);
		this->addChild(P1Lbl);

		auto P2Toggler = gd::CCMenuItemToggler::create(P2TogglerSprite(toggleOn, toggleOff), P2TogglerSprite(toggleOff, toggleOn), this, menu_selector(MultipleColorSelectPopup::P2Callback));
		P2Toggler->setPosition({ -director->getScreenRight( )/2+30.f, -130.f });
		P2Toggler->setScale(0.8f);
		mainMenu->addChild(P2Toggler);

		auto P2Lbl = CCLabelBMFont::create("Player\nColor 2", "bigFont.fnt");
		P2Lbl->setPosition({ 50.f, director->getScreenTop( )/2-130.f });
		P2Lbl->setAnchorPoint({ 0.f, 0.5f });
		P2Lbl->setZOrder(3);
		P2Lbl->setScale(0.35f);
		this->addChild(P2Lbl);



		//auto textField = CCTextFieldTTF::create("#HEXColor", "bigFont.fnt", 20);
		//textField->setZOrder(3);
		//textField->setPosition({ 50.f, director->getScreenTop( )/2 });
		//this->addChild(textField);


		//auto colorPicker = cocos2d::extension::CCControlColourPicker::create( );
		auto colorPicker = reinterpret_cast< cocos2d::extension::CCControlColourPicker*(__thiscall *)() >(GetProcAddress(GetModuleHandleA("libExtensions.dll"), "?create@CCControlColourPicker@extension@cocos2d@@SAPAV123@XZ"))();
		colorPicker->setPosition({ director->getScreenRight( )/2, director->getScreenTop( )/2 + 30.f });
		colorPicker->setZOrder(3);
		colorPicker->setTag(3);
		std::cout<<"1"<<std::endl;
		colorPicker->setDelegate(this);
		std::cout<<"2"<<std::endl;
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

	CCSprite *P1TogglerSprite(CCSprite *toggleOn, CCSprite *toggleOff)
	{
		return P1bool ? toggleOn : toggleOff;
	}
	void P1Callback(CCObject *) {
		P1bool = !P1bool;
		mixedP1 = false;
	}

	CCSprite *P2TogglerSprite(CCSprite *toggleOn, CCSprite *toggleOff)
	{
		return P2bool ? toggleOn : toggleOff;
	}
	void P2Callback(CCObject *) {
		P2bool = !P2bool;
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

void showPicker(CCLayer *layer, int colorId, int *settingsColorObject) {
	return reinterpret_cast< void(__thiscall *)
		(CCLayer *, int, int *) >
		(gd::base+0x993a0)
		(layer, colorId, settingsColorObject);
}

void MyCustomizeObjectLayer::onColorBtn(CCObject *btn) {
	if ( !colorBtnVec.empty( ) ) {
		auto darealbtn = static_cast< ColorButtonSpriteExtra * >(btn);
		
		if ( highlightedBtn==darealbtn) {
			std::cout<<"ColBTN "<<btn<<std::endl;
			auto lel = from<gd::LevelEditorLayer *>(this, 0xAC);
			auto sco = from<int *>(lel->getLevelSettingsObject( ), 0xE8);
			std::cout<<"lel "<<lel<<std::endl;
			std::cout<<"sco "<<sco<<std::endl;
			if ( !darealbtn->isSpecial ) {
				//auto csp = gd::ColorSelectPopup::create(nullptr, getColorForPopup(darealbtn->id), 0, 0);
				showPicker(lel, getColorForPopup(darealbtn->id), from<int *>(lel->getLevelSettingsObject( ), 0xF8));
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
	int *sco = from<int *>(levelSettingsObject, 0xE8);
	auto red = from<unsigned char>(sco, 0xE8);
	auto green = from<unsigned char>(sco, 0xE9);
	auto blue = from<unsigned char>(sco, 0xEA);

	/*auto bgButton = ColorButtonSpriteExtra::create("BG", this, 0);
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
	objButton->btnSprite->setColor({ red, green, blue });*/

	auto wButton = ColorButtonSpriteExtra::create("White", this, 9, true);
	wButton->btnSprite->setColor({ 255, 255, 255 });

	auto dlButton = ColorButtonSpriteExtra::create("3DL", this, 8, false);
	sco = from<int *>(levelSettingsObject, 0xF8);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	dlButton->btnSprite->setColor({ red, green, blue });

	auto c1Button = ColorButtonSpriteExtra::create("1", this, 3, false);
	sco = from<int *>(levelSettingsObject, 0xFC);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c1Button->btnSprite->setColor({ red, green, blue });

	auto c2Button = ColorButtonSpriteExtra::create("2", this, 4, false);
	sco = from<int *>(levelSettingsObject, 0x100);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c2Button->btnSprite->setColor({ red, green, blue });

	auto c3Button = ColorButtonSpriteExtra::create("3", this, 6, false);
	sco = from<int *>(levelSettingsObject, 0x104);
	red = from<unsigned char>(sco, 0xE8); green = from<unsigned char>(sco, 0xE9); blue = from<unsigned char>(sco, 0xEA);
	c3Button->btnSprite->setColor({ red, green, blue });

	auto c4Button = ColorButtonSpriteExtra::create("4", this, 7, false);
	sco = from<int *>(levelSettingsObject, 0x108);
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


	//colorsMenu->addChild(bgButton);
	//colorsMenu->addChild(gButton);
	//colorsMenu->addChild(lButton);
	//colorsMenu->addChild(objButton);
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
	auto appearAction = CCEaseBackOut::create(CCScaleTo::create(.25f, 1.f));

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