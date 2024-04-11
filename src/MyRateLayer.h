#pragma once
#include "includes.h"
#include <cocos2d.h>

class MyRateLayer : public CCLayer
{
	protected:
		
		
	public :
		virtual void keyBackClicked();
		void backButton();
		bool init();
		static MyRateLayer* create();
		void rateCallback(CCObject*);
};