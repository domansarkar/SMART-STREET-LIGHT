/************************ SMART STREET LIGHT ver- 5.0 *******************************
 *  *****************************************************************************
 * 
 * This is a open source project made by DOMAN SARKAR 
 * 
 * The SMART STREET LIGHT project is a advanced street light which consumes less power than traditional street light as it detects the presence of any pedestrian or vehicle on the road which 
 * triggers the lamps to turn on at full brightness and stays on for some time and dims automatically after some time. The motivation behind the project came from the fact that traditional 
 * halogen street lights consume far more power as comapred to the SMART STREET LIGHT and they dont dim and consume very high energy and gets damaed easily. In this time I saw many street 
 * lights just stay damaged for countless years because they have a very short lifespan as comared to LED lights so our project will last longer and consume far more less energy than traditional 
 * street lights.
 * 
 *    FEATURES : -
 *  1. Auto Dim functionality
 *  2. Object detection on road (Turns the brightness to high when something is detected)
 *  3. Battery Operated
 *  4. Battery level and voltage monitoring
 *  5. Error code indication for faster diagnosis
 *  6. Two modes of charging (by solar panel and usb)
 *  7. Solar charging
 *  8. Temperature and humidity monitoring
 *  9. Additional OLED screen for extra functions
 *  10. SOS and buzzer feature
 * 
 *    ADVANTAGES : -
 * 1. Consumes far more less power than traditional halogen lamps and even non dimmable LED street lights.
 * 2. Widely cutomerisacble, you can change the animation speed, the light color, and can modify the project to add more features.
 * 3. Soothing turn on and turn off animation.
 * 4. Will last longer than traditional halogen lamps and also normal LED lamps and does not overheat.
 * 
 *    PARTS LIST : -
 * S.NO.        ITEM                  QUANTITY
 *  1.      HW-201 IR sensor             02
 *  2.      WS2812B LEDs                 21(12 for lamps 9 for studs)
 *  3.      ESP8266                      01
 *  4.      Voltage divider              01
 *  5.      Battery shield               01~02 (for 3 independent module)
 *  6.      Solar Panel                  01
 *  7.      AHT10                        01
 *  8.      SSD1306                      01
 *  9.      Switch                       01
 *  10.     Buzzer(passive)              01
 *  
 *    CONNECTIONS : -
 *    
 *    IR SENSOR 01
 * HW201 ------> ESP8266
 * VCC    -----     5v
 * GND    -----     GND
 * OUT    -----     D7(GPIO 13)
 * 
 *    IR SENSOR 02
 * HW201 ------> ESP8266
 * VCC    -----     5v
 * GND    -----     GND
 * OUT    -----     D0(GPIO 16)
 * 
 *    LEDs
 * WS2812B ------> ESP8266
 * VCC      -----   5v
 * GND      -----   GND
 * DIN      -----   D5(GPIO 14)
 * 
 *    Voltage Divider
 * VD -----------> ESP8266
 * GND      ----- GND
 * VCC      ----- No connection
 * SIGNAL   ----- A0
 * 
 *    AHT10
 * AHT10 --------> ESP8266
 * GND      ----- GND
 * VCC      ----- 3.3V
 * SDA      ----- SDA(GPIO 04)
 * SCL      ----- SCL(GPIO 05)
 * 
 *    SSD1306
 * SSD1306 -------> ESP8266
 * GND      ----- GND
 * VCC      ----- 5V
 * SDA      ----- SDA(GPIO 04)
 * SCL      ----- SCL(GPIO 05)
 * 
 *    Switch
 * Switch -------> ESP8266
 * GND      ----- GND
 * VCC      ----- 5V
 * SIG1     ----- D6(GPIO 12)
 * SIG2     ----- D6(GPIO 12)
 * 
 *    WS2812b (Stud Lights)
 * WS2812b -------> ESP8266
 * GND      ----- GND
 * VCC      ----- 5V
 * DI       ----- D6(GPIO 12)
 * 
 *    Buzzer
 * Buzzer -------> ESP8266
 * GND      ----- GND
 * VCC      ----- 5V
 * SIG      ----- D8(GPIO 15)
 * 
 * NOTE:
 * You can modify this project however you want :)
 * 
 * IMPORTANT :-
 * YOU MUST CHANGE THE VALUES OF :-
 * lat - latitude of device (must be uniqe among all devices)
 * lon - longitude of device (must be unique)
 * auth[] - auth token from blynk project (must be unique)
 * ssid[] - SSID of wifi
 * pass[] - password of wifi
 * R1 - resistor value of R1
 * R2 - resistor value of R2
 * AHT - ENABLED or DISABLED
 * OLED - ENABLED or DISABLED
 * id - id of the device from 0 to 9999 (must be unique)
 * 
 */

//////////////// (**) VLAUES MUST BE CHANGED /////////////////

////////////////////////////////  LIBRARIES ////////////////////////////////
/**************************************************************************/
#include <FastLED.h>          //Library for FastLED (supports WS2812B, WS2812, WS2811 etc.)
#define BLYNK_PRINT Serial    //Library for Blynk IoT
#include <ESP8266WiFi.h>      //Library for esp8266 wifi
#include <BlynkSimpleEsp8266.h> //contains simple and some necessary functions for Blynk IoT
#include <Adafruit_AHTX0.h>   //Library for AHT10 temperature and humidity sensor
#include <Wire.h>             //default wiring and pinout of boards
#include <Adafruit_GFX.h>     //Library for OLED GFX
#include <Adafruit_SSD1306.h> //Library for SSD1306 OLED display

////////////////////////////////  MACROS ////////////////////////////////////
/***************************************************************************/
/******** SSD1306 OLED SCREEN ************/
#define SCREEN_WIDTH 128      //OLED screen width in pixels
#define SCREEN_HEIGHT 64      //OLED screen height in pixels
#define OLED_RESET     -1     //OLED reset pin of there is no reset pin then set it to -1
#define SCREEN_ADDRESS 0x3C   //OLED screen I2C address

/******** ENABLED / DIABLED VALUES ************/
#define ENABLED 1             //Enabled sensor value
#define DISABLED 0            //Disabled sensor value

/******** LAMP LED ************/
#define LED_Pin 14            //Pin of the micro-controller connected to the led's data pin
#define NUM_LEDS 12           //total number of addressable led(s) used (this module will control 3 lamps and each lamp contains 4 led(s))

/******** IR SENSOR ************/
#define IRPin1 13              //Pin of micro-controller for IR sensor (to detect the presence of object)
#define IRPin2 16             //Pin of micro-controller for IRsensor for other side of the road

/******** LAMP TIMERS ************/
#define OnTime 300            //The time till ligts are turned on after detecting presence of object (seconds*100 for optimal results)
#define TimerInc 10           //The incrementor will add the value to timer every loop until it reches the OnTime value after it gets reset

/******** BATTERY VOLTAGE INPUT PIN ************/
#define analogInput A0        //Analog Pin for Battery Voltage Detection

/******** LOW BATTERY LEVEL ************/
#define lowBAT 10             //low battery level to get into battery saving mode

/******** LOCATION OF MODULE ************/
#define lat 29.3301           //**latitude of device loaction  ----------------------------------------------------------------------------------------------(MUST BE CHANGED)*
#define lon 77.9861           //**longitude of device location  ---------------------------------------------------------------------------------------------(MUST BE CHANGED)*

/******** ROAD STUDS ************/
#define NUM_STUDS 9           //Number of studs in one mmodule (9)
#define STUD_PIN 12           //road stud led pin
#define STUD_COLOR 10         //stud led default color
#define STUD_RED 0            //stud red color (police chaser)
#define STUD_BLUE 160         //stud blue color (police chaser)

