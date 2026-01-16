# Pairing & Authentication

The **IR-Blaster-Protocol-V2.0** uses a **pairing-based authentication mechanism** to ensure that only authorized infrared transmitters can control a device. Unlike traditional IR remotes, which accept commands from any sender, this system verifies the sender’s identity before executing any command.

---

## Overview of Pairing

![Pairing Overview](img/ss47.png)

Pairing is a **one-time process** where the receiver learns and stores the identity of a trusted sender. Once paired, the receiver will accept commands only from that sender.

---

## Why Pairing Is Required

![Why Pairing](img/ss48.png)

Traditional IR communication:
- Has no sender identification
- Accepts commands from any remote
- Is vulnerable to accidental or unauthorized control

The pairing mechanism solves these issues by introducing **identity verification**.

---

## Pairing Process

![Pairing Process](img/ss49.png)
![Pairing Process](img/ss50.png)

The pairing process follows these steps:

1. User presses the **pairing button** on the receiver
2. Receiver enters **pairing mode**
3. A valid IR frame is received
4. Sender ID is extracted from metadata
5. Sender ID is stored in EEPROM
6. Receiver exits pairing mode

---

## Pairing Button Role

![Pairing Button](img/ss51.png)
![Pairing Button](img/ss52.png)
![Pairing Button](img/ss53.png)

- The pairing button is connected to the receiver Arduino
- When pressed, it enables pairing mode
- Prevents accidental pairing
- Requires physical access to the device

---

## Authentication During Normal Operation

![Authentication Flow](img/ss54.png)

After pairing:
- Receiver extracts the sender ID from every IR frame
- Sender ID is compared with stored paired ID
- If IDs match → command is accepted
- If IDs do not match → command is rejected

---

## Authentication Decision Logic

![Authentication Decision](img/ss55.png)


This logic ensures secure control without complex encryption.

---

## Security Advantages

![Security Advantages](img/ss56.png)

- Prevents unauthorized IR remotes
- Protects against accidental triggers
- Works offline without cloud or gateway
- Requires minimal hardware

---

## Future Authentication Enhancements

Future versions of the protocol may include:
- Challenge–response authentication
- Time-based pairing windows
- Rolling authentication tokens
- Encrypted metadata fields

These enhancements can be added without changing the basic protocol structure.

---

## Summary

The pairing and authentication mechanism transforms infrared communication into a **secure and controlled system**. By verifying sender identity locally and enforcing authorization at the device level, IR-Blaster-Protocol-V2.0 achieves secure operation while remaining simple, efficient, and gateway-less.
