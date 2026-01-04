#ifndef IR_FRAME_H
#define IR_FRAME_H

#include <Arduino.h>

/* ===============================
   FRAME FLAGS
   =============================== */
enum IRFlags : uint8_t {
  FLAG_PAIRING   = 0x01,
  FLAG_ACK_REQ   = 0x02,
  FLAG_BROADCAST = 0x04
};

/* ===============================
   FRAME STRUCTURE
   =============================== */
struct IRFrame {
  uint8_t  version;
  uint8_t  flags;
  uint8_t  senderID;
  uint8_t  deviceID;
  uint8_t  command;
  uint16_t timestamp;
  uint8_t  crc;
};

#endif
