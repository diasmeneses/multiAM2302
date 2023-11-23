#include <Arduino.h>
#include <Ticker.h>
#include <WiFi.h>
#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp
#include "ThingSpeak.h"

#ifndef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP32 ONLY!)
#error Select ESP32 board.
#endif

#define EAP_IDENTITY "dias.meneses" //if connecting from another corporation, use identity@organisation.domain in Eduroam 
#define EAP_USERNAME "dias.meneses" //oftentimes just a repeat of the identity
#define EAP_PASSWORD "password" //your Eduroam password
const char* ssid = "eduroam"; // Eduroam SSID
const char* host = "arduino.php5.sk"; //external server domain for HTTP connection after authentification
int counter = 0;


/** Initialize DHT sensor 1 */
DHTesp dhtSensor1;
/** Initialize DHT sensor 2 */
DHTesp dhtSensor2;
/** Initialize DHT sensor 3 */
DHTesp dhtSensor3;
/** Initialize DHT sensor 4 */
DHTesp dhtSensor4;
/** Initialize DHT sensor 5 */
DHTesp dhtSensor5;
/** Initialize DHT sensor 6 */
DHTesp dhtSensor6;
/** Initialize DHT sensor 7 */
DHTesp dhtSensor7;
/** Initialize DHT sensor 8 */
DHTesp dhtSensor8;
/** Initialize DHT sensor 9 */
DHTesp dhtSensor9;
/** Initialize DHT sensor 10 */
DHTesp dhtSensor10;

/** Task handle for the light value read task */
TaskHandle_t tempTaskHandle = NULL;
/** Pin number for DHT22 1 data pin */
int dhtPin1 = 5;
/** Pin number for DHT22 2 data pin */
int dhtPin2 = 6;
/** Pin number for DHT22 3 data pin */
int dhtPin3 = 10;
/** Pin number for DHT22 1 data pin */
int dhtPin4 = 11;
/** Pin number for DHT22 2 data pin */
int dhtPin5 = 12;
/** Pin number for DHT22 3 data pin */
int dhtPin6 = 13;
/** Pin number for DHT22 1 data pin */
int dhtPin7 = 14;
/** Pin number for DHT22 2 data pin */
int dhtPin8 = 15;
/** Pin number for DHT22 3 data pin */
int dhtPin9 = 16;
/** Pin number for DHT22 3 data pin */
int dhtPin10 = 17;

/** Ticker for temperature reading */
Ticker tempTicker;
/** Flags for temperature readings finished */
bool gotNewTemperature = false;
/** Data from sensor 1 */
TempAndHumidity sensor1Data;
/** Data from sensor 2 */
TempAndHumidity sensor2Data;
/** Data from sensor 3 */
TempAndHumidity sensor3Data;
/** Data from sensor 4 */
TempAndHumidity sensor4Data;
/** Data from sensor 5 */
TempAndHumidity sensor5Data;
/** Data from sensor 6 */
TempAndHumidity sensor6Data;
/** Data from sensor 7 */
TempAndHumidity sensor7Data;
/** Data from sensor 8 */
TempAndHumidity sensor8Data;
/** Data from sensor 9 */
TempAndHumidity sensor9Data;
/** Data from sensor 10 */
TempAndHumidity sensor10Data;

/* Flag if main loop is running */
bool tasksEnabled = false;



/**
 * Task to reads temperature from DHT22 sensor
 * @param pvParameters
 *		pointer to task parameters
 */
void tempTask(void *pvParameters) {
	Serial.println("tempTask loop started");
	while (1) // tempTask loop
	{
		if (tasksEnabled && !gotNewTemperature) { // Read temperature only if old data was processed already
			// Reading temperature for humidity takes about 250 milliseconds!
			// Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
			sensor1Data = dhtSensor1.getTempAndHumidity();	// Read values from sensor 1
			sensor2Data = dhtSensor2.getTempAndHumidity();	// Read values from sensor 1
			sensor3Data = dhtSensor3.getTempAndHumidity();	// Read values from sensor 1
			sensor4Data = dhtSensor4.getTempAndHumidity();	// Read values from sensor 1
			sensor5Data = dhtSensor5.getTempAndHumidity();	// Read values from sensor 1
			sensor6Data = dhtSensor6.getTempAndHumidity();	// Read values from sensor 1
			sensor7Data = dhtSensor7.getTempAndHumidity();	// Read values from sensor 1
			sensor8Data = dhtSensor8.getTempAndHumidity();	// Read values from sensor 1
			sensor9Data = dhtSensor9.getTempAndHumidity();	// Read values from sensor 1
			sensor10Data = dhtSensor10.getTempAndHumidity();	// Read values from sensor 1
			gotNewTemperature = true;
		}
		vTaskSuspend(NULL);
	}
}

