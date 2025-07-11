/*
 * RS485 Communication Test for XY-MD02 Sensor
 * This is a simplified test to verify RS485 communication
 * Upload this to test your wiring before using the full monitoring system
 */

#include <Arduino.h>

// Function declarations
void testSensor(uint8_t address);
void loopbackTest();
uint16_t calculateCRC(uint8_t *data, uint8_t length);

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
  // Test 1: XY-MD02 command with address 0x01
  Serial.println("\n=== Test 1: XY-MD02 Address 0x01 ===");
  testSensor(0x01);
  
  delay(2000);
  
  // Test 2: Try address 0xFF (broadcast)
  Serial.println("\n=== Test 2: Broadcast Address 0xFF ===");
  testSensor(0xFF);
  
  delay(2000);
  
  // Test 3: Try different common addresses
  Serial.println("\n=== Test 3: Address 0x02 ===");
  testSensor(0x02);
  
  delay(2000);
  
  // Test 4: Simple loopback test
  Serial.println("\n=== Test 4: Loopback Test ===");
  loopbackTest();
  
  delay(10000); // Wait 10 seconds before next round
}

void testSensor(uint8_t address) {
  // Calculate CRC for the command
  uint8_t command[6] = {address, 0x03, 0x00, 0x00, 0x00, 0x02};
  uint16_t crc = calculateCRC(command, 6);
  
  uint8_t fullCommand[8];
  memcpy(fullCommand, command, 6);
  fullCommand[6] = crc & 0xFF;
  fullCommand[7] = (crc >> 8) & 0xFF;
  
  Serial.printf("Trying sensor address: 0x%02X\n", address);
  Serial.print("Command: ");
  for (int i = 0; i < 8; i++) {
    Serial.printf("%02X ", fullCommand[i]);
  }
  Serial.println();
  
  // Clear any existing data
  while (Serial2.available()) {
    Serial2.read();
  }
  
  // Send command
  Serial2.write(fullCommand, sizeof(fullCommand));
  Serial2.flush();
  
  // Wait for response
  Serial.println("Waiting for response...");
  unsigned long startTime = millis();
  int timeout = 2000;
  
  while (Serial2.available() == 0 && millis() - startTime < timeout) {
    delay(10);
  }
  
  if (Serial2.available() > 0) {
    Serial.printf("✅ Received %d bytes after %lu ms:\n", Serial2.available(), millis() - startTime);
    Serial.print("Response: ");
    
    while (Serial2.available()) {
      uint8_t byte = Serial2.read();
      Serial.printf("%02X ", byte);
    }
    Serial.println();
  } else {
    Serial.printf("❌ No response from address 0x%02X\n", address);
  }
}

void loopbackTest() {
  Serial.println("Testing if data can be sent and received...");
  
  // Send a simple test pattern
  uint8_t testData[] = {0xAA, 0x55, 0xAA, 0x55};
  Serial.print("Sending test pattern: ");
  for (int i = 0; i < 4; i++) {
    Serial.printf("%02X ", testData[i]);
  }
  Serial.println();
  
  Serial2.write(testData, sizeof(testData));
  Serial2.flush();
  
  delay(100);
  
  if (Serial2.available() > 0) {
    Serial.println("✅ Received data back (possible loopback or echo):");
    Serial.print("Data: ");
    while (Serial2.available()) {
      uint8_t byte = Serial2.read();
      Serial.printf("%02X ", byte);
    }
    Serial.println();
  } else {
    Serial.println("❌ No loopback detected");
  }
}

// Simple CRC calculation for Modbus
uint16_t calculateCRC(uint8_t *data, uint8_t length) {
  uint16_t crc = 0xFFFF;
  for (uint8_t i = 0; i < length; i++) {
    crc ^= data[i];
    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x0001) {
        crc = (crc >> 1) ^ 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  return crc;
}
