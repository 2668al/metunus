#include <iarduino_RTC.h>
#include <iarduino_I2C_SHT.h>
#include <cgAnem.h>
#include <iarduino_Pressure_BMP.h>
#include <Wire.h>



int sensors_begin() {                         // Функция запуска сенсоров
  iarduino_I2C_SHT sht(0x33);
  CG_Anem cgAnem(ANEM_I2C_ADDR);              // Инициализация датчиков
  iarduino_Pressure_BMP sensor(0x77);
  

  sht.begin();
  sht.setTemChange( 0.1 );                    // Запуск датчика температуры и влажности
  sht.setHumChange( 1.0 );
  delay(100);

  sensor.begin();                             // Запуск барометра
  delay(100);

  cgAnem.init();
  uint8_t sensorChipId = cgAnem.getChipId();  // Запуск датчика скорости ветра
  cgAnem.set_duct_area(100);
  delay(100);
  return 1;
}


int get_sensors_data(String a) {
  a =watch.gettime("d-m-Y, H:i:s, D") + ": " String(sht.getTem()) + String(sht.getHum()) + String(cgAnem.airflowRate) + String(sensor.pressure);
}
