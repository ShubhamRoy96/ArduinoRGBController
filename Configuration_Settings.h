/*
  --------------------------------------------------------------------------------------
    ___ ___  _  _ ___ ___ ___ _   _ ___    _ _____ ___ ___  _  _
   / __/ _ \| \| | __|_ _/ __| | | | _ \  /_\_   _|_ _/ _ \| \| |
  | (_| (_) | .` | _| | | (_ | |_| |   / / _ \| |  | | (_) | .` |
   \___\___/|_|\_|_| |___\___|\___/|_|_\/_/ \_\_| |___\___/|_|\_|
     ___  ___ _____ ___ ___  _  _ ___
    / _ \| _ \_   _|_ _/ _ \| \| / __|
   | (_) |  _/ | |  | | (_) | .` \__ \
    \___/|_|   |_| |___\___/|_|\_|___/
  --------------------------------------------------------------------------------------
*/

/* Screen rotation for oled*/
int ASPECT =        0;// Rotate the display at the start:  0, 1, 2 or 3 = (0, 90, 180 or 270 degrees)


//---------------------------------------------------------------------------------------

/* Uncomment below, to enable custom threshold event triggers*/
//#define uVol_enableThesholdtriggers
//#define enableCustomThesholdtriggers
//#define enableNeopixelGauges // NeoPixel ring bargraph example

//---------------------------------------------------------------------------------------

/* comment out, to disable blank screen on serial timeout to retain info eg: PC crash fault diagnostics  */
#define enableActivityChecker

/* How long the display takes to timeout due to inactive serial data from the windows application */

#define lastActiveDelay 8000

//---------------------------------------------------------------------------------------

/* Uncomment below, to enable positive and negative screen cycler */
#define enableInvertscreen

/* How long before inverting the display */
long invertDelay = 60000; // 60 sec  delay

//---------------------------------------------------------------------------------------
/* Time between "DisplayStyle" changes */

#define displayChangeDelay 18000

//---------------------------------------------------------------------------------------
