#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#define BATTERY_DB "Database/BatteriesDB.txt"

struct Battery{
    std::string ID;
    std::string Type;
    float SoC;
    float SoH;
    std::string Status;
};

//Function to load and return a map for the full database.
std::map<std::string, Battery> LoadBatteryData();

//Function to save the map to the database
bool SaveToDB_Battery(std::map<std::string, Battery>& Data);

// Battery Management functions for the admin
void BatteryMgmt();

void ViewAllBatteries(std::map<std::string, Battery>& batteries);

void AddNewBattery(std::map<std::string, Battery>& batteries);

void UpdateBatteryStatus(std::map<std::string, Battery>& batteries);

void MonitorBatteryHealth(std::map<std::string, Battery>& batteries);