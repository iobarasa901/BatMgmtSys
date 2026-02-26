#include "../Headers/Helpers.h"

bool isValidSelection(int selection){ //This is local
    if (selection >= 1 && selection <= 4){
        return true;
    }else{
        return false;
    }
}

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
                DriverLogin();
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

