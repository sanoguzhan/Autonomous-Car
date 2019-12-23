//Left side of Motors
const int EnL = 5; 
const int HighL = 7;
const int LowL = 11;

//Right side of Motors
const int EnR = 6;
const int HighR = 9;
const int LowR = 8;

//Raspberry Pi connection

const int D0 = 0; // Raspberry Pi pin 21
const int D1 = 1; // Raspberry Pi pin 22
const int D2 = 2; // Raspberry Pi pin 23
const int D3 = 3; // Raspberry Pi pin 24

int a,b,c,d,binary;

void setup() {
  pinMode(EnL, OUTPUT);
  pinMode(HighL, OUTPUT);
  pinMode(LowL, OUTPUT);
  pinMode(EnR, OUTPUT);
  pinMode(HighR, OUTPUT);
  pinMode(LowR, OUTPUT);

  pinMode(D0, INPUT_PULLUP);
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
}


void Data(){
  
  a = digitalRead(D0);
  b = digitalRead(D1);
  c = digitalRead(D2);
  d = digitalRead(D3);

  binary = 8*d + 4*c + 2*b + a;
  
  }
//Forward 
void Backward(){
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnL, 250);
  analogWrite(EnR, 250);

}

//Backward 
void Forward(){
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnL, 250);
  analogWrite(EnR, 250);

}

//Left

void Right(){
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnL,230);
  analogWrite(EnR, 0);

}
void Left(){
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnL, 0);
  analogWrite(EnR,230);

}

void Right_soft(){
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnR,255);
  analogWrite(EnL,255);
  delay(1000);

  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnR,255);
  analogWrite(EnL,255);
  delay(200);

 }

 void Right_medium(){
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnR,255);
  analogWrite(EnL,255);
  delay(600);

  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnR,255);
  analogWrite(EnL,255);
  delay(200);

 }
 void Left_soft(){
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnR,255);
  analogWrite(EnL,255);
      delay(1000);
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnR,255);
  analogWrite(EnL,255);
    delay(200);

 }

 void Left_medium(){
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnR,255);
  analogWrite(EnL,255);
  delay(600);
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnR,255);
  analogWrite(EnL,255);
  delay(200);


 }


void loop() {

  Data();
  if (binary == 0){
    Forward();
    }
   else if(binary == 1){
    Right();
    }
   else if(binary == 2){
     Right();
    }
   else if(binary == 3){
    Right();
    }
  else if(binary == 4){
    Left();
    }
   else if(binary == 5){
    Left();
    }
   else if(binary == 6){
    Left();
    }
    else if(binary > 6){
    Backward();
    }

}
