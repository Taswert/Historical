#pragma once
#include "includes.h"

namespace CreatorLayer
{
	inline bool(__thiscall* CreatorLayer_init)(gd::CreatorLayer* self);
	bool __fastcall CreatorLayer_init_H(gd::CreatorLayer* self);

	inline bool(__thiscall* CreatorLayer_menu)();
	void __fastcall CreatorLayer_menu_H();

	class Callback
	{
	public:
		void onHallOfFame(CCObject*);
	};
}
