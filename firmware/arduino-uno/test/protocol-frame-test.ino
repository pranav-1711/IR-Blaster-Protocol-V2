/*
  IR Blaster – Frame Test
  Board   : Arduino UNO
  Library : IRremote (v4.x)
  IR LED  : Pin 3
*/

#include <Arduino.h>
#include <IRremote.hpp>

#define IR_SEND_PIN 3

IRsend irsend;

/* ---------- CRC8 (XOR based) ---------- */
uint8_t crc8(const uint8_t *data, uint8_t len) {
  uint8_t crc = 0;
  for (uint8_t i = 0; i < len; i++) {
    crc ^= data[i];
  }
  return crc;
}

/* ---------- FRAME TEST ---------- */
void sendFrameTest() {
  // Frame format:
  // [START][DEVICE][COMMAND][VALUE][CRC]
  uint8_t frame[5];

  frame[0] = 0xAA;   // Start byte
  frame[1] = 0x01;   // Device ID
  frame[2] = 0x10;   // Command
  frame[3] = 0x05;   // Value
  frame[4] = crc8(frame, 4); // CRC

  Serial.print("Sending Frame: ");
  for (int i = 0; i < 5; i++) {
    Serial.print("0x");
    Serial.print(frame[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Convert frame bytes to raw IR timings
  uint16_t rawIR[5 * 16];
  uint16_t idx = 0;

  for (int b = 0; b < 5; b++) {
    for (int bit = 7; bit >= 0; bit--) {
      rawIR[idx++] = (frame[b] & (1 << bit)) ? 1600 : 800; // MARK
      rawIR[idx++] = 600;                                 // SPACE
    }
  }

  irsend.sendRaw(rawIR, idx, 38); // 38 kHz carrier
}

void setup() {
  Serial.begin(9600);
  irsend.begin(IR_SEND_PIN);

  Serial.println("IR Frame Test Started");
}

void loop() {
  sendFrameTest();
  delay(3000); // send every 3 seconds
}
