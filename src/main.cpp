#include <Arduino.h>
#include <WiFiManager.h>

#include "config.h"
#include "mdns.h"
#include "mqtt.h"
#include "rc.h"

WiFiManager wifiManager;

uint32_t bellOffTime = 0;

String get_device_name()
{
  return String(DEVICE_TYPE_NAME) + String(": ") + String(ESP.getChipId());
}

void setup()
{
  Serial.begin(115200);

  Serial.println("Booting...");

  pinMode(LED_PIN, OUTPUT);
  pinMode(BELL_PIN, OUTPUT);

  digitalWrite(BELL_PIN, LOW);

  for (int i = 0; i < 5; i++)
  {
    digitalWrite(LED_PIN, LOW);
    delay(25);
    digitalWrite(LED_PIN, HIGH);
    delay(25);
  }

  wifiManager.autoConnect(get_device_name().c_str());

  mdns_init(DEVICE_TYPE_NAME, "mqtt", [](IPAddress ip, uint16_t port) {

    mqtt_init(DEVICE_TYPE_NAME, ip, port, [](String topic, String message) {

      if (topic == BELL_CHANNEL)
      {
        bellOffTime = millis() + atoi(message.c_str());
        
        digitalWrite(BELL_PIN, HIGH);

        Serial.println(String("Now: ") + String(millis()) + String(" Off: ") + String(bellOffTime));
      }

    });

    mqtt_subscribe(BELL_CHANNEL);

  });

  rcswitch_init([](long val) {

    if (val > 0)
    {
      mqtt_publish(RF_CHANNEL, String(val).c_str());
    }

  });
}

void loop()
{
  mqtt_loop();
  rcswitch_loop();

  if (millis() > bellOffTime) {
    digitalWrite(BELL_PIN, LOW);
    bellOffTime = 0;
  }
}

// 2558606
// 2558598
// 2558602
// 2558594
// 2558604
