#include "menu.h"

#include "includes.h"

#include <imgui-hook.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <CCApplication.h>
#include <cocos2d.h>
#include "EditorPauseLayer.h"
#include "state.h"

#include <fstream>
#include <filesystem>
#include "utils.hpp"
#include <chrono>
#include <audio_nodes/FMODAudioEngine.h>

#include "explorer.hpp"
using namespace cocos2d;

bool show = false;

bool gravity = 0;
bool onDisableUI = false;

ImVec4 color1;
ImVec4 color2;
ImVec4 color3;
ImVec4 color4;
ImVec4 color5;
ImVec4 color6;

DWORD libcocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");

typedef void* (__cdecl* fSharedApplication)();
typedef void(__thiscall* fSetAnimationInterval)(void* instance, double delay);
fSharedApplication sharedApplication;
fSetAnimationInterval setAnimInterval;

float interval = 60;
/*
void Fps(int FPS)
{
    interval = 1.0f / FPS;

    HMODULE hMod = LoadLibraryA("libcocos2d.dll");

    sharedApplication = (fSharedApplication)GetProcAddress(hMod, "?sharedApplication@CCApplication@cocos2d@@SAPAV12@XZ");
    setAnimInterval = (fSetAnimationInterval)GetProcAddress(hMod, "?setAnimationInterval@CCApplication@cocos2d@@UAEXN@Z");

    void* application = sharedApplication();
    setAnimInterval(application, interval);

    CCDirector::sharedDirector()->setAnimationInterval(1.0 / 240);
}
*/
void colorSet( ) {
    auto *colors = ImGui::GetStyle( ).Colors;

    color1.x = setting( ).Overlaycolor [ 0 ];
    color1.y = setting( ).Overlaycolor [ 1 ];
    color1.z = setting( ).Overlaycolor [ 2 ];
    color1.w = setting( ).Overlaycolor [ 3 ]-0.4;

    color2.x = setting( ).Overlaycolor [ 0 ];
    color2.y = setting( ).Overlaycolor [ 1 ];
    color2.z = setting( ).Overlaycolor [ 2 ];
    color2.w = 1;

    color3.x = setting( ).Overlaycolor [ 0 ]+0.3;
    color3.y = setting( ).Overlaycolor [ 1 ]+0.3;
    color3.z = setting( ).Overlaycolor [ 2 ]+0.3;
    color3.w = setting( ).Overlaycolor [ 3 ]+0.3;

    color4.x = setting( ).Overlaycolor [ 0 ]-0.1;
    color4.y = setting( ).Overlaycolor [ 1 ]-0.1;
    color4.z = setting( ).Overlaycolor [ 2 ]-0.1;
    color4.w = setting( ).Overlaycolor [ 3 ]-0.1;

    color5.x = setting( ).Overlaycolor [ 0 ]+0.1;
    color5.y = setting( ).Overlaycolor [ 1 ]+0.1;
    color5.z = setting( ).Overlaycolor [ 2 ]+0.1;
    color5.w = setting( ).Overlaycolor [ 3 ]+0.1;

    color6.x = setting( ).BGcolor [ 0 ];
    color6.y = setting( ).BGcolor [ 1 ];
    color6.z = setting( ).BGcolor [ 2 ];
    color6.w = setting( ).BGcolor [ 3 ];

    //colors[ImGuiCol_Text] = ImVec4(1, 0, 0, 1);
    colors [ ImGuiCol_WindowBg ] = color6;
    colors [ ImGuiCol_Border ] = color2;
    colors [ ImGuiCol_BorderShadow ] = color2;
    colors [ ImGuiCol_Button ] = color3;
    colors [ ImGuiCol_ButtonHovered ] = color1;
    colors [ ImGuiCol_ButtonActive ] = color1;
    colors [ ImGuiCol_CheckMark ] = ImVec4(1, 1, 1, 1);
    colors [ ImGuiCol_FrameBg ] = color1;
    colors [ ImGuiCol_FrameBgHovered ] = color3;
    colors [ ImGuiCol_FrameBgActive ] = color3;
    colors [ ImGuiCol_Tab ] = ImVec4(color5.x, color5.y, color5.z, color5.w-0.2);
    colors [ ImGuiCol_TabHovered ] = color3;
    colors [ ImGuiCol_TabActive ] = color3;
    colors [ ImGuiCol_TitleBg ] = color3;
    colors [ ImGuiCol_TitleBgActive ] = color3;
    colors [ ImGuiCol_TitleBgCollapsed ] = color3;
    colors [ ImGuiCol_SliderGrab ] = color4;
    colors [ ImGuiCol_SliderGrabActive ] = color4;
    colors [ ImGuiCol_TextSelectedBg ] = color2;
    colors [ ImGuiCol_Header ] = color1;
    colors [ ImGuiCol_HeaderHovered ] = color1;
    colors [ ImGuiCol_HeaderActive ] = color2;
}

void update_speed_hack() {
    const auto value = setting().onSpeedhack ? setting().gamespeed : 1.f;
    if (auto fme = gd::FMODAudioEngine::sharedEngine())
        if (auto sound = fme->currentSound())
            if (setting().onSpeedhackMusic) sound->setPitch(value);
            else sound->setPitch(1.f);
    CCDirector::sharedDirector()->m_pScheduler->setTimeScale(value);
}

void update_fps_bypass() {
    const auto value = setting().onFPSbypass ? setting().fpslol : 60.f;
    static const auto addr = cocos_symbol<&CCApplication::setAnimationInterval>("?setAnimationInterval@CCApplication@cocos2d@@UAEXN@Z");
    addr(CCApplication::sharedApplication(), 1.0 / value);
    CCDirector::sharedDirector()->setAnimationInterval(1.0 / value);
}


void save() {
    auto file = fopen("Resources/hacksave.dat", "wb");
    if (file) {
        fwrite(&setting(), sizeof(setting()), 1, file);
        fclose(file);
    }
}




void cheatDec()
{
    setting().cheatsCount--;
}

void cheatAdd()
{
    setting().cheatsCount++;
    setting().beforeRestartCheatsCount++;
}




bool oneX = true;

