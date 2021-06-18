#ifndef SETTINGSSCENE_H
#define SETTINGSSCENE_H

#include "raylib.h"
#include "Scene.h"
#include "TextView.h"
#include "Button.h"
#include "OptionsView.h"
#include <bits/stdc++.h>

extern float uiScale;
extern Scene* curScene, *nextScene; 
extern Settings settings;

extern MainMenu* mainMenu;


class SettingsScene : public Scene {
public:
    TextView title;
    TextView restartNotif;
    TextButton backButton;
    
    OptionsView resView;
    OptionsView fxView;
    OptionsView fullscreenView;


    SettingsScene() {

        int centerX = settings.screenWidth / 2;

        title = TextView("Settings" , centerX , 85, 64 * uiScale, BOTTOM, CENTER);


        int buttonWidth = 200 * uiScale;
        int buttonHeight = 50 * uiScale;
        backButton = TextButton(25, 25, buttonWidth, buttonHeight, "Back"); 
    
        backButton.onClick = []() {
            nextScene = (Scene*) mainMenu;
        };



        resView = OptionsView("Resolution: ", centerX - 400, 300);
        restartNotif = TextView("Requires Restart", centerX - 400, 310, 20, TOP, LEFT, RED);

        fullscreenView = OptionsView("Fullscreen:", centerX - 400, 390);
        fxView = OptionsView("Sound Effects:", centerX - 400, 450);


        std::vector<std::string> resolutions = getResOptions();
        for (auto &s: resolutions) {
            resView.addOption(s);
            if (parseRes(s) == std::pair<int,int>({settings.screenWidth, settings.screenHeight})) {
                resView.setSelected(s);
            }
        } 


        fxView.addOption("On");
        fxView.addOption("Off");
        if (settings.audioFx) {
            fxView.setSelected("On");
        } else {
            fxView.setSelected("Off");
        }

        fullscreenView.addOption("On");
        fullscreenView.addOption("Off");
        if (settings.fullScreen) {
            fullscreenView.setSelected("On");
        } else {
            fullscreenView.setSelected("Off");
        }

        resView.onChange = [](std::string id) {
            auto dim = parseRes(id);
            settings.screenWidth = dim.first;
            settings.screenHeight = dim.second;
            saveSettings("kessler_settings.txt", settings);

        };

        fxView.onChange = [](std::string id) {
            if (id == "On") settings.audioFx = true;
            else settings.audioFx = false;
            saveSettings("kessler_settings.txt", settings);
        };

        fullscreenView.onChange = [](std::string id) {
            if (id == "On") settings.fullScreen = true;
            else settings.fullScreen = false;
            saveSettings("kessler_settings.txt", settings);
            bool isFullscreen = IsWindowFullscreen();
            if (isFullscreen != settings.fullScreen) {
                ToggleFullscreen();
            }
        }; 

    }


    void update() {

        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        int mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        int mouseReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
        int mouseUp = IsMouseButtonUp(MOUSE_LEFT_BUTTON);
        int mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        TextButton::buttonStateHandler(backButton, mouseX, mouseY, mouseUp, mouseDown, mousePressed, mouseReleased);
        
        resView.update();
        fxView.update();
        fullscreenView.update();

        bool esc = IsKeyPressed(KEY_ESCAPE);
        if (esc) {
            nextScene = (Scene*) mainMenu;
        }



    }

    void render() {

        ClearBackground(BLACK);
        backButton.render();
        title.render();
        restartNotif.render();


        resView.render();
        fxView.render();
        fullscreenView.render();
    }
};

#endif