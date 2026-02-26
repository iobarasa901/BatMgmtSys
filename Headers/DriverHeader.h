#pragma once
#include <fstream>
#include <sstream>
#include <map>

#define DRIVER_DB "Database/DriversDB.txt"

struct Driver{
    std::string Username;
    std::string Password;
    float Credits;
}; 

//Function to load and return a map for the full database.
std::map<std::string, Driver> LoadDriverData();

//Function to save the map to the database
bool SaveToDB_Driver(std::map<std::string, Driver>& Data);

void DriverScreen(std::string& name);

bool DriverVerification(std::string& name, std::string& pass, std::map<std::string, Driver>& map);

void DriverLogin();

void DriverSwapBattery();

void DriverDeposit(std::string & name);