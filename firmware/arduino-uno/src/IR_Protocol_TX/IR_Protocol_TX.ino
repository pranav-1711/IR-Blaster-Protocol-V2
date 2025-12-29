#include <Arduino.h>
#include <IRremote.hpp>

/* ---------------- CONFIG ---------------- */
#define IR_SEND_PIN 3
#define SENDER_ID   0x42

/* ------------- IR TIMINGS --------------- */
#define HDR_MARK   9000
#define HDR_SPACE  4500
#define BIT_MARK    560
#define ONE_SPACE  1690
#define ZERO_SPACE  560
#define TRAILER     560

IRsend irsend;

/* ----------- METADATA FRAME ------------- */
struct Frame {
  uint8_t  senderId;      // 8 bits
  uint32_t timestamp;     // 32 bits
  uint8_t  crc;           // 8 bits
};

/* ---------------- CRC ------------------- */
uint8_t crc8(uint8_t *data, uint8_t len) {
  uint8_t crc = 0;
  for (uint8_t i = 0; i < len; i++)
    crc ^= data[i];
  return crc;
}

/* -------- FRAME → BITSTREAM ------------- */
void frameToBits(Frame &f, uint8_t *bits, uint8_t &bitLen) {
  bitLen = 0;

  for (int i = 7; i >= 0; i--)
    bits[bitLen++] = (f.senderId >> i) & 1;

  for (int i = 31; i >= 0; i--)
    bits[bitLen++] = (f.timestamp >> i) & 1;

  for (int i = 7; i >= 0; i--)
    bits[bitLen++] = (f.crc >> i) & 1;
}

/* -------- BITS → IR PULSES -------------- */
void bitsToPulses(uint8_t *bits, uint8_t bitLen,
                  uint16_t *pulses, uint16_t &pulseLen) {

  pulseLen = 0;
  pulses[pulseLen++] = HDR_MARK;
  pulses[pulseLen++] = HDR_SPACE;

  for (uint8_t i = 0; i < bitLen; i++) {
    pulses[pulseLen++] = BIT_MARK;
    pulses[pulseLen++] = bits[i] ? ONE_SPACE : ZERO_SPACE;
  }

  pulses[pulseLen++] = TRAILER;
}

/* ---------------- SETUP ----------------- */
void setup() {
  Serial.begin(9600);
  irsend.begin(IR_SEND_PIN);
  Serial.println("IR Protocol V2 TX Ready");
}

/* ---------------- LOOP ------------------ */
void loop() {
  Frame f;
  f.senderId = SENDER_ID;
  f.timestamp = millis();

  uint8_t rawData[5];
  rawData[0] = f.senderId;
  memcpy(&rawData[1], &f.timestamp, 4);
  f.crc = crc8(rawData, 5);

  uint8_t bits[64];
  uint8_t bitLen;
  frameToBits(f, bits, bitLen);

  uint16_t pulses[200];
  uint16_t pulseLen;
  bitsToPulses(bits, bitLen, pulses, pulseLen);

  irsend.sendRaw(pulses, pulseLen, 38);

  Serial.println("Metadata frame sent");
  delay(2000);
}
