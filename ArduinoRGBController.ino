/*
  
  Based on GNATSTATS PC Performance Monitor - Version 1.x  Rupert Hirst & Colin Conway © 2016
  http://tallmanlabs.com  & http://runawaybrainz.blogspot.com/
  
  Licence
  -------

  Attribution-NonCommercial-ShareAlike  CC BY-NC-SA

  This license lets others remix, tweak, and build upon your work non-commercially, as long as they credit you and license their new creations under the identical terms.

  https://creativecommons.org/licenses/
  
  "Arduino RGB Control Centre" was inspired by the Visual Studio project kindly shared by psyrax see: https://github.com/psyrax/SerialMonitor

  ASCII: http://patorjk.com/software/taag/
  ------------------------------------------------------------------------------------------
   _    ___ ___ ___    _   ___ ___ ___ ___
  | |  |_ _| _ ) _ \  /_\ | _ \_ _| __/ __|
  | |__ | || _ \   / / _ \|   /| || _|\__ \
  |____|___|___/_|_\/_/ \_\_|_\___|___|___/

  ------------------------------------------------------------------------------------------
  ATMEL Libraries
  ---------------

  Adafruit SSD1306 library:
  https://github.com/adafruit/Adafruit_SSD1306
   
  Adafruit GFX Library:
  https://github.com/adafruit/Adafruit-GFX-Library
*/

#include "Configuration_Settings.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "bitmap.h"

/*SSD1306 OLED setup*/

#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//---------------------------------------------------------------------------------------
/* Uncomment below, to take out small degree symbol for better spacing
   when hitting 100% cpu/gpu load the percent symbol gets clipped */
//#define noDegree

//---------------------------------------------------------------------------------------
/* Inverted timers for oled*/
long lastInvertTime = 0;
int  invertedStatus = 1;

/* More OLED stuff*/
int displayDraw = 0;
int displayOverride = 0;

/* Timer for active connection to host*/
boolean activeConn = false;
long lastActiveConn = 0;
boolean bootMode = true;

/*vars for serial input*/
String inputString = "";
boolean stringComplete = false;

/* cycle screen DisplayStyles */
int displayChangeMode = 1;
long lastDisplayChange;

/*Variables for controlling LED strip */
String commandString = "";
String REDVal = "";
String GREENVal = "";
String BLUEVal = "";
int PowerPin = 8;
int GreenPin = 10;
int RedPin = 9;
int BluePin = 11;
bool isPowered = false;
bool isON = false;

boolean isConnected = false;
/*
  ------------------------------------------------------------------------------------------
   ___ ___ _____ _   _ ___
  / __| __|_   _| | | | _ \
  \__ \ _|  | | | |_| |  _/
  |___/___| |_|  \___/|_|
  ------------------------------------------------------------------------------------------
*/

void setup() {

  /* OLED SETUP */
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c); // initialize with the I2C addr 0x3D (for the 128x64)

  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setRotation(ASPECT);// Rotate the display at the start:  0, 1, 2 or 3 = (0, 90, 180 or 270 degrees)
  display.clearDisplay();

  /* stops text wrapping*/
  display.setTextWrap(false); // Stop  "Loads/Temps" wrapping and corrupting static characters


  /* Serial setup, start serial port at 9600 bps and wait for port to open:*/
  Serial.begin(9600); // 32u4 USB Serial Baud Rate
  //Serial.begin(115200);
  inputString.reserve(200);

  /*Setup RGB Pins */
  pinMode(PowerPin, OUTPUT);
  pinMode(GreenPin, OUTPUT);
  pinMode(RedPin, OUTPUT);
  pinMode(BluePin, OUTPUT);

  /*Initial Load screen*/
  splashScreen();

}

//END of Setup


/*
  ------------------------------------------------------------------------------------------
   __  __   _   ___ _  _   _    ___   ___  ___
  |  \/  | /_\ |_ _| \| | | |  / _ \ / _ \| _ \
  | |\/| |/ _ \ | || .` | | |_| (_) | (_) |  _/
  |_|  |_/_/ \_\___|_|\_| |____\___/ \___/|_|
  ------------------------------------------------------------------------------------------
*/

void loop() {


  /*Serial stuff*/
  serialEvent();

  /*change display screen*/
  if ((millis() - lastDisplayChange) > displayChangeDelay)
  {
    if (displayChangeMode == 1) {
      displayChangeMode = 2;
      display.fillRect(0, 0, 128 , 64, BLACK);
    } else if (displayChangeMode == 2) {
      displayChangeMode = 1;
      display.fillRect(0, 0, 128 , 64, BLACK);
    }

    lastDisplayChange = millis();
  }


  /* OLED DRAW STATS */
  if (stringComplete) {

    if (bootMode) {

      splashScreen2();
      display.clearDisplay();
      display.display();

      bootMode = false;
    }
    getCommand(); //Get the command to be executed

    if (commandString.equals("STA"))
    {
      //STA command starts/turns ON the LED strips
      isPowered = true;
      display.clearDisplay();
      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(2); //set background txt font size
      display.setCursor(1, 12);
      display.println(F("LIGHTS ON"));
      display.display();
      turnLedOn(PowerPin);
      delay(2000);

#ifdef enableActivityChecker
      activityChecker();
#endif

    }
    else if (commandString.equals("STO"))
    {
      //STA command stops/turns OFF the LED strips
      isPowered = false;
      display.clearDisplay();
      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(2); //set background txt font size
      display.setCursor(1, 12);
      display.println(F("LIGHTS OFF"));
      display.display();
      turnLedOff(PowerPin);
      delay(2000);

#ifdef enableActivityChecker
      activityChecker();
#endif
    }
    else if (commandString.equals("RGB"))
    {
      //Manual text for RGB color value test/debug
      getRGB();
      setRGB();

#ifdef enableActivityChecker
      activityChecker();
#endif
    }
    else
    {
#ifdef enableActivityChecker
      activityChecker();
#endif
            if (!isPowered)
            {
              if (isON) {
                turnLedOff(PowerPin);
              }
            }
      
            if (isPowered)
            {
              if (!isON)
              {
                turnLedOn(PowerPin);
              }
            }
      //turnLedOn(PowerPin);
      lastActiveConn = millis();

      if (displayChangeMode == 1) {
        DisplayCPUStat();
      }
      else if (displayChangeMode == 2) {
        DisplayGPUStat();
      }
    }
    inputString = "";
    stringComplete = false;
    commandString = "";


    /* Keep running anti screen burn, whilst serial is active */
    if ((millis() - lastInvertTime) > invertDelay && displayDraw == 1) {
      lastInvertTime = millis();

#ifdef enableInvertscreen
      inverter();
#endif
    }
  }
}

