# CCNY Smart Grid Load

The Smart Grid Load (*SG-Load*) is a resistive, variable load component of the smart grid testbed at the Smart Grid Lab in the City College of New York. This project implements a control system for the SG-Load that allows:

* Setting the value of 3-phase AC loading to a balanced or unbalanced state.
* Varying the amount of loading for a DC source.
* Automating the amount of loading over time according to a load profile.

## Getting Started
This section summarizes the steps needed to upload the control system sketch to the Arduino and begin using the SG-Load through the keypad interface.\
*Note: The Arduino should already have the latest sketch version uploaded. The installation steps are only needed when debugging or upgrading this sketch.*

### Install & Run
1. [Download](https://github.com/j0ntan/CCNY-SG-Load/archive/master.zip) or clone ( `git clone https://github.com/j0ntan/CCNY-SG-Load.git` ) the repo into your **Arduino sketchbook location** directory, which may be under `{YourUserName}\Documents\Arduino` on a Windows machine.
2. Install the [New LiquidCrystal](https://bitbucket.org/fmalpartida/new-liquidcrystal) library, which replaces the usage of the default LCD library.
3. Open & upload the `CCNY_Smart_Grid_Load.ino` sketch using the Arduino IDE.
4. Switch the Arduino to external power and observe the LCD startup message to verify the sketch upload.

### Basic Input
The simplest way to control the SG-Load is with the front-panel keypad. Let's say we want to set up an unbalanced load at the AC terminals, with 10 switches turned ON for phases A & B, but only 9 for phase C. We can enter the sequence `A10B10C9`, or equivalently `AB10C9`, followed by the ENTER key (`#`), to activate this configuration.

The Arduino can also take input from an XBee-connected PC or the lab's dSPACE controller. These methods are slightly more complex and their descriptions are reserved for the wiki.

---
For further details on the installation & build process, system configurations and working with the SG-Load, check out the [wiki](https://github.com/j0ntan/CCNY-SG-Load/docs/wiki) documentation.