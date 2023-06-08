#include <modbus.h>
#include <modbusDevice.h>
#include <modbusRegBank.h>
#include <modbusSlave.h>

modbusDevice regBank;
modbusSlave slave;

//#define RedPin 4 //LED
//Motor 
const int inputPin3  = 11;   // Pin  7 of L293D IC
//const int inputPin4  = ;   // Pin  2 of L293D IC

void setup()
{   
  regBank.setId(1); ///Set Slave ID

  regBank.add(4);//Red
  //regBank.add(2);//motor
  //regBank.add(40001);

  //pinMode(RedPin,OUTPUT);//Red Light
  pinMode(inputPin3, OUTPUT);
  //pinMode(inputPin4, OUTPUT);  
  slave._device = &regBank; 
  slave.setBaud(9600);
}
void loop(){
      //regBank.set(40001,0);
  /*//Control LED Lights's 
    int RedLED = regBank.get(1);
      if (RedLED <= 0 && digitalRead(RedPin) == HIGH)digitalWrite(RedPin,LOW);
      if (RedLED >= 1 && digitalRead(RedPin) == LOW)digitalWrite(RedPin,HIGH);*/
     
    int YellowLED = regBank.get(4);//Controls the 
      if (YellowLED <= 0 && digitalRead(inputPin3) == HIGH)digitalWrite(inputPin3,LOW);
      if (YellowLED >= 1 && digitalRead(inputPin3) == LOW)digitalWrite(inputPin3,HIGH);
    /*  
    int GreenLED = regBank.get(3);//Controls the Green
      if (GreenLED <= 0 && digitalRead(GreenPin) == HIGH)digitalWrite(GreenPin,LOW);
      if (GreenLED >= 1 && digitalRead(GreenPin) == LOW)digitalWrite(GreenPin,HIGH);
*/
slave.run(); 
}