/******** BUZZER PIN ************/
#define buzzer 15             //buzzer pin

/******** SWITCH PIN ************/
#define swPin 12              //switch and stud led pin

////////////////////////////////  GLOBAL VARIABLES ////////////////////////////////////
/*************************************************************************************/
/******** WIFI AND BLYNK CREDENTIALS ************/
char auth[] = "AUTH"; //**Blynk Auth sent to your mail  -------------------------------------------------------------------------(MUST BE CHANGED)*
char ssid[] = "SSID";         //**SSID of wifi --------------------------------------------------------------------------------------------------------------(MUST BE CHANGED)*
char pass[] = "PASSWORD";  //**Wifi password -----------------------------------------------------------------------------------------------------------(MUST BE CHANGED)

/******** SSD1306 OLED ************/
bool OLED = ENABLED;          //**enable or disable OLED screen ---------------------------------------------------------------------------------------------(MUST BE CHANGED)*
byte dispst = 2;              //display inversion state
byte sign = 0;                //sign is the index of the sign to be displayed

/******** BATTERY VOLTAGE ************/
float vin = 0.0;              //variable to store 
float R1 = 9770;              //R1 resistor in voltage devider
float R2 = 999;               //R2 resistor in voltage devider
byte BATlvl = 0;              //Battery level percentage
bool st = 0;                  //previous state of the battery less than 10% or not

/******** SWITCH PIN*******/
byte H = 20;                  //The H component of HSV color wheel
byte S = 0;                   //The S component of HSV color wheel
byte V = 255;                 //The V component of HSV color wheel
byte DefBri = 90;             //The default brightness of the of lamps (energy saving mode), ranges from 0 to 255(max)
byte MaxBri = 255;            //The maximum brightness of the lamps, ranges from 0 to 255(max)
byte Def = DefBri;            //The default brightness value variable
byte Max = MaxBri;            //The max brightness value variable
byte lowBatDefBri = 50;       //The default brightness of the lamps when battery is less than 10%
byte lowBatMaxBri = 90;       //The max brightness value of the lamps when battery is less than 10%
byte currBri = V;             //Variable to store current brightness of the bulbs
bool prevst = 0;              //previous state of turning on of the lamps
short state = 0;              //state of the presence of object can be 1 or -1
short tempState = 0;          //temporary variable to store state
int timer = 0;                //timer variable for time keeping

/******** LAMPS TOGGLE ON_OFF ************/
bool toggle = 1;              //Toggle state for On Off switch of the lamp
bool Tog = 0;                 //Temporary toggle state
bool togSt = 1;               //prev toggle state

/******** AHT10 TEMPERATURE AND HUMIDITY ************/
bool AHT = ENABLED;           //**AHT10 for humidity and temperature(enabled or disabled) -------------------------------------------------------------------(MUST BE CHANGED)*
float temp = 0;               //variable to store temperature
float hum = 0;                //variable to store humidity

/******** BLYNK DEVICE ID ************/
int id = 1;                   //**device id -----------------------------------------------------------------------------------------------------------------(MUST BE CHANGED)*

/******** MAP ************/
int m = 0;                    //map value update sequence

/******** STUD ANIMATION ************/
int ledno = 0;                //interation variable
bool midst = 1;               //for alternating stud color

/******** SOS SWITCH OFF ************/
bool arr = 0;                 //status of switch on/off from app

////////////////////////////////  METHODS / OBJECTS AND ARRAYS ////////////////////////////////////
/*************************************************************************************************/
/******** LAMPS FASTLED ARRAY ************/
CRGBArray<NUM_LEDS> leds;     //array of all led(s) connected to the micro-contoller

/******** AHT10 ************/
Adafruit_AHTX0 aht;           //object of AHT10 so that predefined functions can be called

/******** SSD1306 OLED DISPLAY ************/
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/******** MAP ************/
WidgetMap myMap(V9);          //Map widget assigned to V9 pin

/******** WS2812B STUDS ARRAY ************/
CRGB studs[NUM_STUDS];        //studs array of leds

////////////////////////////////  BITMAP IMAGE HEX CODE ////////////////////////////////////
/******************************************************************************************/
/******** NO PARKING HEX CODE ************/
static const unsigned char PROGMEM noparking[] =  //hex code for no parking image
{ 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 
    0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 
    0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 
    0x00, 0x03, 0xff, 0xf0, 0x0f, 0xff, 0xc0, 0x00, 
    0x00, 0x07, 0xff, 0x00, 0x00, 0xff, 0xe0, 0x00, 
    0x00, 0x0f, 0xf8, 0x00, 0x00, 0x1f, 0xf0, 0x00, 
    0x00, 0x1f, 0xe0, 0x00, 0x00, 0x07, 0xfc, 0x00, 
    0x00, 0x7f, 0x80, 0x7f, 0xff, 0x81, 0xfe, 0x00, 
    0x00, 0x7f, 0x00, 0xff, 0xff, 0xc0, 0xff, 0x00, 
    0x00, 0xff, 0x80, 0xff, 0xff, 0xe0, 0x7f, 0x00, 
    0x01, 0xff, 0xc0, 0xff, 0xff, 0xf0, 0x1f, 0x80, 
    0x03, 0xff, 0xe0, 0xf8, 0x01, 0xf0, 0x0f, 0xc0, 
    0x07, 0xff, 0xf0, 0xf8, 0x00, 0xf0, 0x0f, 0xe0, 
    0x07, 0xff, 0xf8, 0xf8, 0x00, 0xf0, 0x07, 0xe0, 
    0x0f, 0xdf, 0xfc, 0xf8, 0x00, 0xf0, 0x03, 0xf0, 
    0x0f, 0x8f, 0xfe, 0xf8, 0x00, 0xf0, 0x01, 0xf0, 
    0x1f, 0x87, 0xff, 0x78, 0x00, 0xf0, 0x01, 0xf8, 
    0x1f, 0x03, 0xff, 0xb8, 0x00, 0xf0, 0x00, 0xf8, 
    0x3f, 0x01, 0xff, 0xd8, 0x00, 0xf0, 0x00, 0xfc, 
    0x3e, 0x00, 0xff, 0xe8, 0x00, 0xf0, 0x00, 0x7c, 
    0x3e, 0x00, 0x7f, 0xf0, 0x00, 0xf0, 0x00, 0x7c, 
    0x7e, 0x00, 0x3f, 0xf8, 0x00, 0xf0, 0x00, 0x7e, 
    0x7c, 0x00, 0x1f, 0xfc, 0x00, 0xf0, 0x00, 0x3e, 
    0x7c, 0x00, 0x0f, 0xfe, 0x00, 0xf0, 0x00, 0x3e, 
    0x7c, 0x00, 0x07, 0xff, 0x00, 0xf0, 0x00, 0x3e, 
    0x7c, 0x00, 0x03, 0xff, 0x80, 0xf0, 0x00, 0x1f, 
    0xf8, 0x00, 0x01, 0xff, 0xc0, 0xf0, 0x00, 0x1f, 
    0xf8, 0x00, 0x00, 0xff, 0xe0, 0xf0, 0x00, 0x1f, 
    0xf8, 0x00, 0x00, 0x7f, 0xf1, 0xf0, 0x00, 0x1f, 
    0xf8, 0x00, 0x00, 0xbf, 0xfb, 0xe0, 0x00, 0x1f, 
    0xf8, 0x00, 0x00, 0xdf, 0xfd, 0xc0, 0x00, 0x1f, 
    0xf8, 0x00, 0x00, 0xef, 0xfe, 0x80, 0x00, 0x1f, 
    0xf8, 0x00, 0x00, 0xf7, 0xff, 0x00, 0x00, 0x1f, 
    0xf8, 0x00, 0x00, 0xfb, 0xff, 0x80, 0x00, 0x1f, 
    0x78, 0x00, 0x00, 0xf9, 0xff, 0xc0, 0x00, 0x1f, 
    0x7c, 0x00, 0x00, 0xf8, 0xff, 0xe0, 0x00, 0x3e, 
    0x7c, 0x00, 0x00, 0xf8, 0x7f, 0xf0, 0x00, 0x3e, 
    0x7c, 0x00, 0x00, 0xf8, 0x3f, 0xf8, 0x00, 0x3e, 
    0x7e, 0x00, 0x00, 0xf8, 0x1f, 0xfc, 0x00, 0x3e, 
    0x3e, 0x00, 0x00, 0xf8, 0x0f, 0xfe, 0x00, 0x7c, 
    0x3e, 0x00, 0x00, 0xf8, 0x07, 0xff, 0x00, 0x7c, 
    0x3f, 0x00, 0x00, 0xf8, 0x03, 0xff, 0x80, 0xfc, 
    0x1f, 0x00, 0x00, 0xf8, 0x01, 0xff, 0xc0, 0xfc, 
    0x1f, 0x80, 0x00, 0xf8, 0x00, 0xff, 0xe1, 0xf8, 
    0x0f, 0x80, 0x00, 0xf8, 0x00, 0x7f, 0xf1, 0xf0, 
    0x0f, 0xc0, 0x00, 0xf8, 0x00, 0x3f, 0xfb, 0xf0, 
    0x07, 0xe0, 0x00, 0xf8, 0x00, 0x1f, 0xff, 0xe0, 
    0x07, 0xf0, 0x00, 0xf8, 0x00, 0x0f, 0xff, 0xe0, 
    0x03, 0xf0, 0x00, 0xf8, 0x00, 0x07, 0xff, 0xc0, 
    0x01, 0xf8, 0x00, 0xf8, 0x00, 0x03, 0xff, 0x80, 
    0x00, 0xfe, 0x00, 0xf8, 0x00, 0x01, 0xff, 0x80, 
    0x00, 0xff, 0x00, 0xf8, 0x00, 0x00, 0xff, 0x00, 
    0x00, 0x7f, 0x80, 0x70, 0x00, 0x01, 0xff, 0x00, 
    0x00, 0x3f, 0xe0, 0x00, 0x00, 0x07, 0xfe, 0x00, 
    0x00, 0x0f, 0xf8, 0x00, 0x00, 0x1f, 0xf8, 0x00, 
    0x00, 0x07, 0xff, 0x00, 0x00, 0x7f, 0xe0, 0x00, 
    0x00, 0x03, 0xff, 0xe0, 0x07, 0xff, 0xc0, 0x00, 
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 
    0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 
    0x00, 0x00, 0x0f, 0xff, 0xff, 0xf8, 0x00, 0x00, 
    0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00 };

