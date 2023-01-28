#include <Console.h>

#include <ESP32Console.h>




using namespace ESP32Console;

Console console;

void setup_console(int baudrate) {
  console.setPrompt("console> ");
  console.begin(baudrate);
  // Регистрируем "встроенные" команды консоли, такие как 'reboot', 'version' или 'meminfo' 
  console.registerSystemCommands();
  //console.registerCommand(ConsoleCommand("settime", &my_command, "Set time"));
  // Регистрируем наши собственные команды (для конфигурирования метеостанции)
  // Первый аргумент - это название команды, второй аргумент - это функция, которая 
  // будет выполняться, третий аргумент - краткое описаниекоманды, которое будет 
  // отображаться при вызове help.
  // console.registerCommand(ConsoleCommand("my_command", &my_command, "Some command"));

  // Когда работает консоль, мы не можем использовать функцию Serial.print для вывода 
  // в последовательый порт, вместо нее мы должны использовать функцию printf
  printf("\n\nWelcome to weather station. See 'help' for all commands.");
}
