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

  pinMode(MotorDirPin, OUTPUT);
  pinMode(MotorSpeedPin, OUTPUT);
  digitalWrite(MotorSpeedPin, LOW);
  Pos = analogRead(PotPin);
  SetPos = 160;
  if (Pos < SetPos) Dir = HIGH;
  else Dir = LOW;
  while (abs(analogRead(PotPin) - SetPos) >= 5) {
    analogWrite(MotorSpeedPin, 100);
    digitalWrite(MotorDirPin, Dir);
  }
  digitalWrite(MotorSpeedPin, LOW);
  Serial.println("Begin calibration?");
  while (!Serial.available());
  if (Serial.read() == 'n') software_Reset();
  else;
}

void loop() {
  Serial.println("Position;Temperature");
  for (int i = 160; i <= 865; i =i + 5) {
    Pos = analogRead(PotPin);
    SetPos = i;
    if (Pos < SetPos) Dir = HIGH;
    else Dir = LOW;
    while (abs(analogRead(PotPin) - SetPos) >= 3) {
      analogWrite(MotorSpeedPin, 100);
      digitalWrite(MotorDirPin, Dir);
    }
    digitalWrite(MotorSpeedPin, LOW);
    delay(600000);
    Serial.print(analogRead(PotPin));
    Serial.print(";");
    Serial.println(maxthermo.readThermocoupleTemperature());
  }

  Pos = analogRead(PotPin);
  SetPos = 160;
  if (Pos < SetPos) Dir = HIGH;
  else Dir = LOW;
  while (abs(analogRead(PotPin) - SetPos) >= 5) {
    analogWrite(MotorSpeedPin, 100);
    digitalWrite(MotorDirPin, Dir);
  }
  digitalWrite(MotorSpeedPin, LOW);
  Serial.println("Calibration complete");
  Serial.println("waiting...");
  while (!Serial.available());
  software_Reset();
}

void software_Reset()
{
  asm volatile ("  jmp 0");
}
