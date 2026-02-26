#include "../Headers/Helpers.h"

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

    if (DriverData.empty()) {
        Driver defaultDriver = {"driver1", "pass", 100.0f};
        DriverData["driver1"] = defaultDriver;
        SaveToDB_Driver(DriverData);
        std::cout << "Initialized default driver: username 'driver1', password 'pass', credits 100\n";
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

void DriverSwapBattery(){
    std::map<std::string, Battery> BatteryData = LoadBatteryData();

    Battery oldBat; std::string newBattID;

    std::cout << "Enter the data about your old battery\n."
              << "========================" << std::endl                        
              << "Battery ID: "; std::cin >> oldBat.ID;
    std::cout << "Battery Type: "; std::cin >> oldBat.Type;
    std::cout << "Battery State of Charge: "; std::cin >> oldBat.SoC;
    std::cout << "Battery State of Health: "; std::cin >> oldBat.SoH;
    oldBat.Status="Charging";

    std::cout << "========================" << std::endl;
    std::cout << "Enter the ID of the new battery to pick.\n"; std::cin >> newBattID;
    std::cout << "========================" << std::endl;
    
    BatteryData.erase(newBattID); BatteryData[oldBat.ID] = oldBat;

    std::cout << "Replacement SUccessful" << std::endl;

}

void DriverDeposit(std::string & name){
    float newCredits = promptFloat("Enter the amount of credits you wish to deposit: ");
    std::map<std::string, Driver> DriverData = LoadDriverData();
    DriverData[name].Credits += newCredits;
    while(!SaveToDB_Driver(DriverData)){
        std::cout << "Deposit not saved\n";
    }
    std::cout << "Successfully Deposited\n";
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
                DriverSwapBattery();
                break;
            case 2:
                DriverDeposit(name);
                break;
            case 3:
                running = false;
                break; //Terminate the screen
        }
    }
}