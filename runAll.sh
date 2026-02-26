#!/bin/bash
echo "Starting compilation"
g++ Screens/main.cpp Screens/Admin.cpp Screens/Battery.cpp Screens/Driver.cpp Screens/Helpers.cpp -o runAll
echo "Compilation completed."
echo "Run ./runAll to open the app."   