#include <modbus.h>
#include <modbusDevice.h>
#include <modbusRegBank.h>
#include <modbusSlave.h>

modbusDevice regBank;
modbusSlave slave;

#define RedPin 8
#define YellowPin 9
#define GreenPin 10
#define cellPin A0        //for battery voltage measurement

const float mvpc = 5;     //since arduino can read 5V max
float counts = 0;
float mv = 0;
float multiplier = 1;     //since our voltage divider is dividing volatge in three equal parts
float output = 0;         // voltage output

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
void setup()
{   
  
  regBank.setId(1); ///Set Slave ID
  slave._device = &regBank; 
 

  regBank.add(1);//Red
  regBank.add(2);//Start/Stop
  regBank.add(3);//Forward/Reverse
  regBank.add(30001);     //
  regBank.add(30002);     //for battery voltage value
  pinMode(RedPin,OUTPUT);//Red Light
  pinMode(YellowPin,OUTPUT);//Yellow Light
 pinMode(GreenPin,OUTPUT);//Green light
 
  slave.setBaud(9600);
}
void loop(){

  //Control LED Lights's 
    int RedLED = regBank.get(1);
      if (RedLED <= 0 && digitalRead(RedPin) == HIGH)digitalWrite(RedPin,LOW);
      if (RedLED >= 1 && digitalRead(RedPin) == LOW)digitalWrite(RedPin,HIGH);
     
    int YellowLED = regBank.get(2);//Controls the Start/Stop
      if (YellowLED <= 0 && digitalRead(YellowPin) == HIGH)digitalWrite(YellowPin,LOW);
      if (YellowLED >= 1 && digitalRead(YellowPin) == LOW)digitalWrite(YellowPin,HIGH);
      
    int GreenLED = regBank.get(3);//Controls the Forward/Reverse
      if (GreenLED <= 0 && digitalRead(GreenPin) == HIGH)digitalWrite(GreenPin,LOW);
      if (GreenLED >= 1 && digitalRead(GreenPin) == LOW)digitalWrite(GreenPin,HIGH);

  float voltage;              //for battery
  voltage = battery();
  voltage *= 100;
  regBank.set(30002, voltage);
slave.run(); 
}
