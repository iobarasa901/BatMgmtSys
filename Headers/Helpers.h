#include "BatteryHeader.h"
#include "AdminHeader.h"
#include "DriverHeader.h"

#include <iostream>
#include <iomanip>

void clearInput();

void pause();

int promptInt(const std::string &msg);

float promptFloat(const std::string &msg);

void showBattery(const Battery &b);

std::map<std::pair<float,std::string>, Battery> SoH_Ordered_Map();