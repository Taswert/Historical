#pragma once
#include "includes.h"

namespace EditorPauseLayer
{
	inline bool(__thiscall* EditorPauseLayer_init)(CCLayer* self);
	bool __fastcall EditorPauseLayer_init_hook(CCLayer* self);

	class Callback
	{
	public:
		void EEToggler(CCObject*);
		void VanillaSelectAllButton(CCObject*);
		void SelectAbsolutelyAllButton(CCObject*);
		void PCToggler(CCObject*);
		void HideGridToggler(CCObject*);
		void HideTLinesToggler(CCObject*);
		void SmoothTrailToggler(CCObject*);
		void FreeEditorScrollToggler(CCObject*);
	};

	inline bool(__thiscall* EditorUILayer_init)(gd::EditorUI* self, CCLayer* editor);
	bool __fastcall EditorUILayer_init_H(gd::EditorUI* self, void*, CCLayer* editor);

	inline bool(__thiscall* EditorUILayer_dtor)(gd::EditorUI* self);
	void __fastcall EditorUILayer_dtor_H(gd::EditorUI* self);

}
