#include "includes.h"
#include "EditorPauseLayer.h"
#include "state.h"


CCLayer* editorPauseLayer;

std::string hexConverter(int n) {
	std::string hex;
	//loop runs til n is greater than 0
	while (n > 0) {
		int r = n % 16;
		//remainder converted to hexadecimal digit
		char c = (r < 10) ? ('0' + r) : ('A' + r - 10);
		//hexadecimal digit added to start of the string
		hex = c + hex;
		n /= 16;
	}
	hex = "0x" + hex;
	return hex;
}



void EditorPauseLayer::Callback::SelectAbsolutelyAllButton(CCObject*)
{
	auto leveleditor = from<gd::LevelEditorLayer*>(editorPauseLayer, 0x1A8);
	auto editorUI = leveleditor->getEditorUI();

	auto objs = CCArray::create();
	for (int i = 0; i < (leveleditor->getAllObjects()->count()); i++)
	{
		objs->addObjectsFromArray(reinterpret_cast<CCArray*>(leveleditor->getAllObjects()->objectAtIndex(i)));
	}
	editorUI->selectObjects(objs);
	editorUI->updateButtons();
}

void EditorPauseLayer::Callback::VanillaSelectAllButton(CCObject*)
{
	auto leveleditor = from<gd::LevelEditorLayer*>(editorPauseLayer, 0x1A8);
	auto editorUI = leveleditor->getEditorUI();

	auto objs = CCArray::create();
	for (int i = 0; i < (leveleditor->getAllObjects()->count()); i++)
	{
		objs->addObjectsFromArray(reinterpret_cast<CCArray*>(leveleditor->getAllObjects()->objectAtIndex(i)));
	}

	auto objs2 = CCArray::create();
	for (int i = 0; i < (objs->count()); i++)
	{
		if (reinterpret_cast<gd::GameObject*>(objs->objectAtIndex(i))->getGroup() == leveleditor->getLayerGroup() || leveleditor->getLayerGroup() == -1)
			objs2->addObject(objs->objectAtIndex(i));
	}
	editorUI->selectObjects(objs2);
	editorUI->updateButtons();

	//std::string flalertstr = "Objects count: " + std::to_string(editorUI->getSomeObjects()->count()) +
	//	"\nArray p: " + hexConverter(reinterpret_cast<int>(editorUI->getSomeObjects())) +
	//	"\nEditorUI p: " + hexConverter(reinterpret_cast<int>(editorUI)) +
	//	"\nLevelEditorLayer p: " + hexConverter(reinterpret_cast<int>(leveleditor));

	//std::string flalertstr = "Objects count: " + std::to_string(leveleditor->getAllObjects()->count()) +
	//	"\nArray p: " + hexConverter(reinterpret_cast<int>(leveleditor->getAllObjects())) +
	//	"\nEditorUI p: " + hexConverter(reinterpret_cast<int>(editorUI)) +
	//	"\nLevelEditorLayer p: " + hexConverter(reinterpret_cast<int>(leveleditor));

	 //leveleditor->getAllObjects()
	//leveleditor->removeAllObjects();
	//const auto flalert = gd::FLAlertLayer::create(nullptr, "Success", flalertstr.c_str(), "ladno", nullptr, 320.f, false, 0);
	//std::cout << flalertstr << std::endl;
	//flalert->show();
}

