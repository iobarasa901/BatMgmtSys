#include "../Headers/DriverHeader.h"
#include "../Headers/AdminHeader.h"
#include "../Headers/BatteryHeader.h"
#include <iostream>

bool isValidSelection(int selection);
void HandleDriverLogin();

int main(){
    int selection;

    bool running = true;

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
            
            // NEXT STEP: Call your Battery Swap Menu here
            // OpenSwapMenu(it->second); 
        } else {
            std::cout << "Login Failed: Incorrect password." << std::endl;
        }
    } else {
        std::cout << "Login Failed: Username '" << inputUser << "' not found." << std::endl;
    }
}