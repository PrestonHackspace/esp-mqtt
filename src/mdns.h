#pragma once

#include <functional>
#include <ESP8266mDNS.h>

typedef std::function<void(IPAddress, uint16_t)> MdnsCallback;

void mdns_init(const char *deviceName, const char *serviceName, MdnsCallback callback);
void mdns_discover(const char *serviceName, MdnsCallback callback);
