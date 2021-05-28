#ifndef UTILS_H
#define UTILS_H


#include<bits/stdc++.h>
#include "raylib.h"
// #include "MainMenu.h"
#include "Scene.h"
#include "GameScene.h"

using namespace std;


float uiScale = 1.0;

Scene* curScene, *nextScene; 
int screenWidth,screenHeight;

class MainMenu;
MainMenu* mainMenu;

class GameScene;
GameScene* gameScene;


#endif