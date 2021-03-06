// Copyright (c) 2016 Thomas Reynolds


#define MotorDirPin 2 // Definition of motor connections
#define MotorSpeedPin 3
#define PotPin A9 // Signal connection from string pot


int Position, SetTemp, NumberCycles, NumberStages;
byte Direction;
double TempNow;
volatile int SetPosition, Temp;
unsigned long TestParam[2][3]; //Array to store test paramaters
char Response;



void setup(){
  Serial.begin(9600);
  pinMode(MotorDirPin, OUTPUT);
  pinMode(MotorSpeedPin, OUTPUT);
  Position = analogRead(A8);
  digitalWrite(MotorSpeedPin, LOW);
  Position = analogRead(A8);
  SetPosition = 160;
  if(Position < SetPosition) Direction = HIGH;
  else Direction = LOW;
  while(abs(analogRead(PotPin) - SetPosition) >=5){
    analogWrite(MotorSpeedPin, 100);
    digitalWrite(MotorDirPin, Direction);
  }
  digitalWrite(MotorSpeedPin, LOW);
  Serial.println("Input number of test stages (number of temperatures in each cycle including room temperature)");
  while(Serial.available() == 0);
  NumberStages = Serial.parseInt();
  TestParam[2][NumberStages];
  Serial.println(NumberStages);
  for(int i = 0; i <NumberStages; i++){
    Serial.print("Input temperature (for room enter 0) ");
    Serial.println(i+1);
    while(Serial.available() == 0);
    TestParam[0][i] = Serial.parseInt();
    Serial.println("Input time (minutes)");
    while(Serial.available() == 0);
    TestParam[1][i] = Serial.parseInt() * 60000;
  }
  Serial.println("Input number of cycles");
  while(Serial.available() == 0);
  NumberCycles = Serial.parseInt();
  }

void loop(){
  Serial.println("Test series is as follows:");
  for(int i = 0; i <NumberStages; i++){
    Serial.print(TestParam[0][i]);
    Serial.print(" degrees for ");
    Serial.print(TestParam[1][i]/60000);
    Serial.println(" minutes");
  }
  Serial.print("For ");
  Serial.print(NumberCycles);
  Serial.println(" cycles");
  Serial.println("Repeat test? y/n");
  while(!Serial.available());
  if(Serial.read() == 'n') software_Reset();  
  else;
  for(int LoopCount = 1; LoopCount <= NumberCycles; LoopCount++){
    Serial.print("Cycle");
    Serial.println(LoopCount);
    for(int i = 0; i <NumberStages; i++){
      Serial.print(TestParam[0][i]);
      Serial.print(" degrees for ");
      Serial.print(TestParam[1][i]/60000);
      Serial.println(" minutes");
      goPosition(TestParam[0][i]);
      Serial.println("Movement confirmed");
      delay(TestParam[1][i]);
    }
  }
  Position = analogRead(A8);
  SetPosition = 160;
  if(Position < SetPosition) Direction = HIGH;
  else Direction = LOW;
  while(abs(analogRead(PotPin) - SetPosition) >=5){
    analogWrite(MotorSpeedPin, 100);
    digitalWrite(MotorDirPin, Direction);
  }
  digitalWrite(MotorSpeedPin, LOW);
  Serial.println("Test Complete");
  delay(1000);
  Serial.println("Repeat test? y/n");
  while(!Serial.available());
  if(Serial.read() == 'n') software_Reset();  
  else;
}

void goPosition(int Temp){ // Function to convert input temperature to a furnace positions
  if(Temp == 0) SetPosition = 160;
  else if (Temp < 400) SetPosition = Temp/(0.1020 + (0.0021*Temp)); 
  else SetPosition = (293.9276)+(0.7452*Temp)+(-0.0020*(pow(Temp,2)))+(2.5442E-006*(pow(Temp,3)));
  Position = analogRead(A8);
  Serial.println(SetPosition);
  if(Position < SetPosition) Direction = HIGH;
  else Direction = LOW;
  while(abs(analogRead(PotPin) - SetPosition) >=5){
    analogWrite(MotorSpeedPin, 100);
    digitalWrite(MotorDirPin, Direction);
    }
    digitalWrite(MotorSpeedPin, LOW);
}

void software_Reset()
{
  asm volatile ("  jmp 0");  
} 
