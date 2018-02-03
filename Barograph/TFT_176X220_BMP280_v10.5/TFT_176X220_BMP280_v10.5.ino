//Open-Smart screen resolution of 176X220 pixels.

#include <UTFT.h>
#include <Wire.h>
#include "i2c.h"

#include "i2c_BMP280.h"
BMP280 bmp280;

// Declare which fonts we will be using
extern uint8_t BigFont[];

#define wide_win 219
#define high_win  143 //175-14+14+4
long arr_y[wide_win];
//***********************************************//
// If you use OPEN-SMART TFT breakout board                 //
// You need to add 5V-3.3V level converting circuit.
// Of course you can use OPEN-SMART UNO Black version with 5V/3.3V power switch,
// you just need switch to 3.3V.
// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
//----------------------------------------|
// TFT Breakout  -- OPEN-SMART UNO Black /Red
// GND              -- GND
// 3V3               -- 3.3V
// CS                 -- A3
// RS                 -- A2
// WR                -- A1
// RD                 -- 3.3V
// RST                -- A0
// LED                -- GND
// DB0                -- 8
// DB1                -- 9
// DB2                -- 10
// DB3                -- 11
// DB4                -- 4
// DB5                -- 13
// DB6                -- 6
// DB7                -- 7
// Remember to change the model parameter to suit your display module!
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_RS A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RST A0 // 
UTFT tft(LGDP4524,LCD_RS,LCD_WR,LCD_CS,LCD_RST);
//------------Functions----------------------------------------------------------
//----Maxium
long Max(long a,long b){
 long maxi;
 if (a > b)
 { 
  maxi = a;
 }
 else
 {
  maxi = b;
 }
 //println "(a=";a;" b=";b;") ";
 return maxi;
}
//----Minimum
long Min(long a,long b){
 long mini;
 if (a < b)
 { 
  mini = a;
 }
 else
 {
  mini = b;
 }
 //println "(a=";a;" b=";b;") ";
 return mini;
}

