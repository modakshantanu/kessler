#ifndef UTILS_H
#define UTILS_H


#include<bits/stdc++.h>
#include "raylib.h"
#include "MainMenu.h"
#include "SettingsScene.h"
#include "Scene.h"
#include "Settings.h"
#include "KesslerGameScene.h"

using namespace std;
float uiScale = 1.0;

Scene* curScene, *nextScene; 
Settings settings;

class MainMenu;
MainMenu* mainMenu;

class KesslerGameScene;
KesslerGameScene* gameScene;

class PauseScene;
PauseScene* pauseScene;

class InstructionsScene;
InstructionsScene* instructionsScene;

class SettingsScene;
SettingsScene* settingsScene;

Color shipBlue =  {0, 106, 255, 255};
Color bulletPurple =  {238, 48, 255, 255};
Color bulletDark = {180, 31, 194, 255};

#endif