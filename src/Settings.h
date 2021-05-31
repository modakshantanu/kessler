#ifndef SETTINGS_H
#define SETTINGS_H

#include <bits/stdc++.h>
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
        ofstream outfile(filename);
        if (outfile.is_open()){
            outfile<<toString(result);
            outfile.close();
        }
    }
    return result;
} 

#endif