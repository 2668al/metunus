#include <GyverOLED.h>
#include <iarduino_I2C_SHT.h>

iarduino_I2C_SHT sht(0x33);

GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

void setup() {
  Serial.begin(115200);
  sht.begin();                            
  sht.setTemChange( 0.1 );                      
  sht.setHumChange( 1.0 );
  
  oled.init();        // инициализация
  oled.clear();       // очистка
  oled.setScale(2);   // масштаб текста (1..4)
  oled.home();        // курсор в 0,0
  oled.print("Тест");
  delay(1000);
}
void loop() {
  oled.clear();
  oled.home();
  
  oled.print("т = ");
  oled.print(sht.getTem());
  oled.print("С\r\n");
  
  oled.print("в = ");
  oled.print(sht.getHum());
  oled.print("%\r\n");

  delay(1500);
}