void RenderMain() {
    //ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Calibri.ttf", 18.f);
    //ImFont* font = ImGui::GetIO().Fonts->Fonts.back();

    //HackIH HapihObj;
    //HapihObj.bind("GeometricalHistorical.exe");
    //PointerIH pHidtrigger = { HapihObj.BaseAddress, 0x0016A1B8, 0xF0, 0x18, 0x8, 0x0, 0xE8, 0xFC };
    unsigned long long pointervalue;
    if (oneX) {
        auto file = fopen("Resources/hacksave.dat", "rb");
        if (file) {
            fseek(file, 0, SEEK_END);
            auto size = ftell(file);

            if (size == sizeof(setting())) {
                fseek(file, 0, SEEK_SET);
                fread(&setting(), sizeof(setting()), 1, file);
                fclose(file);
            }
        }

        if (ImGui::Begin("Global", nullptr))
        {
            ImGui::SetWindowPos({ 5, 5 });
            ImGui::EndTabItem();
        }
        if (ImGui::Begin("Cosmetic", nullptr))
        {
            ImGui::SetWindowPos({ 347, 5 });
            ImGui::EndTabItem();
        }
        if (ImGui::Begin("Creator", nullptr))
        {
            ImGui::SetWindowPos({ 712, 5 });
            ImGui::EndTabItem();
        }
        if (ImGui::Begin("Player", nullptr))
        {
            ImGui::SetWindowPos({ 1220, 5 });
            ImGui::EndTabItem();
        }
        if (ImGui::Begin("Icons", nullptr))
        {
            ImGui::SetWindowPos({ 347, 539 });
            ImGui::EndTabItem();
        }
        if (ImGui::Begin("Settings", nullptr))
        {
            ImGui::SetWindowPos({ 712, 569 });
            ImGui::EndTabItem();
        }
        if (ImGui::Begin("Status", nullptr))
        {
            ImGui::SetWindowPos({ 1220, 423 });
            ImGui::EndTabItem();
        }
        if (ImGui::Begin("Credits", nullptr))
        {
            ImGui::SetWindowPos({ 1478, 922 });
            ImGui::EndTabItem();
        }


        if (setting().onCopyHack) { // is enabled ---------------------------- COPY HACK
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bd30), "\x39\xC0\x90", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bd1b), "\x90\x90\x90\x90\x90\x90", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49d535), "\x90\x90", 2, NULL);
        }
        else { // is disabled
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bd30), "\x83\xF8\x01", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bd1b), "\x0F\x84\x56\x01\x00\x00", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49d535), "\x75\x0E", 2, NULL);
        }
        if (setting().onNoclip) { // is enabled --------------------------- NOCLIP
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EE9D9), "\xE9\xF0\x02\x00\x00\x90", 6, NULL);
            //cheatAdd();
        }
        else { // is disabled
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EE9D9), "\x0F\x85\xEF\x02\x00\x00", 6, NULL);
        }
        if (setting().onVerify) { // is enabled --------------------------- VERIFY HACK
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43D4E0), "\xEB", 1, NULL);
        }
        else { // is disabled
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43D4E0), "\x75", 1, NULL);
        }
        if (setting().onCmark) { // is enabled --------------------------- CMARK HACK
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4543E0), "\xB8\x00\x00\x00\x00\x90", 6, NULL);
        }
        else { // is disabled
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4543E0), "\x8B\x81\x04\x02\x00\x00", 6, NULL);
        }
        if (setting().onObjectHack) { // is enabled --------------------------- OBJECT HACK
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43E08E), "\x68\xFF\xFF\xFF\x7F", 5, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448da5), "\x3D\xFF\xFF\xFF\x7F", 5, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4475b2), "\x3D\xFF\xFF\xFF\x7F", 5, NULL);
        }
        else { // is disabled
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43E08E), "\x68\x20\x4E\x00\x00", 5, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448da5), "\x3D\x20\x4E\x00\x00", 5, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4475b2), "\x3D\x20\x4E\x00\x00", 5, NULL);
        }
        if (setting().onRestartBtn) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d4a99), "\x90\x90", 2, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d4a99), "\x75\x29", 2, NULL);
        }
        if (setting().onIconBypass) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47f7ef), "\xE9\x74\x02\x00\x00\x90", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47dde6), "\xE9\xDC\x00\x00\x00\x90", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47d868), "\x90\x90", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47d8f8), "\x90\x90", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47d988), "\x90\x90", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47da18), "\x90\x90", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47daa8), "\x90\x90", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47e19e), "\xEB\x13", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47e28e), "\xEB\x13", 2, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47f7ef), "\x0F\x85\x73\x02\x00\x00", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47dde6), "\x0F\x85\xDB\x00\x00\x00", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47d868), "\x74\x04", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47d8f8), "\x74\x04", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47d988), "\x74\x04", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47da18), "\x74\x04", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47daa8), "\x74\x04", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47e19e), "\x75\x13", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47e28e), "\x75\x13", 2, NULL);
        }
        if (setting().onEditBtn) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d48af), "\x90\x90", 2, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d48af), "\x75\x62", 2, NULL);
        }
        if (setting().onLevelBypass) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4a81d2), "\xE9\x98\x00\x00\x00\x90", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A772F), "\xE9\xA3\x02\x00\x00\x90", 6, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4a81d2), "\x0F\x8E\x97\x00\x00\x00", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A772F), "\x0F\x84\xA2\x02\x00\x00", 6, NULL);
        }
        if (setting().onSafeMode) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EEB14), "\xEB\x6C", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E38E6), "\xE9\x77\x01\x00\x00\x90", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E3949), "\xE9\x14\x01\x00\x00\x90", 6, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EEB14), "\x75\x6C", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E38E6), "\x0F\x85\x76\x01\x00\x00", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E3949), "\x0F\x85\x13\x01\x00\x00", 6, NULL);
        }
        if (setting().onPracticeMusic) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0D1F), "\x90\x90\x90\x90\x90\x90", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F1AD3), "\x90\x90", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EEB89), "\x90\x90", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EEBBB), "\x90\x90", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F1B01), "\x90\x90\x90\x90\x90", 5, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0D1F), "\x0F\x85\xF2\x06\x00\x00", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F1AD3), "\x75\x41", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EEB89), "\x75\x3E", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EEBBB), "\x75\x0C", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F1B01), "\xE8\xFA\x5C\xF2\xFF", 5, NULL);
        }
        if (setting().onEsc) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E384B), "\x00", 1, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E0E46), "\x90\x90\x90\x90\x90\x90", 6, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E384B), "\x01", 1, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E0E46), "\x88\x81\xF9\x02\x00\x00", 6, NULL);
        }
        if (setting().onInstaComplete) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DFC36), "\xC7\x87\x74\x04\x00\x00\x00\x00\x00\x70\x90\x90", 12, NULL);
            //cheatAdd();
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DFC36), "\xF3\x0F\x11\x8F\x74\x04\x00\x00\x9F\xF6\xC4\x44", 12, NULL);
        }
        if (setting().trailStateId == 0)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC406), "\x00", 1, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC376), "\x01", 1, NULL);
        }
        if (setting().trailStateId == 1)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC406), "\x01", 1, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC376), "\x01", 1, NULL);
        }
        if (setting().trailStateId == 2)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC406), "\x00", 1, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC376), "\x00", 1, NULL);
        }
        if (setting().trailStateId == 3)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC406), "\x01", 1, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC376), "\x00", 1, NULL);
        }
        if (setting().onInfiniteJumps) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D8A60), "\x01", 1, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d87e5), "\x01", 1, NULL);
            //cheatAdd();
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D8A60), "\x00", 1, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d87e5), "\x00", 1, NULL);
        }
        if (setting().onWaveTrail) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DF286), "\x00", 1, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DF286), "\x01", 1, NULL);
        }
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
        if (setting().onSongBypass) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4998BF), "\x90\x90", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4998D1), "\x90\x90\x90", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4998E0), "\x90\x90\x90", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49993E), "\x90\x90", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x499950), "\x90\x90\x90", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49995F), "\x90\x90\x90", 3, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4998BF), "\x74\x4E", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4998D1), "\x0F\x4F\xF0", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4998E0), "\x0F\x48\xF1", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49993E), "\x74\x4E", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x499950), "\x0F\x4F\xF0", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49995F), "\x0F\x48\xF1", 3, NULL);
        }
        if (setting().onNoTransition) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xA2C94), "\x90\x90\x90\x90\x90", 5, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xA2C94), "\xF3\x0F\x10\x45\x08", 5, NULL);
        }
        if (setting().onHideAttempts)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f190a), "\x5F", 1, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e31ae), "\x06\x37", 2, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f190a), "\x54", 1, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e31ae), "\xFC\x36", 2, NULL);
        }
        if (setting().onNoDeathEffect)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DC3C1), "\xE9\xE7\x01\x00\x00\x90", 6, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DC3C1), "\x0F\x84\xD5\x01\x00\x00", 6, NULL);
        }
        if (setting().onSecondAttempt)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F1903), "\x6A\x02\x90\x90\x90\x90", 6, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F1903), "\xFF\xB7\xE8\x02\x00\x00", 6, NULL);
        }
        if (setting().onHidePracticeButtons)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4fd154), "\x90\xD7", 2, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4fd154), "\xE8\xD6", 2, NULL);
        }
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
        if (setting().onHideTrigLines)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493348), "\xE9\xCE\x00\x00\x00\x90", 6, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493348), "\x0F\x84\xCD\x00\x00\x00", 6, NULL);
        }
        if (setting().onSmoothEditorTrail)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x490f94), "\x90\x90", 2, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x490f94), "\x72\x79", 2, NULL);
        }
        if (setting().onSolidWave)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d809e), "\x90\x90", 2, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d809e), "\x75\x0c", 2, NULL);
        }
        if (setting().onPracticeCollectCoins)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e9678), "\x90\x90", 2, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e9678), "\x75\x4c", 2, NULL);
        }
        if (setting().onDisableForceGlow)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4de236), "\x00", 1, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4de236), "\x01", 1, NULL);
        }
        if (setting().onZoomBypass)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448a85), "\x90\x90\x90", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448a8a), "\x90\x90\x90", 3, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448a85), "\x0F\x2F\xC8", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448a8a), "\x0F\x28\xC8", 3, NULL);
        }
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
        if (setting().onInstantMirror)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EF215), "\x00", 1, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EF215), "\x3F", 1, NULL);
        }
        if (setting().onNoMirror)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EF0E2), "\xE9\x87\x01\x00\x00\x90", 6, NULL); //E9 87 01 00 00 90
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EF0E2), "\x0F\x84\x86\x01\x00\x00", 6, NULL); //0F 84 86 01 00 00
        }
        if (setting().onTextLength)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41474E), "\xEB", 1, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41474E), "\x7C", 1, NULL);
        }
        if (setting().onCharFilter)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x414717), "\x90\x90", 2, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x414717), "\x75\x04", 2, NULL);
        }
        if (setting().onPlaceOver)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48C8EF), "\xEB", 1, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48C8EF), "\x77", 1, NULL);
        }
        if (setting().onNoPulse)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42992a), "\x90\x90\x90\x90\x90\x90\x90\x90", 8, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42992a), "\xF3\x0F\x11\x87\xAC\x01\x00\x00", 8, NULL);
        }
        if (setting().onNoParticles)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xb4f65), "\x31\xF6", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xb6626), "\x00", 1, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xb4f65), "\x8B\xF0", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xb6626), "\x01", 1, NULL);
        }
        if (setting().onNoObjectsGlow)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x46D1B3), "\xE9\x8F\x01\x00\x00\x90", 6, NULL); //E9 8F 01 00 00 90
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x46D1B3), "\x0F\x85\x8E\x01\x00\x00", 6, NULL); //0F 85 8E 01 00 00
        }
        if (setting().onKrmalMode)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47237E), "\x8B\xD8\x90", 3, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47237E), "\x0F\x45\xD8", 3, NULL);
        }
        if (setting().onCameraV2)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EB75B), "\x90\x90", 2, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EB75B), "\x76\x0E", 2, NULL);
        }
        if (setting().onNoRespBlink)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DBB97), "\x00\x00\x00\x00", 4, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DBB97), "\xCD\xCC\xCC\x3E", 4, NULL);
        }
        if (setting().onFreeWindowResize)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x10f66b), "\x90\x90\x90\x90\x90", 5, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x10f66b), "\xE8\xA0\xF3\xFF\xFF", 5, NULL);
        }
        if (setting().onNoShade)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EA4FF), "\xEB", 1, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EA4FF), "\x77", 1, NULL);
        }
        if (setting().onMaxParticles)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xB6489), "\x07", 1, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xB6489), "\x08", 1, NULL);
        }
        if (setting().onLowVolume)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D02B0), "\xEB", 1, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D02B0), "\x76", 1, NULL);
        }
        if (setting().onInstantTriggers)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x89650), "\xC0", 1, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x89650), "\xC1", 1, NULL);
        }
        if (setting().onTransparentLists)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x487FBF), "\x00\x00\x00\x40", 4, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x489EB5), "\x00\x00\x00\x40", 4, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A4A), "\x60", 1, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A19), "\x20\x20", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A1C), "\x20", 1, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A0F), "\x40\x40", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A12), "\x40", 1, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x487FBF), "\xBF\x72\x3E\xFF", 4, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x489EB5), "\xBF\x72\x3E\xFF", 4, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A4A), "\xFF", 1, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A19), "\xA1\x58", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A1C), "\x2C", 1, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A0F), "\xC2\x72", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A12), "\x3E", 1, NULL);
        }
        if (setting().onTransparentBG)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42CD26), "\x90\xB1\xFF", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42CD2E), "\xFF\xFF", 2, NULL);

            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43B55B), "\x90\xB1\xFF", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43B563), "\xFF\xFF", 2, NULL);

            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4876a2), "\x90\xB1\xFF", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4876aa), "\xFF\xFF", 2, NULL);

            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x488c0e), "\x90\xB1\xFF", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x488c15), "\xFF\xFF", 2, NULL);

            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49b331), "\x90\xB1\xFF", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49b339), "\xFF\xFF", 2, NULL);

            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49ef1c), "\x90\xB1\xFF", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49ef24), "\xFF\xFF", 2, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42CD26), "\x80\xC9\xFF", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42CD2E), "\x00\x66", 2, NULL);

            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43B55B), "\x80\xC9\xFF", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43B563), "\x00\x66", 2, NULL);

            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4876a2), "\x80\xC9\xFF", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4876aa), "\x00\x66", 2, NULL);

            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x488c0e), "\x80\xC9\xFF", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x488c15), "\x00\x66", 2, NULL);

            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49b331), "\x80\xC9\xFF", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49b339), "\x00\x66", 2, NULL);

            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49ef1c), "\x80\xC9\xFF", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49ef24), "\x00\x66", 2, NULL);
        }
        if (setting().onForceVisibility)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x607A3), "\xB0\x01\x90", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60CBA), "\x90\x90\x90\x90\x90\x90", 6, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x607A3), "\x8A\x45\x08", 3, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60CBA), "\x0F\x84\xCB\x00\x00\x00", 6, NULL);
        }
        if (setting().onNoRotation)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60598), "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60598), "\xF3\x0F\x11\x41\x1C\xF3\x0F\x11\x41\x18", 10, NULL);
        }
        if (setting().onFastRestart)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EE9EB), "\xC7\x45\x08\x00\x00\x00\x3F\x90\x90\x90\x90\x90\x90", 13, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EE9EB), "\xF3\x0F\x10\x05\xAC\x7F\x54\x00\xF3\x0F\x11\x45\x08", 13, NULL);
        }
        if (setting().onSolidTrail)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC38C), "\x90\x90\x90", 3, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC38C), "\x89\x41\x10", 3, NULL);
        }
        if (setting().onBetterLevelUpdate)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49e1ed), "\x90\x90", 2, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49e1ed), "\x74\x0e", 2, NULL);
        }
        if (setting().onConfirmExit)
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d6540), "\x90\x90\x90\x90\x90", 5, NULL); //Deleting exit sequence from onQuit button

            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d654d), "\x90\x90", 2, NULL); //Deleting quitSound effect from original exit sequence
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d655d), "\x90\x90\x90\x90\x90", 5, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d6565), "\x90\x90\x90\x90\x90", 5, NULL);
        }
        else
        {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d6540), "\xE8\x9B\xBA\x01\x00", 5, NULL); //Restoring exit sequence from onQuit button

            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d654d), "\x6A\x10", 2, NULL); //Restoring quitSound effect from original exit sequence
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d655d), "\x68\x5C\x22\x51\x00", 5, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d6565), "\xE8\x86\xF1\xF2\xFF", 5, NULL);
        }

        //Fps(setting().fpslol);
        colorSet( );

        oneX = false;
    }
    
    //const auto& font = ImGui::GetIO().Fonts->Fonts.back();
    //ImGui::PushFont(font);
    
    if (show) {

        if ( setting( ).onExplorer )
            render_explorer_window(setting( ).onExplorer);

        //ImGui::PushFont(font);
        if (ImGui::Begin("Global", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
        {
            ImGui::SetWindowFontScale(setting().UISize);
            ImGui::SetNextItemWidth(120 * setting().UISize);
            if (ImGui::DragFloat("##fpsbypass", &setting().fpslol, 1.f, 1.f, 360.f))
                update_fps_bypass();
            ImGui::SameLine();
            if (ImGui::Checkbox("FPS bypass", &setting().onFPSbypass))
                update_fps_bypass();


            if (ImGui::Checkbox("Show Restart Button", &setting().onRestartBtn)) {
                if (setting().onRestartBtn) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d4a99), "\x90\x90", 2, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d4a99), "\x75\x29", 2, NULL);
                }
            }
            if (ImGui::Checkbox("Show Edit Button", &setting().onEditBtn)) {
                if (setting().onEditBtn) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d48af), "\x90\x90", 2, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d48af), "\x75\x62", 2, NULL);
                }
            }
            if (ImGui::Checkbox("Icon Bypass", &setting().onIconBypass)) {
                if (setting().onIconBypass) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47f7ef), "\xE9\x74\x02\x00\x00\x90", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47dde6), "\xE9\xDC\x00\x00\x00\x90", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47d868), "\x90\x90", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47d8f8), "\x90\x90", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47d988), "\x90\x90", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47da18), "\x90\x90", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47daa8), "\x90\x90", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47e19e), "\xEB\x13", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47e28e), "\xEB\x13", 2, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47f7ef), "\x0F\x85\x73\x02\x00\x00", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47dde6), "\x0F\x85\xDB\x00\x00\x00", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47d868), "\x74\x04", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47d8f8), "\x74\x04", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47d988), "\x74\x04", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47da18), "\x74\x04", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47daa8), "\x74\x04", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47e19e), "\x75\x13", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47e28e), "\x75\x13", 2, NULL);
                }
            }
            if (ImGui::Checkbox("Level Bypass", &setting().onLevelBypass)) {
                if (setting().onLevelBypass) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4a81d2), "\xE9\x98\x00\x00\x00\x90", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A772F), "\xE9\xA3\x02\x00\x00\x90", 6, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4a81d2), "\x0F\x8E\x97\x00\x00\x00", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A772F), "\x0F\x84\xA2\x02\x00\x00", 6, NULL);
                }
            }
            if (ImGui::Checkbox("Safe Mode", &setting().onSafeMode)) {
                if (setting().onSafeMode) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EEB14), "\xEB\x6C", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E38E6), "\xE9\x77\x01\x00\x00\x90", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E3949), "\xE9\x14\x01\x00\x00\x90", 6, NULL);
                }
                else {
                    setting().safeNoclip = false;
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EEB14), "\x75\x6C", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E38E6), "\x0F\x85\x76\x01\x00\x00", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E3949), "\x0F\x85\x13\x01\x00\x00", 6, NULL);
                }
            }
            if (ImGui::Checkbox("No Transition", &setting().onNoTransition)) {
                if (setting().onNoTransition) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xA2C94), "\x90\x90\x90\x90\x90", 5, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xA2C94), "\xF3\x0F\x10\x45\x08", 5, NULL);
                }
            }

            if (ImGui::Checkbox("Practice Music Hack", &setting().onPracticeMusic)) {
                if (setting().onPracticeMusic) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0D1F), "\x90\x90\x90\x90\x90\x90", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F1AD3), "\x90\x90", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EEB89), "\x90\x90", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EEBBB), "\x90\x90", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F1B01), "\x90\x90\x90\x90\x90", 5, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0D1F), "\x0F\x85\xF2\x06\x00\x00", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F1AD3), "\x75\x41", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EEB89), "\x75\x3E", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EEBBB), "\x75\x0C", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F1B01), "\xE8\xFA\x5C\xF2\xFF", 5, NULL);
                }
            }

            if (ImGui::Checkbox("Esc During Complete", &setting().onEsc)) {
                if (setting().onEsc) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E384B), "\x00", 1, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E0E46), "\x90\x90\x90\x90\x90\x90", 6, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E384B), "\x01", 1, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E0E46), "\x88\x81\xF9\x02\x00\x00", 6, NULL);
                }
            }
            if (ImGui::Checkbox("Text Length Bypass", &setting().onTextLength))
            {
                if (setting().onTextLength)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41474E), "\xEB", 1, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41474E), "\x7C", 1, NULL);
                }
            }
            if (ImGui::Checkbox("Char Filter Bypass", &setting().onCharFilter))
            {
                if (setting().onCharFilter)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x414717), "\x90\x90", 2, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x414717), "\x75\x04", 2, NULL);
                }
            }
            ImGui::Checkbox("Retry Keybind 'R'", &setting().onRetryBind);
            if (ImGui::Checkbox("Krmal Mode", &setting().onKrmalMode))
            {
                if (setting().onKrmalMode)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47237E), "\x8B\xD8\x90", 3, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47237E), "\x0F\x45\xD8", 3, NULL);
                }
            }
            //if (ImGui::IsItemHovered())
            //{
            //    ImGui::SetTooltip("Invisible objects in game. By Pololak.", setting().UISize);
            //}
            if (ImGui::Checkbox("Camera V2", &setting().onCameraV2))
            {
                if (setting().onCameraV2)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EB75B), "\x90\x90", 2, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EB75B), "\x76\x0E", 2, NULL);
                }
            }
            if (ImGui::Checkbox("Free Window Resize", &setting().onFreeWindowResize))
            {
                if (setting().onFreeWindowResize)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x10f66b), "\x90\x90\x90\x90\x90", 5, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x10f66b), "\xE8\xA0\xF3\xFF\xFF", 5, NULL);
                }
            }
            if (ImGui::Checkbox("Max Particles", &setting().onMaxParticles))
            {
                if (setting().onMaxParticles)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xB6489), "\x07", 1, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xB6489), "\x08", 1, NULL);
                }
            }
            if (ImGui::Checkbox("Allow Low Volume", &setting().onLowVolume))
            {
                if (setting().onLowVolume)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D02B0), "\xEB", 1, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D02B0), "\x76", 1, NULL);
                }
            }
            if (ImGui::Checkbox("Instant Triggers", &setting().onInstantTriggers))
            {
                if (setting().onInstantTriggers)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x89650), "\xC0", 1, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x89650), "\xC1", 1, NULL);
                }
            }
            if (ImGui::Checkbox("Transparent Lists", &setting().onTransparentLists))
            {
                if (setting().onTransparentLists)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x487FBF), "\x00\x00\x00\x40", 4, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x489EB5), "\x00\x00\x00\x40", 4, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A4A), "\x60", 1, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A19), "\x20\x20", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A1C), "\x20", 1, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A0F), "\x40\x40", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A12), "\x40", 1, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x487FBF), "\xBF\x72\x3E\xFF", 4, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x489EB5), "\xBF\x72\x3E\xFF", 4, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A4A), "\xFF", 1, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A19), "\xA1\x58", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A1C), "\x2C", 1, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A0F), "\xC2\x72", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431A12), "\x3E", 1, NULL);
                }
            }
            if (ImGui::Checkbox("Transparent BG", &setting().onTransparentBG))
            {
                if (setting().onTransparentBG)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42CD26), "\x90\xB1\xFF", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42CD2E), "\xFF\xFF", 2, NULL);

                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43B55B), "\x90\xB1\xFF", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43B563), "\xFF\xFF", 2, NULL);

                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4876a2), "\x90\xB1\xFF", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4876aa), "\xFF\xFF", 2, NULL);

                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x488c0e), "\x90\xB1\xFF", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x488c15), "\xFF\xFF", 2, NULL);

                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49b331), "\x90\xB1\xFF", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49b339), "\xFF\xFF", 2, NULL);

                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49ef1c), "\x90\xB1\xFF", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49ef24), "\xFF\xFF", 2, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42CD26), "\x80\xC9\xFF", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42CD2E), "\x00\x66", 2, NULL);

                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43B55B), "\x80\xC9\xFF", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43B563), "\x00\x66", 2, NULL);

                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4876a2), "\x80\xC9\xFF", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4876aa), "\x00\x66", 2, NULL);

                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x488c0e), "\x80\xC9\xFF", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x488c15), "\x00\x66", 2, NULL);

                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49b331), "\x80\xC9\xFF", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49b339), "\x00\x66", 2, NULL);

                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49ef1c), "\x80\xC9\xFF", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49ef24), "\x00\x66", 2, NULL);
                }
            }
            if (ImGui::Checkbox("Force Visibility", &setting().onForceVisibility))
            {
                if (setting().onForceVisibility)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x607A3), "\xB0\x01\x90", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60CBA), "\x90\x90\x90\x90\x90\x90", 6, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x607A3), "\x8A\x45\x08", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60CBA), "\x0F\x84\xCB\x00\x00\x00", 6, NULL);
                }
            }
            if (ImGui::Checkbox("No Rotation", &setting().onNoRotation))
            {
                if (setting().onNoRotation)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60598), "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60598), "\xF3\x0F\x11\x41\x1C\xF3\x0F\x11\x41\x18", 10, NULL);
                }
            }
            if (ImGui::Checkbox("Better Level Update", &setting().onBetterLevelUpdate))
            {
                if (setting().onBetterLevelUpdate)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49e1ed), "\x90\x90", 2, NULL);    
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49e1ed), "\x74\x0e", 2, NULL);
                }
            }
            if (ImGui::Checkbox("Confirm exit", &setting().onConfirmExit))
            {
                if (setting().onConfirmExit)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d6540), "\x90\x90\x90\x90\x90", 5, NULL); //Deleting exit sequence from onQuit button

                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d654d), "\x90\x90", 2, NULL); //Deleting quitSound effect from original exit sequence
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d655d), "\x90\x90\x90\x90\x90", 5, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d6565), "\x90\x90\x90\x90\x90", 5, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d6540), "\xE8\x9B\xBA\x01\x00", 5, NULL); //Restoring exit sequence from onQuit button

                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d654d), "\x6A\x10", 2, NULL); //Restoring quitSound effect from original exit sequence
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d655d), "\x68\x5C\x22\x51\x00", 5, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d6565), "\xE8\x86\xF1\xF2\xFF", 5, NULL);
                }
            }
            if (ImGui::Checkbox("Hide Pause Menu", &setting().onHidePause))
            {
                if (layers().PauseLayerObject)
                {
                    if (setting().onHidePause) layers().PauseLayerObject->setVisible(0);
                    else layers().PauseLayerObject->setVisible(1);
                }
            }
            ImGui::Checkbox("Show Player Hitbox", &setting().onPlayerHitbox);
            ImGui::Checkbox("Show Object Hitbox", &setting().onObjHitbox);
            ImGui::Checkbox("Hitboxes on Death", &setting().onHitboxesOnDeath);
            ImGui::Checkbox("Start Pos Switcher", &setting().onStartPosSwitcher);
            //ImGui::Checkbox("Zero Delay", &setting( ).onZeroDelay);
        }



        if (ImGui::Begin("Cosmetic", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
        {
            ImGui::SetWindowFontScale(setting().UISize);
            ImGui::SetWindowSize({ 250, 425 });
            static ImGuiComboFlags fcomboTrail = 0;
            const char* trailStateArr[] = { "Normal", "Always On", "Always Off", "Reversed" };
            const char* comboTrail = trailStateArr[setting().trailStateId];
            ImGui::SetNextItemWidth(150 * setting().UISize);
            if (ImGui::BeginCombo("Trail State", comboTrail, fcomboTrail))
            {
                for (int n = 0; n < IM_ARRAYSIZE(trailStateArr); n++)
                {
                    const bool is_selected = (setting().trailStateId == n);
                    if (ImGui::Selectable(trailStateArr[n], is_selected))
                        setting().trailStateId = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                if (setting().trailStateId == 0)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC406), "\x00", 1, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC376), "\x01", 1, NULL);
                }
                if (setting().trailStateId == 1)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC406), "\x01", 1, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC376), "\x01", 1, NULL);
                }
                if (setting().trailStateId == 2)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC406), "\x00", 1, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC376), "\x00", 1, NULL);
                }
                if (setting().trailStateId == 3)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC406), "\x01", 1, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC376), "\x00", 1, NULL);
                }
                ImGui::EndCombo();
            }
            if (ImGui::Checkbox("Solid Player Trail", &setting().onSolidTrail))
            {
                if (setting().onSolidTrail)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC38C), "\x90\x90\x90", 3, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC38C), "\x89\x41\x10", 3, NULL);
                }
            }
            if (ImGui::Checkbox("Disable Wave Trail", &setting().onWaveTrail)) {
                if (setting().onWaveTrail) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DF286), "\x00", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DF286), "\x01", 1, NULL);
                }
            }
            if (ImGui::Checkbox("Solid Wave Trail", &setting().onSolidWave))
            {
                if (setting().onSolidWave)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d809e), "\x90\x90", 2, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d809e), "\x75\x0c", 2, NULL);
                }
            }
            if (ImGui::Checkbox("Hide Practice Buttons", &setting().onHidePracticeButtons))
            {
                if (setting().onHidePracticeButtons)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4fd154), "\x90\xD7", 2, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4fd154), "\xE8\xD6", 2, NULL);
                }
            }
            if (ImGui::Checkbox("Hide Attempts", &setting().onHideAttempts))
            {
                if (setting().onHideAttempts)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f190a), "\x5F", 1, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e31ae), "\x06\x37", 2, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f190a), "\x54", 1, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e31ae), "\xFC\x36", 2, NULL);
                }
            }
            if (ImGui::Checkbox("No Death Effect", &setting().onNoDeathEffect))
            {
                if (setting().onNoDeathEffect)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DC3C1), "\xE9\xE7\x01\x00\x00\x90", 6, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DC3C1), "\x0F\x84\xD5\x01\x00\x00", 6, NULL);
                }
            }
            if (ImGui::Checkbox("Collect Coins in Practice", &setting().onPracticeCollectCoins))
            {
                if (setting().onPracticeCollectCoins)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e9678), "\x90\x90", 2, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e9678), "\x75\x4c", 2, NULL);
                }
            }
            if (ImGui::Checkbox("No Mirror Effect", &setting().onNoMirror))
            {
                if (setting().onNoMirror)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EF0E2), "\xE9\x87\x01\x00\x00\x90", 6, NULL); //E9 87 01 00 00 90
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EF0E2), "\x0F\x84\x86\x01\x00\x00", 6, NULL); //0F 84 86 01 00 00
                }
            }
            if (ImGui::Checkbox("Instant Mirror Effect", &setting().onInstantMirror))
            {
                if (setting().onInstantMirror)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EF215), "\x00", 1, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EF215), "\x3F", 1, NULL);
                }
            }
            if (ImGui::Checkbox("Disable Force Glow", &setting().onDisableForceGlow))
            {
                if (setting().onDisableForceGlow)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4de236), "\x00", 1, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4de236), "\x01", 1, NULL);
                }
            }
            if (ImGui::Checkbox("No Pulse", &setting().onNoPulse))
            {
                if (setting().onNoPulse)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42992a), "\x90\x90\x90\x90\x90\x90\x90\x90", 8, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42992a), "\xF3\x0F\x11\x87\xAC\x01\x00\x00", 8, NULL);
                }
            }
            if (ImGui::Checkbox("No Particles", &setting().onNoParticles))
            {
                if (setting().onNoParticles)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xb4f65), "\x31\xF6", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xb6626), "\x00", 1, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xb4f65), "\x8B\xF0", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xb6626), "\x01", 1, NULL);
                }
            }
            if (ImGui::Checkbox("No Objects Glow", &setting().onNoObjectsGlow))
            {
                if (setting().onNoObjectsGlow)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x46D1B3), "\xE9\x8F\x01\x00\x00\x90", 6, NULL); //E9 8F 01 00 00 90
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x46D1B3), "\x0F\x85\x8E\x01\x00\x00", 6, NULL); //0F 85 8E 01 00 00
                }
            }
            if (ImGui::Checkbox("No Shade", &setting().onNoShade))
            {
                if (setting().onNoShade)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EA4FF), "\xEB", 1, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EA4FF), "\x77", 1, NULL);
                }
            }
            if (ImGui::Checkbox("No Respawn Blink", &setting().onNoRespBlink))
            {
                if (setting().onNoRespBlink)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DBB97), "\x00\x00\x00\x00", 4, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DBB97), "\xCD\xCC\xCC\x3E", 4, NULL);
                }
            }
            ImGui::Checkbox("Same Dual Color", &setting().onDualSameCol);
            if (GetAsyncKeyState(0x32))
            {
                if (ImGui::Checkbox("SECOND ATTEMPT", &setting().onSecondAttempt))
                {
                    if (setting().onSecondAttempt)
                    {
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F1903), "\x6A\x02\x90\x90\x90\x90", 6, NULL);
                    }
                    else
                    {
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F1903), "\xFF\xB7\xE8\x02\x00\x00", 6, NULL);
                    }
                }
            }
        }

        if (ImGui::Begin("Creator", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
        {
            ImGui::SetWindowFontScale(setting().UISize);
            ImGui::SetWindowSize({ 250, 450 });

            ImGui::SetNextItemWidth(150 * setting().UISize);
            ImGui::InputFloat("##zoomscale", &setting().zoom, 0.1, 0.1, "%.3f", 0);
            ImGui::SameLine();
            if (ImGui::Button("Apply Zoom") && setting().zoom > 0)
            {
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)gd::base + 0x16A160), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xC8), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x20), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x8), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x124), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x19C), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x488), &pointervalue, sizeof(pointervalue), 0);
                WriteProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x20), &setting().zoom, sizeof(setting().zoom), 0);

                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)gd::base + 0x16A1A8), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xC8), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x20), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x8), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x140), &pointervalue, sizeof(pointervalue), 0);
                WriteProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x24), &setting().zoom, sizeof(setting().zoom), 0);
            }
            ImGui::SetNextItemWidth(150 * setting().UISize);
            ImGui::InputFloat("##gridscale", &setting().grid, 1, 1, "%.1f", 0);
            ImGui::SameLine();
            if (ImGui::Button("Apply Grid") && setting().grid > 0)
            {
                WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4076F8), "\x90\x90\x90\x90\x90\x90\x90\x90", 8, NULL);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)gd::base + 0x16A1B8), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xC8), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x20), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x8), &pointervalue, sizeof(pointervalue), 0);
                WriteProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x180), &setting().grid, sizeof(setting().grid), 0);
            }
            ImGui::SameLine();
            if (ImGui::Button("Default Grid"))
            {
                setting().grid = setting().gridDefault;
                WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4076F8), "\xF3\x0F\x11\x81\x80\x01\x00\x00", 8, NULL);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)gd::base + 0x16A1B8), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xC8), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x20), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x8), &pointervalue, sizeof(pointervalue), 0);
                WriteProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x180), &setting().gridDefault, sizeof(setting().gridDefault), 0);
            }
            /*if (ImGui::Button("Select Hidden Trigger"))
            {
                int inthidtrg = 55;
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)gd::base + 0x0016A1B8), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xF0), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x18), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x8), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x0), &pointervalue, sizeof(pointervalue), 0);
                ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xE8), &pointervalue, sizeof(pointervalue), 0);
                WriteProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xFC), &inthidtrg, sizeof(inthidtrg), 0);
            }*/
            ImGui::Checkbox("Hide Editor UI", &setting().onHideEditorUI);
            if (ImGui::Checkbox("Copy Hack", &setting().onCopyHack)) {
                if (setting().onCopyHack) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bd30), "\x39\xC0\x90", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bd1b), "\x90\x90\x90\x90\x90\x90", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49d535), "\x90\x90", 2, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bd30), "\x83\xF8\x01", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bd1b), "\x0F\x84\x56\x01\x00\x00", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49d535), "\x75\x0E", 2, NULL);
                }
            }
            if (ImGui::Checkbox("CMark Hack", &setting().onCmark)) {
                if (setting().onCmark) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4543E0), "\xB8\x00\x00\x00\x00\x90", 6, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4543E0), "\x8B\x81\x04\x02\x00\x00", 6, NULL);
                }
            }
            if (ImGui::Checkbox("Verify Hack", &setting().onVerify)) {
                if (setting().onVerify) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43D4E0), "\xEB", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43D4E0), "\x75", 1, NULL);
                }
            }
            if (ImGui::Checkbox("Song Bypass", &setting().onSongBypass))
            {
                if (setting().onSongBypass) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4998BF), "\x90\x90", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4998D1), "\x90\x90\x90", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4998E0), "\x90\x90\x90", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49993E), "\x90\x90", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x499950), "\x90\x90\x90", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49995F), "\x90\x90\x90", 3, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4998BF), "\x74\x4E", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4998D1), "\x0F\x4F\xF0", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4998E0), "\x0F\x48\xF1", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49993E), "\x74\x4E", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x499950), "\x0F\x4F\xF0", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49995F), "\x0F\x48\xF1", 3, NULL);
                }
            }
            if (ImGui::Checkbox("Free Editor Scroll", &setting().onEditorScroll))
            {
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
            if (ImGui::Checkbox("Object Hack", &setting().onObjectHack)) {
                if (setting().onObjectHack) { // is enabled --------------------------- OBJECT HACK
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43E08E), "\x68\xFF\xFF\xFF\x7F", 5, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448da5), "\x3D\xFF\xFF\xFF\x7F", 5, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4475b2), "\x3D\xFF\xFF\xFF\x7F", 5, NULL);
                }
                else { // is disabled
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43E08E), "\x68\x20\x4E\x00\x00", 5, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448da5), "\x3D\x20\x4E\x00\x00", 5, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4475b2), "\x3D\x20\x4E\x00\x00", 5, NULL);
                }
            }
            if (ImGui::Checkbox("Hide Grid", &setting().onHideGrid))
            {
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
            if (ImGui::Checkbox("Hide Trigger Lines", &setting().onHideTrigLines))
            {
                if (setting().onHideTrigLines)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493348), "\xE9\xCE\x00\x00\x00\x90", 6, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493348), "\x0F\x84\xCD\x00\x00\x00", 6, NULL);
                }
            }
            if (ImGui::Checkbox("Smooth Editor Trail", &setting().onSmoothEditorTrail))
            {
                if (setting().onSmoothEditorTrail)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x490f94), "\x90\x90", 2, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x490f94), "\x72\x79", 2, NULL);
                }
            }
            if (ImGui::Checkbox("Zoom Bypass", &setting().onZoomBypass)) //Допилить
            {
                if (setting().onZoomBypass)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448a85), "\x90\x90\x90", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448a8a), "\x90\x90\x90", 3, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448a85), "\x0F\x2F\xC8", 3, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448a8a), "\x0F\x28\xC8", 3, NULL);
                }
            }
            if (ImGui::Checkbox("Place Over", &setting().onPlaceOver))
            {
                if (setting().onPlaceOver)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48C8EF), "\xEB", 1, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48C8EF), "\x77", 1, NULL);
                }
            }
            if (ImGui::Checkbox("Editor Extension", &setting().onEditorExtension))
            {
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
            ImGui::Checkbox("Persistent Clipboard", &setting().onPersistentClipboard);
            ImGui::Checkbox("Selected Object Info", &setting().onSelectedObjectLabel);
            ImGui::Checkbox("Any Edit", &setting( ).anyEdit);

            /*if (ImGui::Checkbox("No Death X", &setting().onNoDeathX))
            {
                if (setting().onNoDeathX)
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x90769), "\xEB\x08", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x9077a), "\x90\x90", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x9078e), "\x90\x90", 2, NULL);
                }
                else
                {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x90769), "\x74\x08", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x9077a), "\x74\x41", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x9078e), "\x74\x2D", 2, NULL);
                }
            }*/

        }
            

        if (ImGui::Begin("Player", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
        {
                ImGui::SetWindowFontScale(setting().UISize);
                ImGui::SetWindowSize({ 250, 450 });

                ImGui::SetNextItemWidth(180 * setting().UISize);
                ImGui::InputFloat("##posx", &setting().posx, 1, 100, "%.1f", 0);
                ImGui::SameLine();
                if (ImGui::Button("Apply Position X"))
                {
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)gd::base + 0x0016A1A0), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x144), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x210), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xAC), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x1A4), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xAC), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x2A4), &pointervalue, sizeof(pointervalue), 0);
                    WriteProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x4A8), &setting().posx, sizeof(setting().posx), 0);
                }

                ImGui::SetNextItemWidth(180 * setting().UISize);
                ImGui::InputFloat("##posy", &setting().posy, 1, 100, "%.1f", 0);
                ImGui::SameLine();
                if (ImGui::Button("Apply Position Y"))
                {
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)gd::base + 0x0016A1A0), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x144), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x210), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xAC), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x1A4), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xAC), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x2A4), &pointervalue, sizeof(pointervalue), 0);
                    WriteProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x4AC), &setting().posy, sizeof(setting().posy), 0);
                }

                ImGui::SetNextItemWidth(180 * setting().UISize);
                ImGui::InputFloat("##playersize", &setting().size, 1, 100, "%.1f", 0);
                ImGui::SameLine();
                if (ImGui::Button("Apply Size"))
                {
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)gd::base + 0x0016A1A0), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x144), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x1A4), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x118), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xAC), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xAC), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x2A4), &pointervalue, sizeof(pointervalue), 0);
                    WriteProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x470), &setting().size, sizeof(setting().size), 0);
                }

                ImGui::SetNextItemWidth(180 * setting().UISize);
                ImGui::InputFloat("##playerspeed", &setting().speed, 1, 100, "%.1f", 0);
                ImGui::SameLine();
                if (ImGui::Button("Apply Player Speed"))
                {
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)gd::base + 0x0016A1A0), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x144), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x2BC), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xAC), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xAC), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x2A4), &pointervalue, sizeof(pointervalue), 0);
                    WriteProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x474), &setting().speed, sizeof(setting().speed), 0);
                }

                ImGui::SetNextItemWidth(180 * setting().UISize);
                if (ImGui::DragFloat("##speedhack", &setting().gamespeed, 0.05f, 0.f, 10.f))
                {
                    update_speed_hack();
                    if (setting().gamespeed < 0.f) setting().gamespeed = 0;
                }
                    
                ImGui::SameLine();
                if (ImGui::Checkbox("Speedhack", &setting().onSpeedhack))
                {
                    update_speed_hack();
                }

                if (ImGui::Button("Change Gravity"))
                {
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)gd::base + 0x16A1A0), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x144), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x2A8), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x394), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xAC), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xAC), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x2A4), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x46C), &gravity, sizeof(gravity), 0);
                    gravity = !gravity;
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)gd::base + 0x16A1A0), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x144), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x2A8), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x394), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xAC), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0xAC), &pointervalue, sizeof(pointervalue), 0);
                    ReadProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x2A4), &pointervalue, sizeof(pointervalue), 0);
                    WriteProcessMemory(GetCurrentProcess(), (void*)((unsigned long long)pointervalue + 0x46C), &gravity, sizeof(gravity), 0);
                }
                ImGui::Checkbox("Hide Player", &setting().onHidePlayer);

                if (ImGui::Checkbox("Noclip", &setting().onNoclip)) {
                    if (setting().onNoclip) {
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EE9D9), "\xE9\xF0\x02\x00\x00\x90", 6, NULL);
                        cheatAdd();
                    }
                    else {
                        setting().safeNoclip = false;
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EE9D9), "\x0F\x85\xEF\x02\x00\x00", 6, NULL);
                        cheatDec();
                    }
                }
                if (ImGui::Checkbox("Safe Noclip", &setting().safeNoclip)) {
                    if (setting().safeNoclip) {
                        setting().onSafeMode = true;
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EEB14), "\xEB\x6C", 2, NULL);
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E38E6), "\xE9\x77\x01\x00\x00\x90", 6, NULL);
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E3949), "\xE9\x14\x01\x00\x00\x90", 6, NULL);
                        if (!setting().onNoclip) cheatAdd();
                        setting().onNoclip = true;
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EE9D9), "\xE9\xF0\x02\x00\x00\x90", 6, NULL);
                    }
                    else {
                        setting().onSafeMode = false;
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EEB14), "\x75\x6C", 2, NULL);
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E38E6), "\x0F\x85\x76\x01\x00\x00", 6, NULL);
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E3949), "\x0F\x85\x13\x01\x00\x00", 6, NULL);
                        setting().onNoclip = false;
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EE9D9), "\x0F\x85\xEF\x02\x00\x00", 6, NULL);
                        cheatDec();
                    }
                }
                ImGui::Checkbox("Speedhack Music", &setting().onSpeedhackMusic);
                if (ImGui::Checkbox("InstaComplete", &setting().onInstaComplete)) {
                    if (setting().onInstaComplete) {
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DFC36), "\xC7\x87\x74\x04\x00\x00\x00\x00\x00\x70\x90\x90", 12, NULL);
                        cheatAdd();
                    }
                    else {
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DFC36), "\xF3\x0F\x11\x8F\x74\x04\x00\x00\x9F\xF6\xC4\x44", 12, NULL);
                        cheatDec();
                    }
                }
                if (ImGui::Checkbox("Infinite Jumps", &setting().onInfiniteJumps)) {
                    if (setting().onInfiniteJumps) {
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D8A60), "\x01", 1, NULL);
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d87e5), "\x01", 1, NULL);
                        cheatAdd();
                    }
                    else {
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D8A60), "\x00", 1, NULL);
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d87e5), "\x00", 1, NULL);
                        cheatDec();
                    }
                }
                if (ImGui::Checkbox("Fast Restart", &setting().onFastRestart))
                {
                    if (setting().onFastRestart)
                    {
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EE9EB), "\xC7\x45\x08\x00\x00\x00\x3F\x90\x90\x90\x90\x90\x90", 13, NULL);
                    }
                    else
                    {
                        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EE9EB), "\xF3\x0F\x10\x05\xAC\x7F\x54\x00\xF3\x0F\x11\x45\x08", 13, NULL);
                    }
                }
        }

        if (ImGui::Begin("Status", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
        {
            ImGui::SetWindowFontScale(setting().UISize);
            ImGui::Checkbox("Show Percents", &setting().onShowPercents);
            ImGui::Checkbox("Cheat Indicator", &setting().onCheatIndicator);
            ImGui::Checkbox("Clk Timer", &setting().onClkTimer);
            ImGui::Checkbox("Safe Mode label", &setting().onSafeModeLabel);
            ImGui::Checkbox("Clock", &setting().onGlobalTime);
            ImGui::Checkbox("Show FPS", &setting().onFPSShow);
            ImGui::Checkbox("Attempts", &setting().onAttemptsLabel);
            ImGui::Checkbox("Jumps", &setting().onJumpsLabel);
            ImGui::Checkbox("isColliding", &setting().onCollidingLabel);
            ImGui::Checkbox("Noclip Deaths counter", &setting().onNoclipDeaths);
            ImGui::Checkbox("Noclip Accuracy", &setting().onNoclipAcc);

            ImGui::SetNextItemWidth(150 * setting().UISize);
            ImGui::InputText("##message", setting().message, IM_ARRAYSIZE(setting().message));
            ImGui::SameLine();
            ImGui::Checkbox("Message", &setting().onMessage);
                
        }

        if (ImGui::Begin("Icons", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
        {
            ImGui::SetWindowFontScale(setting().UISize);

            ImGui::ColorEdit3("##primarycolorpulse", setting().PrimaryPulse, ImGuiColorEditFlags_NoInputs);
            ImGui::SameLine();
            ImGui::Checkbox("Primary Color Pulse", &setting().onPrimaryPulse); 
            //if (ImGui::IsItemHovered())
            //    ImGui::SetTooltip("I am a tooltip with.", setting().UISize);

            ImGui::ColorEdit3("##secondarycolorpulse", setting().SecondaryPulse, ImGuiColorEditFlags_NoInputs);
            ImGui::SameLine();
            ImGui::Checkbox("Secondary Color Pulse", &setting().onSecondaryPulse);

            ImGui::ColorEdit3("##GlowColorPulse", setting().GlowPulse, ImGuiColorEditFlags_NoInputs);
            ImGui::SameLine();
            ImGui::Checkbox("Glow Color Pulse", &setting().onGlowPulse);

            ImGui::ColorEdit3("##WaveTrailColorPulse", setting().WaveTrailPulse, ImGuiColorEditFlags_NoInputs);
            ImGui::SameLine();
            ImGui::Checkbox("Wave Trail Color Pulse", &setting().onWaveTrailPulse);

            //ImGui::Checkbox("Always Glow ON", &setting().onGlow);
        }

        if (ImGui::Begin("Settings", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
        {
            ImGui::SetWindowFontScale(setting().UISize);
                if (ImGui::ColorEdit4("Color Style", setting().Overlaycolor)) {

                    colorSet( );
                }
                
                if (ImGui::ColorEdit4("Background Color", setting().BGcolor)) {

                    auto* colors = ImGui::GetStyle().Colors;

                    color6.x = setting().BGcolor[0];
                    color6.y = setting().BGcolor[1];
                    color6.z = setting().BGcolor[2];
                    color6.w = setting().BGcolor[3];

                    colors[ImGuiCol_WindowBg] = color6;
                }
                

                ImGui::DragFloat("UI Size", &setting().UISize, 0.01f, 0.5f, 3.25f);
                if (setting().UISize < 0.5f) setting().UISize = 0.5f;
                if (setting().UISize > 3.25f) setting().UISize = 3.25f;
                
                ImGui::Checkbox("Auto Save Hacks", &setting().onAutoSave);
                ImGui::SameLine();
                if (ImGui::Button("Save Hack State"))
                {
                    save();
                    gd::FLAlertLayer::create(nullptr, "Saved!", "Your hack state is saved!", "Ok", nullptr, 240.f, false, 0)->show();
                }
                ImGui::EndTabItem();
                ImGui::Checkbox("Cocos Explorer", &setting( ).onExplorer);
                if (GetAsyncKeyState(0x34) && GetAsyncKeyState(0x37))
                {
                    ImGui::Checkbox("Debug Labels", &setting().onDebugLabels);
                    
                }
                //ImGui::Checkbox("ZA WARUDO", &setting().onTimeStop);
            }

        if (ImGui::Begin("Credits", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
        {
            ImGui::SetWindowFontScale(setting().UISize);
            ImGui::Text("Historical Hack V4.65");
            ImGui::Text("Made by TASWERT ALLO!");
            ImGui::Text("Thanks to Rainix for GUI and other help!");
            ImGui::Text("Special thanks to mgostiH, Mat, Pololak,");
            ImGui::Text("HJfod, Absolute and GD Programming server!");
            //ImGui::Text("Testers: Herowhither, Virex, Verbole");
            //ImGui::Text("HHV4.0 'Tester Edition'. Thank you for your help!");
            // 
            //Rainix Pololak - Some help with code and addresses. Huge impact.
            //Mat Absolute - I took some addresses and other stuff from their projects lol. Especially from Mat's projects.
            //mgostiH HJfod maki zmx iAndy_hd3 - Guys who responded to me, when I needed some help.
            ImGui::EndTabItem();
        }


    }
    ImGui::End();
    if (!setting().onSpeedhackCounted && setting().gamespeed < 1.f && setting().onSpeedhack)
    {
        setting().onSpeedhackCounted = true;
        cheatAdd();
    }
    else if (setting().onSpeedhackCounted && (setting().gamespeed >= 1.f || !setting().onSpeedhack))
    {
        setting().onSpeedhackCounted = false;
        cheatDec();
    }

    update_fps_bypass();
    update_speed_hack();
    colorSet( );

    if (setting().onFPSbypass /* && CCDirector::sharedDirector()->getAnimationInterval() != (1.0 / setting().fpslol) */) {
        update_fps_bypass();
    }
}

void setup_imgui_menu() {
    ImGuiHook::setToggleCallback([]() { show = !show; });
    ImGuiHook::setRenderFunction(RenderMain);
    ImGuiHook::setupHooks([](auto addr, auto hook, auto orig) {
        MH_CreateHook(addr, hook, orig);
        MH_EnableHook(addr);
        });
}
