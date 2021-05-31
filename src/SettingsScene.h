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
    vector<OptionButton> opts;
    OptionsView optView;

    SettingsScene() {

        int centerX = settings.screenWidth / 2;

        title = TextView("Settings" , centerX , 85, 64 * uiScale, BOTTOM, CENTER);


        int buttonWidth = 200 * uiScale;
        int buttonHeight = 50 * uiScale;
        backButton = TextButton(25, 25, buttonWidth, buttonHeight, "Back"); 
    
        backButton.onClick = []() {
            nextScene = (Scene*) mainMenu;
        };



        optView = OptionsView("Resolution: ", centerX - 400, 300);
        restartNotif = TextView("Requires Restart", centerX - 400, 310, 20, TOP, LEFT, RED);

        std::vector<std::string> resolutions = getResOptions();
        for (auto &s: resolutions) {
            optView.addOption(s);
            if (parseRes(s) == std::pair<int,int>({settings.screenWidth, settings.screenHeight})) {
                optView.setSelected(s);
            }
        } 
        
        optView.onChange = [](std::string id) {
            auto dim = parseRes(id);
            settings.screenWidth = dim.first;
            settings.screenHeight = dim.second;
            saveSettings("kessler_settings.txt", settings);

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
        
        optView.update();

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
        for (auto &e: opts) {
            e.render();
        }

        optView.render();
    }
};

#endif