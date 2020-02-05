// Copyright (c) 2016 Thomas Reynolds

#include <SPI.h> // Required for temperature sensor
#include <Adafruit_MAX31856.h> //Required for temperature sensor

#define MotorDirPin 2 // Definition of motor connections
#define MotorSpeedPin 3
#define PotPin A9 // Signal connection from string pot

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10, 11, 12, 13);

int Pos, SetPos;
byte Dir;

void setup() {
  Serial.begin(115200);

  maxthermo.begin();
  maxthermo.setThermocoupleType(MAX31856_TCTYPE_R);
  pinMode(MotorSpeedPin, OUTPUT);
  digitalWrite(MotorSpeedPin, LOW);
  Serial.println("MOTOR HAS BEEN DISCONNECTED");
  Serial.println("Position;Temperature");
}

void loop() {
    delay(1000);
    Serial.print(analogRead(PotPin));
    Serial.print(";");
    Serial.println(maxthermo.readThermocoupleTemperature());
 }
