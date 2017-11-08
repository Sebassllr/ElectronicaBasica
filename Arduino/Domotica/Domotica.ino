#include <Wire.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>

float sensibilidad = 0.085;
float offset = 0.100; // Equivale a la amplitud del ruido
const float POTENCIA_MAXIMA = 60;//Cambiar
int ledAlerta = 9;//Iniciar estos leds
int bombillo1 = 6;
int bombillo2 = 7;
int bombillo3 = 8;
int infrarojo = 2;//ASIGNAR ESTE PIN

LiquidCrystal_I2C lcd(0x3F, 16, 2);
IRrecv irrecv(infrarojo);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);

  pinMode(ledAlerta, OUTPUT);
  pinMode(bombillo1, OUTPUT);
  pinMode(bombillo2, OUTPUT);
  pinMode(bombillo3, OUTPUT);
  digitalWrite(bombillo1, LOW);
  digitalWrite(bombillo2, LOW);
  digitalWrite(bombillo3, LOW);
  delay(500);
}

void loop() {
  float P = get_potencia(); // P=IV watts
  escribir_potencia();

  verificar(P);

  //Verificar si el control ha sido presionado
  if (irrecv.decode(&results))
  {
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
  delay(500);
}

void verificar(int potencia) {
  if (potencia > POTENCIA_MAXIMA) {
    digitalWrite(ledAlerta, HIGH);
    delay(10000);
    digitalWrite(bombillo1, HIGH);
    digitalWrite(ledAlerta, LOW);
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
    if (corriente > Imax)Imax = corriente;
    if (corriente < Imin)Imin = corriente;
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
