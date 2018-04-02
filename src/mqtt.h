#pragma once

#include <PubSubClient.h>
#include <WiFiClient.h>

#include "config.h"

typedef std::function<void(String, String)> MqttMessageCallback;

// extern PubSubClient mqttClient;

void mqtt_init(const char *deviceName, IPAddress ip, uint16_t port, MqttMessageCallback callbacks);
void mqtt_subscribe(const char *topic);
void mqtt_publish(const char* topic, const char* payload);
void mqtt_loop();
