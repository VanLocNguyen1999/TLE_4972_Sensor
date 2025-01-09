/*
 * main.h
 *
 *  Created on: Dec 30, 2024
 *      Author: vanlo
 */

#ifndef APP_MAIN_H_
#define APP_MAIN_H_

//
#include <Arduino.h>

 #define SICI_PIN 				7
 #define RELAY_OCD2_PIN 		8 // Relay for controlling 20.6V for OCD2
 #define RELAY_PROGRAM_PIN 		9 // Relay for controlling programming voltage (e.g., EEPROMprogramming)
#define MEASRNG_VALUE 0x18 // S6: Low sensitivity
#define OPMODE_VALUE 0x03 // SE: Single-ended mode
#define OCD1DEGLITCH_VALUE 0x01 // 500 ns
#define OCD2DEGLITCH_VALUE 0x01 // 500 ns
#define OCD1_ENABLE 0x01 // Enable OCD1
#define OCD2_ENABLE 0x01 // Enable OCD2
// Command definitions
#define ENTER_INTERFACE_CMD 0xABCD
#define DISABLE_FAILURE_INDICATION_CMD 0x8010
#define READ_COMMAND 0x0400
#define EEPROM_SET_ALL_ZEROS_CMD 0x83E0
#define EEPROM_PROGRAM_ZEROS_DATA 0x024E
#define WRITE_COMMAND 0x024F
#define EEPROM_REFRESH_CMD 0x024C
void sendCommand(uint16_t command);
uint16_t readResponse();
void applyOCD2Voltage(bool enable);
void applyProgrammingVoltage(bool enable);
uint8_t calculateCRC5(uint16_t data) {
	uint8_t crc = 0x1F; // Initial CRC value
	for (int i = 0; i < 16; ++i) {
		if ((data & 0x8000) ^ (crc & 0x80)) {
			crc = (crc << 1) ^ 0x13;
		} else {
			crc <<= 1;
		}
		data <<= 1;
	}
	return crc >> 3; // Keep only the 5 bits
}
void setup() {

	Serial.begin(9600);

	pinMode(SICI_PIN, OUTPUT);

	pinMode(RELAY_OCD2_PIN, OUTPUT);

	pinMode(RELAY_PROGRAM_PIN, OUTPUT);

// Initialize pins to default states
	digitalWrite(SICI_PIN, LOW);
	digitalWrite(RELAY_OCD2_PIN, LOW); // Ensure OCD2 voltage is off
	digitalWrite(RELAY_PROGRAM_PIN, LOW); // Ensure programming voltage is off


	// Power on the 5V supply (Sensor will remain ON)
	// No relay control needed for 5V as it's constant
	// Wait precisely for 300 µs after power-on
	delayMicroseconds(300);
	// Apply programming voltage to OCD2 pin
	applyOCD2Voltage(true);
	delay(100); // Wait for voltage stabilization
	// Enter interface mode within the required timing window
	sendCommand(ENTER_INTERFACE_CMD);
	delay(100);
	// Disable failure indication
	sendCommand(DISABLE_FAILURE_INDICATION_CMD);
	delay(100);
	// Apply programming voltage and set new EEPROM values
	applyProgrammingVoltage(true);
	delay(100);
	// Build the new EEPROM value
	uint16_t newValue = (MEASRNG_VALUE & 0x1F) | ((OPMODE_VALUE & 0x03) << 5)
			| ((OCD1DEGLITCH_VALUE & 0x07) << 7)
			| ((OCD2DEGLITCH_VALUE & 0x0F) << 10) | (OCD1_ENABLE << 14)
			| (OCD2_ENABLE << 15);
	// Set all EEPROM to zeros
	sendCommand(EEPROM_SET_ALL_ZEROS_CMD);
	delay(100);
	sendCommand(EEPROM_PROGRAM_ZEROS_DATA);
	delay(100);
	// Program new values
	sendCommand(WRITE_COMMAND | newValue);
	delay(100);
	applyProgrammingVoltage(false); // Disable programming voltage
	applyOCD2Voltage(false); // Disable OCD2 voltage
	// Refresh EEPROM
	sendCommand(EEPROM_REFRESH_CMD);
	delay(100);
	// Read back and verify the EEPROM value
	sendCommand(READ_COMMAND);
	delay(100);
	uint16_t response = readResponse();
	Serial.print("Updated EEPROM value: ");
	Serial.println(response, HEX);
}
 void loop() {
 // Main loop (if needed)
 }
void sendCommand(uint16_t command) {

	uint8_t commandCRC = calculateCRC5(command);
	Serial.write((command >> 8) & 0xFF); // Send high byte
	Serial.write(command & 0xFF);
	Serial.write(commandCRC);
}
uint16_t readResponse() {
	// Send low byte
	// Send CRC
	while (Serial.available() < 3) {
	} // Wait for 3 bytes (2 data + 1 CRC)
	uint8_t highByte = Serial.read();
	uint8_t lowByte = Serial.read();
	uint8_t receivedCRC = Serial.read();
	uint16_t response = (highByte << 8) | lowByte;
	uint8_t calculatedCRC = calculateCRC5(response);
	if (calculatedCRC == receivedCRC) {
		return response;
	} else {
		Serial.println("CRC error!");
		return 0; // Return 0 or handle error as needed
	}
 }
void applyOCD2Voltage(bool enable) {
	if (enable) {
		digitalWrite(RELAY_OCD2_PIN, HIGH); // Enable OCD2 voltage relay
		delay(30);
	} else {
		// Wait for 30 milliseconds to ensure stable voltage
		digitalWrite(RELAY_OCD2_PIN, LOW); // Ensure OCD2 voltage relay is disabled
	}
 }
void applyProgrammingVoltage(bool enable) {
	if (enable) {
		digitalWrite(RELAY_PROGRAM_PIN, HIGH); // Enable programming voltage relay
		delay(30);
		// Wait for 30 milliseconds to ensure stable programming voltage
	} else {
		digitalWrite(RELAY_PROGRAM_PIN, LOW); // Ensure programming voltage relay is disabled
	}
}


#endif /* APP_MAIN_H_ */
