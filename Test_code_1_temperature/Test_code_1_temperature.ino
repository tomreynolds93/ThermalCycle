//Copyright (c) 2018 Thomas Reynolds

#define MotorDirPin 2 // Definition of motor connections
#define MotorSpeedPin 3
#define PotPin A9 // Signal connection from string pot

int Position, NumberCycles;
byte Direction;
unsigned long TimeAtTemp, TimeAtRoom, TimeAtTempms, TimeAtRoomms;

void setup() {
  Serial.begin(9600);
  pinMode(MotorDirPin, OUTPUT);
  pinMode(MotorSpeedPin, OUTPUT);
  digitalWrite(MotorSpeedPin, LOW);
  Position = analogRead(PotPin);
  if (Position < 160) Direction = HIGH;
  else Direction = LOW;
  while (abs(analogRead(PotPin) - 160) >= 5) {
    analogWrite(MotorSpeedPin, 100);
    digitalWrite(MotorDirPin, Direction);
  }
  digitalWrite(MotorSpeedPin, LOW);
  Serial.println("length of time at temperature per cycle (mins)");
  Serial.read();
  while (!Serial.available());
  TimeAtTemp = Serial.parseInt();
  TimeAtTempms = TimeAtTemp * 60000;
  Serial.read();
  Serial.print(TimeAtTemp);
  Serial.println(" mins at temperature");
  Serial.println("Length of time at room tempearture per cycle (mins)");
  while (!Serial.available());
  TimeAtRoom = Serial.parseInt();
  Serial.print(TimeAtRoom);
  TimeAtRoomms = TimeAtRoom * 60000;
  Serial.read();
  Serial.println(" mins at room temperature");
  Serial.println("Input number of cycles");
  while (!Serial.available());
  NumberCycles = Serial.parseInt();
  Serial.print("For ");
  Serial.print(NumberCycles);
  Serial.println(" cycles");
}

void loop() {
  Serial.println("Test series is as follows:");
  Serial.print(TimeAtTemp);
  Serial.println(" mins at temperature");
  Serial.print(TimeAtRoom);
  Serial.println(" mins at room temperature");
  Serial.print("For ");
  Serial.print(NumberCycles);
  Serial.println(" cycles");
  Serial.read();
  Serial.println("Begin Test? y/n");
  while (!Serial.available());
  if (Serial.read() == 'n') software_Reset();
  else;
  for (int LoopCount = 1; LoopCount <= NumberCycles; LoopCount++) {
    Serial.print("Cycle");
    Serial.println(LoopCount);
    goTemp();
    Serial.println("Samples Inserted");
    delay(TimeAtTempms);
    goHome();
    Serial.println("Samples Withdrawn");
    delay(TimeAtRoomms);
  }
  Serial.println("Test Complete");
  Serial.read();
  Serial.println("Repeat Test? y/n");
  while (!Serial.available());
  if (Serial.read() == 'n') software_Reset();
  else;

}

void goHome() {
  Position = analogRead(PotPin);
  if (Position < 160) Direction = HIGH;
  else Direction = LOW;
  while (abs(analogRead(PotPin) - 160) >= 5) {
    analogWrite(MotorSpeedPin, 100);
    digitalWrite(MotorDirPin, Direction);
  }
  digitalWrite(MotorSpeedPin, LOW);
}

void goTemp() {
  Position = analogRead(PotPin);
  if (Position < 745) Direction = HIGH;
  else Direction = LOW;
  while (abs(analogRead(PotPin) - 745) >= 5) {
    analogWrite(MotorSpeedPin, 100);
    digitalWrite(MotorDirPin, Direction);
  }
  digitalWrite(MotorSpeedPin, LOW);
}

void software_Reset()
{
  asm volatile ("  jmp 0");
}
