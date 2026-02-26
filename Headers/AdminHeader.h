#pragma once
#include <fstream>
#include <sstream>
#include <map>

#define ADMIN_DB "Database/AdminDB.txt"

struct Admin{
    std::string Username;
    std::string Password;
};

//Function to load and return a map for the full database.
std::map<std::string, Admin> LoadAdminData();

//Function to save the map to the database
bool SaveToDB_Admin(std::map<std::string, Admin>& Data);

void DriverRegistration();

void AdminRegistration();

void BatteryMgmt();

void AdminScreen(std::string& name);

bool AdminVerification(std::string& name, std::string& pass, std::map<std::string, Admin>& map);

void AdminLogin();