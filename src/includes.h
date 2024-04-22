#pragma once

#define WIN32_LEAN_AND_MEAN
#define CURL_STATICLIB
#include <curl/curl.h>

#include <Windows.h>
#include <cocos2d.h>
#include <MinHook.h>
#include <gd.h>

#include <cocoa/CCAffineTransform.h>
#include <ExtensionMacros.h>W
#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include <spine/extension.h>
#include <GUI/CCControlExtension/CCControlUtils.h>
#include <GUI/CCControlExtension/CCControlColourPicker.h>
#include <GUI/CCControlExtension/CCControl.h>
using std::uintptr_t;

// pretty everything in cocos2d starts with CC, so there is a low chance of collisions
// it also makes using cocos a lot nicer imo
using namespace cocos2d;
