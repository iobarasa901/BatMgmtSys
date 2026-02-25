#include "../Headers/DriverHeader.h"
#include "../Headers/BatteryHeader.h"
#include <iostream>
#include <limits>

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

bool DriverVerification(std::string& name, std::string& pass, std::map<std::string, Driver>& map){
    if (map[name].Password == pass){
        return true;
    }else{
        return false;
    }
}

bool isValidSelectionDriver(int selection){ //This is local
    if (selection >= 1 & selection <= 3){
        return true;
    }else{
        return false;
    }
}

void DriverLogin(){
    std::cout << "=== Loading Driver Data ===" << std::endl;
    std::map<std::string, Driver> DriverData = LoadDriverData();
    std::cout << "=== Finished Loading Driver Data ===" << std::endl;

    std::string searchName,searchPass;

    std::cout << "========================" << std::endl
            << "Login in as Driver Below" << std::endl
            << "========================" << std::endl
            << std::endl
            << "Enter username: ";
    std::cin >> searchName;
    std::cout << "Enter Password: ";
    std::cin >> searchPass;

    while (!DriverVerification(searchName, searchPass, DriverData)){ //Check for credentials 
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid Credentials. Try again";
        std::cout << "Enter Username: "; std::cin >> searchName;
        std::cout << "Enter Password: "; std::cin >> searchPass;
    }

    DriverScreen(searchName);    
}

void DriverScreen(std::string& name){
    int selection;

    bool running = true; //To keep user in the dashboard until they decide to exit

    while(running){
        std::cout << std::endl
                << "========================" << std::endl
                << "Welcome " << name << std::endl
                << "========================" << std::endl
                << "Select an option to proceed with." << std::endl
                << std::endl
                << "1. Swap Battery" << std::endl
                << "2. Deposit" << std::endl
                << "3. Sign Out" << std::endl
                << std::endl;

        while(!(std::cin >> selection)){ //If user puts in a non-number
                std::cin.clear(); 
                std::cin.ignore(10000, '\n'); //Clear input buffer
                std::cout << "Please enter a number. Try again: ";
            }

        while(!isValidSelectionDriver(selection)){ //If use puts in a number thats not in range
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid Selection. Try again: "; std::cin >> selection;
        }

        switch(selection){
            case 1:
                //Open Battery Swap Screen
                break;
            case 2:
                //Open Deposit Screen
                break;
            case 3:
                running = false;
                break; //Terminate the screen
        }
    }
}

// ===== Input Helpers =====

void clearInput(){
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void pause(){
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int promptInt(const std::string &msg){
    int v;
    while (true){
        std::cout << msg;
        if (std::cin >> v){ 
            clearInput(); 
            return v; 
        }
        std::cout << "Invalid integer. Try again.\n";
        clearInput();
    }
}

float promptFloat(const std::string &msg){
    float v;
    while (true){
        std::cout << msg;
        if (std::cin >> v){ 
            clearInput(); 
            return v; 
        }
        std::cout << "Invalid number. Try again.\n";
        clearInput();
    }
}

void showBattery(const Battery &b){
    std::cout << "ID:" << b.ID << " Type:" << b.Type 
         << " SoC:" << b.SoC << " SoH:" << b.SoH 
         << " Status:" << b.Status << '\n';
}

// ===== Driver Dashboard =====

void driverDashboard(std::map<std::string, Driver>& drivers, std::map<int, Battery>& batteries, Driver &me){
    while (true){
        std::cout << "\n========== Driver Dashboard ==========\n";
        std::cout << "User: " << me.Username << " | Credits: " << me.Credits << "\n";
        std::cout << "1) View low batteries (SoC < 30%)\n";
        std::cout << "2) Swap battery\n";
        std::cout << "3) Deposit credits\n";
        std::cout << "4) Show balance\n";
        std::cout << "5) Sign out\n";
        int choice = promptInt("Choose: ");

        if (choice == 1){
            std::cout << "\n--- Low Battery List (SoC < 30) ---\n";
            bool found = false;
            for (const auto &p : batteries){
                if (p.second.SoC < 30.0f){ 
                    showBattery(p.second);
                    found = true;
                }
            }
            if (!found) std::cout << "No low batteries.\n";
            pause();

        } else if (choice == 2){
            int id = promptInt("Enter battery ID to swap: ");
            auto it = batteries.find(id);
            if (it == batteries.end()){ 
                std::cout << "Battery not found.\n"; 
            } else {
                // Toggle between Available and InUse
                if (it->second.Status == "Available"){
                    it->second.Status = "InUse";
                    std::cout << "Battery " << id << " marked as InUse.\n";
                } else {
                    it->second.Status = "Available";
                    std::cout << "Battery " << id << " marked as Available.\n";
                }
                SaveToDB_Battery(batteries);
            }
            pause();

        } else if (choice == 3){
            float amt = promptFloat("Amount to deposit: ");
            if (amt > 0){ 
                me.Credits += amt; 
                drivers[me.Username] = me;  // Update map
                SaveToDB_Driver(drivers);    // Save to file
                std::cout << "Deposited $" << amt << ". New balance: " << me.Credits << "\n"; 
            } else {
                std::cout << "Must deposit positive amount.\n";
            }
            pause();

        } else if (choice == 4){
            std::cout << "Your current balance: $" << me.Credits << "\n";
            pause();

        } else if (choice == 5){
            std::cout << "Signing out...\n";
            break;  // Exit loop, return to main menu

        } else {
            std::cout << "Invalid option.\n";
        }
    }
}