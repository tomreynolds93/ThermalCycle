//Copyright (c) 2018 Thomas Reynolds

#define MotorDirPin 2 // Definition of motor connections
#define MotorSpeedPin 3
#define PotPin A9 // Signal connection from string pot

int Position, NumberCycles;
byte Direction;
unsigned long TimeAtTemp1, TimeAtTemp2, TimeAtTemp3, TimeAtRoom;
const int T1pos=816, T2pos=740, T3pos=623;

void setup() {
  Serial.begin(115200);
  pinMode(MotorDirPin, OUTPUT);
  pinMode(MotorSpeedPin, OUTPUT);
  digitalWrite(MotorSpeedPin, LOW);
  goHome();
  digitalWrite(MotorSpeedPin, LOW);
  Serial.println("length of time at T1 per cycle (mins)");
  while (!Serial.available());
  TimeAtTemp1 = Serial.parseInt() * 60000;
  Serial.print(TimeAtTemp1 / 60000);
  Serial.println(" mins at T1");
  Serial.println("Length of time at T2 per cycle (mins)");
  while (!Serial.available());
  TimeAtTemp2 = Serial.parseInt() * 60000;
  Serial.print(TimeAtTemp2 / 60000);
  Serial.println(" mins at T2");
  Serial.println("length of time at T3 per cycle (mins)");
  while (!Serial.available());
  TimeAtTemp3 = Serial.parseInt() * 60000;
  Serial.print(TimeAtTemp3 / 60000);
  Serial.println(" mins at T3");
  Serial.println("length of time at room temp per cycle (mins)");
  while (!Serial.available());
  TimeAtRoom = Serial.parseInt() * 60000;
  Serial.print(TimeAtRoom / 60000);
  Serial.println(" mins at room");
  Serial.println("Input number of cycles");
  while (!Serial.available());
  NumberCycles = Serial.parseInt();
  Serial.print("For ");
  Serial.print(NumberCycles);
  Serial.println(" cycles");
}

void loop() {
  Serial.println("Test series is as follows:");
  Serial.print(TimeAtTemp1/60000);
  Serial.println(" mins at T1");
  Serial.print(TimeAtTemp2/60000);
  Serial.println(" mins at T2");
  Serial.print(TimeAtTemp3/60000);
  Serial.println(" mins at T3");
  Serial.print(TimeAtRoom/60000);
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
    goT1();
    Serial.println("Samples at T1");
    delay(TimeAtTemp1);
    goT2();
    Serial.println("Samples at T2");
    delay(TimeAtTemp2);
    goT3();
    Serial.println("Samples at T3");
    delay(TimeAtTemp3);
    goHome();
    Serial.println("Samples Withdrawn");
    delay(TimeAtRoom);
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

void goT1() {
  Position = analogRead(PotPin);
  if (Position < T1pos) Direction = HIGH;
  else Direction = LOW;
  while (abs(analogRead(PotPin) - T1pos) >= 1) {
    analogWrite(MotorSpeedPin, 100);
    digitalWrite(MotorDirPin, Direction);
  }
  digitalWrite(MotorSpeedPin, LOW);
}

void goT2() {
  Position = analogRead(PotPin);
  if (Position < T2pos) Direction = HIGH;
  else Direction = LOW;
  while (abs(analogRead(PotPin) - T2pos) >= 1) {
    analogWrite(MotorSpeedPin, 100);
    digitalWrite(MotorDirPin, Direction);
  }
  digitalWrite(MotorSpeedPin, LOW);
}

void goT3() {
  Position = analogRead(PotPin);
  if (Position < T3pos) Direction = HIGH;
  else Direction = LOW;
  while (abs(analogRead(PotPin) - T3pos) >= 1) {
    analogWrite(MotorSpeedPin, 100);
    digitalWrite(MotorDirPin, Direction);
  }
  digitalWrite(MotorSpeedPin, LOW);
}

void software_Reset()
{
  asm volatile ("  jmp 0");
}
