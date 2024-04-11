#pragma once

namespace PlayLayer
{
	bool(__thiscall* init)(gd::PlayLayer* self, gd::GJGameLevel* level);
	bool __fastcall init_H(gd::PlayLayer* self, void* edx, gd::GJGameLevel* level);

	bool(__thiscall* update)(gd::PlayLayer* self, float dt);
	void __fastcall update_H(gd::PlayLayer* self, void*, float dt);
}