void EditorPauseLayer::Callback::EEToggler(CCObject*)
{
	setting().onEditorExtension = !setting().onEditorExtension;
	if (setting().onEditorExtension)
	{
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x732000), "\x00\x00\x00\x00", 4, NULL); //left extend
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x733000), "\x00\x00\x80\x3F", 4, NULL); //left extend

		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x447695), "\x00\x20\x73", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44b249), "\x00\x30\x73", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44b25a), "\x00\x00\x80\x3f", 4, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44b317), "\x00\x30\x73", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44b328), "\x00\x00\x80\x3f", 4, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x492da1), "\x00\x20\x73", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x492edb), "\x00\x20\x73", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x492f13), "\x00\x20\x73", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49302d), "\x00\x20\x73", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4935b3), "\x00\x20\x73", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493652), "\x00\x20\x73", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493ba7), "\x00\x20\x73", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493c33), "\x00\x20\x73", 3, NULL);

		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44c76c), "\xF3\x0F\x5C\x0D\x78\x82\x54\x00", 8, NULL);


		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x722000), "\x00\x60\xEA\x4B", 4, NULL); // right extend

		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4476c7), "\x00\x20\x72", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44b263), "\x00\x20\x72", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44b331), "\x00\x20\x72", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x492dac), "\x00\x20\x72", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x492eec), "\x00\x20\x72", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x492f1b), "\x00\x20\x72", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493018), "\x00\x20\x72", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49359e), "\x00\x20\x72", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49363d), "\x00\x20\x72", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493b92), "\x00\x20\x72", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493c23), "\x00\x20\x72", 3, NULL);

		//WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x54830c), "\x00\x60\xEA\x4B", 4, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44c7f9), "\x0F\x60\xEA\x4B", 4, NULL);
	}
	else
	{
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x732000), "\x70\x03\xBB\x01", 4, NULL); //left extend

		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x447695), "\x78\x82\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44b249), "\x7C\x82\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44b25a), "\x00\x80\x96\x43", 4, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44b317), "\x7C\x82\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44b328), "\x00\x80\x96\x43", 4, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x492da1), "\x78\x82\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x492edb), "\x78\x82\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x492f13), "\x78\x82\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49302d), "\x78\x82\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4935b3), "\x78\x82\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493652), "\x78\x82\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493ba7), "\x78\x82\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493c33), "\x78\x82\x54", 3, NULL);

		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44c76c), "\xF3\x0F\x59\xC8\xF3\x0F\x5C\xC8", 8, NULL);


		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x722000), "\x0B\x2E\x5F\x35", 4, NULL); //right extend

		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4476c7), "\x0C\x83\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44b263), "\x0C\x83\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44b331), "\x0C\x83\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x492dac), "\x0C\x83\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x492eec), "\x0C\x83\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x492f1b), "\x0C\x83\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493018), "\x0C\x83\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49359e), "\x0C\x83\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49363d), "\x0C\x83\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493b92), "\x0C\x83\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493c23), "\x0C\x83\x54", 3, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44c7f9), "\x00\x7E\x6A\x47", 4, NULL);
	}
}

void EditorPauseLayer::Callback::PCToggler(CCObject*)
{
	setting().onPersistentClipboard = !setting().onPersistentClipboard;
}

void EditorPauseLayer::Callback::HideGridToggler(CCObject*)
{
	setting().onHideGrid = !setting().onHideGrid;
	if (setting().onHideGrid)
	{
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x492de0), "\xE9\x5A\x01\x00\x00\x90", 6, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x492f8a), "\xE9\x54\x01\x00\x00\x90", 6, NULL);
	}
	else
	{
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x492de0), "\x0F\x86\x59\x01\x00\x00", 6, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x492f8a), "\x0F\x86\x53\x01\x00\x00", 6, NULL);
	}
}

void EditorPauseLayer::Callback::HideTLinesToggler(CCObject*)
{
	setting().onHideTrigLines = !setting().onHideTrigLines;
	if (setting().onHideTrigLines)
	{
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493348), "\xE9\xCE\x00\x00\x00\x90", 6, NULL);
	}
	else
	{
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493348), "\x0F\x84\xCD\x00\x00\x00", 6, NULL);
	}
}

void EditorPauseLayer::Callback::SmoothTrailToggler(CCObject*)
{
	setting().onSmoothEditorTrail = !setting().onSmoothEditorTrail;
	if (setting().onSmoothEditorTrail)
	{
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x490f94), "\x90\x90", 2, NULL);
	}
	else
	{
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x490f94), "\x72\x79", 2, NULL);
	}
}

void EditorPauseLayer::Callback::FreeEditorScrollToggler(CCObject*)
{
	setting().onEditorScroll = !setting().onEditorScroll;
	if (setting().onEditorScroll) {
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44C845), "\xEB", 1, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44C85C), "\xEB", 1, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44C875), "\xEB", 1, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44C88C), "\xEB", 1, NULL);
	}
	else {
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44C845), "\x77", 1, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44C85C), "\x77", 1, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44C875), "\x77", 1, NULL);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44C88C), "\x77", 1, NULL);
	}
}



