#include <Adafruit_MAX31856.h>

#define MotorDirPin 2 // Definition of motor connections
#define MotorSpeedPin 3
#define PotPin A9 // Signal connection from string pot
volatile int Position, SetPos, NewPos;
volatile byte Dir;

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10, 11, 12, 13);
// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10);

void setup() {
  Serial.begin(115200);
  Serial.println("MAX31856 thermocouple test");
  SetPos = 160;
  NewPos = 160;

  maxthermo.begin();

  maxthermo.setThermocoupleType(MAX31856_TCTYPE_K);

  Serial.print("Thermocouple type: ");
  switch (maxthermo.getThermocoupleType() ) {
    case MAX31856_TCTYPE_B: Serial.println("B Type"); break;
    case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
    case MAX31856_TCTYPE_J: Serial.println("J Type"); break;
    case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
    case MAX31856_TCTYPE_N: Serial.println("N Type"); break;
    case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
    case MAX31856_TCTYPE_S: Serial.println("S Type"); break;
    case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
    case MAX31856_VMODE_G8: Serial.println("Voltage x8 Gain mode"); break;
    case MAX31856_VMODE_G32: Serial.println("Voltage x8 Gain mode"); break;
    default: Serial.println("Unknown"); break;
  }


}

void loop() {
  if (!Serial.available()) {
    Serial.print("Position: ");
    Serial.println(analogRead(A9));
    Serial.print("Set Position: ");
    Serial.println(NewPos);
    Serial.print("Cold Junction Temp: ");
    Serial.println(maxthermo.readCJTemperature());

    Serial.print("Thermocouple Temp: ");
    Serial.println();
    // Check and print any faults
    uint8_t fault = maxthermo.readFault();
    if (fault) {
      if (fault & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault");
      if (fault & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
      if (fault & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction High Fault");
      if (fault & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction Low Fault");
      if (fault & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple High Fault");
      if (fault & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple Low Fault");
      if (fault & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage Fault");
      if (fault & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple Open Fault");
    }
    delay(1000);
  }
  else {
    NewPos = Serial.parseInt();
    Serial.flush();
    //    NewPos = checkPos(SetPos);
    Serial.print("Set Position: ");
    Serial.println(NewPos);
    goPos(NewPos);
  }
}

//int checkPos(int InPos) {
//  if ((Pos < 160) || (Pos > 800)) {
//    Serial.println("Outside of range");
//    return analogRead(PotPin);
//  }
//  else return InPos;
//}

void goPos(int pos) {
  Position = analogRead(PotPin);
  if (Position < pos) Dir = HIGH;
  else Dir = LOW;
  while (abs(analogRead(PotPin) - pos) >= 5) {
    analogWrite(MotorSpeedPin, 100);
    digitalWrite(MotorDirPin, Dir);
  }
  digitalWrite(MotorSpeedPin, LOW);
}
