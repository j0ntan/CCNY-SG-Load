if [ ! -d "include" ]; then
    mkdir include
fi
if [ ! -d "src" ]; then
    mkdir src
fi

cp ../../../CCNY_Smart_Grid_Load/include/ArduinoInterface.h include/
cp ../../../CCNY_Smart_Grid_Load/include/HardwareArduino.h include/
cp ../../../CCNY_Smart_Grid_Load/src/HardwareArduino.cpp src/
