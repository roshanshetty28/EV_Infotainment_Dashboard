#include <modbus.h>
#include <modbusDevice.h>
#include <modbusRegBank.h>
#include <modbusSlave.h>

modbusDevice regBank;
modbusSlave slave;

#define RedPin 8
#define YellowPin 9

#define echoPin 2         // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3         //attach pin D3 Arduino to pin Trig of HC-SR04
#define alarm 10           // buzzer
long duration;            // variable for the duration of sound wave travel
int distance;             // variable for the distance measurement

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
 

  regBank.add(1);//Red
  regBank.add(2);//Start/Stop
  regBank.add(3);//Forward/Reverse
  regBank.add(10001);     //ultrasonic
  regBank.add(30001);     //ultrasonic distance
  pinMode(RedPin,OUTPUT);//Red Light
  pinMode(YellowPin,OUTPUT);//Yellow Light
   pinMode(trigPin, OUTPUT);   // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);    // Sets the echoPin as an INPUT
  pinMode(alarm, OUTPUT);     //for buzzer
 
  slave.setBaud(9600);
}
void loop(){
regBank.set(10001, 0); //initializing Ultrasonic led in hmi to zero
  //Control LED Lights's 
    int RedLED = regBank.get(1);
      if (RedLED <= 0 && digitalRead(RedPin) == HIGH)digitalWrite(RedPin,LOW);
      if (RedLED >= 1 && digitalRead(RedPin) == LOW)digitalWrite(RedPin,HIGH);
     
    int YellowLED = regBank.get(2);//Controls the Start/Stop
      if (YellowLED <= 0 && digitalRead(YellowPin) == HIGH)digitalWrite(YellowPin,LOW);
      if (YellowLED >= 1 && digitalRead(YellowPin) == LOW)digitalWrite(YellowPin,HIGH);
    float dist;                 // for ultrasonic
  dist = ultrasonic();
  regBank.set(30001, dist);
  if (dist <= 15) {
    ledblink();
  }  
    
slave.run(); 
}
