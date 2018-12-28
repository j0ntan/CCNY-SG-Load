if [ ! -d "include" ]; then
    mkdir include
fi
if [ ! -d "src" ]; then
    mkdir src
fi

cp ../../../CCNY_Smart_Grid_Load/include/LoadProfile.h include/
cp ../../../CCNY_Smart_Grid_Load/src/LoadProfile.cpp src/
cp ../../../CCNY_Smart_Grid_Load/include/SDCard.h include/
cp ../../../CCNY_Smart_Grid_Load/src/SDCard.cpp src/
