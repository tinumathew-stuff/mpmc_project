#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "fedoraa";
const char* password = "Yolo@1234";
const char* udpAddress = "192.168.241.3"; // IP address of the PC running the server
const int udpPort = 5000; // Port to send data to

WiFiUDP udp;

void setup() {
  Serial.begin(9600); // Start Serial communication
  WiFi.begin(ssid, password);

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  udp.begin(udpPort); // Begin listening on the specified port
}

void loop() {
  // Check if there is data from Serial to send over UDP
  if (Serial.available() > 0) {
    String receivedString = Serial.readStringUntil('\n');
    
    // Send the received string over UDP
    udp.beginPacket(udpAddress, udpPort);
    udp.print(receivedString);
    udp.endPacket();
  }

  // Check for incoming UDP packets
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char incomingPacket[255]; // Buffer for incoming data
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = '\0'; // Null-terminate the string
    }
    Serial.print(incomingPacket); // Send data to STM32 via Serial
  }
}
