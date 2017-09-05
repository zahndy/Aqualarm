//    Aqualarm 
//    Aquarium Temperature Alarm
//    Copyright (C) <2017>  <Xander Jansen>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "U8glib.h"    
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 20
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000    
 
int samples[NUMSAMPLES];
int led2 = 13; 
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
    
float Temperature=0; 
int counter=0;
void drawTemp(void) {
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(18, 10);
  u8g.print("TEMPERATURE");
  u8g.setFont(u8g_font_fub25);
  u8g.setPrintPos(12, 50);
  u8g.print(String(Temperature).substring(0,5));
  u8g.setPrintPos(100, 50);
   u8g.print("C");
   if(counter==2){
    u8g.setPrintPos(-4, 22);
       if(Temperature < 20)
     {
       u8g.print("!");
       u8g.setPrintPos(118, 22);
       u8g.print("!");
       u8g.setPrintPos(26, 64);
       u8g.setFont(u8g_font_unifont);
       u8g.print("TOO COLD!");
     }
     if(Temperature > 32)
     { u8g.print("!");
       u8g.setPrintPos(118, 22);
       u8g.print("!");
       u8g.setPrintPos(26, 64);
       u8g.setFont(u8g_font_unifont);
       u8g.print("TOO WARM!");
     }
   }
  // Celcius symbol
  if(counter>3){
  u8g.drawCircle(100, 23, 2); 
  }
}

void setup(void) {
  pinMode(4, OUTPUT); 
  analogReference(EXTERNAL);
  digitalWrite(led2, HIGH);
}

float MeasureTemp() {
  uint8_t i;
  float average =0;
 
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(A0);
   delay(10);
  }
 
  // average all the samples out
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
 
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
 
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
 
  return steinhart;
}
void loop(void) {
 
 counter++;
 Temperature = MeasureTemp();
 
 noTone(4);
  if(counter==2 || counter ==4){
    digitalWrite(led2, LOW);
     if(Temperature < 20)
       {
         if(Temperature > 2)
          tone(4, 440, 550); 
      }
      if(Temperature > 32)
       {
          tone(4, 440, 550); 
      }
     u8g.firstPage();  
      do {
        
        drawTemp();
      } while( u8g.nextPage() );
      
      delay(380);
   }
   else{
    digitalWrite(led2, HIGH);
    if(Temperature < 18)
      {
        if(Temperature > 2)
          tone(4, 840, 550); 
      }
      if(Temperature > 34)
      {
          tone(4, 840, 550); 
      }
      delay(450);
   }
  if(counter>3){counter=0;
  }  
  
  
}
