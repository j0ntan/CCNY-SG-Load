if [ ! -d "include" ]; then
    mkdir include
fi
if [ ! -d "src" ]; then
    mkdir src
fi

cp ../../../CCNY_Smart_Grid_Load/include/Display.h include/
cp ../../../CCNY_Smart_Grid_Load/include/HardwareDisplay.h include/
cp ../../../CCNY_Smart_Grid_Load/src/HardwareDisplay.cpp src/
