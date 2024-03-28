#include <SPI.h>
#include <RF24.h>
#include <DHT.h>

#define DHTPIN 7          // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);

RF24 radio(2, 15);        // CE, CSN pins
const uint64_t pipe = 0xE6E4F0F1E9LL; // Change this address for the receiver node

void setup() {
  Serial.begin(9600);
  dht.begin();
  radio.begin();
  radio.openWritingPipe(pipe);
}

void loop() {
  delay(2000);  // Delay between sensor readings

  float temperature = dht.readTemperature();  // Read temperature from DHT sensor
  float humidity = dht.readHumidity();        // Read humidity from DHT sensor

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Create a data structure to hold temperature and humidity
  struct SensorData {
    float temp;
    float hum;
  } sensorData;

  sensorData.temp = temperature;
  sensorData.hum = humidity;

  // Send the sensor data over NRF24L01
  radio.write(&sensorData, sizeof(sensorData));

  // Print sent data to serial monitor
  Serial.print("Sent - Temperature: ");
  Serial.print(sensorData.temp);
  Serial.print(" °C, Humidity: ");
  Serial.print(sensorData.hum);
  Serial.println("%");
}