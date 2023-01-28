#include <iarduino_I2C_SHT.h>
#include <Wire.h>
#include <iarduino_Pressure_BMP.h>
#include "cgAnem.h"


int sensors_init();

int tem_hum();
int wind_speed();
int water_level();
int soil_temp();
int soil_moist();
int press();
int RTC();

int get_sensors_data();