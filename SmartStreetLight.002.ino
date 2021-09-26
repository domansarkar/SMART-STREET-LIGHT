/********SMART STREET LIGHT ver- 2.0
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
 *  2.      WS2812B LEDs                 12
 *  3.      ESP8266                      01
 *  4.      Voltage divider              01
 *  5.      Battery shield               01~02 (for 3 independent module)
 *  6.      Solar Panel                  01
 *  
 *    CONNECTIONS : -
 *    
 *    IR SENSOR 01
 * HW201 ------> ESP8266
 * VCC    -----     5v
 * GND    -----     GND
 * OUT    -----     D1(GPIO 05)
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
 * NOTE:
 * You can modify this project however you want :)
 * 
 */
#include <FastLED.h>          //Library for FastLED (supports WS2812B, WS2812, WS2811 etc.)
#define BLYNK_PRINT Serial    //Library for Blynk IoT
#include <ESP8266WiFi.h>      //Library for esp8266 wifi
#include <BlynkSimpleEsp8266.h> //contains simple and some necessary functions for Blynk IoT

#define LED_Pin 14            //Pin of the micro-controller connected to the led's data pin
#define NUM_LEDS 12           //total number of addressable led(s) used (this module will control 3 lamps and each lamp contains 4 led(s))

#define IRPin1 5              //Pin of micro-controller for IR sensor (to detect the presence of object)
#define IRPin2 16             //Pin of micro-controller for IRsensor for other side of the road

#define OnTime 100            //The time till ligts are turned on after detecting presence of object (seconds*100 for optimal results)
#define TimerInc 10           //The incrementor will add the value to timer every loop until it reches the OnTime value after it gets reset
#define Wait 100              //The delay time of each loop (miliseconds)

#define analogInput A0        //Analog Pin for Battery Voltage Detection

#define lowBAT 10             //low battery level to get into battery saving mode

char auth[] = "AUTH"; //Blynk Auth sent to your mail
char ssid[] = "SSID";         //SSID of wifi
char pass[] = "PASSWORD";  //Wifi password

float vin = 0.0;              //variable to store 
float R1 = 9770;              //R1 resistor in voltage devider
float R2 = 999;               //R2 resistor in voltage devider
byte BATlvl = 0;                //Battery level percentage
bool st = 0;                    //previous state of the battery less than 10% or not

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

CRGBArray<NUM_LEDS> leds;     //array of all led(s) connected to the micro-contoller

void setup() {                //setup of the progtam
  
  FastLED.addLeds<NEOPIXEL,LED_Pin>(leds, NUM_LEDS); //initializing FastLED
  
  Serial.begin(250000);       //Serial monitor intialization
  pinMode(IRPin1, INPUT);     //defining pinmode of IR sensor1
  pinMode(IRPin2, INPUT);     //defining pinmode of IR sensor2

  panelHSV(H, S, Def, 0);  //initializing lamp1 panelHSV(H-value, S-value, v-value, panel-number (0 - n-1))
  panelHSV(H, S, Def, 1);  //initializing lamp2
  panelHSV(H, S, Def, 2);  //initializing lamp3

  pinMode(analogInput, INPUT); //initializing analog input pin

  Blynk.begin(auth, ssid, pass);  //Start connection to Blynk servers
  
  }

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
      if(state != 0)            //if state is not zero then ill call fadeOn and save the state in tempstate and reset the timer
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
      delay(Wait);              //delay after each call
    }
    else{                       //if timer variable gets greater than onTime then ill get reset to 0 anyways
      timer = 0;
    }
}

 void BATLevel(){               //BATLevel is for battery management and calculating the battery level and battery voltage
  Blynk.run();
   int value = analogRead(analogInput); //read analog value from voltage devider
   float vout = 0.0;            //vout is the range of value to 0.0 to 3.3v
   
   vout = (value * 3.3) / 1022.0; //conver6ting to range
   vin = vout / (R2/(R1+R2));   //calculating vin (actual voltage of battery with some abnormalities)
   vin-=0.2295;                 //excluding abnormalities
   if(vin<0)                    //if voltage recorded is negetive make it zero
   {
    vin=0.00;
   }
   Serial.print(vin,4);         //for debugging purpose
   Serial.print("\t");
   if(vin <= 4.3 && vin > 4.06){  //for the calculation of battery percentage
    BATlvl = 100;
   }
   else if(vin <= 4.06 && vin > 3.98){
    BATlvl = 90;
   }
   else if(vin <= 3.98 && vin > 3.92){
    BATlvl = 80;
   }
   else if(vin <= 3.92 && vin > 3.87){
    BATlvl = 70;
   }
   else if(vin <= 3.87 && vin > 3.82){
    BATlvl = 60;
   }
   else if(vin <= 3.82 && vin > 3.79){
    BATlvl = 50;
   }
   else if(vin <= 3.79 && vin > 3.77){
    BATlvl = 40;
   }
   else if(vin <= 3.77 && vin > 3.74){
    BATlvl = 30;
   }
   else if(vin <= 3.74 && vin > 3.68){
    BATlvl = 20;
   }
   else if(vin <= 3.68 && vin > 3.45){
    BATlvl = 10;
   }
   else if(vin <= 3.45 && vin > 3.00){
    BATlvl = 5;
   }
   else{                        //if no condition satisfies then set error code
    BATlvl = 0;
   }
   if(BATlvl <= lowBAT && st == 0)  //if battery level is less than 10% then turn on power saving mode
   {
    Def = lowBatDefBri;         //set def to low battery default brightness
    Max = lowBatMaxBri;         //set max to low battery max brightness
    Serial.println("low : ");
    Serial.println(currBri);
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
    Serial.print("high : ");
    Serial.println(currBri);
    if(currBri == lowBatDefBri)
    {
      Serial.println("curr-def");
      fadeOn(currBri, DefBri, H, S, V, 1); //fade on to match normal battery charge state
    }
    else if(currBri == lowBatMaxBri){
      Serial.println("curr-max");
      fadeOn(currBri, MaxBri, H, S, V, 1);
    }
    st = 0;                     //reset state
   }
   Serial.println(BATlvl);      //debugging purpose
   Blynk.virtualWrite(V0, vin); //sending value to BLYNK servers
  Blynk.virtualWrite(V1, BATlvl); //sending value to BLYNK servers
 }

 void RUN(int j){
  BATLevel();                   //BATLevel is called in every iteration of the loop
  lamps();                      //lamps is called every loop
  Blynk.run();                  //for processing incoming command 
  if(j == 0){                   //This will run once in every 10 loops of void loop
   if(BATlvl < 1)               //If battery level is less than 1 then show error code
   {
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
   }
  }
 }  
 
int j=0;
void loop(){                    //loop of program
  RUN(j);                       //RUN will be called everytime and every process is called finite ammount of time inside it
  
  j++;                          //incrementing function  variable
  if(j>9)                      //if j is more than 9 reset it to 0
  {
    j = 0;
  }
}
