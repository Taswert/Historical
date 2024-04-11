#pragma once
#include "includes.h"

namespace InfoLayer
{
	inline bool(__thiscall* InfoLayer_init)(cocos2d::CCLayer* self, CCObject* gjuserscore);
	bool __fastcall InfoLayer_init_hook(cocos2d::CCLayer* self, void* edx, CCObject* gjuserscore);

}