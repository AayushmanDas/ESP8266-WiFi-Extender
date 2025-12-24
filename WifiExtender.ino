#include <ESP8266WiFi.h>
#include <lwip/napt.h>
#include <lwip/dns.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Screen size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// WIFI SETUP
#define STASSID "REPLACE_WITH_YOUR_SSID" // SSID IS JUST YOUR NETWORK'S NAME
#define STAPSK  "REPLACE_WITH_YOUR_PASSWORD"

// AP SETUP
#define AP_NAME "ESP8266_Wifi_Extender" // You can change this to be whatever you want the network name to be
#define AP_PASS "extender8266" // You can also change this to set the password for th extender

//LED SETUP
#define RED_LED D5
#define GREEN_LED D6
#define BLUE_LED D7

// NAPT Configuration
#define NAPT 1000
#define NAPT_PORT 10

void setup() {

  // Flash button
  pinMode(0, INPUT_PULLUP);

  // LED buttons
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  Serial.begin(115200);
  Serial.println("\nStarting NAPT Extender...");

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawRect(0, 0, 128, 64, WHITE);

  // Connect to Upstairs Router
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    display.setCursor(10, 10);
    display.print("...");
    display.display();
    delay(500);
  }

  Serial.println("\nConnected to Upstairs Router!");
  Serial.print("My IP: ");
  Serial.println(WiFi.localIP());

  // Fix DNS part smth smth
  auto& server = WiFi.softAPDhcpServer();
  server.setDns(WiFi.dnsIP(0));

  // Create the downstairs SoftAP
  WiFi.softAPConfig(
    IPAddress(172, 217, 28, 254), 
    IPAddress(172, 217, 28, 254), 
    IPAddress(255, 255, 255, 0)
  );
  
  // Set the Wifi name and password for the extender
  WiFi.softAP(AP_NAME, AP_PASS);
  Serial.print("Downstairs AP Created: ");
  Serial.println(WiFi.softAPIP());

  // Enable the "Bridge" (NAPT)
  ip_napt_init(NAPT, NAPT_PORT);
  ip_napt_enable_no(SOFTAP_IF, 1);
  Serial.println("NAPT Bridge Enabled. Internet is flowing!");
}

void setLED(int R, int G, int B) {
  // Set led colours
  analogWrite(RED_LED, R);
  analogWrite(GREEN_LED, G);
  analogWrite(BLUE_LED, B);
}

void updateUI() {

  display.clearDisplay();
  display.drawRect(0, 0, 128, 64, WHITE);
  static bool isInverted = false;

  if (WiFi.status() != WL_CONNECTED) {

    // If connection is lost for some reason
    display.setCursor(20, 30);
    display.print("Connection Lost!");
    isInverted = !isInverted;
    display.invertDisplay(isInverted);
    setLED(400, 0, 0); // red light

  } else {
    // Make sure we dont give the person a seizure needlessly
    display.invertDisplay(false);

    //Access the required data
    long rssi = WiFi.RSSI(); 
    int strength = constrain(map(rssi, -100, -30, 0, 100), 0, 100);
    int clients = WiFi.softAPgetStationNum();

    // You knpww what time it is, disco time!
    if (strength < 40) {
      setLED(300, 70, 0); // yellow light
    } else {
      setLED(0, 200, 0); // green light
    }

    // UI stuff
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(25, 5);
    display.print("WIFI EXTENDER"); 
    display.drawLine(0, 15, 128, 15, WHITE);

    display.setCursor(10, 22);
    display.print("Network : ");
    display.print(STASSID);

    // The signal bar
    display.drawRect(10, 32, 102, 10, WHITE); 
    display.fillRect(11, 33, strength, 8, WHITE); 

    // Client info
    display.setCursor(10, 50);
    display.print("Users: ");
    display.print(clients);
  
    display.setCursor(80, 50);
    display.print(strength);
    display.print("%");

  }

  display.display();
}

void loop() {

  // Press flash button if the screen glitches or smth
  if (digitalRead(0) == LOW) {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setCursor(20, 20);
    display.print("Resetting...");
    display.display();
    delay(500);
  }

  static unsigned long lastRefresh = 0;
  const int interval = 1000;

  if (millis() - lastRefresh >= interval) {
    lastRefresh = millis();
    updateUI();
  }

}