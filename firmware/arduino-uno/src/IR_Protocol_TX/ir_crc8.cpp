#include "ir_crc8.h"

uint8_t crc8(const uint8_t *data, uint8_t length) {
  uint8_t crc = 0x00;
  while (length--) {
    crc ^= *data++;
    for (uint8_t i = 0; i < 8; i++) {
      crc = (crc & 0x01) ? (crc >> 1) ^ 0x8C : (crc >> 1);
    }
  }
  return crc;
}
