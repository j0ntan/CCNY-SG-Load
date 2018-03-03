if [ ! -d "include" ]; then
    mkdir include
fi
if [ ! -d "src" ]; then
    mkdir src
fi

cp ../../../CCNY_Smart_Grid_Load/include/ArduinoInterface.h include/
cp ../../../CCNY_Smart_Grid_Load/include/HardwareArduino.h include/
cp ../../../CCNY_Smart_Grid_Load/src/HardwareArduino.cpp src/
cp ../../../CCNY_Smart_Grid_Load/include/XBee.h include/
cp ../../../CCNY_Smart_Grid_Load/include/HardwareXBee.h include/
cp ../../../CCNY_Smart_Grid_Load/include/Keypad.h include/
cp ../../../CCNY_Smart_Grid_Load/include/Monitor.h include/
