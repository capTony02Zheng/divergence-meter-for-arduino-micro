# divergence-meter-for-arduino-micro
 This is a project use for creating an divergence meter. Thank you for visiting.
-----------------------------------------------------------------------

 Code are well written but need the user to update the IRdata to match the 1838IR remote control. Also the ds3231 module need to be updated with
other code, which is not provided here.

 The PCB is developed with kiCAD.



 To start up from nothing, here's the few steps you need to follow:
-----------------------------------------------------------------------
 Get all the component you need, which includes:

// An arduino micro board;
// A pcb that is printed with the file provided; (this pcb is 51mm * 130mm in size)
// A ds3231 time module;
// An IR1838 module with corresponding remote control;
// One or two 5v to 170v boost module;
// multiple resistors of 10k and 100k;
// four hv5523pg-g chips; you can buy some here: https://www.microchip.com/en-us/product/hv5522#document-table;

After gathering all the component, it is time for you to do the soldering work. It is recommended to solder the 4 chips first, then the arduino board.

About the boost module:
pin "170vin1" and "170vin2" should be connected to the 170v output from the boost module. pin "5VFB" shoule be connected to the 5v input on the boost module.

About external power source(Optional):
if you decided to use other battery module to support the arduino, the pins "BatteryIn" and "BatteryGround" is provided, the supporting volts should not exceeds the the range that arduino micro can accept.

About the resistors:
It is recommended to connect 10k or 20k resistors at the pins "170V" and "170V2";
for places like "t1dp" and "t2dp", a 100k resistor is recommended.

About the pins:
you can directly connected the nixie tubes pins to the correponding hole in the pcb, like the t1n1 means tube1 number1, at displays it is the number "1" of the left most nixie tube. or you can use ph2.0 sockets with 12 pins.



Features and controls of the divergence meter:
-----------------------------------------------------------------------
There are 6 mode for the divergence meter, the first one displays current time; the second one displays current date; the third one
displays Celsius temperature recorded by ds3231; the fourth mode displays displays worldline; the fifth mode displays the number you entered;
the sixth one diplays number from 0 to 9 to protect the tubes;

//About the control:

Power: close or open;

Menu: First time you pressed you entered menu mode, then you can push button from 0 to 5 to choose corresponding mode, which will help you leave the menu mode automatically. Or at menu mode you can use arrow to go to the next or previous mode, then press menu again to exit the menu mode;

Play: At worldline mode you can use it to get in and out of random display mode; At manual input mode you can use it to open or close displays.

"-": as decimal point in manual input mode.

"C": At manual input mode you can use it to change wether the decimal point and number are display in one tube or not, which can save some spaces. At worldline mode you can change the number to be displayed one by one or all by instant.

"return": clear all the input data at the manual display mode.

left and right arrow: when at worldline mode and the menu mode is off, you can use it to display the next or previous worldline.
