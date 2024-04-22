#pragma once
#include "includes.h"

class MyCustomizeObjectLayer : public CCLayer
{
public:
	virtual void keyBackClicked( );
	bool init(gd::LevelEditorLayer *lel, CCArray *selObjs, gd::GameObject *selObj);
	static MyCustomizeObjectLayer *create(gd::LevelEditorLayer* lel, CCArray* selObjs, gd::GameObject* selObj);
	void okButtonCallback(CCObject *btn);

	void onEdit(CCObject *btn);
	void onColInc(CCObject *btn);
	void onColorBtn(CCObject *btn);
	//void highlightButton(std::vector<gd::CCMenuItemSpriteExtra*>);
};