auto EETogglerSprite(CCSprite* toggleOn, CCSprite* toggleOff)
{
	return (setting().onEditorExtension) ? toggleOn : toggleOff;
}

auto PCTogglerSprite(CCSprite* toggleOn, CCSprite* toggleOff)
{
	return (setting().onPersistentClipboard) ? toggleOn : toggleOff;
}

auto HideGridSprite(CCSprite* toggleOn, CCSprite* toggleOff)
{
	return (setting().onHideGrid) ? toggleOn : toggleOff;
}

auto HideTLinesSprite(CCSprite* toggleOn, CCSprite* toggleOff)
{
	return (setting().onHideTrigLines) ? toggleOn : toggleOff;
}

auto SmoothTrailSprite(CCSprite* toggleOn, CCSprite* toggleOff)
{
	return (setting().onSmoothEditorTrail) ? toggleOn : toggleOff;
}

auto FreeEditorScrollSprite(CCSprite* toggleOn, CCSprite* toggleOff)
{
	return (setting().onEditorScroll) ? toggleOn : toggleOff;
}

bool __fastcall EditorPauseLayer::EditorPauseLayer_init_hook(CCLayer* self)
{
	editorPauseLayer = self;
	bool result = EditorPauseLayer::EditorPauseLayer_init(self);
	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();
	auto scrnRight = director->getScreenRight();
	auto menu = CCMenu::create();
	menu->setPosition({ 30, 120 });

	auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
	auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");


	auto EEButton = gd::CCMenuItemToggler::create(EETogglerSprite(toggleOn, toggleOff), EETogglerSprite(toggleOff, toggleOn), self, menu_selector(EditorPauseLayer::Callback::EEToggler));
	auto EElabel = CCLabelBMFont::create("Editor Extension", "bigFont.fnt");
	EEButton->setScale(0.70f);
	EElabel->setScale(0.33f);
	EElabel->setAnchorPoint({0.f, 0.5f});
	EElabel->setPositionX(20);
	//EEButton->setPosition({ 30, 120 });
	menu->addChild(EEButton);
	menu->addChild(EElabel);
	

	auto PCButton = gd::CCMenuItemToggler::create(PCTogglerSprite(toggleOn, toggleOff), PCTogglerSprite(toggleOff, toggleOn), self, menu_selector(EditorPauseLayer::Callback::PCToggler));
	auto PClabel = CCLabelBMFont::create("Persistent Clipboard", "bigFont.fnt");
	PCButton->setScale(0.70f);
	PClabel->setScale(0.30f);
	PClabel->setAnchorPoint({ 0.f, 0.5f });
	PClabel->setPosition({ 20, 30 });
	PCButton->setPositionY(30);
	menu->addChild(PCButton);
	menu->addChild(PClabel);


	auto HGButton = gd::CCMenuItemToggler::create(HideGridSprite(toggleOn, toggleOff), HideGridSprite(toggleOff, toggleOn), self, menu_selector(EditorPauseLayer::Callback::HideGridToggler));
	auto HGlabel = CCLabelBMFont::create("Hide Grid", "bigFont.fnt");
	HGButton->setScale(0.70f);
	HGlabel->setScale(0.33f);
	HGlabel->setAnchorPoint({ 0.f, 0.5f });
	HGlabel->setPosition({ 20, 60 });
	HGButton->setPositionY(60);
	menu->addChild(HGButton);
	menu->addChild(HGlabel);


	auto HTLButton = gd::CCMenuItemToggler::create(HideTLinesSprite(toggleOn, toggleOff), HideTLinesSprite(toggleOff, toggleOn), self, menu_selector(EditorPauseLayer::Callback::HideTLinesToggler));
	auto HTLlabel = CCLabelBMFont::create("Hide Trigger Lines", "bigFont.fnt");
	HTLButton->setScale(0.70f);
	HTLlabel->setScale(0.30f);
	HTLlabel->setAnchorPoint({ 0.f, 0.5f });
	HTLlabel->setPosition({ 20, 90 });
	HTLButton->setPositionY(90);
	menu->addChild(HTLButton);
	menu->addChild(HTLlabel);


	auto SETButton = gd::CCMenuItemToggler::create(SmoothTrailSprite(toggleOn, toggleOff), SmoothTrailSprite(toggleOff, toggleOn), self, menu_selector(EditorPauseLayer::Callback::SmoothTrailToggler));
	auto SETlabel = CCLabelBMFont::create("Smooth Editor Trail", "bigFont.fnt");
	SETButton->setScale(0.70f);
	SETlabel->setScale(0.30f);
	SETlabel->setAnchorPoint({ 0.f, 0.5f });
	SETlabel->setPosition({ 20, 120 });
	SETButton->setPositionY(120);
	menu->addChild(SETButton);
	menu->addChild(SETlabel);


	auto FESButton = gd::CCMenuItemToggler::create(FreeEditorScrollSprite(toggleOn, toggleOff), FreeEditorScrollSprite(toggleOff, toggleOn), self, menu_selector(EditorPauseLayer::Callback::FreeEditorScrollToggler));
	auto FESlabel = CCLabelBMFont::create("Free Editor Scroll", "bigFont.fnt");
	FESButton->setScale(0.70f);
	FESlabel->setScale(0.30f);
	FESlabel->setAnchorPoint({ 0.f, 0.5f });
	FESlabel->setPosition({ 20, 150 });
	FESButton->setPositionY(150);
	menu->addChild(FESButton);
	menu->addChild(FESlabel);

	//auto passInfoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
	auto vanillaSelectAllSprite = gd::ButtonSprite::create("Select All\non layer", 0x32, 0, 0.6f, true, "bigFont.fnt", "GJ_button_04.png", 30.0);
	auto vanillaSelectAllButton = gd::CCMenuItemSpriteExtra::create(vanillaSelectAllSprite, nullptr, self, menu_selector(EditorPauseLayer::Callback::VanillaSelectAllButton));
	vanillaSelectAllButton->setPosition({scrnRight - 80, 30});
	menu->addChild(vanillaSelectAllButton);

	auto SelectAbsolutelyAllSprite = gd::ButtonSprite::create("Select All", 0x32, 0, 0.6f, true, "bigFont.fnt", "GJ_button_04.png", 30.0);
	auto SelectAbsolutelyAllButton = gd::CCMenuItemSpriteExtra::create(SelectAbsolutelyAllSprite, nullptr, self, menu_selector(EditorPauseLayer::Callback::SelectAbsolutelyAllButton));
	SelectAbsolutelyAllButton->setPosition({ scrnRight - 80, 75 });
	menu->addChild(SelectAbsolutelyAllButton);
	
	if (setting().onDebugLabels)
	{
		auto eplbl = CCLabelBMFont::create("", "chatFont.fnt");
		eplbl->setString(CCString::createWithFormat("epl pointer: %p", self)->getCString());
		eplbl->setAnchorPoint({ 1.f, 0.5f });
		eplbl->setPosition(CCDirector::sharedDirector()->getScreenRight() - 120, CCDirector::sharedDirector()->getScreenTop() - 10);
		eplbl->setScale(0.5f);
		self->addChild(eplbl);
	}

	self->addChild(menu);
	return result;
}

//bool __fastcall EditorPauseLayer::EditorUILayer_init_H(gd::EditorUI* self, void*, CCLayer* editor)
//{
//	//editUI = self;
//	//self->setRotation(45.f);
//	bool result = EditorUILayer_init(self, editor);
//	if (setting().onPersistentClipboard)
//	{
//		if (!savedClipboard.empty()) {
//			self->clipboard() = savedClipboard;
//			self->updateButtons();
//		}
//	}
//	return result;
//}
//
//
//void __fastcall EditorPauseLayer::EditorUILayer_dtor_H(gd::EditorUI* self)
//{
//	if (setting().onPersistentClipboard)
//	{
//		savedClipboard = self->clipboard();
//	}
//	if (isPressed)
//	{
//		isPressed = !isPressed;
//		self->getLevelEditorLayer()->removeAllObjects();
//	}
//	EditorUILayer_dtor(self);
//}
//EditorPauseLayer Address
//gd::base + 3e150 //GHS 1.91
//gd::base + 73550 //GD 2.1
