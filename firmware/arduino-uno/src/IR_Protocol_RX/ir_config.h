#ifndef IR_CONFIG_H
#define IR_CONFIG_H

/* ===============================
   PROTOCOL CONSTANTS
   =============================== */

#define IR_PROTOCOL_VERSION   0x02

// Timing (microseconds)
#define IR_PREAMBLE_US        9000
#define IR_MARK_US            560
#define IR_SPACE_0_US         560
#define IR_SPACE_1_US         1680
#define IR_GAP_US             4500

// Security
#define REPLAY_WINDOW_MS      2000
#define EEPROM_PAIR_ADDR      0x00

// GPIO (can be overridden per board)
#define IR_TX_PIN             3
#define IR_RX_PIN             2
#define PAIR_BUTTON_PIN       4

#endif
