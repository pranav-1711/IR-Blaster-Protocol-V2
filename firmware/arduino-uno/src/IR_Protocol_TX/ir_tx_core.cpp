#include "ir_tx_core.h"
#include "ir_config.h"
#include "ir_crc8.h"

static void mark(uint16_t us) {
  unsigned long start = micros();
  while (micros() - start < us) {
    digitalWrite(IR_TX_PIN, HIGH);
    delayMicroseconds(13);
    digitalWrite(IR_TX_PIN, LOW);
    delayMicroseconds(13);
  }
}

static void space(uint16_t us) {
  digitalWrite(IR_TX_PIN, LOW);
  delayMicroseconds(us);
}

static void sendBit(bool bit) {
  mark(IR_MARK_US);
  space(bit ? IR_SPACE_1_US : IR_SPACE_0_US);
}

static void sendByte(uint8_t data) {
  for (uint8_t i = 0; i < 8; i++) {
    sendBit(data & 0x01);
    data >>= 1;
  }
}

void irTxInit() {
  pinMode(IR_TX_PIN, OUTPUT);
  digitalWrite(IR_TX_PIN, LOW);
}

void irSendFrame(const IRFrame &frame) {
  uint8_t payload[7] = {
    frame.version,
    frame.flags,
    frame.senderID,
    frame.deviceID,
    frame.command,
    lowByte(frame.timestamp),
    highByte(frame.timestamp)
  };

  uint8_t crc = crc8(payload, sizeof(payload));

  mark(IR_PREAMBLE_US);
  space(IR_GAP_US);

  for (uint8_t b : payload) sendByte(b);
  sendByte(crc);

  mark(IR_MARK_US);
  space(5000);
}
