/********SMART STREET LIGHT ver- 4.0
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
 *  7.      AHT10                        01
 *  8.      SSD1306                      01
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