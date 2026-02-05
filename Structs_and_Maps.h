#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#define BATTERY_DB "Database/BatteriesDB.txt"
#define DRIVER_DB "Database/DriversDB.txt"
#define ADMIN_DB "Database/AdminDB.txt"


//When Reading and writing to Database files, do it in the order that they're arranged here for Standardisation.
struct Battery{
    int ID;
    std::string Type;
    float SoC;
    float SoH;
    std::string Status;
};
struct Driver{
    std::string Username;
    std::string Password;
    float Credits;
}; 
struct Admin{
    std::string Username;
    std::string Password;
};

//Functions to load and return maps for each's full database.
std::map<int, Battery> LoadBatteryData(){
    std::map<int, Battery> BatteryData;

    std::ifstream batteryDB;
    batteryDB.open(BATTERY_DB);

    std::string line;

    while (std::getline(batteryDB, line)){
        std::istringstream data(line);

        int ID; std::string Type, Status; float SoC, SoH;

        if(!(data >> ID >> Type >> SoC >> SoH >> Status)){continue;} //To ignore all malformed lines

        Battery tempBat = {ID, Type, SoC, SoH, Status};
        BatteryData[ID] = tempBat;
    }

    batteryDB.close();

    return BatteryData;
}

std::map<std::string, Driver> LoadDriverData(){
    std::map<std::string, Driver> DriverData;

    std::ifstream DriverDB;
    DriverDB.open(DRIVER_DB);

    std::string line;

    while (std::getline(DriverDB, line)){
        std::istringstream data(line);

        std::string Username, Password; float Credits;

        if(!(data >> Username >> Password >> Credits)){continue;} //To ignore all malformed lines

        Driver tempDriver = {Username, Password, Credits};
        DriverData[Username] = tempDriver;
    }

    DriverDB.close();

    return DriverData;

}

std::map<std::string, Admin> LoadAdminData(){
    std::map<std::string, Admin> AdminData;

    std::ifstream AdminDB;
    AdminDB.open(ADMIN_DB);

    std::string line;

    while (std::getline(AdminDB, line)){
        std::istringstream data(line);

        std::string Username, Password;

        if(!(data >> Username >> Password)){continue;} //To ignore all malformed lines

        Admin tempAdmin = {Username, Password};
        AdminData[Username] = tempAdmin;
    }

    AdminDB.close();

    return AdminData;
}

//Functions to save the maps to the database
bool SaveToDB_Battery(std::map<int, Battery>& Data){
    std::ofstream outFile;

    remove(BATTERY_DB);
    outFile.open(BATTERY_DB);

    for (const auto& each : Data){
        outFile << each.second.ID << ' ' 
                << each.second.Type << ' ' 
                << each.second.SoC << ' ' 
                << each.second.SoH << ' ' 
                << each.second.Status << ' ' 
                << '\n';
    }

    outFile.close();

    return true;
}

bool SaveToDB_Driver(std::map<std::string, Driver>& Data){
    std::ofstream outFile;

    remove(DRIVER_DB);
    outFile.open(DRIVER_DB);

    for (const auto& each : Data){
        outFile << each.second.Username << ' ' 
                << each.second.Password << ' '
                << each.second.Credits << ' '
                << '\n';
    }

    outFile.close();

    return true;
}

bool SaveToDB_Admin(std::map<std::string, Admin>& Data){
    std::ofstream outFile;

    remove(ADMIN_DB);
    outFile.open(ADMIN_DB);

    for (const auto& each : Data){
        outFile << each.second.Username << ' ' 
                << each.second.Password << ' '
                << '\n';
    }

    outFile.close();

    return true;
}