#pragma once
#include "includes.h"


namespace EditorUI
{
	class Callback
	{
	public:
		void toTheFirstLayerButton(CCObject* obj);
		void toTheEmptyLayerButton(CCObject* obj);
		void onDuplicate(CCObject *obj);
		void onDebugCopyObjAddress(CCObject *obj);
	};
}
