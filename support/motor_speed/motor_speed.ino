// First Motor connections
int ENA = 3;
int IN1 = 9;
int IN2 = 8;


void setup() {

  pinMode(ENA, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
 
  //Motors OFF
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

}

void loop() {

  changeSpeed();
  delay(1000);
}



void changeSpeed() {
  // Motors ON
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  
  // Speed Up
  for (int i = 0; i < 256; i++) {
    analogWrite(ENA, i);

    delay(20);
  }
  
  // Speed Down
  for (int i = 255; i >= 0; --i) {
    analogWrite(ENA, i);

    delay(20);
  }
  
  // Motors OFF
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  
}
