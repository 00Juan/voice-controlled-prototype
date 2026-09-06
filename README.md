# Control de Voz para Maqueta Industrial (PLC TIA Portal)

Este proyecto consiste en el control por voz de una maqueta industrial programada con **Siemens TIA Portal**. El sistema permite operar la maqueta utilizando comandos de voz dictados desde una aplicación Android, que se comunica con una pasarela de hardware basada en un **ESP32** y un **Arduino**.

## 🎥 Demostración
Puedes ver el funcionamiento del sistema en este vídeo:
[![Demostración en YouTube](https://img.youtube.com/vi/ovliQjpnDwM/0.jpg)](https://youtube.com/shorts/ovliQjpnDwM?feature=share)
*(Haz clic en la imagen para ver el video)*

## 🛠️ Arquitectura del Sistema

El flujo de comunicación es el siguiente:

1. **Aplicación Android (MIT App Inventor):**
   El usuario interactúa con la aplicación (o usa los botones manuales) y dicta los comandos de voz. La app se conecta por Bluetooth al ESP32.
2. **ESP32 (`0_Codigo_ESP32.cpp`):**
   Recibe el string de texto vía Bluetooth. Parsea el texto buscando palabras clave (ej. "alimentar", "abrir", "cerrar", "subir", "bajar") y lo traduce a un código numérico (byte). Este código se envía al Arduino mediante un protocolo serie custom a través de un solo cable (pin 12).
3. **Arduino + ENC28J60 (`1_Codigo_Arduino.cpp`):**
   Lee la trama de bits enviada por el ESP32, reconstruye el byte y lo expone como un registro (Input Register 50) en un servidor **Modbus TCP/IP**.
4. **PLC (TIA Portal):**
   El autómata actúa como cliente Modbus TCP, lee el registro del Arduino y ejecuta la acción correspondiente en la maqueta industrial (cintas, pinzas, motores, desviadores, etc.).

## 📱 Aplicación Android
La aplicación ha sido desarrollada utilizando **MIT App Inventor**. Dispone de una interfaz gráfica intuitiva con botones representativos de las acciones de la maqueta y controles para activar el reconocimiento de voz y la conexión Bluetooth.

* El archivo fuente del proyecto de la app se encuentra en el repositorio: `2_CONTROL_VOZ_PLC_V2.aia` (o `3_CONTROL_VOZ_PLC_V2.aia`). Puedes importarlo en MIT App Inventor para editarlo.

## 💻 Proyecto TIA Portal
El proyecto original de TIA Portal supera los 200 MB, por lo que está alojado de forma externa para no sobrecargar el repositorio de Git.
* 🔗 **[Descargar Proyecto TIA Portal (.zip)](https://drive.google.com/file/d/10he3JN_0ZLxJ5LxBvVIu5buYr1D3WUBP/view?usp=sharing)**

## 📂 Contenido del Repositorio
* `0_Codigo_ESP32.cpp`: Firmware del microcontrolador ESP32 (Recepción BT y Parseo de Voz).
* `1_Codigo_Arduino.cpp`: Firmware del Arduino Uno/Mega + Ethernet (Servidor Modbus TCP).
* `*.aia`: Código fuente y diseño de la aplicación de MIT App Inventor.
