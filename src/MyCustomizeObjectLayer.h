#pragma once
#include "includes.h"

class MyCustomizeObjectLayer : public CCLayer
{
public:
	virtual void keyBackClicked( );
	bool init(gd::LevelEditorLayer *lel);
	static MyCustomizeObjectLayer *create(gd::LevelEditorLayer* lel);
	void onPashalko(CCObject *btn);
	void okButtonCallback(CCObject *btn);
};