void setup()
{
  // randomSeed(analogRead(0));
  // Setup the LCD
  tft.InitLCD();
  tft.setFont(BigFont);
   Serial.begin(115200);
   Serial.print("Probe BMP280: ");
    if (bmp280.initialize()) Serial.println("Sensor found");
    else
    {
        Serial.println("Sensor missing");
        while (1) {}
    }
    // onetime-measure:
    bmp280.setEnabled(0);
    bmp280.triggerMeasurement();
}
//----------main---------------------------------------------------------
void loop()
{
unsigned long time;
long  y,y_next,x2,y2,y_max,y_min,delta_xy;
float pascal,oldpascal,temperature;
unsigned int mm_rs;

    bmp280.awaitMeasurement();
    bmp280.getTemperature(temperature);
    bmp280.getPressure(pascal);
    mm_rs=round(pascal*7.500637e-3);
//    static float meters, metersold;
//    bmp280.getAltitude(meters);
//    metersold = (metersold * 10 + meters)/11;

    bmp280.triggerMeasurement();
    Serial.print(" Pressure: ");
    Serial.print(pascal);
    Serial.print(" pascal ;   Pressure: ");
    Serial.print(mm_rs);
    Serial.print(" mm.rt.st;  T: ");
    Serial.print(temperature);
    Serial.println(" C");

//------- Draw gyperbola grid
tft.clrScr(); 
  tft.setColor(0, 0, 255); //blue color
  tft.setBackColor(0, 0, 0);
  for (int i=1; i<176; i+=10)
    tft.drawLine(1, i, 176-i, 1);
  tft.setColor(0, 255, 0); //green color
  tft.setBackColor(0, 0, 0);
  for (int i=1; i<176; i+=10)
    tft.drawLine(175, 176-i, i, 175);
//----------------------------------------------    
// Clear the screen and draw the frame
  tft.clrScr();    
//-------Draw digital values on tft screen----------    
  tft.setBackColor(64, 64, 64);
  tft.setColor(255,255,0);
  tft.print("Pressure=",1, 1);
  tft.setColor(255, 255, 255);
  tft.setBackColor(0, 0, 0);
  tft.printNumI(mm_rs, 145, 1);
     tft.setBackColor(0, 0, 0);
     tft.setColor(0,255,0);
     tft.print("Averaged press",0, 20);
     tft.print("ure values are",0, 37);
     tft.print("output after  ",0, 52);
     tft.print("393 seconds.  ",0, 67);
     tft.setColor(0,255,255);
     tft.print("A total of 220",0, 82);
     tft.print("points in 24 h",0, 97);
     tft.print("(X-axis=24 h) ",0, 112);
     tft.setColor(255,255,0);
     tft.print("The Y-axis is ",0, 127);
     tft.print("autoscaled.   ",0, 142);
   //  tft.setBackColor(47, 79, 79);
     tft.setColor(255,0,0);
     tft.print("Firmware 10.5 ",0, 162);
     

//---------Create values of massive-------------------
for ( int x = 0; x<=wide_win ;x++){
    arr_y[x]=pascal;
    //arr_y[x]=100000+4000*sin(x/10.0)*exp(0.004*x);
    //Serial.println (arr_y[x]);
  }
 oldpascal=pascal;
//-----------begin try infinity loop--------------------
label1:

   for ( int x = 0; x<393 ;x++)  //x<393 это задержка 392.7 сек * 220 точек = 86400 сек (24 часа)
   {
    bmp280.awaitMeasurement();
    bmp280.getTemperature(temperature);
    bmp280.getPressure(pascal);
    mm_rs=round(pascal*7.500637e-3);
   
    oldpascal = (oldpascal * 10 + pascal)/11;
   
    bmp280.triggerMeasurement();
    delay(1000);    // задержка 1 сек
   }
    Serial.print(" Pressure: ");
    Serial.print(pascal);
    Serial.print(" pascal ;   Pressure: ");
    Serial.print(mm_rs);
    Serial.print(" mm.rt.st;  T: ");
    Serial.print(temperature);
    Serial.println(" C");
/*
//-----Shift array to right on one element----------
for ( int x = 0; x<=wide_win ;x++){
    arr_y[wide_win-x]=arr_y[wide_win-x-1];
    //Serial.println (arr_y[x]);
  }
    
//---Save first element array by load pressure---------
    arr_y[0]=pascal; 
*/

//-----Shift array to left on one element----------
for ( int x = 0; x<=wide_win-1 ;x++){
    arr_y[x]=arr_y[x+1];
    //Serial.println (arr_y[x]);
  }
    
//---Save last element array by load pressure---------
    arr_y[wide_win]=oldpascal; 




//------Calculate max and min value of massive-----
y_max=arr_y[0];
y_min=arr_y[0];
for (int x = 1; x<=wide_win ;x++){
    y_max=Max(y_max,arr_y[x]);
    //Serial.print ("y_max=");
    //Serial.println (y_max);
    y_min=Min(y_min,arr_y[x]);
    //Serial.print ("y_min=");
    //Serial.println (y_min);
  }
delta_xy=y_max-y_min;
    Serial.println (y_max);
    Serial.println (y_min);
    Serial.println (delta_xy);

// Clear the screen and draw the frame
  tft.clrScr();    
//-------Draw digital values on tft screen----------    
  tft.setBackColor(64, 64, 64);
  tft.setColor(255,255,0);
  tft.print("Pmm=",1, 1);
  tft.setColor(255, 255, 255);
  tft.setBackColor(0, 0, 0);
  tft.printNumI(mm_rs, 60, 1);

  tft.setBackColor(153, 0, 102);
  tft.setColor(255,255,0);
  tft.print("Max=",110, 1);
  tft.setColor(255, 255, 255);
  tft.setBackColor(0, 0, 0);
  tft.printNumI(round(y_max*7.500637e-3), 170, 1);
   
  tft.setBackColor(64, 64, 64);
  tft.setColor(0,255,0);
  tft.print("TdC=",1, 163-2);
  tft.setColor(255, 255, 255);
  tft.setBackColor(0, 0, 0);
  tft.printNumI(round(temperature-4), 60, 163-2);
    
  tft.setBackColor(0, 0, 128);
  tft.setColor(255,255,0);
  tft.print("Min=",110, 163-2);
  tft.setColor(255, 255, 255);
  tft.setBackColor(0, 0, 0);
  tft.printNumI(round(y_min*7.500637e-3), 170, 163-2);
  
//------- Draw crosshairs
  tft.setColor(0, 0, 255); //blue color
  tft.setBackColor(0, 0, 0);
  tft.drawLine(109, 15, 109, 160);
  tft.drawLine(1, 88, 218, 88);
 for (int i=10; i<211; i+=9)
    tft.drawLine(i, 86, i, 90);
  for (int i=18; i<154; i+=10)
    tft.drawLine(107, i, 111, i);  
      
//----------resize Y-axiss and draving line-------
tft.setColor(255,255,0);  //yellow color 
for (int x = 0; x<=wide_win-1 ;x++)
 {
   y=high_win-(arr_y[wide_win-1-x]-y_min)*high_win/delta_xy + 17;
   y_next=high_win-(arr_y[wide_win-1-x+1]-y_min)*high_win/delta_xy + 17;
   //Serial.println (y);
   tft.drawLine(wide_win-1-x,y,wide_win-1-x+1,y_next);
  //tft.drawPixel(x,y);
   }
 

 // Serial.print("Time: ");
 // time = millis();
 //---выводит количество миллисекунд с момента начала выполнения программы
 // Serial.println(time);
 
  delay (10); //T=1000ms debug mode
  goto label1; // :)
}
