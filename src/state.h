#pragma once
#include <cocos2d.h>

struct LayersStruct {
    cocos2d::CCLayer* PauseLayerObject;
};


struct SettingStruct {
    int trailStateId = 0;
    int cheatsCount = 0;
    int beforeRestartCheatsCount = 0;

    int NoclipByte;
    int CurrentNoclipByte;

    bool onCopyHack = false;
    bool onNoclip = false;
    bool onFPSbypass = false;
    bool onVerify = false;
    bool onCmark = false;
    bool onObjectHack = false;
    bool onRestartBtn = false;
    bool onEditBtn = false;
    bool onSafeMode = false;
    bool onPracticeMusic = false;
    bool onNoTransition = false;
    bool onIconBypass = false;
    bool onLevelBypass = false;
    bool onEsc = false;
    bool onInstaComplete = false;
    bool onInvisibility = false;
    bool onInfiniteJumps = false;
    bool onWaveTrail = false;
    bool onEditorScroll = false;
    bool onSongBypass = false;
    bool safeNoclip = false;
    bool onCheatIndicator = false;
    bool onHideAttempts = false;
    bool onNoDeathEffect = false;
    bool onSecondAttempt = false;
    bool onHidePracticeButtons = false;
    bool onHideGrid = false;
    bool onHideTrigLines = false;
    bool onSmoothEditorTrail = false;
    bool onSolidWave = false;
    bool onPracticeCollectCoins = false;
    bool onDisableForceGlow = false;
    bool onZoomBypass = false;
    bool onEditorExtension = false;
    bool onNoMirror = false;
    bool onInstantMirror = false;
    bool onTextLength = false;
    bool onCharFilter = false;
    bool onPlaceOver = false;
    bool onNoPulse = false;
    bool onNoParticles = false;
    bool onNoObjectsGlow = false;
    bool onShowPercents = false;
    bool onPersistentClipboard = false;
    bool onRetryBind = false;
    bool onFastRestart = false;
    bool onSpeedhack = false;
    bool onSpeedhackMusic = false;
    bool onFPSShow = false;
    bool onNoDeathX = false;
    bool onSafeModeLabel = false;
    bool onGlobalTime = false;
    bool onAttemptsLabel = false;
    bool onJumpsLabel = false;
    bool onMessage = false;
    bool onNoclipAcc = false;
    bool onDualSameCol = false;
    bool onKrmalMode = false;
    bool onCameraV2 = false;
    bool onNoRespBlink = false;
    bool onHidePlayer = false;
    bool onFreeWindowResize = false;
    bool onNoShade = false;
    bool onMaxParticles = false;
    bool onLowVolume = false;
    bool onInstantTriggers = false;
    bool onTransparentLists = false;
    bool onTransparentBG = false;
    bool onForceVisibility = false;
    bool onNoRotation = false;
    bool onHideEditorUI = false;
    bool onSolidTrail = false;
    bool onAutoSave = false;
    bool onBetterLevelUpdate = false;
    bool onCollidingLabel = false;
    bool onSelectedObjectLabel = false;
    bool onConfirmExit = false;
    bool onHidePause = false;
    bool onObjHitbox = false;
    bool onPlayerHitbox = false;
    bool onHitboxesOnDeath = false;
    bool onStartPosSwitcher = false;
    bool onNoclipDeaths = false;
    bool onClkTimer = false;
    bool anyEdit = false;
    //bool onZeroDelay = false;

    bool onDebugLabels = false;
    bool onExplorer = false;

    bool onNoclipOutOfMe = false;
    bool onSpeedhackCounted = false;

    float fpslol = 60.f;
    float zoom = 1;
    float grid = 30;
    float gridDefault = 30;
    float posx = 0;
    float posy = 105;
    float size = 1;
    float speed = 1;
    float gamespeed = 1.f;
    float UISize = 1.5f;

    float Overlaycolor[4] = { 0, 0.61, 0.37, 1 };
    float BGcolor[4] = { 0.3, 0.3, 0.3, 1 };

    float GlowPulse[3] = { 1.f, 1.f, 1.f };
    float SecondaryPulse[3] = { 1.f, 1.f, 1.f };
    float PrimaryPulse[3] = { 1.f, 1.f, 1.f };
    float WaveTrailPulse[3] = { 1.f, 1.f, 1.f };

    bool onGlowPulse = false;
    bool onSecondaryPulse = false;
    bool onPrimaryPulse = false;
    bool onWaveTrailPulse = false;

    char message[30] = "Your Text";


    int roleType = 0;
};
    

SettingStruct& setting();
LayersStruct& layers();