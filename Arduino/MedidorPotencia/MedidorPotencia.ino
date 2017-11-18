#include <DS1302.h>
#include <Wire.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>

float sensibilidad = 0.085;
float offset = 0.100; // Equivale a la amplitud del ruido
const float POTENCIA_MAXIMA = 60;//Cambiar
float potenciaDia = 0;
//PINS BOMBILLOS
int bombillo1 = 6;
int bombillo2 = 7;
int bombillo3 = 8;
int infrarojo = 2;
int buzzer = 5;
//PINS DEL LED RGB
int lred = 11;
int lgreen = 10;
int lblue = 9;
//Tiempos 
String fecha = "";
String hora = "";

LiquidCrystal_I2C lcd(0x3F, 16, 2);
IRrecv irrecv(infrarojo);
decode_results results;
DS1302 rtc(2, 3, 4); //Esto debe cambiar

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  rtc.halt(false);
  rtc.writeProtect(false);
  rtc.setDOW(WEDNESDAY);
  rtc.setTime(11, 13, 01);
  rtc.setDate(17, 11, 2017);
  rtc.writeProtect(true);
  pinMode(bombillo1, OUTPUT);
  pinMode(bombillo2, OUTPUT);
  pinMode(bombillo3, OUTPUT);
  pinMode(lred, OUTPUT);
  pinMode(lgreen, OUTPUT);
  pinMode(lblue, OUTPUT);
  digitalWrite(bombillo1, LOW);
  digitalWrite(bombillo2, LOW);
  digitalWrite(bombillo3, LOW);
  delay(500);
}

void loop() {
  get_fecha();
  control();
  float p = get_potencia();
  escribir_potencia();
  verificar(p);
  delay(500);
}

void control() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    switch (results.value) {
      case 0xFF42BD:
        digitalWrite(bombillo1, !digitalRead(bombillo1));
        break;
      case 0xFF4AB5:
        digitalWrite(bombillo2, !digitalRead(bombillo2));
        break;
      case 0x52AD:
        digitalWrite(bombillo3, !digitalRead(bombillo3));
        break;
    }
    irrecv.resume();
  }
}

void verificar(int potencia) {
  if (potencia > POTENCIA_MAXIMA) {
    analogWrite(buzzer,2000);
    colors('r');
    delay(10000);
    digitalWrite(bombillo1, HIGH);
    colors('g');
  }else if(potencia > POTENCIA_MAXIMA -5){
    colors('y');
  }
}

float get_corriente(int puerto)
{
  float voltajeSensor;
  float corriente = 0;
  long tiempo = millis();
  float Imax = 0;
  float Imin = 500000;
  while (millis() - tiempo < 500) //realizamos mediciones durante 0.5 segundos
  {
    voltajeSensor = analogRead(puerto) * (5.0 / 1023.0);//lectura del sensor
    corriente = 0.9 * corriente + 0.1 * ((voltajeSensor - 2.515) / sensibilidad); //Ecuación  para obtener la corriente
    if (corriente > Imax) Imax = corriente;
    if (corriente < Imin) Imin = corriente;
  }
  return (((Imax - Imin) / 2) - offset);
}

float get_potencia() {
  float p1 = get_corriente(A0) * 110 * 0.707;
  float p2 = get_corriente(A1) * 110 * 0.707;
  float p3 = get_corriente(A2) * 110 * 0.707;
  if (p1 < 6) p1 = 0;
  if (p2 < 6) p2 = 0;
  if (p3 < 6) p3 = 0;
  return p1 + p2 + p3;
}

void escribir_potencia() {
  Serial.print("Potencia: ");
  Serial.print(get_potencia());
  Serial.println("W");
  lcd.setCursor(0, 0);
  lcd.print("Potencia:");
  lcd.print(get_potencia());
  lcd.print("W");
}

void get_fecha() {
  fecha = rtc.getDateStr(FORMAT_LONG, FORMAT_LITTLEENDIAN, '/');
  hora = rtc.getTimeStr();
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

void guardarTotalPotencia(){
  int segundos = getValue(hora,':',2).toInt();
  if (segundos % 15 == 0){
    mandar();//HAY QUE IMPLEMENTAR ESTE MÉTODO
    potenciaDia += get_potencia();
  }else if(segundos == 0){
    int dia = getValue(fecha, '/',0).toInt();
    rtc.setDate(++dia, 11, 2017);  
  }
}

//MANDA EL VALOR DE POTENCIA ACTUAL A LA BASE DE DATOS
void mandar(){
  
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
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}




















