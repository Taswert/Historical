#include "includes.h"
#include "InfoLayer.h"

bool __fastcall InfoLayer::InfoLayer_init_hook(cocos2d::CCLayer* self, void* edx, cocos2d::CCObject* gjuserscore)
{
	bool result = InfoLayer::InfoLayer_init(self, gjuserscore);

	//auto lbl = CCLabelBMFont::create("LOLLOLLOLLOLLOLLOLLOL", "bigFont.fnt");
	//self->addChild(lbl);

	return result;
}