//END of Main Loop


/*
  ------------------------------------------------------------------------------------------
    __ _   _ _  _  ___ _____ ___ ___  _  _ ___
  | __| | | | \| |/ __|_   _|_ _/ _ \| \| / __|
  | _|| |_| | .` | (__  | |  | | (_) | .` \__ \
  |_|  \___/|_|\_|\___| |_| |___\___/|_|\_|___/
  ------------------------------------------------------------------------------------------
*/

//-------------------------------------------  Serial Events -------------------------------------------------------------

void serialEvent() {

  while (Serial.available()) {          //  32u4 USB Serial Available?
    char inChar = (char)Serial.read();  // Read 32u4 USB Serial
    //Serial.print(inChar); // Debug Incoming Serial

    inputString += inChar;
    if (inChar == '|') {
      stringComplete = true;

      //display.drawRect(82, 0, 44, 10, WHITE); // Position Test
      display.fillRect(115, 0, 42, 10, BLACK); // Flash top right corner when updating
      display.display();
    }
  }
}

//------------------------------------------- activityChecker  -----------------------------------------------------------

void activityChecker() {
  if (millis() - lastActiveConn > lastActiveDelay)

    activeConn = false;
  else
    activeConn = true;
  if (!activeConn) {
    if (invertedStatus)

      //Turn off display when there is no activity
      display.invertDisplay(0);
    display.clearDisplay();
    display.display();

    //Experimental,  attempt to stop intermittent screen flicker when in no activity mode "screen off" (due to inverter function?) fill the screen 128x64 black rectangle
    display.fillRect(0, 0, 128, 64, BLACK);
    display.display();
    displayDraw = 0;
  }
}

//-------------------------------------------- Anti Screen Burn inverter ------------------------------------------------


void inverter() {
  if ( invertedStatus == 1 ) {
    invertedStatus = 0;
  } else {
    invertedStatus = 1;
  };
  display.invertDisplay(invertedStatus);
  display.display();
}

//--------------------------------------------- Splash Screens --------------------------------------------------------
void splashScreen() {

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 5);
  display.print(F("Booting"));
  display.display();      // Show initial text
  delay(200);
  //display.fillRect(0, 0, 128 , 32, BLACK);
  display.setCursor(10, 5);
  display.print(F("Booting."));
  display.display();      // Show initial text
  delay(200);
  //  display.fillRect(0, 0, 128 , 32, BLACK);
  display.setCursor(10, 5);
  display.print(F("Booting.."));
  display.display();      // Show initial text
  delay(200);
  //  display.fillRect(0, 0, 128 , 32, BLACK);
  display.setCursor(10, 5);
  display.println(F("Booting..."));
  display.display();      // Show initial text
  delay(200);
  display.fillRect(0, 0, 128 , 64, BLACK);
  display.drawBitmap(0, 0, CyberEye, 128, 64, WHITE);
  display.display();
  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(1500);
  display.stopscroll();
  delay(500);
  display.startscrollleft(0x00, 0x0F);
  delay(3000);
  display.stopscroll();
  display.fillRect(0, 0, 128 , 64, BLACK);
  display.clearDisplay();
  display.display();
}

//---------------------------------------------------------------------------------------------------------------------


void splashScreen2() {
  display.drawBitmap(0, 0, CyberEye, 128, 64, WHITE);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
}

//--------------------------------------------- LED Functions --------------------------------------------------------
void getCommand()
{
  if (inputString.length() > 0)
  {
    commandString = inputString.substring(1, 4);
  }
}

void turnLedOn(int pin)
{
  isON = true;
  digitalWrite(pin, HIGH);
}

void turnLedOff(int pin)
{
  isON = false;
  digitalWrite(pin, LOW);
}

void setRGB()
{
  int g = GREENVal.toInt();
  int r = REDVal.toInt();
  int b = BLUEVal.toInt();
  analogWrite(GreenPin, g);
  analogWrite(RedPin, r);
  analogWrite(BluePin, b);
}

String getRGB()
{
  String value = inputString.substring(4, inputString.length());

  GREENVal = value.substring(0, 3);
  REDVal = value.substring(3, 6);
  BLUEVal = value.substring(6);
  return value;
}
