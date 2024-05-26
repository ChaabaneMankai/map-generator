// Uses most of the functions in the library
#include <MD_MAX72xx.h>

// Turn on debug statements to the serial output
#define  DEBUG  1

#if  DEBUG
#define PRINT(s, x) { Serial.print(F(s)); Serial.print(x); }
#define PRINTS(x) Serial.print(F(x))
#define PRINTD(x) Serial.println(x, DEC)

#else
#define PRINT(s, x)
#define PRINTS(x)
#define PRINTD(x)

#endif

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES	4

//#define CLK_PIN   13  // or SCK
//#define DATA_PIN  11  // or MOSI
//#define CS_PIN 2  // or SS
#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN 2  // or SS

// SPI hardware interface
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Specific SPI hardware interface
//MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, SPI1, CS_PIN, MAX_DEVICES);
// Arbitrary pins
//MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// We always wait a bit between updates of the display
#define  DELAYTIME  100  // in milliseconds
int row=6;
int column=30;
int lim =50; //limit
int direction=1;
int matrix[8][32]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};










#include <Servo.h>          //Servo motor library. This is standard library
#include <NewPing.h>        //Ultrasonic sensor function library. You must install this library

//our L298N control pins
const int LeftMotorForward = 7;
const int LeftMotorBackward = 6;
const int RightMotorForward = 4;
const int RightMotorBackward = 5;

//sensor pins
#define trig_pin A1 //analog input 1
#define echo_pin A2 //analog input 2

#define maximum_distance 20000
boolean goesForward = false;
int distance = 100;

NewPing sonar(trig_pin, echo_pin, maximum_distance); //sensor function
Servo servo_motor; //our servo name


void setup(){
  #if  DEBUG
  Serial.begin(9600);
#endif
  PRINTS("\n[MD_MAX72XX Test & Demo]");

  if (!mx.begin())
   PRINTS("\nMD_MAX72XX initialization failed");




  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);
  
  servo_motor.attach(10); //our servo pin

  servo_motor.write(90);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  //mx.setPoint(row,column,1);
}

void loop(){

  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);
  distance =lookForward();
  delay(300);
  distanceRight = lookRight();
  delay(300);
  distanceLeft = lookLeft();
  delay(300);
  if (direction==1){
  if(distanceRight<=lim && matrix[row+1][column]==0){
    matrix[row+1][column]=1;
    mx.setPoint(row+1,column,1);
  }
  else if(distanceLeft<=lim && matrix[row-1][column]==0){
    matrix[row-1][column]=1;
    mx.setPoint(row-1,column,1);
  }
  if(distance<=lim && matrix[row][column-1]==0){
    matrix[row][column-1]=1;
    mx.setPoint(row,column-1,1);
    if (distanceRight>lim){
      turnRight();
      delay(1000);
      direction=3;
    }
    else if(distanceLeft>lim){
      turnLeft();
      delay(1000);
      direction=2;
    }
    else{
      //mx.setPoint(0,0,1);
    }
  }
  else{
    moveForward();
    delay(2000); //2000 for 46cm
    moveStop();
    column--;
    delay(1000);
  }
  }
  else if (direction==2){
  if(distanceRight<=lim && matrix[row][column-1]==0){
    matrix[row][column-1]=1;
    mx.setPoint(row,column-1,1);
  }
  else if(distanceLeft<=lim && matrix[row][column+1]==0){
    matrix[row][column+1]=1;
    mx.setPoint(row,column+1,1);
  }
  if(distance<=lim && matrix[row-1][column]==0){
    matrix[row-1][column]=1;
    mx.setPoint(row-1,column,1);
    if (distanceRight>lim){
      turnRight();
      delay(1000);
      direction=1;
    }
    else if(distanceLeft>lim){
      turnLeft();
      delay(1000);
      direction=4;
    }
    else{
      //mx.setPoint(0,0,1);
    }
  }
  else{
    row--;
    moveForward();
    delay(2000 ); //2000 for 46cm
    moveStop();
    delay(1000);
  } 
  }
  else if (direction==3){
  if(distanceRight<=lim && matrix[row][column+1]==0){
    matrix[row][column+1]=1;
    mx.setPoint(row,column+1,1);
  }
  else if(distanceLeft<=lim && matrix[row][column-1]==0){
    matrix[row][column-1]=1;
    mx.setPoint(row,column-1,1);
  }
  if(distance<=lim && matrix[row+1][column]==0){
    matrix[row+1][column]=1;
    mx.setPoint(row+1,column,1);
    if (distanceRight>lim){
      turnRight();
      delay(1000);
      direction=4;
    }
    else if(distanceLeft>lim){
      turnLeft();
      delay(1000);
      direction=1;
    }
    else{
      //mx.setPoint(0,0,1);
    }
  }
  else{
    row++;
    moveForward();
    delay(2000 ); //2000 for 46cm
    moveStop();
    delay(1000);
  }
  }
  else if (direction==4){
  if(distanceRight<=lim && matrix[row-1][column]==0){
    matrix[row-1][column]=1;
    mx.setPoint(row-1,column,1);
  }
  else if(distanceLeft<=lim && matrix[row+1][column]==0){
    matrix[row+1][column]=1;
    mx.setPoint(row+1,column,1);
  }
  if(distance<=lim && matrix[row][column+1]==0){
    matrix[row][column+1]=1;
    mx.setPoint(row,column+1,1);
    if (distanceRight>lim){
      turnRight();
      delay(1000);
      direction=2;
    }
    else if(distanceLeft>lim){
      turnLeft();
      delay(1000);
      direction=3;
    }
    else{
      //mx.setPoint(0,0,1);
    }
  }
  else{
    column++;
    moveForward();
    delay(2000 ); //2000 for 46cm
    moveStop();
    delay(1000);
  }
  }



  /*if (distance <= 20){
    mx.setPoint(row,column,1);
    moveStop();
    delay(300);
    moveBackward();
    delay(400);
    moveStop();
    delay(300);
    distanceRight = lookRight();
    delay(300);
    distanceLeft = lookLeft();
    delay(300);

    if (distance >= distanceLeft){
      turnRight();
      moveStop();
    }
    else{
      turnLeft();
      moveStop();
    }
  }
  else{
    moveForward(); 
    if(old_distance==-1){
      old_distance = distance;
    }
    else{
      if(old_distance-distance>10){
        column++;
        old_distance = distance;
      }
    }
  }*/
  //if (direction==1) mx.setPoint(1,0,1);
  //if (direction==2) mx.setPoint(2,0,1);
  //if (direction==3) mx.setPoint(3,0,1);
  //if (direction==4) mx.setPoint(4,0,1);
}
int lookForward(){
  servo_motor.write(90);
  delay(500);
  int distancee = readPing();
  delay(100);
  servo_motor.write(90);
  return distancee;
}


int lookRight(){  
  servo_motor.write(0);
  delay(500);
  int distancee = readPing();
  delay(100);
  servo_motor.write(90);
  return distancee;
}

int lookLeft(){
  servo_motor.write(180);
  delay(500);
  int distancee = readPing();
  delay(100);
  servo_motor.write(90);
  return distancee;
}

int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}

void moveStop(){
  
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}

void moveForward(){

  if(!goesForward){

    //goesForward=true;
    
    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(RightMotorForward, HIGH);
  
    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW); 
  }
}

void moveBackward(){

  //goesForward=false;

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
}

void turnRight(){

  

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
  delay(1250);
  //delay(1800);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);

 
  
  
}

void turnLeft(){

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);

  delay(1250);
  //delay(1800);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);

}

 
