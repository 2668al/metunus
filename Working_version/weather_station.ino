
#define AP_SSID ""
#define AP_PASS ""

#define WIFI_TIMEOUT 30000         // 30 sec
#define WEB_SERVER_MODE_TIME 60000 // 60 sec
#define SLEEP_TIME 10000           // 10 sec

#define MODE_BUTTON_PIN 2
#define CONSOLE_JUMPER_PIN 3

#include <LittleFS.h>
#include <WiFi.h>
#include <GyverPortal.h>

#include "sensors.h"
#include "console.h"
#include "data_channels.h"

GyverPortal portal;

int filesystem_error = 0;
int web_server_mode_ms = 0;
int console_mode = 0;

int data_channel = chNone;

// Ф-ция создания html странички
void build_html_page() {
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);
  GP.EMBED("/data.txt");
  GP.BUTTON_LINK("/data.txt", "Открыть");
  GP.BUTTON_DOWNLOAD("/data.txt", "Скачать");
  GP.BUILD_END();
}

// Ф-ция подключения к точке доступа WiFi
int connect_to_wifi(const char* ssid, const char* pass, int timeout) {
  int try_to_connect_ms = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    try_to_connect_ms += 500;
    if (try_to_connect_ms >= timeout) {
      return -1;
    }
  }
  Serial.println(WiFi.localIP());
  return 1;
}

// Ф-ция реакции на нажатие кнопки на веб-страничке
void web_action() {
  if (portal.download()) {
    Serial.println(portal.uri());
    // открыть файл любым способом и передать в библиотеку
    // путь к файлу - uri
    portal.sendFile(LittleFS.open(portal.uri(), "r"));
  }
}

// Получение текущего времени от м/сх RTC
int get_time_from_rtc() {
  return 1;
}
// Ф-ция сохранения данных опроса в файле
void save_sensors_data(int data) {}
// Ф-ция пробуждения
void wakeup() {}
// Ф-ция перехода в "сон"
void go_to_sleep(int sleep_time) {
  delay(sleep_time);
}

void setup() {
  Serial.begin(115200);
  // Настраиваем датчики
  // Настраиваем OLED дисплей
  // "Включаем" файловую систему LittleFS
  if (!LittleFS.begin()) {
    Serial.println("FS Error");
    filesystem_error = 1; // Устанавливаем filesystem_error flag
  }
  // Настраиваем пин для кнопки
  pinMode(MODE_BUTTON_PIN, INPUT);
  pinMode(CONSOLE_JUMPER_PIN, INPUT);
  delay(500);
  if (digitalRead(MODE_BUTTON_PIN) == HIGH) {
    // Нажата кнопка выбора режима работы
    // Запускаем режим веб-сервера, чтобы дать возможность скачать файл с
    // данными
    if (connect_to_wifi(AP_SSID, AP_PASS, WIFI_TIMEOUT) == -1) {
      // Ошибка подключения к WiFi, переходим к обычному режиму работы
      Serial.println("WiFi error");
      delay(3000);
    } else {
      // Переходим в режим веб-сервера на WEB_SERVER_MODE_TIME мс
      Serial.println("Web-server mode");
      portal.attachBuild(build_html_page);
      portal.attach(web_action);
      portal.start();
      do {
        int millis_tick_start = millis();
        portal.tick();
        int millis_tick_end = millis();
        web_server_mode_ms += (millis_tick_end - millis_tick_start);
      } while (web_server_mode_ms <= WEB_SERVER_MODE_TIME);
      Serial.println("Exit from web-server mode");
      delay(2000);
      Serial.println("Restarting...");
      delay(2000);
      // Перезагружаемся...
      ESP.restart();
    }
  } else if (digitalRead(CONSOLE_JUMPER_PIN) == HIGH) {
    // Установлен джампер выбора режима консоли для настройки метеостанции
    setup_console(9600);
    console_mode = 1;
  } else {
    /* Кнопка выбора режима не нажата и джампер запуска консоли не установлен -
      переходим в обычный режим работы.
      В этом режиме esp32 просыпается раз в 10 минут (например), считывает текущее
      время с часов реального времени и если сейчас начало нового часа, то
      происходит опрос датчиков, сохранение данных в энергонезависимой памяти и
      отправка этих данных по указанному в настройках каналу связи. После этого
      esp32 снова засыпает. Пока, на начальном этапе разработки, мы не будем
      использовать режим глубокого сна - для простоты потестируем без него. */
  }
}

void loop() {
  if (!console_mode) {
    int current_time;
    int sensors_data;
    wakeup();
    current_time =
      get_time_from_rtc(); // узнаем текущее время от внешнего модуля RTC
    if (current_time) {
      // Настало время опросить датчики
      // Опрашиваем...
      sensors_data = get_sensors_data();
      // Сохраняем данные в файл...
      save_sensors_data(sensors_data);
      // Передаем данные по каналу связи...
      send_sensors_data(sensors_data, data_channel);
    }
    go_to_sleep(SLEEP_TIME);
  }
}
