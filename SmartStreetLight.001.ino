/********SMART STREET LIGHT ver- 1.0
 * 
 * This is a open source project made by DOMAN SARKAR 
 * 
 * The SMART STREET LIGHT project is a advanced street light which consumes less power than traditional street light as it detects the presence of any pedestrian or vehicle on the road which 
 * triggers the lamps to turn on at full brightness and stays on for some time and dims automatically after some time. The motivation behind the project came from the fact that traditional 
 * halogen street lights consume far more power as comapred to the SMART STREET LIGHT and they dont dim and consume very high energy and gets damaed easily. In this time I saw many street 
 * lights just stay damaged for countless years because they have a very short lifespan as comared to LED lights so our project will last longer and consume far more less energy than traditional 
 * street lights.
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
 * NOTE:
 * You can modify this project however you want :)
 * 
 */
#include <FastLED.h>          //Library for FastLED (supports WS2812B, WS2812, WS2811 etc.)

#define LED_Pin 14            //Pin of the micro-controller connected to the led's data pin
#define NUM_LEDS 12           //total number of addressable led(s) used (this module will control 3 lamps and each lamp contains 4 led(s))
#define H 20                  //The H component of HSV color wheel
#define S 90                  //The S component of HSV color wheel
#define V 255                 //The V component of HSV color wheel
#define DefBri 90             //The default brightness of the of lamps (energy saving mode), ranges from 0 to 255(max)
#define MaxBri 255            //The maximum brightness of the lamps, ranges from 0 to 255(max)

#define IRPin1 5              //Pin of micro-controller for IR sensor (to detect the presence of object)
#define IRPin2 16             //Pin of micro-controller for IRsensor for other side of the road

#define OnTime 500            //The time till ligts are turned on after detecting presence of object (seconds*100 for optimal results)
#define TimerInc 10           //The incrementor will add the value to timer every loop until it reches the OnTime value after it gets reset
#define Wait 100              //The delay time of each loop (miliseconds)

bool prevst = 0;              //previous state of turning on of the lamps
int timer = 0;                //timer variable for time keeping
short state = 0;              //state of the presence of object can be 1 or -1
short tempState = 0;          //temporary variable to store state

CRGBArray<NUM_LEDS> leds;     //array of all led(s) connected to the micro-contoller

void setup() {                //setup of the progtam
  
  FastLED.addLeds<NEOPIXEL,LED_Pin>(leds, NUM_LEDS); //initializing FastLED
  
  Serial.begin(250000);       //Serial monitor intialization
  pinMode(IRPin1, INPUT);     //defining pinmode of IR sensor1
  pinMode(IRPin2, INPUT);     //defining pinmode of IR sensor2

  panelHSV(H, S, DefBri, 0);  //initializing lamp1 panelHSV(H-value, S-value, v-value, panel-number (0 - n-1))
  panelHSV(H, S, DefBri, 1);  //initializing lamp2
  panelHSV(H, S, DefBri, 2);  //initializing lamp3
  
  }

void panelHSV(int h, int s, int v, int no)  //function to change each lamp's characteristics
{
  int i = 4*no;
  leds[i+0] = CHSV(h, s, v);    //setting HSV value for each led in the lamp
  leds[i+1] = CHSV(h, s, v);
  leds[i+2] = CHSV(h, s, v);
  leds[i+3] = CHSV(h, s, v);
  FastLED.show();               //called to update the current values to the led(s)
}

void fadeOn(int low, int high, int h, int s, int v, int rot){ //function for animation when the lamps are turning on
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
      if(i >= low+c+c && i <= high+c+c){  fade for lamp1
        panelHSV(h, s, i-c-c, 0);
      }                           //effects of turning on will be   low     lim
    }                             //                       lamp1      |---|  |
  }                               //                       lamp2      ||---| |
}                                 //                       lamp3      | |---||

void fadeOff(int high, int low, int h, int s, int v, int rot){ //function for fade off effect
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
}

void lamps(){                   //lamps function will get called in every iteration of void loop
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
        fadeOn(DefBri, MaxBri, H, S, V, state);
        tempState = state;
        timer = 0;
      }
      else if(timer == OnTime && tempState != 0 && digitalRead(IRPin1) == 0 && digitalRead(IRPin2) == 0)  //if timer is equal to onTime then turn on time is exhausted and the lamp will fade off
      {
        fadeOff(MaxBri, DefBri, H, S, V, tempState * -1);
        tempState = 0;
        prevst = 0;
      }
      delay(Wait);              //delay after each call
    }
    else{                       //if timer variable gets greater than onTime then ill get reset to 0 anyways
      timer = 0;
    }
}


void loop(){                    //loop of program
 
  lamps();                      //lamps is called every loop

}
