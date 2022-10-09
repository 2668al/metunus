/*Warning! After the power on the sensor need 10-15 seconds for heating. During that period, module doesn't' update data*/
#include <Arduino.h>
#include <Wire.h>
#include <iarduino_I2C_SHT.h>
#include "cgAnem.h"
 
iarduino_I2C_SHT sht(0x33);        



CG_Anem cgAnem(ANEM_I2C_ADDR);

void setup()
{
  sht.begin();                                  //   Инициируем работу с датчиком температуры и влажности.
  sht.setTemChange( 0.1 );                      //   Указываем фиксировать изменение температуры окружающей среды более чем на 0.1°С.
  sht.setHumChange( 1.0 );                      //   Указываем фиксировать изменение влажности воздуха более чем на 1%.
  Serial.begin(115200);
  delay(1000);
  if (cgAnem.init()) // try to init the sensor module
    Serial.println("Sensor secsessfully found");
  else
    Serial.println("Sensor wiring error");

  uint8_t sensorChipId = cgAnem.getChipId(); /*Returns chip id, default value: 0x11.*/
  cgAnem.set_duct_area(100); // set here duct area for flow consumption calculation in sm^2. If duct area not seted cgAnem.airConsumption will be -255 (default value)

  
  delay(3000);
}

/*Warning! After the power on the sensor need 10-15 seconds for heating. During that period, module doesn't' update data*/
void loop()
{
 
  Serial.print("Температура = ");           //
  Serial.print( sht.getTem() );             //   Выводим текущую температуру окружающей среды, от -40 до +125°С.
  Serial.print(" °С.\r\n");
  
  
  Serial.print("Влажность = ");             //
  Serial.print( sht.getHum() );             //   Выводим текущую влажность воздуха, от 0 до 100%.
  Serial.print(" %.\r\n"); 
        
  if (cgAnem.data_update())
  {
    Serial.println("Air flow rate: " + String(cgAnem.airflowRate) + " m/s");
    Serial.println("Current temperature: " + String(cgAnem.temperature) + " C");
    Serial.println("Air flow consumption:" + String(cgAnem.airConsumption) + " m^3/hour");
  }
  else
    Serial.println("transient process do not finished, measurements are not relevant");
  delay(1300);
}
