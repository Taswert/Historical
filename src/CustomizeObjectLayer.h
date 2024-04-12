#pragma once
#include "includes.h"

namespace CustomizeObjectLayer
{
	class CustomizeObjectLayer : public gd::FLAlertLayer
	{
	public:
		bool init(gd::GameObject *obj, CCArray *objs);
		void hightlightSelected(gd::ButtonSprite *spr) {
			reinterpret_cast< void(__thiscall *)(CustomizeObjectLayer *, gd::ButtonSprite *) >(gd::base+0x2e4c0)(this, spr);
		}
	};


	inline CustomizeObjectLayer*(__thiscall *create)(gd::GameObject *obj, CCArray *objs);
	CustomizeObjectLayer* __fastcall create_H(gd::GameObject *obj, CCArray *objs);
	
	
	//inline bool(__thiscall *CustomizeObjectLayer_init)(CustomizeObjectLayer *self, gd::GameObject *obj, CCArray *objs);
	//bool __fastcall CustomizeObjectLayer_init_H(CustomizeObjectLayer *self, void *edx, gd::GameObject *obj, CCArray *objs);

	//class Callback
	//{
	//public:
	//	//void quit(CCObject *obj);
	//};
}
