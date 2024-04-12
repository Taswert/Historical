#include "CustomizeObjectLayer.h"
#include "includes.h"
#include "utils.hpp"

CustomizeObjectLayer::CustomizeObjectLayer* __fastcall CustomizeObjectLayer::create_H(gd::GameObject *obj, CCArray *objs) {
	CustomizeObjectLayer *layer = new CustomizeObjectLayer;
	if ( layer->init(obj, objs) ) layer->autorelease( );
	else CC_SAFE_DELETE(layer);
	return layer;
}

bool CustomizeObjectLayer::CustomizeObjectLayer::init(gd::GameObject *obj, CCArray *objs) {

	CCLayer::init( );
	CCLayerColor *cclcol = CCLayerColor::create(ccc4(0, 0, 0, 0));
	cclcol->setZOrder(1);
	cclcol->setScale(10.f);
	this->addChild(cclcol);
	auto actionCol = CCFadeTo::create(0.1f, 75);
	cclcol->runAction(actionCol);
	auto addingLayer = CCLayer::create( );

	auto touchDispatcher = CCDirector::sharedDirector( )->m_pTouchDispatcher;
	touchDispatcher->incrementForcePrio( );
	touchDispatcher->incrementForcePrio( );
	registerWithTouchDispatcher( );
	setTouchEnabled(true);
	setKeypadEnabled(true);
	setMouseEnabled(true);

	return true;
}