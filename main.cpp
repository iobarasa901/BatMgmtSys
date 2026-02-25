#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include <vector>
#include "Structs_and_Maps.h"

// ===== Input Helpers =====

void clearInput(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pause(){
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int promptInt(const string &msg){
    int v;
    while (true){
        cout << msg;
        if (cin >> v){ 
            clearInput(); 
            return v; 
        }
        cout << "Invalid integer. Try again.\n";
        clearInput();
    }
}

float promptFloat(const string &msg){
    float v;
    while (true){
        cout << msg;
        if (cin >> v){ 
            clearInput(); 
            return v; 
        }
        cout << "Invalid number. Try again.\n";
        clearInput();
    }
}

void showBattery(const Battery &b){
    cout << "ID:" << b.ID << " Type:" << b.Type 
         << " SoC:" << b.SoC << " SoH:" << b.SoH 
         << " Status:" << b.Status << '\n';
}
// ===== Driver Dashboard =====

void driverDashboard(map<string, Driver>& drivers, map<int, Battery>& batteries, Driver &me){
    while (true){
        cout << "\n========== Driver Dashboard ==========\n";
        cout << "User: " << me.Username << " | Credits: " << me.Credits << "\n";
        cout << "1) View low batteries (SoC < 30%)\n";
        cout << "2) Swap battery\n";
        cout << "3) Deposit credits\n";
        cout << "4) Show balance\n";
        cout << "5) Sign out\n";
        int choice = promptInt("Choose: ");

        if (choice == 1){
            cout << "\n--- Low Battery List (SoC < 30) ---\n";
            bool found = false;
            for (const auto &p : batteries){
                if (p.second.SoC < 30.0f){ 
                    showBattery(p.second);
                    found = true;
                }
            }
            if (!found) cout << "No low batteries.\n";
            pause();

        } else if (choice == 2){
            int id = promptInt("Enter battery ID to swap: ");
            auto it = batteries.find(id);
            if (it == batteries.end()){ 
                cout << "Battery not found.\n"; 
            } else {
                // Toggle between Available and InUse
                if (it->second.Status == "Available"){
                    it->second.Status = "InUse";
                    cout << "Battery " << id << " marked as InUse.\n";
                } else {
                    it->second.Status = "Available";
                    cout << "Battery " << id << " marked as Available.\n";
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
                cout << "Deposited $" << amt << ". New balance: " << me.Credits << "\n"; 
            } else {
                cout << "Must deposit positive amount.\n";
            }
            pause();

        } else if (choice == 4){
            cout << "Your current balance: $" << me.Credits << "\n";
            pause();

        } else if (choice == 5){
            cout << "Signing out...\n";
            break;  // Exit loop, return to main menu

        } else {
            cout << "Invalid option.\n";
        }
    }
}