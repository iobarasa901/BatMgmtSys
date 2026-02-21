#pragma once
#include <fstream>
#include <sstream>
#include <map>

#define BATTERY_DB "Database/BatteriesDB.txt"

struct Battery{
    int ID;
    std::string Type;
    float SoC;
    float SoH;
    std::string Status;
};

//Function to load and return a map for the full database.
std::map<int, Battery> LoadBatteryData();

//Function to save the map to the database
bool SaveToDB_Battery(std::map<int, Battery>& Data);