/******** ROAD CLOSED HEX CODE ************/
static const unsigned char PROGMEM roadclosed[] =   //hex code for road closed image
{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 
    0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 
    0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 
    0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 
    0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 
    0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 
    0xf7, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
    0xf3, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xdf, 
    0xf1, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0x9f, 
    0xf0, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0x1f, 
    0xf0, 0x7f, 0xf0, 0x00, 0x00, 0x0f, 0xfe, 0x1f, 
    0xf0, 0x3f, 0xf8, 0x00, 0x00, 0x1f, 0xfc, 0x1f, 
    0xf0, 0x1f, 0xfc, 0x00, 0x00, 0x3f, 0xf8, 0x1f, 
    0xf0, 0x0f, 0xfe, 0x00, 0x00, 0x7f, 0xf0, 0x1f, 
    0xf0, 0x07, 0xff, 0x00, 0x00, 0xff, 0xe0, 0x1f, 
    0xf0, 0x03, 0xff, 0x80, 0x01, 0xff, 0xc0, 0x1f, 
    0xf0, 0x01, 0xff, 0xc0, 0x03, 0xff, 0x80, 0x1f, 
    0xf0, 0x00, 0xff, 0xe0, 0x07, 0xff, 0x00, 0x1f, 
    0xf0, 0x00, 0x7f, 0xf0, 0x0f, 0xfe, 0x00, 0x1f, 
    0xf0, 0x00, 0x3f, 0xf8, 0x1f, 0xfc, 0x00, 0x1f, 
    0xf0, 0x00, 0x1f, 0xfc, 0x3f, 0xf8, 0x00, 0x1f, 
    0xf0, 0x00, 0x0f, 0xfe, 0x7f, 0xf0, 0x00, 0x1f, 
    0xf0, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x1f, 
    0xf0, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x1f, 
    0xf0, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x1f, 
    0xf0, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x1f, 
    0xf0, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x1f, 
    0xf0, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x1f, 
    0xf0, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x1f, 
    0xf0, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x1f, 
    0xf0, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x1f, 
    0xf0, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x1f, 
    0xf0, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x1f, 
    0xf0, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x1f, 
    0xf0, 0x00, 0x0f, 0xfe, 0x7f, 0xf0, 0x00, 0x1f, 
    0xf0, 0x00, 0x1f, 0xfc, 0x3f, 0xf8, 0x00, 0x1f, 
    0xf0, 0x00, 0x3f, 0xf8, 0x1f, 0xfc, 0x00, 0x1f, 
    0xf0, 0x00, 0x7f, 0xf0, 0x0f, 0xfe, 0x00, 0x1f, 
    0xf0, 0x00, 0xff, 0xe0, 0x07, 0xff, 0x00, 0x1f, 
    0xf0, 0x01, 0xff, 0xc0, 0x03, 0xff, 0x80, 0x1f, 
    0xf0, 0x03, 0xff, 0x80, 0x01, 0xff, 0xc0, 0x1f, 
    0xf0, 0x07, 0xff, 0x00, 0x00, 0xff, 0xe0, 0x1f, 
    0xf0, 0x0f, 0xfe, 0x00, 0x00, 0x7f, 0xf0, 0x1f, 
    0xf0, 0x1f, 0xfc, 0x00, 0x00, 0x3f, 0xf8, 0x1f, 
    0xf0, 0x3f, 0xf8, 0x00, 0x00, 0x1f, 0xfc, 0x1f, 
    0xf0, 0x7f, 0xf0, 0x00, 0x00, 0x0f, 0xfe, 0x1f, 
    0xf0, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0x1f, 
    0xf1, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0x9f, 
    0xf3, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xdf, 
    0xf7, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
    0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 
    0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 
    0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 
    0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

/******** DIRECTION HEX CODE ************/
static const unsigned char PROGMEM directions[] =   //hex code for direction image
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 
    0x00, 0x0f, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/******** ERROR HEX CODE ************/
static const unsigned char PROGMEM errors[] =       //hex code for error image
{ 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 
    0x00, 0x00, 0x0f, 0xfc, 0x7f, 0xf0, 0x00, 0x00, 
    0x00, 0x00, 0x3f, 0xf8, 0x3f, 0xfc, 0x00, 0x00, 
    0x00, 0x00, 0xff, 0xf0, 0x1f, 0xff, 0x00, 0x00, 
    0x00, 0x03, 0xff, 0xf0, 0x1f, 0xff, 0xc0, 0x00, 
    0x00, 0x07, 0xff, 0xe0, 0x0f, 0xff, 0xe0, 0x00, 
    0x00, 0x0f, 0xff, 0xe0, 0x0f, 0xff, 0xf0, 0x00, 
    0x00, 0x1f, 0xff, 0xe0, 0x0f, 0xff, 0xfc, 0x00, 
    0x00, 0x7f, 0xff, 0xe0, 0x0f, 0xff, 0xfe, 0x00, 
    0x00, 0x7f, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0x00, 
    0x00, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0x00, 
    0x01, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0x80, 
    0x03, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xc0, 
    0x07, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xe0, 
    0x07, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xe0, 
    0x0f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xf0, 
    0x0f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xf0, 
    0x1f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xf8, 
    0x1f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xf8, 
    0x3f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xfc, 
    0x3f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xfc, 
    0x3f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xfc, 
    0x7f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xfe, 
    0x7f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xfe, 
    0x7f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xfe, 
    0x7f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xfe, 
    0x7f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 
    0x7f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 
    0x7f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xfe, 
    0x7f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xfe, 
    0x7f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xfe, 
    0x7f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xfe, 
    0x3f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xfc, 
    0x3f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xfc, 
    0x3f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xfc, 
    0x1f, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xfc, 
    0x1f, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xf8, 
    0x0f, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xf0, 
    0x0f, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0xf0, 
    0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 
    0x07, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xe0, 
    0x03, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xc0, 
    0x01, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0x80, 
    0x00, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0x00, 
    0x00, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0x00, 
    0x00, 0x7f, 0xff, 0xe0, 0x0f, 0xff, 0xfe, 0x00, 
    0x00, 0x3f, 0xff, 0xf0, 0x1f, 0xff, 0xfc, 0x00, 
    0x00, 0x0f, 0xff, 0xf0, 0x1f, 0xff, 0xf0, 0x00, 
    0x00, 0x07, 0xff, 0xf8, 0x3f, 0xff, 0xe0, 0x00, 
    0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 
    0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 
    0x00, 0x00, 0x0f, 0xff, 0xff, 0xf8, 0x00, 0x00, 
    0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00 };

/******** WARNING HEX CODE ************/
static const unsigned char PROGMEM warning[] =      //hex code for warning image
{ 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x3e, 0x7c, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x7e, 0x7e, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x7c, 0x3e, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xfc, 0x3f, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x01, 0xf9, 0x9f, 0x80, 0x00, 0x00, 
    0x00, 0x00, 0x01, 0xf3, 0xcf, 0x80, 0x00, 0x00, 
    0x00, 0x00, 0x03, 0xf7, 0xef, 0xc0, 0x00, 0x00, 
    0x00, 0x00, 0x03, 0xe7, 0xe7, 0xc0, 0x00, 0x00, 
    0x00, 0x00, 0x07, 0xe7, 0xe7, 0xe0, 0x00, 0x00, 
    0x00, 0x00, 0x07, 0xc7, 0xe3, 0xe0, 0x00, 0x00, 
    0x00, 0x00, 0x0f, 0xc7, 0xe3, 0xf0, 0x00, 0x00, 
    0x00, 0x00, 0x0f, 0x87, 0xe1, 0xf0, 0x00, 0x00, 
    0x00, 0x00, 0x1f, 0x87, 0xe1, 0xf8, 0x00, 0x00, 
    0x00, 0x00, 0x1f, 0x07, 0xe0, 0xf8, 0x00, 0x00, 
    0x00, 0x00, 0x3f, 0x07, 0xe0, 0xfc, 0x00, 0x00, 
    0x00, 0x00, 0x3e, 0x07, 0xe0, 0x7c, 0x00, 0x00, 
    0x00, 0x00, 0x7e, 0x07, 0xe0, 0x7e, 0x00, 0x00, 
    0x00, 0x00, 0x7c, 0x07, 0xe0, 0x3e, 0x00, 0x00, 
    0x00, 0x00, 0xfc, 0x07, 0xe0, 0x3f, 0x00, 0x00, 
    0x00, 0x00, 0xf8, 0x07, 0xe0, 0x1f, 0x00, 0x00, 
    0x00, 0x01, 0xf8, 0x07, 0xe0, 0x1f, 0x80, 0x00, 
    0x00, 0x01, 0xf0, 0x07, 0xe0, 0x0f, 0x80, 0x00, 
    0x00, 0x03, 0xf0, 0x07, 0xe0, 0x0f, 0xc0, 0x00, 
    0x00, 0x03, 0xe0, 0x07, 0xe0, 0x07, 0xc0, 0x00, 
    0x00, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x00, 
    0x00, 0x07, 0xc0, 0x07, 0xe0, 0x03, 0xe0, 0x00, 
    0x00, 0x0f, 0xc0, 0x07, 0xe0, 0x03, 0xf0, 0x00, 
    0x00, 0x0f, 0x80, 0x07, 0xe0, 0x01, 0xf0, 0x00, 
    0x00, 0x1f, 0x80, 0x07, 0xe0, 0x01, 0xf8, 0x00, 
    0x00, 0x1f, 0x00, 0x07, 0xe0, 0x00, 0xf8, 0x00, 
    0x00, 0x3f, 0x00, 0x07, 0xe0, 0x00, 0xfc, 0x00, 
    0x00, 0x3e, 0x00, 0x07, 0xe0, 0x00, 0x7c, 0x00, 
    0x00, 0x7e, 0x00, 0x07, 0xe0, 0x00, 0x7e, 0x00, 
    0x00, 0x7c, 0x00, 0x07, 0xe0, 0x00, 0x3e, 0x00, 
    0x00, 0xfc, 0x00, 0x07, 0xe0, 0x00, 0x3f, 0x00, 
    0x00, 0xf8, 0x00, 0x07, 0xe0, 0x00, 0x1f, 0x00, 
    0x01, 0xf8, 0x00, 0x03, 0xc0, 0x00, 0x1f, 0x80, 
    0x01, 0xf0, 0x00, 0x01, 0x80, 0x00, 0x0f, 0x80, 
    0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 
    0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 
    0x07, 0xe0, 0x00, 0x03, 0xc0, 0x00, 0x07, 0xe0, 
    0x07, 0xc0, 0x00, 0x07, 0xe0, 0x00, 0x03, 0xe0, 
    0x0f, 0xc0, 0x00, 0x07, 0xe0, 0x00, 0x03, 0xf0, 
    0x0f, 0x80, 0x00, 0x07, 0xe0, 0x00, 0x01, 0xf0, 
    0x1f, 0x80, 0x00, 0x07, 0xe0, 0x00, 0x01, 0xf8, 
    0x1f, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0xf8, 
    0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 
    0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 
    0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 
    0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe };

////////////////////////////////  SETUP ////////////////////////////////////
/**************************************************************************/
void setup() {                //setup of the progtam
  /******** WIFI POWER ************/
  WiFi.setOutputPower(20.5);     //setting wifi power to minimum

  /******** FASTLED LAMPS ************/
  FastLED.addLeds<NEOPIXEL,LED_Pin>(leds, NUM_LEDS); //initializing FastLED for lamps

  /******** SERIAL MONITOR ************/
  Serial.begin(115200);       //Serial monitor intialization

  /******** IR SENSOR ************/
  pinMode(IRPin1, INPUT);     //defining pinmode of IR sensor1
  pinMode(IRPin2, INPUT);     //defining pinmode of IR sensor2

  /******** LAMPS ************/
  panelHSV(H, S, Def, 0);     //initializing lamp1 panelHSV(H-value, S-value, v-value, panel-number (0 - n-1))
  panelHSV(H, S, Def, 1);     //initializing lamp2
  panelHSV(H, S, Def, 2);     //initializing lamp3

  /******** BATTERY VOLTAGE ************/
  pinMode(analogInput, INPUT); //initializing analog input pin

  /******** RESET WATCHDOG ************/
  yield();                    //reset watchdog timer

  /******** BLYNK ************/
  Blynk.begin(auth, ssid, pass);  //Start connection to Blynk servers

  /******** PRINT DEVICE ID ************/
  Serial.print("device id : "); //for debugging
  Serial.println(id);

  /******** AHT10 ************/
  if (! aht.begin()) {        //if sensor not found disable it automatically
    Serial.println("Could not find AHT10? Check wiring");
    AHT = DISABLED;
  }

  /******** SSD1306 ************/
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {  //if OLED display not found then 
    Serial.println("SSD1306 allocation failed");
    OLED = DISABLED;
  }

  /******** FASTLED STUDS ************/
  FastLED.addLeds<WS2812B, STUD_PIN, GRB>(studs, NUM_STUDS);  //initializing fast led driver for studs

  /******** BUZZER ************/
  pinMode(buzzer, OUTPUT);    //buzzer output pin

  /******** SWITCH ************/
  pinMode(swPin, OUTPUT);     //switch and stud led pin

  /******** GOOGLE MAPS LOCATION ************/
  String tempvalue = "https://www.google.co.in/maps?q=" + String(lat, 4) + "+" + String(lon, 4);  //string to show google map link of the device
  Blynk.virtualWrite(V15, tempvalue); //updating value of text field in app
}

////////////////////////////////  FUNCTIONS ////////////////////////////////////
/******************************************************************************/
/******** DISPLAY NO PARKING ************/
void noParking(int i){        //for displaying signs on OLED (No parking)
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20, 20);
  display.println("NO");
  display.setTextSize(1);
  display.setCursor(10, 40);
  display.println("PARKING!");
  display.drawBitmap(64, 0, noparking, 64, 64, SSD1306_WHITE);
  if(i%2==0)                  //for inverting display
  {
    display.invertDisplay(true);
    display.display();
  }
  else{                       //inverting display to normal color
    display.invertDisplay(false);
    display.display();
  }
}

