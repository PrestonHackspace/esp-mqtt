#include "mdns.h"

void mdns_init(const char *deviceName, const char *serviceName, MdnsCallback callback) {
  if (!MDNS.begin(deviceName))
  {
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  mdns_discover(serviceName, callback);
}

void mdns_discover(const char serviceName[], MdnsCallback callback) {
  Serial.println("Sending mDNS query");
  int n = MDNS.queryService(serviceName, "tcp"); // Send out query for esp tcp services
  Serial.println("mDNS query done");

  if (n == 0)
  {
    Serial.println(String("no ") + serviceName + String(" services found, retrying..."));
    delay(1000);
    return mdns_discover(serviceName, callback);
  }
  else
  {
    Serial.println(String(n) + String(" service(s) found"));

    for (int i = 0; i < n; ++i)
    {
      // Print details for each service found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(MDNS.hostname(i));
      Serial.print(" (");
      Serial.print(MDNS.IP(i));
      Serial.print(":");
      Serial.print(MDNS.port(i));
      Serial.println(")");

      return callback(MDNS.IP(i), MDNS.port(i));
    }
  }
}
