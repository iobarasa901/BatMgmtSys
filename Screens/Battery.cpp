#include "../Headers/BatteryHeader.h"
#include <iomanip>
#include <iostream>

//Function to load and return a map for the full database.
std::map<int, Battery> LoadBatteryData(){
    std::map<int, Battery> BatteryData;

    std::ifstream batteryDB;
    batteryDB.open(BATTERY_DB);

    std::string line;

    while (std::getline(batteryDB, line)){//Fetches a whole line from the database and stores it into 'line'
        std::istringstream data(line); /* converts the whole string line into a 'list' of strings. 
        e.g "1231 Li-Ion Charging" gets converted to: 1231 "Li-Ion" "Charging" */

        int ID; std::string Type, Status; float SoC, SoH; //Define the variables that we want to store the broken down strings from line into

        if(!(data >> ID >> Type >> SoC >> SoH >> Status)){continue;} //To ignore all malformed lines as well as store the data into the variables

        Battery tempBat = {ID, Type, SoC, SoH, Status}; //create a temp battery from the struct to store into the map
        BatteryData[ID] = tempBat; //store the battery with its key to the map
    } // go to the next line

    batteryDB.close();

    return BatteryData;
}

//Function to save the map to the database
bool SaveToDB_Battery(std::map<int, Battery>& Data){
    std::ofstream outFile;

    remove(BATTERY_DB); // delete the existing file
    outFile.open(BATTERY_DB); // create a file identiacl to the one deleted

    for (const auto& each : Data){ // store one entry of the input map into 'each'
        outFile << each.second.ID << ' ' //each.second is the Driver struct since the map has the key as the 1st data and the struct as the 2nd
                << each.second.Type << ' ' // all of these store the diver struct data from the map to the file
                << each.second.SoC << ' ' 
                << each.second.SoH << ' ' 
                << each.second.Status << ' ' 
                << '\n'; //end the line
    }

    outFile.close();

    return true; // will put some logic here to only return true if the writing to the db is successful
}

//Battery management for the admin
void BatteryMgmt() {
    std::map<int, Battery> batteries = LoadBatteryData();

    int choice;
    bool running = true;

    while (running) {
        std::cout << "\n========================================" << std::endl;
        std::cout << "       Battery Management (Admin)       " << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "1. View All Batteries" << std::endl;
        std::cout << "2. Add New Battery" << std::endl;
        std::cout << "3. Update Battery Status" << std::endl;
        std::cout << "4. Monitor Battery Health (SoH Check)" << std::endl;
        std::cout << "5. Back" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Select: ";

        while (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Please enter a number: ";
        }

        switch (choice) {
            case 1:
                ViewAllBatteries(batteries);
                break;
            case 2:
                AddNewBattery(batteries);
                break;
            case 3:
                UpdateBatteryStatus(batteries);
                break;
            case 4:
                MonitorBatteryHealth(batteries);
                break;
            case 5:
                running = false;
                break;
            default:
                std::cout << "Invalid option. Try again." << std::endl;
        }
    }
}

// ── Helper: View all batteries ─────────────────────────────────────────────
void ViewAllBatteries(std::map<int, Battery>& batteries) {
    if (batteries.empty()) {
        std::cout << "No batteries in system." << std::endl;
        return;
    }
    std::cout << "\n--- Battery Inventory ---" << std::endl;
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(10) << "Type"
              << std::setw(8)  << "SoC(%)"
              << std::setw(8)  << "SoH(%)"
              << std::setw(20) << "Status" << std::endl;
    std::cout << std::string(52, '-') << std::endl;

    for (auto& [id, b] : batteries) {
        std::cout << std::left
                  << std::setw(6)  << b.ID
                  << std::setw(10) << b.Type
                  << std::setw(8)  << b.SoC
                  << std::setw(8)  << b.SoH
                  << std::setw(20) << b.Status << std::endl;
    }
}

// ── Helper: Add new battery ────────────────────────────────────────────────
void AddNewBattery(std::map<int, Battery>& batteries) {
    Battery newBattery;
    int id;

    std::cout << "\n--- Add New Battery ---" << std::endl;
    std::cout << "Enter Battery ID (integer): ";
    while (!(std::cin >> id)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid. Enter a number: ";
    }

    if (batteries.count(id)) {
        std::cout << "Battery ID " << id << " already exists." << std::endl;
        return;
    }

    std::cout << "Enter Type (e.g. Li-Ion, NiMH): ";
    std::cin >> newBattery.Type;

    // New batteries start at 100% SoC, prompt for SoH
    newBattery.ID     = id;
    newBattery.SoC    = 100.0f;
    std::cout << "Enter State of Health % (0-100): ";
    std::cin >> newBattery.SoH;

    // Auto-flag low health on entry
    if (newBattery.SoH < 70.0f) {
        newBattery.Status = "Maintenance Required";
        std::cout << "⚠ SoH below 70% — battery flagged as Maintenance Required." << std::endl;
    } else {
        newBattery.Status = "Available";
    }

    batteries[id] = newBattery;
    SaveToDB_Battery(batteries);
    std::cout << "✓ Battery " << id << " added successfully." << std::endl;
}

// ── Helper: Update battery status ─────────────────────────────────────────
void UpdateBatteryStatus(std::map<int, Battery>& batteries) {
    int id;
    std::cout << "\n--- Update Battery Status ---" << std::endl;
    std::cout << "Enter Battery ID: ";
    std::cin >> id;

    auto it = batteries.find(id);
    if (it == batteries.end()) {
        std::cout << "Battery ID " << id << " not found." << std::endl;
        return;
    }

    Battery& b = it->second;
    std::cout << "Current Status: " << b.Status << std::endl;
    std::cout << "Select new status:" << std::endl;
    std::cout << "1. Depleted" << std::endl;
    std::cout << "2. Charging" << std::endl;
    std::cout << "3. Ready (sets SoC to 100%)" << std::endl;
    std::cout << "4. Available" << std::endl;
    std::cout << "5. InUse" << std::endl;
    std::cout << "Select: ";

    int statusChoice;
    std::cin >> statusChoice;

    switch (statusChoice) {
        case 1: b.Status = "Depleted";  break;
        case 2: b.Status = "Charging";  break;
        case 3: b.Status = "Ready"; b.SoC = 100.0f; break;
        case 4: b.Status = "Available"; break;
        case 5: b.Status = "InUse";     break;
        default:
            std::cout << "Invalid choice." << std::endl;
            return;
    }

    SaveToDB_Battery(batteries);
    std::cout << "✓ Battery " << id << " status updated to: " << b.Status << std::endl;
}

// ── Helper: Monitor SoH and flag unhealthy batteries ──────────────────────
void MonitorBatteryHealth(std::map<int, Battery>& batteries) {
    std::cout << "\n--- Battery Health Report (SoH < 70%) ---" << std::endl;

    bool anyFlagged = false;
    for (auto& [id, b] : batteries) {
        if (b.SoH < 70.0f) {
            anyFlagged = true;
            if (b.Status != "Maintenance Required") {
                b.Status = "Maintenance Required";
            }
            std::cout << "⚠ Battery " << id
                      << " | Type: " << b.Type
                      << " | SoH: " << b.SoH << "%"
                      << " → Flagged: Maintenance Required" << std::endl;
        }
    }

    if (!anyFlagged) {
        std::cout << "✓ All batteries are healthy." << std::endl;
    } else {
        SaveToDB_Battery(batteries);
        std::cout << "Database updated." << std::endl;
    }
}