/******** DISPLAY ROAD CLOSED ************/
void roadClosed(int i){       //to display road closed in OLED display
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 20);
  display.println("ROAD");
  display.setTextSize(1);
  display.setCursor(15, 40);
  display.println("CLOSED!");
  display.drawBitmap(64, 0, roadclosed, 64, 64, SSD1306_WHITE);
  if(i%2==0)
  {
    display.invertDisplay(true);
    display.display();
  }
  else{
    display.invertDisplay(false);
    display.display();
  }
}

/******** DISPLAY DIRECTION ************/
void dir(int i){                //to display direction in OLED display
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 20);
  display.println("TURN");
  display.setTextSize(1);
  display.setCursor(18, 40);
  display.println("AHEAD!");
  display.drawBitmap(64, 0, directions, 64, 64, SSD1306_WHITE);
  if(i%2==0)
  {
    display.invertDisplay(true);
    display.display();
  }
  else{
    display.invertDisplay(false);
    display.display();
  }
}

/******** DISPLAY ERROR ************/
void err(int i){                //to display error message in OLED display
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(3, 20);
  display.println("CHECK");
  display.setTextSize(1);
  display.setCursor(10, 40);
  display.println("BATTERY!");
  display.drawBitmap(64, 0, errors, 64, 64, SSD1306_WHITE);
  if(i%2==0)
  {
    display.invertDisplay(true);
    display.display();
  }
  else{
    display.invertDisplay(false);
    display.display();
  }
}

