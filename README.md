🛰️ ESP8266 Wi-Fi Range Extender
A functional, DIY Wi-Fi range extender built on the ESP8266. This project uses NAPT (Network Address Port Translation) to bridge connections and eliminate home dead zones, featuring a custom UI for real-time monitoring.

✨ Features
NAPT Bridging: High-speed internet passthrough using the LwIP stack.

Live Dashboard: SSD1306 OLED display showing real-time RSSI (Signal Strength) and connected client count.

RGB Status Indicator: 

🟢 Green: Strong Signal

🟡 Yellow: Weak Signal (RSSI < 40%)

🔴 Red: Connection Lost

Non-blocking Code: Built with millis() logic to ensure UI refreshes don't lag the network traffic.

🛠️ Hardware Requirements
Microcontroller: ESP8266 (NodeMCU or D1 Mini)

Display: SSD1306 OLED (128x64 I2C)

Indicator: RGB LED (Common Cathode)

Resistors: 3x 220Ω (For the RGB legs)

[!IMPORTANT] Power Note: The Fritzing diagram shows a LiPo battery for illustrative purposes. However, for a 24/7 Wi-Fi Extender, it is highly recommended to use a stable 5V USB wall adapter to ensure consistent network performance and avoid battery depletion.

<p align="center"> <img src="WiFi Extender_bb.png" width="600" alt="Circuit Diagram"> </p>

🚀 Setup Instructions
Libraries: Install Adafruit_SSD1306 and Adafruit_GFX.

Configuration: Update STASSID and STAPSK with your network credentials.

Flash: Upload the code to your ESP8266.

Deploy: Place the extender in a location where the RGB LED stays Green for optimal performance.

📜 License
This project is licensed under the MIT License.
