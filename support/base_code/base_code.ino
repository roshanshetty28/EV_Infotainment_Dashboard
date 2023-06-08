#include <modbus.h>
#include <modbusDevice.h>
#include <modbusRegBank.h>
#include <modbusSlave.h>

modbusDevice regBank;
modbusSlave slave;

int ENA = 10;
int IN1 = 9;
int IN2 = 8;

void setup()
{  
  regBank.setId(1); ///Set Slave ID
  slave._device = &regBank; 
 
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  regBank.add(40001);
  regBank.add(30001);
  slave.setBaud(9600);
}
void loop(){

unsigned int i = regBank.get(40001);
analogWrite(ENA,i);
int rpm=21.96*i;
regBank.set(30003,rpm);  
slave.run(); 
}
