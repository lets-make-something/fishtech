#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

struct Screw{
  int speed;
  int ena,a,b;
  int pwm;
  int count;
  void setup(int ena,int a,int b){
    this->ena=ena;
    this->a=a;
    this->b=b;
    pinMode(ena,OUTPUT);
    pinMode(a,OUTPUT);
    pinMode(b,OUTPUT);
    speed=0;
    pwm=2;
    count=0;
  }
  void loop(){
    if(speed>0){
      digitalWrite(this->ena,HIGH);
      digitalWrite(this->b,LOW);      
      digitalWrite(this->a,HIGH);
    }else if(speed<0){
      digitalWrite(this->ena,HIGH);
      digitalWrite(this->a,LOW);      
      digitalWrite(this->b,HIGH);      
    }else{
      digitalWrite(this->ena,LOW);
      digitalWrite(this->a,LOW);
      digitalWrite(this->b,LOW);
    }
    count++;
    if(count>pwm){
      count=0;
    }
  }
};

Screw l,r;
int count=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  l.setup(2,3,4);
  r.setup(5,6,7);
  count=0;
}

void command(char *cmd){
  int inputchar = cmd[0];
  if('q'==inputchar){
    l.speed=1;
  }else if('w'==inputchar){
    r.speed=1;
  }else if('a'==inputchar){
    l.speed=0;
  }else if('s'==inputchar){
    r.speed=0;
  }else if('z'==inputchar){
    l.speed=-1;
  }else if('x'==inputchar){
    r.speed=-1;
  }else{
    l.speed=0;
    r.speed=0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    command(text);
  }
  int input;
  input = Serial.read();
  if(input != -1 ){
    char text[] = " \n";
    text[0]=input;
    command(text);
  }

  l.loop();
  r.loop();
  
  delay(10);
  count++;
}
