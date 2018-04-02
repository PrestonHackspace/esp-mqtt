#include "rc.h"

RCSwitch rcSwitch = RCSwitch();

RcMessageCallback callback;

void rcswitch_init(RcMessageCallback cb)
{
  callback = cb;

  rcSwitch.enableTransmit(TX_PIN);
  rcSwitch.enableReceive(digitalPinToInterrupt(RX_PIN));
}

void rcswitch_loop()
{
  if (rcSwitch.available())
  {
    int value = rcSwitch.getReceivedValue();

    if (value == 0)
    {
      Serial.print("Unknown encoding");
    }
    else
    {
      Serial.print("Received ");
      Serial.print(value);
      Serial.print(" / ");
      Serial.print(rcSwitch.getReceivedBitlength());
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println(rcSwitch.getReceivedProtocol());
    }

    rcSwitch.resetAvailable();

    if (callback) callback(value);
  }
}
