#include "structs.h"
#include <fstream>

int batteryTesting(){
    std::ofstream outData;
    outData.open("BatteriesDB.txt", std::ios::app);

    Battery bat1 = {1213 , "Li-Ion", 99.99, 80.94, "Charging"};

    outData <<bat1.ID << " ";
    outData << bat1.Type << " ";
    outData << bat1.SoC << " ";
    outData << bat1.SoH << " ";
    outData << bat1.Status << '\n';

    outData.close();

    return 0;
}

int main(){
    batteryTesting();

    return 0;
}
