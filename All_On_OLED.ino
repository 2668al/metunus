#include <GyverOLED.h>
#include <iarduino_I2C_SHT.h>
#include <Wire.h>
#include <iarduino_Pressure_BMP.h>
#include "cgAnem.h"


iarduino_I2C_SHT sht(0x33);
CG_Anem cgAnem(ANEM_I2C_ADDR);
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
iarduino_Pressure_BMP sensor(0x77);


void setup() {
  Serial.begin(115200);
  sht.begin();
  sht.setTemChange( 0.1 );        // Настройки температуры и влажности
  sht.setHumChange( 1.0 );
  delay(1000);

  sensor.begin();     // Старт барометра
  delay(1000);
  
  oled.init();        // инициализация
  oled.clear();       // очистка
  oled.setScale(2);   // масштаб текста (1..4)
  oled.home();        // курсор в 0,0
  oled.print("Тест");
  delay(1000);

  cgAnem.init();
  uint8_t sensorChipId = cgAnem.getChipId();
  cgAnem.set_duct_area(100);                    // Какие-то настрйки для скорости ветра
}


void loop() {
  oled.clear();               // Отчистка дисплея
  oled.home();
  
  oled.print("т =\r\n");
  oled.print(sht.getTem());   // Вывод температуры
  oled.print("С\r\n");
  
  oled.print("в =\r\n");
  oled.print(sht.getHum());   // Вывод влажности
  oled.print("%\r\n");

  delay(3000);

  oled.clear();               // Отчиска дисплея
  oled.home();

  cgAnem.data_update();
  oled.print("с =\r\n" + String(cgAnem.airflowRate) + "m/s\r\n");     // Вывод скорости ветра

  sensor.read(1);
  oled.print("д =\r\n");
  oled.print(sensor.pressure);  // Вывод давления
  oled.print("mmrt");

  delay(3000);
}