# Voice control for industrial mockup (TIA Portal PLC)

This project consists of voice controlling an industrial mockup programmed with **Siemens TIA Portal**. The system allows operating the mockup using voice commands dictated from an Android application, which communicates with a hardware gateway based on an **ESP32** and an **Arduino**.

## 🎥 Demonstration
You can watch the system in action in this video:
[![YouTube demonstration](https://img.youtube.com/vi/ovliQjpnDwM/0.jpg)](https://youtube.com/shorts/ovliQjpnDwM?feature=share)
*(Click the image to watch the video)*

## 🛠️ System architecture

The communication flow is as follows:

1. **Android application (MIT App Inventor):**
   The user interacts with the app (or uses manual buttons) and dictates voice commands. The app connects to the ESP32 via Bluetooth.
2. **ESP32 (`0_Codigo_ESP32.cpp`):**
   Receives the text string via Bluetooth. It parses the text looking for keywords (e.g., "alimentar", "abrir", "bajar" in Spanish) and translates them into a numeric code (byte). This code is sent to the Arduino using a custom bit-banging serial protocol through a single wire (pin 12).
3. **Arduino + ENC28J60 (`1_Codigo_Arduino.cpp`):**
   Reads the bit stream sent by the ESP32, reconstructs the byte, and exposes it as a register (Input register 50) on a **Modbus TCP/IP** server.
4. **PLC (TIA Portal):**
   The PLC acts as a Modbus TCP client, reads the register from the Arduino, and executes the corresponding action in the industrial mockup (conveyor belts, grippers, motors, diverters, etc.).

## 📱 Android application
The application was developed using **MIT App Inventor**. It features an intuitive graphical interface with buttons representing the mockup actions and controls to activate voice recognition and Bluetooth connection.

* The source file for the app project is located in the repository: `2_CONTROL_VOZ_PLC_V2.aia` (or similar `.aia` file). You can import it into MIT App Inventor to edit it.

## 💻 TIA Portal project
The original TIA Portal project exceeds 200 MB, so it is hosted externally to avoid overloading the Git repository.
* 🔗 **[Download TIA Portal project (.zip)](https://drive.google.com/file/d/10he3JN_0ZLxJ5LxBvVIu5buYr1D3WUBP/view?usp=sharing)**

## 📂 Repository contents
* `0_Codigo_ESP32.cpp`: ESP32 microcontroller firmware (BT reception and voice parsing).
* `1_Codigo_Arduino.cpp`: Arduino Uno/Mega + Ethernet firmware (Modbus TCP server).
* `*.aia`: Source code and design of the MIT App Inventor application.
