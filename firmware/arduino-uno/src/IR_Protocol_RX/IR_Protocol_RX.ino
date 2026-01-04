#include <EEPROM.h>
#include "ir_rx_core.h"
#include "ir_config.h"

/* ===============================
   APPLICATION COMMAND DEFINITIONS
   =============================== */
#define CMD_TURN_ON   0x10
#define CMD_TURN_OFF  0x11
#define CMD_TOGGLE    0x12
#define CMD_STATUS    0x13

/* ===============================
   PAIRING CONTROL
   =============================== */
#define CLEAR_PRESS_MS 10000   // long press duration

unsigned long buttonPressTime = 0;
bool buttonHeld = false;

const char* commandToString(uint8_t cmd) {
  switch (cmd) {
    case CMD_TURN_ON:  return "TURN ON";
    case CMD_TURN_OFF: return "TURN OFF";
    case CMD_TOGGLE:   return "TOGGLE";
    case CMD_STATUS:   return "STATUS";
    default:           return "UNKNOWN";
  }
}

/* ===============================
   SETUP
   =============================== */
void setup() {
  pinMode(IR_RX_PIN, INPUT);
  pinMode(PAIR_BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  Serial.println("IR-Blaster-Protocol-V2.0 Receiver Ready");
}

/* ===============================
   MAIN LOOP
   =============================== */
void loop() {

  /* ===============================
     BUTTON HANDLING (PAIR / CLEAR)
     =============================== */
  if (digitalRead(PAIR_BUTTON_PIN) == LOW) {
    if (!buttonHeld) {
      buttonHeld = true;
      buttonPressTime = millis();
    }

    // ---- Long press → Clear pairing ----
    if (millis() - buttonPressTime >= CLEAR_PRESS_MS) {
      EEPROM.write(EEPROM_PAIR_ADDR, 0xFF);
      Serial.println("[PAIRING] Cleared successfully");
      delay(1000);   // debounce
    }
    return;
  } else {
    buttonHeld = false;
  }

  /* ===============================
     RECEIVE FRAME
     =============================== */
  IRFrame frame;
  if (!irReceiveFrame(frame)) return;

  /* ---- Short press → Pair sender ---- */
  if (digitalRead(PAIR_BUTTON_PIN) == LOW) {
    EEPROM.write(EEPROM_PAIR_ADDR, frame.senderID);
    Serial.println("[PAIRING] Sender paired successfully");
    delay(1000);   // debounce
    return;
  }

  /* ---- Authorization Check ---- */
  if (EEPROM.read(EEPROM_PAIR_ADDR) != frame.senderID) {
    Serial.println("[SECURITY] Frame rejected (unpaired sender)");
    return;
  }

  /* ===============================
     METADATA DUMP
     =============================== */
  Serial.println("\n========== IR FRAME ==========");

  Serial.print("Protocol Version : 0x");
  Serial.println(frame.version, HEX);

  Serial.print("Flags            : 0x");
  Serial.println(frame.flags, HEX);

  Serial.print("Sender ID        : 0x");
  Serial.println(frame.senderID, HEX);

  Serial.print("Device ID        : 0x");
  Serial.println(frame.deviceID, HEX);

  Serial.print("Command          : ");
  Serial.print(commandToString(frame.command));
  Serial.print(" (0x");
  Serial.print(frame.command, HEX);
  Serial.println(")");

  Serial.print("Timestamp        : ");
  Serial.print(frame.timestamp);
  Serial.println(" ms");

  Serial.print("CRC              : 0x");
  Serial.println(frame.crc, HEX);

  Serial.println("Status           : AUTHORIZED & VALID");
  Serial.println("==============================");
}
