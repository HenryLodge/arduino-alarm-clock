#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <LiquidCrystal.h>
#include <Servo.h>

int rs=7;
int en=8;
int d4=9;
int d5=10;
int d6=11;
int d7=12;

int buzzPin = 4;

int buttonPin = 2;
int buttonState = 0;

int alrm_h = 22;
int alrm_m = 18;
int alrm_s = 0;

LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
Servo servo;

void setup() {
  Serial.begin(9600);
  
  lcd.begin(16,2);

  servo.attach(6);
  servo.write(90);

  pinMode(buzzPin,OUTPUT);

  pinMode(buttonPin, INPUT);
  
}
 
void loop() {
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

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    lcd.write('0');
  }
  lcd.print(number);
}
