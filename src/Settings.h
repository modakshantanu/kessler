#ifndef SETTINGS_H
#define SETTINGS_H

#include <bits/stdc++.h>
#include "raylib.h"
using namespace std;

struct Settings {
    int screenWidth;
    int screenHeight;
};


string toString(Settings &settings) {
    string res = "";
    res += "SCREENWIDTH ";
    res += to_string(settings.screenWidth);
    res += "\nSCREENHEIGHT ";
    res += to_string(settings.screenHeight);
    res += "\n";
    return res;
}

void saveSettings(string filename, Settings s) {
    ofstream outfile(filename);
    if (outfile.is_open()){
        outfile<<toString(s);
        outfile.close();
    }
}

Settings loadSettings(std::string filename) {
    
    Settings result = {1280, 720};

    ifstream file(filename);
    string line;    
    bool corrupted = false; 
    int idx = 0;

    if (file.is_open()) {
        while(getline(file, line)) {
            idx++;
            stringstream str(line);
            string key, value;

            str>>key; 
            if (str>>value) {
                if (key == "SCREENWIDTH") result.screenWidth = stoi(value);
                if (key == "SCREENHEIGHT") result.screenHeight = stoi(value);
            } else {
                corrupted = true;
            }
        }
        file.close();
        
    } else {
        corrupted = true;
    }
    if (idx != 2) corrupted = true;

    if (corrupted) {
        saveSettings(filename,result);
    }
    return result;
} 

vector<string> getResOptions() {
    int monitorWidth = GetMonitorWidth(0);
    int monitorHeight = GetMonitorHeight(0);
    
    set<pair<int,int>> reslist; 
    
    reslist.insert({1280, 720}); // Default
    if (1920 <= monitorWidth && 1080 <= monitorHeight) reslist.insert({1920, 1080});
    if (2560 <= monitorWidth && 1440 <= monitorHeight) reslist.insert({2560, 1440});
    if (3840 <= monitorWidth && 2160 <= monitorHeight) reslist.insert({3840, 2160});

    reslist.insert({monitorWidth, monitorHeight}); // Monitor Dimensions
    reslist.insert({GetScreenWidth(), GetScreenHeight()}); // Current Dimensions just in case

    vector<string> result;

    for (auto &it: reslist) {
        int w = it.first, h = it.second;
        result.push_back(to_string(w) + "x" + to_string(h));
    }
    return result;
}

pair<int,int> parseRes(string str) {
    string ws, hs;
    bool x = false;
    for (unsigned i = 0; i < str.size(); i++) {
        if (str[i] == 'x') x = true;
        else if (!x) ws += str[i];
        else hs += str[i];
    }
    return {stoi(ws), stoi(hs)};
}

#endif