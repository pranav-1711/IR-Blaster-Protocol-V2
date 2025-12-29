/*
  IR-Blaster-Protocol-V2.0
  Receiver Firmware (RX)

  Board   : Arduino UNO
  Library : IRremote v4.x
  IR RX   : TSOP1738 / VS1838B
  Pin     : D2
*/

#include <Arduino.h>
#include <IRremote.hpp>

/* ---------- CONFIG ---------- */
#define IR_RECEIVE_PIN 2
#define ONE_SPACE_THRESHOLD 1000   // >1000us = logic 1

/* ---------- PAIRING ---------- */
#define MAX_PAIRED_DEVICES 5
uint8_t pairedIds[MAX_PAIRED_DEVICES];
uint8_t pairedCount = 0;

/* ---------- CRC8 ---------- */
uint8_t crc8(const uint8_t *data, uint8_t len) {
  uint8_t crc = 0;
  for (uint8_t i = 0; i < len; i++) {
    crc ^= data[i];
  }
  return crc;
}

/* ---------- PAIRING HELPERS ---------- */
bool isPaired(uint8_t id) {
  for (uint8_t i = 0; i < pairedCount; i++) {
    if (pairedIds[i] == id) return true;
  }
  return false;
}

void addPair(uint8_t id) {
  if (!isPaired(id) && pairedCount < MAX_PAIRED_DEVICES) {
    pairedIds[pairedCount++] = id;
  }
}

/* ---------- BIT DECODER ---------- */
uint8_t decodeBit(uint16_t spaceMicros) {
  return (spaceMicros > ONE_SPACE_THRESHOLD) ? 1 : 0;
}

/* ---------- SETUP ---------- */
void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("IR-Blaster-Protocol-V2 RX Ready");
}

/* ---------- LOOP ---------- */
void loop() {
  if (!IrReceiver.decode()) {
    return;
  }

  uint8_t bits[64];
  uint8_t bitLen = 0;

  for (uint8_t i = 3; i < IrReceiver.irparams.rawlen - 1; i += 2) {
    bits[bitLen++] =
      decodeBit(IrReceiver.irparams.rawbuf[i] * MICROS_PER_TICK);
  }

  // -------- FRAME FORMAT --------
  // [ SenderID (8) | Timestamp (32) | CRC (8) ]

  uint8_t senderId = 0;
  for (int i = 0; i < 8; i++) {
    senderId = (senderId << 1) | bits[i];
  }

  uint32_t timestamp = 0;
  for (int i = 8; i < 40; i++) {
    timestamp = (timestamp << 1) | bits[i];
  }

  uint8_t recvCrc = 0;
  for (int i = 40; i < 48; i++) {
    recvCrc = (recvCrc << 1) | bits[i];
  }

  // -------- CRC CHECK --------
  uint8_t rawData[5];
  rawData[0] = senderId;
  memcpy(&rawData[1], &timestamp, 4);

  if (crc8(rawData, 5) != recvCrc) {
    Serial.println("CRC FAILED - Frame Rejected");
    IrReceiver.resume();
    return;
  }

  // -------- PAIRING --------
  if (!isPaired(senderId)) {
    addPair(senderId);
    Serial.print("New device paired: 0x");
    Serial.println(senderId, HEX);
  }

  // -------- ACCEPT FRAME --------
  Serial.print("Sender ID : 0x");
  Serial.println(senderId, HEX);

  Serial.print("Timestamp : ");
  Serial.println(timestamp);

  Serial.println("Frame Accepted\n");

  IrReceiver.resume();
}
