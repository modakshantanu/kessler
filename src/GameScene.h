#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "raylib.h"
#include "mathutils.h"
#include "utils.h"
#include "Scene.h"
#include "Settings.h"

extern Settings settings;
extern float uiScale;

extern MainMenu* mainMenu; 
extern PauseScene* pauseScene; 

extern Scene* curScene, *nextScene; 


class GameScene: public Scene {
public:
    
    float paddleWidth = 20;
    float paddleHeight =100;

    Vector2 left, right, ball;
    Vector2 dLeft, dRight, dBall;

    Camera2D camera = {0};

    GameScene() {

        float padding = 20 * settings.screenWidth / 1280.0;
        paddleWidth *= settings.screenWidth / 1280.0;
        paddleHeight *= settings.screenWidth / 1280.0;

        left = {padding, (float) settings.screenHeight / 2};
        right = {settings.screenWidth - padding, (float) settings.screenHeight / 2};
        ball = {settings.screenWidth / 2.0f, settings.screenHeight / 2.0f};

        dLeft = {0,0};
        dRight = {0,0};
        dBall = {3,3};
        

    }


    void reset() {
        float padding = 20 * settings.screenWidth / 1280.0;

        left = {padding, (float) settings.screenHeight / 2};
        right = {settings.screenWidth - padding, (float) settings.screenHeight / 2};
        ball = {settings.screenWidth / 2.0f, settings.screenHeight / 2.0f};

        dLeft = {0,0};
        dRight = {0,0};
        dBall = {3,3};
        camera = {0};
    }

    void update() {
        bool leftUp = IsKeyDown(KEY_A);
        bool leftDown = IsKeyDown(KEY_D);
        bool rightUp = IsKeyDown(KEY_LEFT);
        bool rightDown = IsKeyDown(KEY_RIGHT);

        if (leftUp) dLeft = {0, -5};
        else if (leftDown) dLeft = {0, 5};
        else dLeft = {0,0};

        if (rightUp) dRight = {0,-5};
        else if (rightDown) dRight = {0,5};
        else dRight = {0,0};

        right = right + dRight;
        left = left + dLeft;
        ball = ball + dBall;

        if (right.y < 0) right.y = 0;
        if (left.y < 0) left.y = 0;

        if (right.y > settings.screenHeight) right.y = settings.screenHeight;
        if (left.y > settings.screenHeight) left.y = settings.screenHeight;

        if (ball.x < 0 || ball.x > settings.screenWidth) dBall.x = -dBall.x;
        if (ball.y < 0 || ball.y > settings.screenHeight) dBall.y = -dBall.y;

        camera.target = left;
        camera.offset = {0, settings.screenHeight / 2};
        camera.zoom = 1;
        camera.rotation  =0;



        Rectangle leftRect = {left.x - paddleWidth / 2 , left.y - paddleHeight / 2, paddleWidth, paddleHeight};
        Rectangle rightRect = {right.x - paddleWidth / 2 , right.y - paddleHeight / 2, paddleWidth, paddleHeight};

        if (math::intersects(leftRect, ball)) dBall.x = -dBall.x;
        if (math::intersects(rightRect, ball)) dBall.x = -dBall.x;


        // camera.offset = left;


        if (IsKeyPressed(KEY_ESCAPE)) {
            nextScene = (Scene*) pauseScene;
        }

    }

    void render() {
        // BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        Rectangle leftRect = {left.x - paddleWidth / 2 , left.y - paddleHeight / 2, paddleWidth, paddleHeight};
        Rectangle rightRect = {right.x - paddleWidth / 2 , right.y - paddleHeight / 2, paddleWidth, paddleHeight};

        DrawRectangleRec(leftRect, WHITE);
        DrawRectangleRec(rightRect, WHITE);

        DrawCircleV(ball, 8, BLUE);
        EndMode2D();
        // DrawFPS(10, 10);
        // EndDrawing();
    }
};

#endif