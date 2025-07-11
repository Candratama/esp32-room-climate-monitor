/*
 * RS485 Communication Test for XY-MD02 Sensor
 * This is a simplified test to verify RS485 communication
 * Upload this to test your wiring before using the full monitoring system
 */

#include <Arduino.h>

// Pin definitions (same as your main project)
#define RS485_RX_PIN 16     // GPIO16 - Connect to RXD of RS485 module
#define RS485_TX_PIN 17     // GPIO17 - Connect to TXD of RS485 module

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);
  
  Serial.println("RS485 Communication Test Starting...");
  Serial.println("This will test communication with XY-MD02 sensor");
  delay(2000);
}

void loop() {
  // XY-MD02 command to read temperature and humidity
  uint8_t command[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x0B};
  
  Serial.println("\n--- Sending Command ---");
  Serial.print("Command: ");
  for (int i = 0; i < 8; i++) {
    Serial.printf("%02X ", command[i]);
  }
  Serial.println();
  
  // Send command
  Serial2.write(command, sizeof(command));
  Serial2.flush();
  
  // Wait for response
  Serial.println("Waiting for response...");
  unsigned long startTime = millis();
  int timeout = 2000; // 2 second timeout
  
  while (Serial2.available() == 0 && millis() - startTime < timeout) {
    delay(10);
  }
  
  if (Serial2.available() > 0) {
    Serial.printf("Received %d bytes after %lu ms:\n", Serial2.available(), millis() - startTime);
    Serial.print("Response: ");
    
    while (Serial2.available()) {
      uint8_t byte = Serial2.read();
      Serial.printf("%02X ", byte);
    }
    Serial.println();
    
    Serial.println("✅ Communication successful!");
  } else {
    Serial.println("❌ No response from sensor");
    Serial.println("Check:");
    Serial.println("- Wiring (VCC, GND, TXD, RXD)");
    Serial.println("- Sensor power supply (5-24V)");
    Serial.println("- Sensor address (default 0x01)");
    Serial.println("- RS485 A+/B- connections");
  }
  
  delay(5000); // Wait 5 seconds before next test
}
