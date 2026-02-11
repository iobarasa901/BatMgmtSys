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
std::map<std::string, Driver> LoadDriverData(){//same logic as loading battery data
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

//Function to save the map to the database
bool SaveToDB_Driver(std::map<std::string, Driver>& Data){ // same logic as the SaveToDB_Battery
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

