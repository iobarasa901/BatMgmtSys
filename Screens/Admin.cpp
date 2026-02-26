#include "../Headers/AdminHeader.h"
#include "../Headers/DriverHeader.h"
#include <iostream>

bool SaveToDB_Admin(std::map<std::string, Admin>& Data){ //Same logic as the SaveToDB_Battery
    std::ofstream outFile;

    remove(ADMIN_DB);
    outFile.open(ADMIN_DB);

    if(!outFile.is_open())
    {
        return false;
    }

    for (const auto& each : Data){
        outFile << each.second.Username << ' ' 
                << each.second.Password << ' '
                << '\n';
    }

    outFile.close();

    return true;
}

std::map<std::string, Admin> LoadAdminData(){ //Same logic as loading battery data
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

bool AdminVerification(std::string& name, std::string& pass, std::map<std::string, Admin>& map){ //Logic to verify user
    if (map[name].Password == pass){
        return true;
    }else{
        return false;
    }
};

bool isValidSelectionAdmin(int selection){ //This is local
    if (selection >= 1 & selection <= 4){
        return true;
    }else{
        return false;
    }
}

void DriverRegistration(){
    std::string newName, newPass;

    std::cout << "========================" << std::endl
              << "Registering new Driver" << std::endl
              << "========================" << std::endl
              << "Enter the unique username of the new driver: "; std::cin >> newName;
    std::cout << "Enter the password for the new driver: "; std::cin >> newPass;

    std::map<std::string, Driver> DriverData = LoadDriverData();

    if (DriverData.count(newName) > 0){ //To check if the username already exists
        std::cout << "Username Unavailable\nEnter Username again: ";std::cin >> newName;
        std::cout << "Enter the password for the new driver"; std::cin >> newPass;
    }

    std::cout << "Saving Driver Information\n";
    Driver tempDriver = {newName, newPass, 0.0};

    DriverData[newName] = tempDriver;
    if(!SaveToDB_Driver(DriverData)){
        std::cout << "Failed to save to database\n";
    }
}

void AdminRegistration(){
    std::string newName, newPass;

    std::cout << "========================" << std::endl
              << "Registering new Admin" << std::endl
              << "========================" << std::endl
              << "Enter the unique username of the new admin: "; std::cin >> newName;
    std::cout << "Enter the password for the new admin"; std::cin >> newPass;

    std::map<std::string, Admin> AdminData = LoadAdminData();

    if (AdminData.count(newName) > 0){ //To check if the username already exists
        std::cout << "Username Unavailable\nEnter Username again: ";std::cin >> newName;
        std::cout << "Enter the password for the new admin: "; std::cin >> newPass;
    }

    std::cout << "Saving Admin Information\n";
    Admin tempAdmin = {newName, newPass};

    AdminData[newName] = tempAdmin;
    if(!SaveToDB_Admin(AdminData)){
        std::cout << "Failed to save to database\n";
    }
}


// Main Login Screen
void AdminLogin(){
    std::cout << "=== Loading Admin Data ===" << std::endl;
    std::map<std::string, Admin> AdminData = LoadAdminData();
    std::cout << "=== Finished Loading Admin Data ===" << std::endl;

    std::string searchName,searchPass;

    std::cout << "========================" << std::endl
              << "Login in as Admin Below" << std::endl
              << "========================" << std::endl
              << std::endl
              << "Enter username: ";
    std::cin >> searchName;
    std::cout << "Enter Password: ";
    std::cin >> searchPass;

    while (!AdminVerification(searchName, searchPass, AdminData)){ //Check for credentials 
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid Credentials. Try again\n";
        std::cout << "Enter Username: "; std::cin >> searchName;
        std::cout << "Enter Password: "; std::cin >> searchPass;
    }

    AdminScreen(searchName);
}

//Admin Dashboard
void AdminScreen(std::string& name){
    int selection;

    bool running = true; //To keep user in the dashboard until they decide to exit

    while(running){
        std::cout << std::endl
                << "========================" << std::endl
                << "Welcome " << name << std::endl
                << "========================" << std::endl
                << "Select an option to proceed with." << std::endl
                << std::endl
                << "1. Driver registration" << std::endl
                << "2. Admin registration" << std::endl
                << "3. Battery Management" << std::endl
                << "4. Sign Out" << std::endl
                << std::endl;

        while(!(std::cin >> selection)){ //If user puts in a non-number
                std::cin.clear(); 
                std::cin.ignore(10000, '\n'); //Clear input buffer
                std::cout << "Please enter a number. Try again: ";
            }

        while(!isValidSelectionAdmin(selection)){ //If use puts in a number thats not in range
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid Selection. Try again: "; std::cin >> selection;
        }

        switch(selection){
            case 1:
                DriverRegistration();
                break;
            case 2:
                AdminRegistration();
                break;
            case 3:
                BatteryMgmt();
                break;
            case 4:
                running = false;
                break; //Terminate the app
        }
    }
}