#include "includes.h"
#include "PauseLayer.h"

bool onToggler = false;

void PauseLayer::Callback::toggler(CCObject*)
{
	
}

bool __fastcall PauseLayer::hook(CCLayer* self)
{
	
	CCLayer* PauseLayerObject = self;

	bool result = PauseLayer::init(self);
	auto label = CCLabelBMFont::create("Hello world!", "bigFont.fnt");
	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();
	auto menu = CCMenu::create();
	//auto toggler = gd::PauseLayer::createToggleButton(menu_selector(PauseLayer::Callback::toggler), onToggler, menu, "Noclip", {20, 20});


	label->setScale(1.0f);
	label->setPosition(size.width / 2, size.height / 2);
	//self->addChild(label);

	return result;
}