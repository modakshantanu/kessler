#ifndef RESSCENE_H
#define RESSCENE_H

#include "raylib.h"
#include "Scene.h"
#include "TextView.h"
#include "Button.h"
#include "OptionsView.h"
#include "utils.h"
#include "KesslerGameScene.h"
#include "PauseScene.h"
#include "MainMenu.h"
#include "InstructionsScene.h"
#include <bits/stdc++.h>

extern float uiScale;
extern Scene* curScene, *nextScene; 
extern Settings settings;

extern MainMenu* mainMenu;
extern KesslerGameScene* kesslerGameScene;
extern InstructionsScene* instructionsScene;
extern SettingsScene* settingsScene;
extern PauseScene* pauseScene;


class ResScene : public Scene {
public:

    ResScene() {
    }


    void update() {

        
        SetWindowSize(settings.screenWidth, settings.screenHeight);
        mainMenu = new MainMenu();
        gameScene = new KesslerGameScene();
        pauseScene = new PauseScene();
        settingsScene = new SettingsScene();
        instructionsScene = new InstructionsScene();
        resScene = new ResScene();


        nextScene = (Scene*) settingsScene;

    }

    void render() {
    }
};

#endif