#include <Arduino.h>
#include <IRremote.hpp>

/* -------------- CONFIG ------------------ */
#define IR_RECV_PIN 2
#define MAX_PAIRED  5

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

/* ------------- IR TIMINGS --------------- */
#define BIT_MARK    560
#define ONE_SPACE  1690
#define ZERO_SPACE  560

/* ------------ PAIRED LIST --------------- */
uint8_t paired[MAX_PAIRED];
uint8_t pairedCount = 0;

/* ---------------- CRC ------------------- */
uint8_t crc8(uint8_t *data, uint8_t len) {
  uint8_t crc = 0;
  for (uint8_t i = 0; i < len; i++)
    crc ^= data[i];
  return crc;
}

bool isPaired(uint8_t id) {
  for (uint8_t i = 0; i < pairedCount; i++)
    if (paired[i] == id) return true;
  return false;
}

void addPair(uint8_t id) {
  if (!isPaired(id) && pairedCount < MAX_PAIRED)
    paired[pairedCount++] = id;
}

/* ----------- PULSE → BIT ---------------- */
uint8_t decodeBit(uint16_t space) {
  return (space > 1000) ? 1 : 0;
}

/* ---------------- SETUP ----------------- */
void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println("IR Protocol V2 RX Ready");
}

/* ---------------- LOOP ------------------ */
void loop() {
  if (!irrecv.decode(&results)) return;

  uint8_t bits[64];
  uint8_t bitLen = 0;

  // Skip header (marks/spaces)
  for (uint8_t i = 3; i < results.rawlen - 1; i += 2)
    bits[bitLen++] = decodeBit(results.rawbuf[i] * MICROS_PER_TICK);

  uint8_t senderId = 0;
  for (int i = 0; i < 8; i++)
    senderId = (senderId << 1) | bits[i];

  uint32_t timestamp = 0;
  for (int i = 8; i < 40; i++)
    timestamp = (timestamp << 1) | bits[i];

  uint8_t recvCrc = 0;
  for (int i = 40; i < 48; i++)
    recvCrc = (recvCrc << 1) | bits[i];

  uint8_t rawData[5];
  rawData[0] = senderId;
  memcpy(&rawData[1], &timestamp, 4);

  if (crc8(rawData, 5) != recvCrc) {
    Serial.println("CRC FAILED");
    irrecv.resume();
    return;
  }

  if (!isPaired(senderId)) {
    addPair(senderId);
    Serial.println("New device paired");
  }

  Serial.print("Sender ID: 0x");
  Serial.println(senderId, HEX);
  Serial.print("Timestamp: ");
  Serial.println(timestamp);

  irrecv.resume();
}
