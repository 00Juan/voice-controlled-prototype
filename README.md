# Voice control for an industrial mockup (TIA Portal PLC)

Operating an industrial mockup programmed in **Siemens TIA Portal** with voice commands dictated from an Android phone, **without modifying the PLC logic** beyond giving it a new source of orders.

## Why this exists

A mockup like this is normally operated from an HMI or a row of physical pushbuttons. The question here was whether a new way of interacting with an already-programmed installation could be added without touching its automation: the PLC keeps its program, and only gains one more input to read.

The mockup includes conveyor belts, a pneumatic gripper with rotation, motors with forward and reverse running, a diverter and a retainer.

## 🎥 Demonstration

[![YouTube demonstration](https://img.youtube.com/vi/ovliQjpnDwM/0.jpg)](https://youtube.com/shorts/ovliQjpnDwM?feature=share)
*(Click the image to watch the video)*

## System architecture

1. **Android app (MIT App Inventor)** — a grid of buttons for every action on the mockup, plus voice recognition and a Bluetooth connection button, so the command can be dictated or pressed. The recognised text is sent over Bluetooth to the ESP32.
2. **ESP32** (`0_Codigo_ESP32.cpp`) — scans the text for Spanish keywords (`alime`, `abr`, `cerr`, `otor`, `esvia`, `reten`…) and resolves three values: mode, object acted upon and desired state. They are combined into a single three-digit numeric code.
3. **ESP32 → Arduino** — that code travels over a **custom bit-banged protocol on a single wire** (pin 12). A state machine marks the start of the transmission with a synchronisation pulse, sends the bits one by one with fixed timing, and waits for the Arduino to acknowledge before returning to idle. For communication to exist it is not enough to have a sender, a receiver, a message and a medium: both ends must agree in advance on how what is transmitted should be interpreted, and that is all this protocol is, using the simplest means possible.
4. **Arduino + ENC28J60** (`1_Codigo_Arduino.cpp`) — reconstructs the code bit by bit and exposes it as **input register 50** of a **Modbus TCP** server.
5. **PLC (TIA Portal)** — acts as a Modbus client, polls that register and executes the corresponding action on the physical mockup.

## Repository contents

```
0_Codigo_ESP32.cpp          ESP32 firmware: Bluetooth reception, keyword parsing, bit-banged TX
1_Codigo_Arduino.cpp        Arduino + Ethernet firmware: bit-banged RX and Modbus TCP server
2_CONTROL_VOZ_PLC_V2.aia    MIT App Inventor source of the Android app (importable)
```

## TIA Portal project

The original TIA Portal project exceeds 200 MB, so it is hosted outside the repository:

🔗 **[Download the TIA Portal project (.zip)](https://drive.google.com/file/d/10he3JN_0ZLxJ5LxBvVIu5buYr1D3WUBP/view?usp=sharing)**

## More about the project

[00juan.dev/#/p/voice-controlled-prototype](https://00juan.dev/#/p/voice-controlled-prototype)
