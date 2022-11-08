#include <iarduino_I2C_SHT.h>
#include "WiFi.h" 
#include "WiFiClientSecure.h" 
#include "UniversalTelegramBot.h" 
#include "ArduinoJson.h"  

iarduino_I2C_SHT sht(0x33);

int numNewMessages;
// Укажите свои данные сети
const char* ssid = "";
const char* password = "";

// Используйте @myidbot, чтобы получить ID пользователя или группы
// Помните, что бот сможет вам писать только после нажатия
// вами кнопки /start
#define CHAT_ID ""

// Запустите бот Telegram
#define BOTtoken ""  // укажите токен бота

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

//Каждую секунду проверяет новые сообщения
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;


// Запрос показаний датчика BME280 и запись их в переменную типа String
String getReadings(){
  float temperature, humidity;
  temperature = sht.getTem();
  humidity = sht.getHum();
  String message = "Temperature: " + String(temperature) + " ºC \n";
  message += "Humidity: " + String (humidity) + " % \n";
  return message;
}

//Задаем действия при получении новых сообщений
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following command to get current readings.\n\n";
      welcome += "/readings \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/readings") {
      String readings = getReadings();
      bot.sendMessage(chat_id, readings, "");
    }  
  }
}


void setup() {
  Serial.begin(115200);
  sht.begin();                            
  sht.setTemChange( 0.1 );                      
  sht.setHumChange( 1.0 );

  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000); 
    Serial.println("Подключение к Wi-Fi..."); 
  }
}

void loop() { 
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
  while(numNewMessages) {
    Serial.println("Получено новое сообщение");
    handleNewMessages(numNewMessages);
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }
}
