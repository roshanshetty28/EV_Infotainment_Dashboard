
#include <modbusDevice.h>
#include <modbusRegBank.h>
#include <modbusSlave.h>

modbusDevice regBank;
modbusSlave slave;
                              //Motor B
const int RedPin  = 11;    // Pin  7 of L293D IC
//const int inputPin4  = 8;    // Pin  2 of L293D IC
//int EN1 = 10;                 // Pin 1 of L293D IC

void setup()
{
   regBank.setId(1); ///Set Slave ID
   regBank.add(1);
    
    
    pinMode(RedPin, OUTPUT);
    
    slave._device = &regBank; 
  slave.setBaud(9600);
}

void loop()

{
  
       
      int RedLED = regBank.get(1);
      if (RedLED <= 0 && digitalRead(RedPin) == HIGH)digitalWrite(RedPin,LOW);
      if (RedLED >= 1 && digitalRead(RedPin) == LOW)digitalWrite(RedPin,HIGH);
     slave.run(); 
}
