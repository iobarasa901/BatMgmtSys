#include "../Headers/Helpers.h"

void clearInput(){
    std::cin.clear();
    std::cin.ignore(1000, '\n');
}

void pause(){
    std::cout << "Press Enter to continue...";
    std::cin.ignore(1000, '\n');
}

int promptInt(const std::string &msg){
    int v;
    while (true){
        std::cout << msg;
        if (std::cin >> v){ 
            clearInput(); 
            return v; 
        }
        std::cout << "Invalid integer. Try again.\n";
        clearInput();
    }
}

float promptFloat(const std::string &msg){
    float v;
    while (true){
        std::cout << msg;
        if (std::cin >> v){ 
            clearInput(); 
            return v; 
        }
        std::cout << "Invalid number. Try again.\n";
        clearInput();
    }
}

void showBattery(const Battery &b){
    std::cout << "ID:" << b.ID << " Type:" << b.Type 
              << " SoC:" << b.SoC << " SoH:" << b.SoH 
              << " Status:" << b.Status << '\n';
}

std::map<std::pair<float,std::string>, Battery> SoH_Ordered_Map(){
    std::map<std::string, Battery> NormalBatteryMap = LoadBatteryData();
    std::map<std::pair<float,std::string>, Battery> SoH_Map;

    for (const auto& each : NormalBatteryMap){
        SoH_Map[{each.second.SoH, each.second.ID}]= each.second;
    }

    return SoH_Map;
}