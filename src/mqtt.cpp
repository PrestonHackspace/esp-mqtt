#include "mqtt.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void mqtt_init(const char *deviceName, IPAddress ip, uint16_t port, MqttMessageCallback callback)
{
  Serial.println("Connect to MQTT: " + ip.toString());

  mqttClient.setServer(ip, port);

  mqttClient.setCallback([callback](const char *topic, byte *payload, unsigned int length) {
    Serial.println(String("Message arrived [") + String(topic) + String("]"));

    auto message = new char[length + 1];

    strcpy(message, (char *)payload);
    message[length] = 0;

    if (callback)
    {
      callback(String(topic), String(message));
    }

    delete message;

    Serial.println();
  });

  if (mqttClient.connect(deviceName))
  {
    Serial.println("Connected");
  }
  else
  {
    Serial.println("Failed to connect to MQTT broker!");
  }
}

void mqtt_subscribe(const char *topic)
{
  mqttClient.subscribe(topic);
}

void mqtt_publish(const char *topic, const char *payload)
{
  bool success = mqttClient.publish(topic, payload);

  if (!success)
  {
    Serial.println("Failed to publish MQTT message!");
  }
}

void mqtt_loop()
{
  if (!mqttClient.connected())
  {
    Serial.println("MQTT no longer connected! Will reboot in 5 seconds!");
    delay(5000);
    ESP.reset();
  }

  mqttClient.loop();
}