/******** DISPLAY WARNING ************/
void warn(int i){               //to display warning in OLED display
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(15, 20);
  display.println("LOW");
  display.setTextSize(1);
  display.setCursor(10, 40);
  display.println("BATTERY!");
  display.drawBitmap(64, 0, warning, 64, 64, SSD1306_WHITE);
  if(i%2==0)
  {
    display.invertDisplay(true);
    display.display();
  }
  else{
    display.invertDisplay(false);
    display.display();
  }
}

/******** OLED DISPLAY SIGNS ************/
void OLEDdisp(int i){             //to display signs in OLED display depending on the app
  if(OLED){
    if(dispst<1)                  //resetting dispplay state 
    {
      dispst = 2;
    }
    switch(i){
      case 1: display.clearDisplay();
              display.invertDisplay(false);
              display.display();
      break;
      case 2: noParking(dispst);
      break;
      case 3: roadClosed(dispst);
      break;
      case 4: dir(dispst);
      break;
      case 5: err(dispst);
      break;
      case 6: warn(dispst);
    }
    dispst--;                     //decrement the display state for alternating the display color inversion
  }
}

/******** LIGHTING LAMPS (PANELS) ************/
void panelHSV(int h, int s, int v, int no)  //function to change each lamp's characteristics
{
  int i = 4*no;
  leds[i+0] = CHSV(h, s, v);    //setting HSV value for each led in the lamp
  leds[i+1] = CHSV(h, s, v);
  leds[i+2] = CHSV(h, s, v);
  leds[i+3] = CHSV(h, s, v);
  currBri = v;
  FastLED.show();               //called to update the current values to the led(s)
}

/******** FADE ON (LOW BRIGHTNESS --> HIGH BRIGHTNESS) ************/
void fadeOn(int low, int high, int h, int s, int v, int rot){ //function for animation when the lamps are turning on
  Blynk.run();
  float c = (fabs(high-low)/3); //c is to find the 3 equal parts of the difference of the low to high value
  int lim = high + (c*2);       //lim value till the loop will run
  //Serial.print(c);            //for debugging purposes
  //Serial.print("    ");
  //Serial.println(lim);
  if(rot>=0)                    //if rotation is set to a +ve value then this will run
  {
    for(int i=low;i<lim;i+=2)   //loop from low to lim and incremented by 2
    {
      if(i >= low && i <= high){  //fade for lamp1
        panelHSV(h, s, i, 0);
      }
      if(i >= low+c && i <= high+c){  //fade for lamp2
        panelHSV(h, s, i-c, 1);
      }
      if(i >= low+c+c && i <= high+c+c){  //fade for lamp3
        panelHSV(h, s, i-c-c, 2);
      }
    }
  }
  else{                         //if roation is set to -ve value then this will run
    for(int i=low;i<lim;i+=2)   //loop from low to lim and incremented by 2
    {
      if(i >= low && i <= high){  //fade for lamp3
        panelHSV(h, s, i, 2);
      }
      if(i >= low+c && i <= high+c){  //fade for lamp2
        panelHSV(h, s, i-c, 1);
      }
      if(i >= low+c+c && i <= high+c+c){  //fade for lamp1
        panelHSV(h, s, i-c-c, 0);
      }                           //effects of turning on will be   low     lim
    }                             //                       lamp1      |---|  |
  }                               //                       lamp2      ||---| |
                                 //                       lamp3      | |---||
  panelHSV(h, s, high, 0);
  panelHSV(h, s, high, 1);
  panelHSV(h, s, high, 2);
}

