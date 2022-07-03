#ifndef _H_MENU
#define _H_MENU

#include "GUI.h"
class Menu : public GUI{
public:
    Menu();
    void update() override;

    ImVec2 size;
    ImVec2 pos;

    bool start = false;
    bool reset = false;

    float mass = 0.05f;
    float gas = 0.03f;
    float density = 200.5f;
    float viscosity = 0.025f;



};

#endif