#pragma once
#include <fstream>
#include <sstream>
#include <string>
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

// Battery Management functions for the admin
void BatteryMgmt();
void ViewAllBatteries(std::map<int, Battery>& batteries);
void AddNewBattery(std::map<int, Battery>& batteries);
void UpdateBatteryStatus(std::map<int, Battery>& batteries);
void MonitorBatteryHealth(std::map<int, Battery>& batteries);