/******** FADE OFF (HIGH BRIGHTNESS --> LOW BRIGHTNESS) ************/
void fadeOff(int high, int low, int h, int s, int v, int rot){ //function for fade off effect
  Blynk.run();
  float c = (fabs(high-low)/3);
  int lim = high + (c*2);
  //Serial.print(c);
  //Serial.print("    ");
  //Serial.println(lim);
  if(rot>=0)
  {
    for(int i=lim-1;i>=low;i-=2)  //for loop from lim to low as brightness will decrease and its decremented by 2
    {
      if(i >= low && i <= high){
        panelHSV(h, s, i, 0);
      }
      if(i >= low+c && i <= high+c){
        panelHSV(h, s, i-c, 1);
      }
      if(i >= low+c+c && i <= high+c+c){
        panelHSV(h, s, i-c-c, 2);
      }
    }
  }
  else{
    for(int i=lim-1;i>=low;i-=2)
    {
      if(i >= low && i <= high){
        panelHSV(h, s, i, 2);
      }
      if(i >= low+c && i <= high+c){
        panelHSV(h, s, i-c, 1);
      }
      if(i >= low+c+c && i <= high+c+c){
        panelHSV(h, s, i-c-c, 0);
      }
    }
  }
  panelHSV(h, s, low, 0);
  panelHSV(h, s, low, 1);
  panelHSV(h, s, low, 2);
}

/******** MANAGE TURNING ON AND OFF OF LAMPS (DEFAULT BRIGHTNESS --> MAX BRIGHTNESS) ************/
void lamps(){                   //lamps function will get called in every iteration of void loop
  Blynk.run();
    if(timer <= OnTime)         //if timer value is less than or equal to 300 then yhis will run
    {
      if(digitalRead(IRPin1) == 1 && prevst == 0) //checks if IR sensor detects an object and also prevous state of the sensor is false then flips state to 1
      {
        state = 1;
      }
      else if(digitalRead(IRPin2) == 1 && prevst == 0)
      {
        state = -1;
      }
      else{
        state = 0;
        timer += TimerInc;      //timer incremented if abouve statements are not true
      }
      if(digitalRead(IRPin1) || digitalRead(IRPin2))  //if any object detected after the lamps fades on then ill reset the timer
      {
        timer = 0;
        prevst = 1;
      }
      if(state != 0)            //if state is not zero then it will call fadeOn and save the state in tempstate and reset the timer
      {
        fadeOn(Def, Max, H, S, V, state);
        tempState = state;
        timer = 0;
      }
      else if(timer == OnTime && tempState != 0 && digitalRead(IRPin1) == 0 && digitalRead(IRPin2) == 0)  //if timer is equal to onTime then turn on time is exhausted and the lamp will fade off
      {
        fadeOff(Max, Def, H, S, V, tempState * -1);
        tempState = 0;
        prevst = 0;
      }
    }
    else{                       //if timer variable gets greater than onTime then ill get reset to 0 anyways
      timer = 0;
    }
}

/******** BATTERY LEVEL CALCULATION ************/
 void BATLevel(){               //BATLevel is for battery management and calculating the battery level and battery voltage
  Blynk.run();
   int value = analogRead(analogInput); //read analog value from voltage devider
   float vout = 0.0;            //vout is the range of value to 0.0 to 3.3v
   
   vout = (value * 1) / 1024.0; //conver6ting to range
   vin = vout / (R2/(R1+R2));   //calculating vin (actual voltage of battery with some abnormalities)
   vin-=0.1705;                 //excluding abnormalities
   if(vin<0)                    //if voltage recorded is negetive make it zero
   {
    vin=0.00;
   }
   //Serial.print(vin,4);         //for debugging purpose
   //Serial.print("\t");
   if(vin <= 4.3 && vin > 4.06){  //for the calculation of battery percentage
    BATlvl = 100;               //Battery level is 100%
   }
   else if(vin <= 4.06 && vin > 3.98){
    BATlvl = 90;                //Battery level is 90%
   }
   else if(vin <= 3.98 && vin > 3.92){
    BATlvl = 80;                //Battery level is 80%
   }
   else if(vin <= 3.92 && vin > 3.87){
    BATlvl = 70;                //Battery level is 70%
   }
   else if(vin <= 3.87 && vin > 3.82){
    BATlvl = 60;                //Battery level is 60%
   }
   else if(vin <= 3.82 && vin > 3.79){
    BATlvl = 50;                //Battery level is 50%
   }
   else if(vin <= 3.79 && vin > 3.77){
    BATlvl = 40;                //Battery level is 40%
   }
   else if(vin <= 3.77 && vin > 3.74){
    BATlvl = 30;                //Battery level is 30%
   }
   else if(vin <= 3.74 && vin > 3.68){
    BATlvl = 20;                //Battery level is 20%
   }
   else if(vin <= 3.68 && vin > 3.45){
    BATlvl = 10;                //Battery level is 10%
   }
   else if(vin <= 3.45 && vin > 3.00){
    BATlvl = 5;                 //Battery level is 05%
   }
   else{                        //if no condition satisfies then set error code
    BATlvl = 0;                 //Battery error
   }
   if(BATlvl <= lowBAT && st == 0)  //if battery level is less than 10% then turn on power saving mode
   {
    Def = lowBatDefBri;         //set def to low battery default brightness
    Max = lowBatMaxBri;         //set max to low battery max brightness
    //Serial.println("low : ");
    //Serial.println(currBri);
    if(currBri == DefBri)
    {
      fadeOff(currBri, lowBatDefBri, H, S, V, 1);  //fadeoff to match current brightness and low battery brightness
    }
    else if(currBri == MaxBri)
    {
      fadeOff(currBri, lowBatMaxBri, H, S, V, 1);
    }
    st = 1;                     //change the state so that next time only else if gets executed
   }
   else if(BATlvl > lowBAT+10 && st == 1){ //if battery charges then come out of power saving mode
    Def = DefBri;               //set default brightness
    Max = MaxBri;
    //Serial.print("high : ");
    //Serial.println(currBri);
    if(currBri == lowBatDefBri)
    {
      //Serial.println("curr-def");
      fadeOn(currBri, DefBri, H, S, V, 1); //fade on to match normal battery charge state
    }
    else if(currBri == lowBatMaxBri){
      //Serial.println("curr-max");
      fadeOn(currBri, MaxBri, H, S, V, 1);
    }
    st = 0;                     //reset state
   }
   //Serial.println(BATlvl);      //debugging purpose
   Blynk.virtualWrite(V0, vin); //sending value to BLYNK servers
  Blynk.virtualWrite(V1, BATlvl); //sending value to BLYNK servers
 }

/******** GET DATA OF V10 PIN ************/
BLYNK_WRITE(V10){               //getting value of V10 pin
  toggle = param.asInt();
}

/******** GET DATA OF V11 PIN ************/
BLYNK_WRITE(V11){               //getting value of V11 pin
  toggle = param.asInt();
}

/******** GET DATA OF V4 PIN ************/
BLYNK_WRITE(V4){                //getting value of V4 pin
  hum = param.asFloat();
}

/******** GET DATA OF V5 PIN ************/
BLYNK_WRITE(V5){                //getting value of V5 pin
  temp = param.asFloat();
}

/******** GET DATA OF V12 PIN ************/
BLYNK_WRITE(V12){                //getting value of V12 pin
  sign = param.asFloat();
}

/******** GET DATA OF V13 PIN ************/
BLYNK_WRITE(V13){                //getting value of V13 pin
  arr = param.asFloat();
}

