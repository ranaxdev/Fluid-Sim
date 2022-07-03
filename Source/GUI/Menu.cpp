#include "Menu.h"

Menu::Menu() {
    Menu::size = ImVec2(360.0f, 480.0f);
    Menu::pos = ImVec2(SCREEN_W-size.x, 0.0f);


    // Init flags
    Menu::flag_list = {
            ImGuiWindowFlags_MenuBar
    };
    updateFlags();
}

void Menu::update() {

    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(pos);

    ImGui::Begin("Sim Menu", &GUI::state, flags);
    ImGui::Text("Select options below \n\n");


    if(ImGui::Button("Begin")){
        start = true;
    }
    ImGui::SameLine();
    if(ImGui::Button("Reset")){
        reset = true;
    }

    ImGui::SliderFloat("Mass", &mass, 0.01f, 1.0f);
    ImGui::SliderFloat("Gas", &gas, 0.01f, 0.1f);
    ImGui::SliderFloat("Density", &density, 10.0f, 1000.0f);
    ImGui::SliderFloat("Viscosity", &viscosity, 0.005f, 0.1f);
    ImGui::End();
}
