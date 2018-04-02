#pragma once

#include <RCSwitch.h>

#include "config.h"

typedef std::function<void(long)> RcMessageCallback;

void rcswitch_init(RcMessageCallback cb);
void rcswitch_loop();