/******** TURN ON / OFF LAMPS ************/
void lampsOnOff(){              //for toggling on and off the lamp
  if(toggle && togSt)           //if toggle and status is ture then fade on
  {
    Blynk.virtualWrite(V11, 1);
    //Serial.println("on");
    FastLED.setBrightness(255);
    fadeOn(0, currBri, H, S, V, 1);
    state = 1;
    lamps();
    togSt = 0;                  //toogle state changed so that above does not run repeteadly
  }
  else if(!toggle && !togSt)    //if toggle and toogle state is false then fade off
  {
    Blynk.virtualWrite(V11, 0);
    //Serial.println("off");
    byte tmpbri = currBri;
    fadeOff(currBri, 0, H, S, V, 1);
    currBri = tmpbri;
    //FastLED.setBrightness(0);
    togSt = 1;
  }
 }

/******** UPDATE TEMPERATURE AND HUMIDITY VALUES ************/
void updateAHT(){               //update AHT10 for temperature and humidity
  if(AHT){                      //if AHT is enabled then read values otherwise return null
    sensors_event_t humidity, temp; //objects of sensor_event_t
    aht.getEvent(&humidity, &temp); //getting event values in humidity and temp
    Blynk.virtualWrite(V5, temp.temperature); //member of temp
    Blynk.virtualWrite(V4, humidity.relative_humidity); //member of humidity
  }
  return;
}

/******** SENDING MAP POINTER AND DATA ************/
void Map(){                     //Map function for placing points in Map widget
  String val;                   //String for storing all the different values
  if(m > 9)                     //restting m
  {
    m = 0;
  }
  if(m == 0){                   //if m is 0 then following will run
    val = "Dev ID : " + String(id);     //device id
    myMap.location(id, lat, lon, val);  //update map pointer value
    //Serial.println(val);
  }
  else if(AHT && m == 1)             //post AHT value to map if AHT is enabled
  {
    Blynk.syncVirtual(V4, V5, V19); //updating pin values
    val = "T : " + String(temp) + " °C || H : " + String(hum) + " %rh"; //val contains value shown on map pointer
    myMap.location(id, lat, lon, val);  //posting map pointer to map widget
    //Serial.println(val);        //Debugging purpose
  }
  else if(toggle && m == 2)     //if lamps are on post battery value
  {
    val = "Batt Lvl : " + String(BATlvl) + " % || Batt Stat : ";  //val for storing battery values
    if(BATlvl <= lowBAT)
    val += "Check Battery!";    //posting battery error
    else if(BATlvl > lowBAT+10)
    val += "Normal!";           //battery normal
    myMap.location(id, lat, lon, val);
    //Serial.println(val);
  }
  else if(m == 3)               //if m is 2 then send lamp on-off status
  {
    val = "Status : ";
    if(toggle)
    val += "ON";                //lamp is turned on
    else
    val += "OFF";               //lamp is turned off
    myMap.location(id, lat, lon, val);
    //Serial.println(val);
  }
  else{
    val = "Dev ID : " + String(id);
    myMap.location(id, lat, lon, val);
    //Serial.println(val);
    m = 9;                      //end the conditions ans resetting m to exit condition
  }
  m++;                          //increment m
}

/******** APPLY SAME COLOR TO EVERY STUD LED ************/
void road(int h, int s, int v){ //road for applying every stud led with same color
  for(int i=0;i<9;i++)
  {
    studs[i] = CHSV(h, s, v);
  }
  FastLED.show();
}

/******** SIREN AND CHASER LED ************/
void siren(){                   //siren for producing chaser sound and led blinking
  pinMode(swPin, OUTPUT);       //changing pinmode to output from input
  bool tempTogg = toggle;       //temp toogle state
  road(STUD_RED, 255, MaxBri);     //caser red flash
  Blynk.virtualWrite(V11, HIGH);
  for(int i=1800;i<2000;i+=10)  //varing tone
  {
    tone(buzzer, i);            //tone(buzzer pin, frequency of tone)
    delay(5);
  }
  road(0, 0, 0);                //turn of stud led
  Blynk.virtualWrite(V11, LOW);
  noTone(buzzer);               //noTone is called to stop sound
  delay(100); 
  road(STUD_RED, 255, MaxBri);
  for(int i=1800;i<2000;i+=10)
  {
    tone(buzzer, i);
    delay(5);
  }
  road(0, 0, 0);
  Blynk.virtualWrite(V11, HIGH);
  noTone(buzzer);
  delay(1000);
  road(STUD_BLUE, 255, MaxBri);
  for(int i=300;i<2000;i+=2)
  {
    tone(buzzer, i);
    delay(2);
  }
  road(STUD_RED, 255, MaxBri);
  Blynk.virtualWrite(V11, LOW);
  for(int i=2000;i>300;i-=2)
  {
    tone(buzzer, i);
    delay(2);
  }
  road(STUD_BLUE, 255, MaxBri);
  do{                           //this will run until the siren is turned off from the app
  String tempvalue = "https://www.google.co.in/maps?q=" + String(lat, 4) + "+" + String(lon, 4);
  Blynk.virtualWrite(V15, tempvalue);
  Blynk.syncVirtual(V13);
  for(int j=0;j<20;j++)
  {
    if(midst)                   //if midst is 1 then red color otherwise blue
    {
      road(STUD_RED, 255, MaxBri);
      midst = 0;
      Blynk.virtualWrite(V11, HIGH);
    }
    else{
      road(STUD_BLUE, 255, MaxBri);
      midst = 1;
      Blynk.virtualWrite(V11, LOW);
    }
    for(int i=300;i<2000;i+=10)
    {
      tone(buzzer, i);
      delay(2);
    }  
  }
  for(int j=0;j<50;j++)         //different chaser sound
  {
    if(midst)
    {
      road(STUD_RED, 255, MaxBri);
      midst = 0;
      if(j%5 == 0)
      Blynk.virtualWrite(V11, HIGH);
    }
    else{
      road(STUD_BLUE, 255, MaxBri);
      midst = 1;
      if(j%5 == 0)
      Blynk.virtualWrite(V11, LOW);
    }
  for(int i=300;i<2000;i+=50)
  {
    tone(buzzer, i);
    delay(2);
  }
  }
  }while(arr);                  //do while will run until arr gets 0
  Blynk.virtualWrite(V11, tempTogg);
  noTone(buzzer);               //to stop the sound from buzzer
}

/******** DISPLAY HELP SEQUENCE ************/
void dispHELP(int i){
  if(OLED){
    switch(i){
    case -1: display.clearDisplay();
             display.setTextColor(WHITE);
             display.invertDisplay(false);
             display.setCursor(0,0);
             display.setTextSize(2);
             display.print("Press the button to cancel.");
    break;
    case 0: display.clearDisplay();
            display.setTextColor(WHITE);
            display.invertDisplay(false);
            display.setCursor(0,0);
            display.setTextSize(2);
            display.print("Continuing\nin ...\n\n");
    break;
    case 3: display.setTextSize(2);
            display.print("3 ");
    break;
    case 2: display.setTextSize(2);
            display.print("2 ");
    break;
    case 1: display.setTextSize(2);
            display.print("1 ");
    break;
    case 4: display.clearDisplay();
            display.setTextColor(WHITE);
            display.invertDisplay(false);
            display.setCursor(0,0);
            display.setTextSize(2);
            display.print("CANCELLED! ");
    break;
    case 5: display.clearDisplay();
            display.setTextColor(WHITE);
            display.invertDisplay(false);
            display.setCursor(0,0);
            display.setTextSize(2);
            display.print("HELP is \narriving \nstay \nhidden!");
    break;
    default: display.clearDisplay();
    }
    display.display();
  }
}

