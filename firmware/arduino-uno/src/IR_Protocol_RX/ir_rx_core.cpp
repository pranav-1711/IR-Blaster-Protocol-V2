#include "ir_rx_core.h"
#include "ir_config.h"
#include "ir_crc8.h"

static uint16_t lastTimestamp = 0;

static uint16_t pulse(bool level) {
  unsigned long start = micros();
  while (digitalRead(IR_RX_PIN) == level) {
    if (micros() - start > 15000) break;
  }
  return micros() - start;
}

static bool readBit() {
  pulse(LOW);
  return pulse(HIGH) > 1000;
}

static uint8_t readByte() {
  uint8_t value = 0;
  for (uint8_t i = 0; i < 8; i++) {
    if (readBit()) value |= (1 << i);
  }
  return value;
}

bool irReceiveFrame(IRFrame &f) {
  if (pulse(LOW) < IR_PREAMBLE_US - 500) return false;
  pulse(HIGH);

  f.version   = readByte();
  f.flags     = readByte();
  f.senderID  = readByte();
  f.deviceID  = readByte();
  f.command   = readByte();
  f.timestamp = readByte() | (readByte() << 8);
  f.crc       = readByte();

  uint8_t buf[7] = {
    f.version, f.flags, f.senderID,
    f.deviceID, f.command,
    lowByte(f.timestamp), highByte(f.timestamp)
  };

  if (crc8(buf, sizeof(buf)) != f.crc) return false;
  if ((uint16_t)(f.timestamp - lastTimestamp) < REPLAY_WINDOW_MS) return false;

  lastTimestamp = f.timestamp;
  return true;
}
