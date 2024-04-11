#pragma once
#ifndef __INFOLAYER_H__
#define __INFOLAYER_H__

#include <gd.h>

namespace gd
{
	class InfoLayer : public cocos2d::CCLayer {
	public:
		auto level() {
			return from<GJGameLevel*>(this, 0x124); //idk if this is my offset or not lol
		}
		static auto scene(GJGameLevel* level) {
			return reinterpret_cast<cocos2d::CCScene * (__fastcall*)(GJGameLevel*)>(base + 0x3b240)(level); //0x3b4c0 - GD 1.92
		}
	};

}

#endif
//DIDN'T MAKE IT TO THE END