# IR-Blaster-Protocol-V2.0

IR-Blaster-Protocol-V2.0 is a structured infrared (IR) communication protocol that extends traditional IR signaling by introducing **pairing-based authentication**, **metadata embedding**, and **frame-level integrity validation**.

Unlike conventional IR remotes that transmit only command codes, this protocol enables **identity-aware, time-aware IR communication**, making it suitable for future **IoT bridging**, **device federation**, and **secure control environments**.



## 🚀 Key Features

- 📡 Metadata-enabled IR frames (Sender ID + Timestamp)
- 🔐 Pairing-based authentication mechanism
- 🧪 CRC-16 frame integrity verification
- 🧩 Modular protocol design
- 🔌 Arduino-Uno compatible
- 🌐 Future-ready for IoT gateway integration


## 📁 Project Structure

```
docs/
├── protocol-specification.md
├── pairing-authentication.md
├── metadata-format.md
├── system-architecture.md
├── hardware-setup.md
├── pin-connections.md
└── future-enhancements.md

firmware/
├── test/
└── arduino-uno/src/
   └── IR_Protocol_RX
   └── IR_Protocol_TX
```

## 🧠 How It Works

1. A transmitter constructs an IR frame containing:
   - Sender identity
   - Transmission timestamp
   - Payload data
   - CRC checksum
2. The frame is transmitted using a 38 kHz IR carrier.
3. The receiver:
   - Validates pairing
   - Verifies CRC integrity
   - Extracts metadata and payload
4. Only trusted devices are allowed to communicate.

## 🔧 Hardware Requirements

- Arduino Uno R3 (×2)
- IR LED + 220 Ω resistor
- TSOP1738 IR Receiver (or equivalent)
- Breadboard & jumper wires

## 📜 Use Cases

- Secure IR-controlled appliances
- Device-to-device authentication
- IR-based IoT gateways
- Smart classrooms & labs
- Research and academic demonstrations

## 🧪 Testing

A standalone protocol frame test is available:

firmware/arduino-uno/test/protocol-frame-test.ino

## 📚 Documentation

All protocol details, diagrams, and design explanations are available in the `docs/` directory.

## 📄 License

This project is released under the MIT License.
