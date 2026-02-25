//Everything in this file is temporary and for testing purposes only

#include "Headers/DriverHeader.h"
#include "Headers/AdminHeader.h"
#include "Headers/BatteryHeader.h"
#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>

std::string randStr(int length) {
    // 1. Define the pool of characters to choose from
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 generator(rd());
    
    // Distribution range: 0 to (pool size - 1)
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    std::string randomString;
    randomString.reserve(length); // Optimization: Reserve memory upfront

    for (int i = 0; i < length; ++i) {
        // Pick a random index and add that character to our string
        int randomIndex = distribution(generator);
        randomString += characters[randomIndex];
    }

    return randomString;
}
int randNum(){
    std::srand(std::time(0));
    return std::rand();
}

const std::string randString = randStr(7);
const int randNumber = randNum();

void batteryTesting(){
    std::ofstream outData;
    outData.open(BATTERY_DB, std::ios::app);

    Battery bat1 = {randNumber , "Li-Ion", 99.99, 80.94, "Charging"};

    outData << bat1.ID << " ";
    outData << bat1.Type << " ";
    outData << bat1.SoC << " ";
    outData << bat1.SoH << " ";
    outData << bat1.Status << '\n';

    outData.close();
}

void driverTesting(){
    std::ofstream outData;
    outData.open(DRIVER_DB, std::ios::app);

    Driver driver1 = {randString, "Pass123", 13134};

    outData << driver1.Username << " ";
    outData << driver1.Password << " ";
    outData << driver1.Credits << '\n' ;

    outData.close();
}

void adminTesting(){
    std::ofstream outData;
    outData.open(ADMIN_DB, std::ios::app);

    Admin admin1 = {randString, "Pass123"};

    outData << admin1.Username << " ";
    outData << admin1.Password << '\n' ;
    outData.close();
}

int main(){
    batteryTesting();
    std::map<int, Battery> BatteryData = LoadBatteryData();
    std::cout << "The battery " << BatteryData[randNumber].ID << " has Type: " << BatteryData[randNumber].Type << std::endl;
    BatteryData[randNumber].Type = "Li-Po";
    SaveToDB_Battery(BatteryData);

    driverTesting();
    std::map<std::string, Driver> DriverData = LoadDriverData();
    std::cout << DriverData[randString].Username << " has Kes " << DriverData[randString].Credits << std::endl;
    DriverData[randString].Credits = 999999;
    SaveToDB_Driver(DriverData);

    adminTesting();
    std::map<std::string, Admin> AdminData = LoadAdminData();
    std::cout << AdminData[randString].Username << " will now be called ";
    AdminData[randString].Username = randStr(10);
    std::cout << AdminData[randString].Username;
    SaveToDB_Admin(AdminData);

    return 0;
}
