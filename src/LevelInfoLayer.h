#pragma once
#include "includes.h"

namespace LevelInfoLayer
{
	inline bool(__thiscall* LevelInfoLayer_init)(cocos2d::CCLayer* self, gd::GJGameLevel* idk);
	bool __fastcall LevelInfoLayer_init_hook(cocos2d::CCLayer* self, void* edx, gd::GJGameLevel* idk);

	//inline void(__thiscall* LevelInfoLayer_onLevelInfo)(cocos2d::CCLayer* levelInfoLayer);
	//void __fastcall LevelInfoLayer_onLevelInfo_hook(cocos2d::CCLayer* levelInfoLayer);

	
	
	

	class Callback
	{
	public:
		void arrowLeftButton(CCObject*);
		void arrowRightButton(CCObject*);
		void infoButton(CCObject*);
		void rateButton(CCObject*);
		void unrateButton(CCObject*);
		void addToHofButton(CCObject*);
		void removeFromHofButton(CCObject*);
	};
}

