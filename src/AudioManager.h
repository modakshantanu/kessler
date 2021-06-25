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
Sound btnDown, btnUp;
Sound boost;
Sound empty;

enum {BULLET, POP, BOOM, BTNDOWN, BTNUP, EMPTY};

extern Settings settings;


class AudioManager {
public:
    float boostVol = 0;
    void init() {
        InitAudioDevice();
        bullet = LoadSound("res/bullet_sound.mp3");
        pop = LoadSound("res/pop.mp3");
        empty = LoadSound("res/empty.mp3");
        boom = LoadSound("res/boom.mp3");

        btnDown = LoadSound("res/buttondown.mp3");
        btnUp = LoadSound("res/buttonup.mp3");
        
        boost = LoadSound("res/boost.mp3");
        SetSoundVolume(boost, 0);
        if (boost.sampleCount) PlaySound(boost);
    }

    // static void playButtonClick() {
    //     PlaySoundMulti(buttonClick);
    // }

    // static void playBullet() {
    //     PlaySoundMulti(bullet);
    // }
    void boostOn() {

        if (!settings.audioFx) return;
        
        if (!IsSoundPlaying(boost)) PlaySound(boost);
        if (boostVol == 0) boostVol = 0.01;

        if (boostVol != 1) {
            boostVol *= 1.6;
            boostVol = (min(1.0f, boostVol));
        }
        SetSoundVolume(boost, boostVol);
    }

    void boostOff() {
        
        if (boostVol > 0.010001) {
            boostVol /= 1.6;
            if (boostVol < 0.02) boostVol = 0;
        }
        SetSoundVolume(boost, boostVol);

    }

    void play(int sound) {
        if (!IsAudioDeviceReady()) return;
        if (!settings.audioFx) return;

        Sound* chosen = NULL;
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

            case BTNDOWN:
                chosen = &btnDown;
                break;
            
            case BTNUP:
                chosen = &btnUp;
                break;

            case EMPTY:
                chosen = &empty;
                break;
        }

        if (chosen->sampleCount > 0) {
            PlaySoundMulti(*chosen);
        }

    }
};

#endif