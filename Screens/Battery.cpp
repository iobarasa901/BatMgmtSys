#include "../Headers/BatteryHeader.h"

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