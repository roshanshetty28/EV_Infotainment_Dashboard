#include <modbus.h>
#include <modbusDevice.h>
#include <modbusRegBank.h>
#include <modbusSlave.h>

modbusDevice regBank;
modbusSlave slave;

#define startstop 4
#define YellowPin 5
#define GreenPin 6
#define Trunk 7
#define Interior 11

#define echoPin 2         // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3         //attach pin D3 Arduino to pin Trig of HC-SR04
#define alarm 12           // buzzer
long duration;            // variable for the duration of sound wave travel
int distance;             // variable for the distance measurement*/

#define cellPin A0        //for battery voltage measurement
const float mvpc = 5;     //since arduino can read 5V max
float counts = 0;
float mv = 0;
float multiplier = 3;     //since our voltage divider is dividing volatge in three equal parts
float output = 0;         // voltage output

//Motor
int ENA = 10;
int IN1 = 9;
int IN2 = 8;

float battery() {
  float percentage;
  counts = analogRead(cellPin);
  mv = counts * mvpc;
  output = (mv * multiplier) / 1024;
  percentage = ((output - 11) / 2.5) * 100;
  if (percentage >= 0) {
    regBank.set(30001, percentage);  //for numeric display
 }
  else {
    percentage = 0;
    regBank.set(30001, percentage); //for numeric display
  }
  return output;
}

float ultrasonic() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  return distance;
}

void ledblink() {
  digitalWrite(alarm, HIGH);
  if (digitalRead(alarm)) {
    regBank.set(10001, 1);
  }
  delay(500);
  digitalWrite(alarm, LOW);
  delay(500);
}

void setup()
{ 
  regBank.setId(1); ///Set Slave ID
  slave._device = &regBank; 
 
  regBank.add(1);//Start/Stop
  regBank.add(2);//Forward/Reverse
  regBank.add(3);//Headlight
  regBank.add(4);//Trunk
  regBank.add(5);//Interior
  regBank.add(10001);     //ultrasonic
  regBank.add(30003);     //motor
  regBank.add(30001);     //Batery percentage
  regBank.add(30002);     //battery voltage value
  regBank.add(30004);     //ultrasonic distance
  regBank.add(30005);     //ultrasonic distance
  pinMode(RedPin,OUTPUT);//Red Light
  pinMode(YellowPin,OUTPUT);//Yellow Light
  pinMode(GreenPin,OUTPUT);//Green light
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  pinMode(trigPin, OUTPUT);   // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);    // Sets the echoPin as an INPUT
  pinMode(alarm, OUTPUT);     //for buzzer*/
  regBank.add(40001);
  regBank.add(30001);
  slave.setBaud(9600);
}
void loop(){
regBank.set(10001, 0); //initializing Ultrasonic led in hmi to zero

  //Control LED Lights's 
    int RedLED = regBank.get(1);//Controls the Start/Stop
      if (RedLED <= 0 && digitalRead(startstop) == HIGH)digitalWrite(startstop,LOW);
      if (RedLED >= 1 && digitalRead(startstop) == LOW)digitalWrite(startstop,HIGH);
     
    int YellowLED = regBank.get(2);//Controls the Forward/Reverse
      if (YellowLED <= 0 && digitalRead(YellowPin) == HIGH)digitalWrite(YellowPin,LOW);
      if (YellowLED >= 1 && digitalRead(YellowPin) == LOW)digitalWrite(YellowPin,HIGH);
      
    int GreenLED = regBank.get(3);//Headlight
      if (GreenLED <= 0 && digitalRead(GreenPin) == HIGH)digitalWrite(GreenPin,LOW);
      if (GreenLED >= 1 && digitalRead(GreenPin) == LOW)digitalWrite(GreenPin,HIGH);
  // motor
  int trunkLED = regBank.get(4);//Trunk
      if (trunkLED <= 0 && digitalRead(Trunk) == HIGH)digitalWrite(Trunk,LOW);
      if (trunkLED >= 1 && digitalRead(Trunk) == LOW)digitalWrite(Trunk,HIGH);
  int intLED = regBank.get(5);//Interior
      if (intLED <= 0 && digitalRead(Interior) == HIGH)digitalWrite(Interior,LOW);
      if (intLED >= 1 && digitalRead(Interior) == LOW)digitalWrite(Interior,HIGH);

float dist;                 // for ultrasonic
  dist = ultrasonic();
  regBank.set(30004, dist);
  if (dist <= 15) {
    ledblink();
  }
  
unsigned int i = regBank.get(40001);
analogWrite(ENA,i);
int rpm=21.96*i;
regBank.set(30003,rpm); 
regBank.set(30005,rpm);

  float voltage;              //for battery
  voltage = battery();
  voltage *= 100;
  regBank.set(30002, voltage);
slave.run(); 
}
