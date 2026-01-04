#include "ir_tx_core.h"
#include "ir_config.h"

void setup() {
  irTxInit();
}

void loop() {
  IRFrame frame;
  frame.version   = IR_PROTOCOL_VERSION;
  frame.flags     = 0x00;
  frame.senderID  = 0xA1;
  frame.deviceID  = 0x01;
  frame.command   = 0x10;
  frame.timestamp = millis() & 0xFFFF;

  irSendFrame(frame);
  delay(1000);
}
