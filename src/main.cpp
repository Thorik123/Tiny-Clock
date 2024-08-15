#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <TM1637TinyDisplay.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#define debug(msg) Serial.print(msg)
#define debugln(msg) Serial.println(msg)

#define VERSION 1.0
#define NAME "Tiny-Clock"

#define CLK_PIN 0
#define DIO_PIN 2

TM1637TinyDisplay display(CLK_PIN, DIO_PIN);

const char *WIFI_SSID = "Polsek Batealit";
const char *WIFI_PASSWORD = "thoriktronik";

WiFiUDP udp;
NTPClient client(udp, "pool.ntp.org", 25200, 60000);

void connectWiFi();
void logInfo();

void setup()
{
  Serial.begin(115200);
  delay(50);

  logInfo();
  connectWiFi();

  client.begin();
  display.setBrightness(3, true);
  display.clear();
}

void loop()
{
  client.update();

  char buffer[5];
  sprintf(buffer, "%02d%02d", client.getHours(), client.getMinutes());

  if ((client.getSeconds() % 2) != 0)
  {
    display.showNumberDec(atoi(buffer), 0b00000000, true, 4, 0);
  }
  else
  {
    display.showNumberDec(atoi(buffer), 0b01000000, true, 4, 0);
  }
}

void logInfo()
{
  debug(F("\n|-------------------------------|\n"));
  debug(F("|          Tiny  Clock          |\n"));
  debug(F("|         by  thorik_tk         |\n"));
  debug(F("|            version            |\n"));
  debug(F("|              "));
  debug(VERSION);
  debug(F("             |\n"));
  debug(F("|-------------------------------|\n"));
  delay(500);
}

void connectWiFi()
{
  debug(F("Connecting to WiFi\n"));
  debug(F("SSID -> "));
  debug(WIFI_SSID);
  debug(F("\n"));

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    debug(F("."));
    delay(500);
  }
  debug(F("\nConnected, IP -> "));
  debug(WiFi.localIP());
  debug(F("\n"));
}