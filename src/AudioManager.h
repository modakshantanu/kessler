#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "raylib.h"
#include "Settings.h"
#include "utils.h"

using namespace std;

Sound buttonClick;
Sound bullet;
Sound pop;
Sound boom;

enum {BULLET, POP, BOOM};

extern Settings settings;

class AudioManager {
public:
    void init() {
        InitAudioDevice();
        buttonClick = LoadSound("res/button_click.wav");
        bullet = LoadSound("res/bullet_sound.wav");
        pop = LoadSound("res/pop.wav");
        boom = LoadSound("res/boom.wav");
    }

    // static void playButtonClick() {
    //     PlaySoundMulti(buttonClick);
    // }

    // static void playBullet() {
    //     PlaySoundMulti(bullet);
    // }

    void play(int sound) {
        if (!IsAudioDeviceReady()) return;
        if (!settings.audioFx) return;

        Sound* chosen;
        switch (sound) {
            case BULLET:
                chosen = &bullet;
                break;
            case POP:
                chosen = &pop;
                break;
            case BOOM:
                chosen = &boom;
                break;
        }

        if (chosen->sampleCount > 0) {
            PlaySoundMulti(*chosen);
        }

    }
};

#endif