/******** SWITCH PRESS EVENT ************/
void swPress(){                 //swPress detects long switch pressed
  Serial.println("ACTIVE!");
  for(int i=0;i<30;i++)         //if user does not press the button for more than 3 sec then return void
  {
    if(digitalRead(swPin))
    {
      return;
    }
    delay(100);
  }
  for(int i=1800;i<2000;i+=10)  //varing tone
  {
    tone(buzzer, i);            //tone(buzzer pin, frequency of tone)
    delay(5);
  }
  noTone(buzzer);               //noTone is called to stop sound
  delay(100); 
  for(int i=1800;i<2000;i+=10)
  {
    tone(buzzer, i);
    delay(5);
  }
  noTone(buzzer);
  dispHELP(-1);
  Serial.println("You can press the button again to cancel. Continuing in ...");
  delay(3000);                  //after that user can cancel or continue
  dispHELP(0);
  for(int i=0;i<30;i++)
  {
    if(!digitalRead(swPin))     //if detects button press then return
    {
      Serial.println("Cancelled!");
      dispHELP(4);
      delay(2000);
      return;
    }
    else if(i == 10){
      Serial.print("3 ");
      dispHELP(3);
    }
    else if(i == 20){
      Serial.print("2 ");
      dispHELP(2);
    }
    else if(i == 29){
      Serial.println("1.");
      dispHELP(1);
      Serial.println("HELP!!");
      Blynk.virtualWrite(V13, HIGH);  //turn on help switch
      delay(500);
      dispHELP(5);
      siren();                  //calling siren
      dispHELP(6);
    }
    delay(100);
  }
  pinMode(swPin, INPUT_PULLUP); //changing pinmode so that switch can be operated
  delay(100);
}

/******** ROAD STUDS ANIMATION ************/
void roadStud(int j){           //roadStud for ligting the studs
  pinMode(swPin, OUTPUT);
  delay(50);
  if(ledno > 2)
  {
    ledno = 0;
  }
  if(midst){                    //if else is used to light up middle lane lights
    studs[3] = CHSV(STUD_COLOR, 255, currBri);
    studs[4] = CHSV(STUD_COLOR, 255, currBri);
    studs[5] = CHSV(STUD_COLOR, 255, currBri);
    FastLED.show();
    midst = 0;
  }
  else{
    studs[3] = CHSV(STUD_COLOR, 255, 0);
    studs[4] = CHSV(STUD_COLOR, 255, 0);
    studs[5] = CHSV(STUD_COLOR, 255, 0);
    FastLED.show();
    midst = 1;
  }
  switch(ledno){                //switch case for left and right lane lights
    case 0: studs[2] = CHSV(STUD_COLOR, 255, 0);
            studs[6] = CHSV(STUD_COLOR, 255, 0);
            FastLED.show();
            studs[0] = CHSV(STUD_COLOR, 255, currBri);
            studs[8] = CHSV(STUD_COLOR, 255, currBri);
            FastLED.show();
    break;
    case 1: studs[0] = CHSV(STUD_COLOR, 255, 0);
            studs[8] = CHSV(STUD_COLOR, 255, 0);
            FastLED.show();
            studs[1] = CHSV(STUD_COLOR, 255, currBri);
            studs[7] = CHSV(STUD_COLOR, 255, currBri);
            FastLED.show();
    break;
    case 2: studs[1] = CHSV(STUD_COLOR, 255, 0);
            studs[7] = CHSV(STUD_COLOR, 255, 0);
            FastLED.show();
            studs[2] = CHSV(STUD_COLOR, 255, currBri);
            studs[6] = CHSV(STUD_COLOR, 255, currBri);
            FastLED.show();
  }
  ledno++;
}

/******** RUN TASK SCHEDULER ************/
 void RUN(int j){               //RUN function will contain all functions which will be called perodically 
  /******** THIS WILL RUN EVEN WHEN LAMPS ARE TURNED OFF ************/
  //Serial.println(j);
  yield();
  if(j == 1)                      //this will run always and once per full turn of RUN and will run even after lamp is turned off
  {
    //Serial.println(currBri);
    Blynk.syncVirtual(V12);     //updating latest value of V12 pin
    Map();                      //calling map function
  }
  else if(j == 2)               //this will also run every loop even after lamp is turned off but will run after the next RUN(1) called so that code does not get laggy
  {
    updateAHT();                //calling updateAHT function
  }
  else if(j == 3)
  {
    Blynk.virtualWrite(V13, LOW);  //turn on help switch
    Serial.println("READY!");
    pinMode(swPin, INPUT_PULLUP); //changing pinmode to input for switch
    delay(10);                    //delay for pullup
    if(!digitalRead(swPin))       //if it detects button press then call swPress
    {
      swPress();
    }
  }

  /******** CHECKS FOR ON / OFF COMMAND ************/
  if(!toggle && !Tog)                   //if lamp is turned off
  {
    delay(100);
    //Serial.println("toggle off");
    pinMode(swPin, OUTPUT);
    delay(100);
    road(0,0,0);
    lampsOnOff();               //calling lampsOnOff
    for(int i=0;i<NUM_LEDS;i++)
    {
      leds[i] = CHSV(0, 0, 0);
    }
    FastLED.show();
    OLEDdisp(1);                //turn off display
    delay(100);
    pinMode(swPin, INPUT_PULLUP);
    delay(100);
    pinMode(LED_Pin, INPUT_PULLUP);
    delay(100);
    Tog = 1;
  }
  if(!toggle){
    return;                     //return void for RUN
  }

  /******** THIS WILL RUN IF ONLY THE LAMPS ARE ON ************/
  Tog = 0;
  pinMode(LED_Pin, OUTPUT);
  //delay(10);
  BATLevel();                   //BATLevel is called in every iteration of the loop
  lamps();                      //lamps is called every loop
  lampsOnOff();                 //calling lampsOnOff for truning on lamp again
  //delay(10);
  roadStud(j);                  //for running stud lights
  if(j == 0){                   //This will run once in every 10 loops of void loop
   if(BATlvl <= lowBAT && BATlvl > 0)
   {
      OLEDdisp(6);              //display warning on screen
   }
   else if(BATlvl < 1)               //If battery level is less than 1 then show error code
   {
    if(sign != 1){
      OLEDdisp(5);                //display error on screen
    }
    panelHSV(H, S, currBri, 0); //error code of two red flash (check battery)
    panelHSV(0, 255, currBri, 1);
    panelHSV(H, S, currBri, 2);
    delay(50);
    panelHSV(H, S, currBri, 0);
    panelHSV(H, S, currBri, 1);
    panelHSV(H, S, currBri, 2);
    delay(100);
    panelHSV(H, S, currBri, 0);
    panelHSV(0, 255, currBri, 1);
    panelHSV(H, S, currBri, 2);
    delay(50);
    panelHSV(H, S, currBri, 0);
    panelHSV(H, S, currBri, 1);
    panelHSV(H, S, currBri, 2);
    //delay(50);
   }
   else{                        //otherwise display road sign
    OLEDdisp(sign);
   }
  }
  if(j%3 == 0 && j > 0)         //update display every 3 consecutive run of RUN function
  {
    Serial.println(sign);
    OLEDdisp(sign);             //displaying road signs in the OLED
  }
 }

////////////////////////////////  LOOP  ////////////////////////////////////
/**************************************************************************/
int j=0;                        //looping variable 
void loop(){                    //loop of program
  RUN(j);                       //RUN will be called everytime and every process is called finite ammount of time inside it
  //delay(50);
  Blynk.run();
  j++;                          //incrementing function  variable
  if(j>9)                       //if j is more than 9 reset it to 0
  {
    j = 0;                      //resetting looping variable
  }
}
