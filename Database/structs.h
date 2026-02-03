#pragma once 
#include <iostream>

//When Reading and writing to the Batteries.txt , do it in the order that they're arranged here for Standardisation.
struct Battery{
    int ID;
    std::string Type;
    float SoC;
    float SoH;
    std::string Status;
}; 

struct Driver{
    std::string Username;
    std::string Password;
    float Credits;
}; 

struct Admin{
    std::string Username;
    std::string Password;
};