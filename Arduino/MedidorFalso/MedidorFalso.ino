#include <DS1302.h>
#include <Wire.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
//PINS BOMBILLOS
int bombillo1 = 2;
int bombillo2 = 3;
int bombillo3 = 11;
int noluz1 = 5;
int noluz2 = 6;
int noluz3 = 7;
int infrarojo = 40;
int buzzer = 12;
//PINS DEL LED RGB
int lred = 8;
int lgreen = 9;
int lblue = 10;
boolean luz = true;
String fecha = "";
String hora = "";
LiquidCrystal_I2C lcd(0x27, 16, 4);
IRrecv irrecv(infrarojo);
decode_results results;
//DS1302 rtc(28,29 ,30); //Esto debe cambiar

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);  
  /*rtc.halt(false);
  rtc.writeProtect(false);
  rtc.setDOW(WEDNESDAY);
  rtc.setTime(16, 11, 01);
  rtc.setDate(20, 11, 2017);
  rtc.writeProtect(true);*/
  pinMode(bombillo1, OUTPUT);
  pinMode(bombillo2, OUTPUT);
  pinMode(bombillo3, OUTPUT);
  pinMode(noluz1, OUTPUT);
  pinMode(noluz2, OUTPUT);
  pinMode(noluz3, OUTPUT);
  digitalWrite(noluz1, LOW);
  digitalWrite(noluz2, LOW);
  digitalWrite(noluz3, LOW);
  pinMode(lred, OUTPUT);
  pinMode(lgreen, OUTPUT);
  pinMode(lblue, OUTPUT);
  digitalWrite(bombillo1, HIGH);
  digitalWrite(bombillo2, HIGH);
  digitalWrite(bombillo3, HIGH);
  delay(500);
}

void loop() {
  lcd.clear();
  colors('g');
  get_fecha();
  control();
  verificar();
  delay(500);
}

void verificar(){
  if(digitalRead(bombillo1) == LOW && digitalRead(bombillo2) == LOW && digitalRead(bombillo3) == LOW){
    lcd.clear();
    get_fecha();
    colors('r');
    analogWrite(buzzer,2000);
    lcd.setCursor(0,3);
    lcd.print("Consumo Excesivo");
    delay(10000);
    digitalWrite(bombillo1,HIGH);
    analogWrite(buzzer,0);
    colors('g');
    lcd.clear();
    get_fecha();
  }else if(digitalRead(bombillo1) == LOW && digitalRead(bombillo3) == LOW){
    lcd.clear();
    get_fecha();
    colors('y');
    lcd.setCursor(0,3);
    lcd.print("Consumo Alto");
  }
}

void control() {

  if (irrecv.decode(&results)) {
    lcd.setCursor(0,2);
    lcd.print(results.value,HEX);
    switch (results.value) {
      case 0xFF42BD:
        if(luz)digitalWrite(bombillo1, !digitalRead(bombillo1));
        else digitalWrite(noluz1, !digitalRead(noluz1));
        break;
      case 0xFF4AB5:
        if(luz)digitalWrite(bombillo2, !digitalRead(bombillo2));
        else digitalWrite(noluz2, !digitalRead(noluz2));
        break;
      case 0xFF52AD:
        if(luz)digitalWrite(bombillo3, !digitalRead(bombillo3));
        else digitalWrite(noluz3, !digitalRead(noluz3));
        break;
      case 0xFF629D:
        digitalWrite(bombillo1,HIGH);
        digitalWrite(bombillo2,HIGH);
        digitalWrite(bombillo3,HIGH);
        luz = !luz;
        break;        
    }
    irrecv.resume();
  }
}

void get_fecha() {
  /*fecha = rtc.getDateStr(FORMAT_LONG, FORMAT_LITTLEENDIAN, '/');
  hora = rtc.getTimeStr();
  lcd.setCursor(0,0);
  lcd.print(fecha);
  lcd.setCursor(0,1);
  lcd.print(hora);*/
}

void colors(char color) {
  switch (color) {
    case 'y':
      analogWrite(lred, 255);
      analogWrite(lgreen, 255);
      analogWrite(lblue, 0);
      break;
    case 'g':
      analogWrite(lred, 0);
      analogWrite(lgreen, 255);
      analogWrite(lblue, 0);
      break;
    case 'r':
      analogWrite(lred, 255);
      analogWrite(lgreen, 0);
      analogWrite(lblue, 0);
      break;
  }
}

//Parecido a split de java
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
