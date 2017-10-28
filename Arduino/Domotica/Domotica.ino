#include <Wire.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>

float Sensibilidad = 0.094; //sensibilidad en V/A para nuestro sensor
float offset = 0.100; // Equivale a la amplitud del ruido
const float POTENCIA_MAXIMA = 35;//Cambiar
int ledAlerta = 9;//Iniciar estos leds
int ledNormal = 11;
int infrarojo;//ASIGNAR ESTE PIN
LiquidCrystal_I2C lcd(0x3F,16,2);
int contar = 0;
IRrecv irrecv(infrarojo);
decode_results results;

void setup() {  
  Serial.begin(9600);
  irrecv.enableIRIn();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);

  pinMode(ledAlerta,OUTPUT);
  pinMode(ledNormal,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);
  digitalWrite(ledNormal,HIGH);
  delay(500);
}

void loop() {   
  float P=get_potencia(); // P=IV watts

if (irrecv.decode(&results))
  {
    int valor = results.value;
    switch (valor) {
      case 0xFF6897:
        //0
        break;
      case 0xFF30CF:
        //1
        break;
      case 0xFF18E7:
        //2
        break;
      case 0xFF7A85:
        //3
        break;
      case 0xFF10EF:
        //4
        break;
    }
  }
  escribir_potencia();
  Serial.print("Potencia total: ");
  Serial.print(P, 3);  
  Serial.println("W");
  verificar(P);
  delay(500);
  contar++;
}

void verificar(int potencia){
  if(potencia > POTENCIA_MAXIMA){
    digitalWrite(ledNormal,LOW);
    digitalWrite(ledAlerta,HIGH);
    delay(10000);
    digitalWrite(8,HIGH);    
    delay(1000);
    alerta();
    
  }else{

    digitalWrite(ledAlerta,LOW);
    digitalWrite(ledNormal,HIGH);
  }
}

void alerta(){
  //TODO: Implementar este método
  //Es el método para cuando hay consumo excesivo, relés
}

float get_corriente1()
{
  float voltajeSensor;
  float corriente=0;
  long tiempo=millis();
  float Imax=0;
  float Imin=0;
  while(millis()-tiempo<500)//realizamos mediciones durante 0.5 segundos
  { 
    voltajeSensor = analogRead(A0) * (5.0 / 1023.0);//lectura del sensor
    corriente=0.9*corriente+0.1*((voltajeSensor-2.515)/Sensibilidad); //Ecuación  para obtener la corriente
    if(corriente>Imax)Imax=corriente;
    if(corriente<Imin)Imin=corriente;
  }
  return(((Imax-Imin)/2)-offset);
}

float get_corriente2()
{
  float voltajeSensor;
  float corriente=0;
  long tiempo=millis();
  float Imax=0;
  float Imin=0;
  while(millis()-tiempo<500)//realizamos mediciones durante 0.5 segundos
  { 
    voltajeSensor = analogRead(A1) * (5.0 / 1023.0);//lectura del sensor
    corriente=0.9*corriente+0.1*((voltajeSensor-2.515)/Sensibilidad); //Ecuación  para obtener la corriente
    if(corriente>Imax)Imax=corriente;
    if(corriente<Imin)Imin=corriente;
  }
  return(((Imax-Imin)/2)-offset);
}

float get_potencia(){
  float p1 = get_corriente1() * 110 * 0.707;
  float p2 = get_corriente2() * 110 * 0.707;
  if (p1 < 9) p1 = 0;
  if (p2 < 9) p2 = 0;
 // Serial.println(get_corriente1());
//  Serial.println(get_corriente2()); 
  return p1 + p2;
}

void escribir_potencia(){
  lcd.setCursor(0,0);
  lcd.print("Potencia:");
  lcd.print(get_potencia());
  lcd.print("W");
}

