#ifndef UTILS_H
#define UTILS_H


#include<bits/stdc++.h>
#include "raylib.h"
#include "MainMenu.h"
#include "SettingsScene.h"
#include "Scene.h"
#include "Settings.h"
#include "KesslerGameScene.h"
#include "AudioManager.h"

using namespace std;
float uiScale = 1.0;

Scene* curScene, *nextScene; 
Settings settings;

class MainMenu;
MainMenu* mainMenu;

class ResScene;
ResScene* resScene;

class KesslerGameScene;
KesslerGameScene* gameScene;

class PauseScene;
PauseScene* pauseScene;

class InstructionsScene;
InstructionsScene* instructionsScene;

class SettingsScene;
SettingsScene* settingsScene;

class AudioManager;
AudioManager audio;

Color shipBlue =  {0, 106, 255, 255};
Color bulletPurple =  {238, 48, 255, 255};
Color bulletDark = {180, 31, 194, 255};
Color crossHairPurple = {178, 36, 191, 255};

#endif