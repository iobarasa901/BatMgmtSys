#include "../Headers/DriverHeader.h"
#include "../Headers/AdminHeader.h"
#include "../Headers/BatteryHeader.h"
#include <iostream>

bool isValidSelection(int selection);
void HandleDriverLogin();
void initializeData();
void driverDashboard(std::map<std::string, Driver>&, std::map<int, Battery>&, Driver&);

int main(){
    int selection;

    bool running = true;

    initializeData();

    while(running){
        std::cout << "================================================" << std::endl
                  << "Hello, Welcome to BEBE Battery Management System" << std::endl
                  << "================================================" << std::endl
                  << std::endl
                  << "Select the log in option to proceed with." << std::endl
                  << std::endl
                  << "1. Driver" << std::endl
                  << "2. Admin" << std::endl
                  << "3. Exit" << std::endl
                  << "4. Help with signing up" << std::endl
                  << std::endl;

        
        while(!(std::cin >> selection)){ //If user puts in a non-number
            std::cin.clear(); 
            std::cin.ignore(10000, '\n'); //Clear input buffer
            std::cout << "Please enter a number. Try again: ";
        }

        while(!isValidSelection(selection)){ //If use puts in a number thats not in range
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid Selection. Try again: "; std::cin >> selection;
        }

        switch(selection){
            case 1:
                //Open Driver Login Screen
                HandleDriverLogin();
                break;
            case 2:
                AdminLogin();
                //Open Admin Login Screen
                break;
            case 3:
                std::cout << "Exiting System. Goodbye!" << std::endl;
                running = false;
                break; //Terminate the app
            case 4:
                //Show that only existing admin can register new admins/drivers
                std::cout << "NOTE: Only existing admins can sign up new users\n";
                break;
        }

    }

    return 0;
}

bool isValidSelection(int selection){ //This is local
    if (selection >= 1 && selection <= 4){
        return true;
    }else{
        return false;
    }
}

void HandleDriverLogin() {
    std::cout << "\n--- Driver Login Screen ---" << std::endl;
    
    // 1. Load the database into a map 
    std::map<std::string, Driver> driverDB = LoadDriverData();
    
    if (driverDB.empty()) {
        std::cout << "System Error: Driver database is empty or unavailable." << std::endl;
        return;
    }

    std::string inputUser, inputPass;
    std::cout << "Username: ";
    std::cin >> inputUser;
    std::cout << "Password: ";
    std::cin >> inputPass;

    // 2. Use find() to check if user exists without adding a blank entry
    auto it = driverDB.find(inputUser);

    if (it != driverDB.end()) {
        // 'it->second' refers to the Driver struct associated with that username
        if (it->second.Password == inputPass) {
            std::cout << "\n================================" << std::endl;
            std::cout << "Login Successful!" << std::endl;
            std::cout << "Driver: " << it->second.Username << std::endl;
            std::cout << "Credits: $" << it->second.Credits << std::endl;
            std::cout << "================================\n" << std::endl;
            
            // Call the driver dashboard
            std::map<int, Battery> batteries = LoadBatteryData();
            std::map<std::string, Driver> drivers = LoadDriverData();
            driverDashboard(drivers, batteries, it->second); 
        } else {
            std::cout << "Login Failed: Incorrect password." << std::endl;
        }
    } else {
        std::cout << "Login Failed: Username '" << inputUser << "' not found." << std::endl;
    }
}
// Mods before 7 pm on 25th Feb
void initializeData() {
    // Check if AdminDB is empty
    std::map<std::string, Admin> admins = LoadAdminData();
    if (admins.empty()) {
        Admin defaultAdmin = {"admin", "pass"};
        admins["admin"] = defaultAdmin;
        SaveToDB_Admin(admins);
        std::cout << "Initialized default admin: username 'admin', password 'pass'\n";
    }

    // Check if DriversDB is empty
    std::map<std::string, Driver> drivers = LoadDriverData();
    if (drivers.empty()) {
        Driver defaultDriver = {"driver1", "pass", 100.0f};
        drivers["driver1"] = defaultDriver;
        SaveToDB_Driver(drivers);
        std::cout << "Initialized default driver: username 'driver1', password 'pass', credits 100\n";
    }

    // Check if BatteriesDB is empty
    std::map<int, Battery> batteries = LoadBatteryData();
    if (batteries.empty()) {
        batteries[1] = {1, "Li-Ion", 85.0f, 90.0f, "Available"};
        batteries[2] = {2, "Li-Ion", 20.0f, 80.0f, "Available"};  // Low SoC example
        batteries[3] = {3, "NiMH", 100.0f, 95.0f, "Charging"};
        batteries[4] = {4, "Li-Ion", 50.0f, 85.0f, "InUse"};
        batteries[5] = {5, "NiMH", 10.0f, 70.0f, "Available"};   // Another low SoC
        SaveToDB_Battery(batteries);
        std::cout << "Initialized 5 sample batteries\n";
    }
}
//End of modifications before 7 pm on 25th Feb