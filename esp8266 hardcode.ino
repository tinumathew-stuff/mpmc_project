#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "YourHotspotSSID";
const char* password = "YourHotspotPassword";
const char* udpAddress = "192.168.43.1"; // IP address of the PC running the server
const int udpPort = 12345; // Port to send data to

WiFiUDP udp;

void setup() {
  Serial.begin(9600); // Start Serial communication
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (Serial.available() > 0) {
    // Read incoming serial data
    String receivedString = Serial.readStringUntil('\n');
    
    // Send the received string over UDP
    udp.beginPacket(udpAddress, udpPort);
    udp.print(receivedString);
    udp.endPacket();
  }
}
