//
// Created by szymo on 01/03/2023.
//

#ifndef OPENGLGP_PROJECTSETTINGS_H
#define OPENGLGP_PROJECTSETTINGS_H

enum class Tags{
    DEFAULT,
    CAMERA,
    SCENE,
    LIGHT
};

enum class ComponentNames{
    RENDERER,
    CAMERA,
    DIRECTIONALLIGHT,
    SPOTLIGHT,
    POINTLIGHT,
    BOXCOLLIDER
};

enum class Key{
    KEY_1 = 49, KEY_2 = 50, KEY_3 = 51, KEY_4 = 52, KEY_5 = 53, KEY_6 = 54, KEY_7 = 55, KEY_8 = 56, KEY_9 = 57, KEY_0 = 48,
    KEY_Q = 81, KEY_W = 87, KEY_E = 69, KEY_R = 82, KEY_T = 84, KEY_Y = 89, KEY_U = 85, KEY_I = 73, KEY_O = 79, KEY_P = 80,
    KEY_A = 65, KEY_S = 83, KEY_D = 68, KEY_F = 70, KEY_G = 71, KEY_H = 72, KEY_J = 74, KEY_K = 75, KEY_L = 76,
    KEY_Z = 90, KEY_X = 88, KEY_C = 67, KEY_V = 86, KEY_B = 66, KEY_N = 78, KEY_M = 77,
    KEY_ARROW_LEFT = 263, KEY_ARROW_RIGHT = 262, KEY_ARROW_UP = 265, KEY_ARROW_DOWN = 264,
    KEY_TAB = 258,
    KEY_LEFT_SHIFT = 340,
    KEY_LEFT_CTRL = 341,
    KEY_LEFT_ALT = 342,
    KEY_SPACE = 32,
    KEY_ESC = 256,
    KEY_ENTER = 257,
    KEY_BACKSPACE = 259
};


#endif //OPENGLGP_PROJECTSETTINGS_H
