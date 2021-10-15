#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <IRremote.h>

int rs=7;
int en=8;
int d4=9;
int d5=10;
int d6=11;
int d7=12;

int buzzPin = 4;

int buttonPin = 2;
int buttonState = 0;

int alrm_h = 18;
int alrm_m = 25;
int alrm_s = 0;

int IRpin = 13;
IRrecv IR(IRpin);
decode_results cmd;
String myCom;

LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
Servo servo;

void setup() {
  Serial.begin(9600);
  
  lcd.begin(16,2);

  servo.attach(6);
  servo.write(90);

  pinMode(buzzPin,OUTPUT);

  pinMode(buttonPin, INPUT);

  IR.enableIRIn();
  
}
 
void loop() {
  while (IR.decode(&cmd)==0){ 

  tmElements_t tm;

  lcd.setCursor(0,0);

  buttonState = digitalRead(buttonPin);

  if (RTC.read(tm)) {
    lcd.print("TIME: ");
    lcd.setCursor(6,0);
    print2digits(tm.Hour);
    lcd.setCursor(8,0);
    lcd.print(":");
    lcd.setCursor(9,0);
    print2digits(tm.Minute);
    lcd.setCursor(11,0);
    lcd.print(":");
    lcd.setCursor(12,0);
    print2digits(tm.Second);

    lcd.setCursor(0,1);
    lcd.print("ALRM: ");
    lcd.setCursor(6,1);
    print2digits(alrm_h);
    lcd.setCursor(8,1);
    lcd.print(":");
    lcd.setCursor(9,1);
    print2digits(alrm_m);
    lcd.setCursor(11, 1);
    lcd.print(":");
    lcd.setCursor(12, 1);
    print2digits(alrm_s);

    }
   
  if (tm.Hour == alrm_h && tm.Minute == alrm_m && tm.Second == alrm_s) {
    servo.write(39);
    delay(500);
    tone(buzzPin, 100);
      }

  if (buttonState == HIGH) {
    servo.write(90);
    noTone(buzzPin);
  }
  
  }
  Serial.println(cmd.value,HEX);
  delay(1500);
  IR.resume();
 
  if (cmd.value==0xFF629D){
    alrm_h += 1;
  }
  if (cmd.value==0xFFA857){
    alrm_h -= 1;
  }
  if (cmd.value==0xFF906F){
    alrm_m += 1;
  }
  if (cmd.value==0xFFE01F){
    alrm_m -= 1;
  }
  if (cmd.value==0xFFA25D){
    servo.write(39); 
    delay(2000);
    servo.write(90);
  }

}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    lcd.write('0');
  }
  lcd.print(number);
}