/**
 * triggerGetTemp
 * Sets flag dhtUpdated to true for handling in loop()
 * called by Ticker tempTicker
 */
void triggerGetTemp() {
	if (tempTaskHandle != NULL) {
		 xTaskResumeFromISR(tempTaskHandle);
	}
}

/**
 * Arduino setup function (called once after boot/reboot)
 */
void setup() {
	Serial.begin(115200);
	Serial.println("Example for 3 DHT11/22 sensors");

	// Initialize temperature sensor 1
	dhtSensor1.setup(dhtPin1, DHTesp::DHT22);
	// Initialize temperature sensor 2
	dhtSensor2.setup(dhtPin2, DHTesp::DHT22);
	// Initialize temperature sensor 3
	dhtSensor3.setup(dhtPin3, DHTesp::DHT22);
	// Initialize temperature sensor 1
	dhtSensor4.setup(dhtPin4, DHTesp::DHT22);
	// Initialize temperature sensor 2
	dhtSensor5.setup(dhtPin5, DHTesp::DHT22);
	// Initialize temperature sensor 3
	dhtSensor6.setup(dhtPin6, DHTesp::DHT22);
	// Initialize temperature sensor 1
	dhtSensor7.setup(dhtPin7, DHTesp::DHT22);
	// Initialize temperature sensor 2
	dhtSensor8.setup(dhtPin8, DHTesp::DHT22);
	// Initialize temperature sensor 3
	dhtSensor9.setup(dhtPin9, DHTesp::DHT22);
	// Initialize temperature sensor 3
	dhtSensor10.setup(dhtPin10, DHTesp::DHT22);

	// Start task to get temperature
	xTaskCreatePinnedToCore(
			tempTask,											 /* Function to implement the task */
			"tempTask ",										/* Name of the task */
			4000,													 /* Stack size in words */
			NULL,													 /* Task input parameter */
			5,															/* Priority of the task */
			&tempTaskHandle,								/* Task handle. */
			1);														 /* Core where the task should run */

	if (tempTaskHandle == NULL) {
		Serial.println("[ERROR] Failed to start task for temperature update");
	} else {
		// Start update of environment data every 30 seconds
		tempTicker.attach(30, triggerGetTemp);
	}

	// Signal end of setup() to tasks
	tasksEnabled = true;
} // End of setup.


/**
 * loop
 * Arduino loop function, called once 'setup' is complete (your own code
 * should go here)
 */
void loop() {
	if (gotNewTemperature) {
		Serial.println("Sensor 1 data:");
		Serial.println("Temp: " + String(sensor1Data.temperature,2) + "'C Humidity: " + String(sensor1Data.humidity,1) + "%");
		Serial.println("Sensor 2 data:");
		Serial.println("Temp: " + String(sensor2Data.temperature,2) + "'C Humidity: " + String(sensor2Data.humidity,1) + "%");
		Serial.println("Sensor 3 data:");
		Serial.println("Temp: " + String(sensor3Data.temperature,2) + "'C Humidity: " + String(sensor3Data.humidity,1) + "%");
		Serial.println("Sensor 4 data:");
		Serial.println("Temp: " + String(sensor4Data.temperature,2) + "'C Humidity: " + String(sensor4Data.humidity,1) + "%");
		Serial.println("Sensor 5 data:");
		Serial.println("Temp: " + String(sensor5Data.temperature,2) + "'C Humidity: " + String(sensor5Data.humidity,1) + "%");
		Serial.println("Sensor 6 data:");
		Serial.println("Temp: " + String(sensor6Data.temperature,2) + "'C Humidity: " + String(sensor6Data.humidity,1) + "%");
		Serial.println("Sensor 7 data:");
		Serial.println("Temp: " + String(sensor7Data.temperature,2) + "'C Humidity: " + String(sensor7Data.humidity,1) + "%");
		Serial.println("Sensor 8 data:");
		Serial.println("Temp: " + String(sensor8Data.temperature,2) + "'C Humidity: " + String(sensor8Data.humidity,1) + "%");
		Serial.println("Sensor 9 data:");
		Serial.println("Temp: " + String(sensor9Data.temperature,2) + "'C Humidity: " + String(sensor9Data.humidity,1) + "%");
		Serial.println("Sensor 10 data:");
		Serial.println("Temp: " + String(sensor10Data.temperature,2) + "'C Humidity: " + String(sensor10Data.humidity,1) + "%");
		gotNewTemperature = false;
	}
} // End of loop