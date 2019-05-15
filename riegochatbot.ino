// Incluimos librerías
#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h> // Libreria LCD_I2C

LiquidCrystal_I2C lcd(0x27,16,2); // si no sale con esta direccion  puedes usar (0x3f,16,2) || (0x27,16,2)  ||(0x20,16,2) 

// Variables pines
const byte pinPulsador = 8;
const byte pinRele = 9;

// Instancia a la clase RTC_DS3231
RTC_DS3231 rtc;

// Parámetros de riego
const byte diasRiego = 1;
const byte horaRiego = 18;
byte contadorDias = 0;
boolean centinelaHora = true;

// Tiempo de riego
const long duracionRiego =10000;// Cuánto tiempo riega en milisegundos
unsigned long marcaTiempo; // Marca de tiempo para contar los milisegundos

//Inicializamos variable para lectura del Button Light
const int pinOn = 7;
int On = HIGH;

void setup() {

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sistema de Riego"); 
  lcd.setCursor (0,1);
  lcd.print("  Automatizado");
  delay(4000);
  lcd.clear();
  delay(700);
  lcd.noBacklight();  
  
  // Configuración comunicación serie
  Serial.begin(9600);

  // Modo de los pines
  pinMode(pinPulsador, INPUT_PULLUP);
  pinMode(pinRele, OUTPUT);
  pinMode(2, OUTPUT); //LED ROJO
  pinMode(4, OUTPUT); //LED VERDE
  pinMode(pinOn, INPUT); //Boton backlight LCD

  // Iniciamos el rtc
  if (!rtc.begin()) {
    lcd.print("Falta RTC");
    while (1);
  }

    // Ajuste de fecha y hora. Año/Mes/Dia - Hora/Minutos/Segundos
  rtc.adjust(DateTime(2019, 5, 10, 12, 12, 00));
}

void loop() {
  
  // Obtenemos la fecha
  DateTime tiempoReferencia = rtc.now();
  
  //Lectura digital del pin boton consulta
  On = digitalRead(pinOn);

    //mandar mensaje a puerto serie en función del valor leido
  if (On == LOW) {
      lcd.backlight();
      delay(500);
      lcd.clear();
      if(digitalRead(pinRele) == 0){      
      lcd.print("  Riego Apagado");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" Temperatura: ");
      lcd.setCursor(13,0);
      lcd.print(analogRead(A0));
      lcd.setCursor(2,1);
      lcd.print("Hora:");
      lcd.print(tiempoReferencia.hour(), DEC);
      lcd.print(':');
      lcd.print(tiempoReferencia.minute(), DEC);
      lcd.print(':');
      lcd.print(tiempoReferencia.second(), DEC);
      lcd.print(' ');
      lcd.println();
      lcd.display();
      delay(2000);
      }
      else{
        lcd.print(" Riego Encendido");
        }
      delay(10000);
      lcd.noBacklight();
      lcd.clear();
  }

  
  // Leer el pulsador
  byte estadoPulsador = digitalRead(pinPulsador);

  // Esperamos para evitar dobles pulsaciones
  delay(300);

  // Comprobamos si se ha pulsado el pulsador
  if (estadoPulsador == HIGH && analogRead(A0) <= 700) {
    lcd.clear();
    lcd.backlight();
    lcd.print(" Regando....");
      
    // Cambiamos de estado el relé
    digitalWrite(pinRele, !digitalRead(pinRele));

    // Comenzamos a contar el tiempo
    marcaTiempo = millis();
  }
  
  if(estadoPulsador == HIGH && analogRead(A0) > 700){
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print(" Temperatura no");
    lcd.setCursor(1,1);
    lcd.print("Apta para Riego");
    delay(3000);
    lcd.noBacklight();
    lcd.clear();
    }

  // Mostramos la fecha por el monitor LCD
  
  //lcd.setCursor(0,0);
  //lcd.print(" Fecha:");
  //lcd.print(tiempoReferencia.day(), DEC);
  //lcd.print('/');
  //lcd.print(tiempoReferencia.month(), DEC);
  //lcd.print('/');
  //lcd.print(tiempoReferencia.year(), DEC);
  //lcd.print(' ');
  //lcd.setCursor(1,1);
  //lcd.print("Hora:");
  //lcd.print(tiempoReferencia.hour(), DEC);
  //lcd.print(':');
  //lcd.print(tiempoReferencia.minute(), DEC);
  //lcd.print(':');
  //lcd.print(tiempoReferencia.second(), DEC);
  //lcd.print(' ');
  //lcd.println();

 
  // Si es la hora de regar
  if (tiempoReferencia.hour() == horaRiego && centinelaHora) {
    // Marcamos para que no vuelva a entrar
    centinelaHora = false;

    // Sumamos una al contador de días
    contadorDias = contadorDias + 1;

    // Si han pasado los días para regar
    if (contadorDias == diasRiego) {
      // Regamos
      digitalWrite(pinRele, HIGH);

      // Comenzamos a contar el tiempo
      marcaTiempo = millis();

      // Iniciamos el contador de días
      contadorDias = 0;
    }
    // Si es otra hora marcamos para que vuelva a detectar
  } else if (tiempoReferencia.hour() != horaRiego) {
    centinelaHora = true;
  }

  // Si está regando
  if (digitalRead(pinRele)) {
    
    // Protegemos para cuando se reinicie millis()
    if (millis() < marcaTiempo) {
      marcaTiempo = 0;
    }
    
    // Comprobamos si ha pasado el tiempo
    if (millis() - marcaTiempo >= duracionRiego) {
      // Apagamos el riego
      digitalWrite(pinRele, LOW);
      lcd.clear();
      lcd.print("  Riego Apagado");
      delay(3000);
      lcd.noBacklight();
      lcd.clear();
    }
  }


//Lectura de los LEDs
  if(digitalRead(pinRele))
  {
    digitalWrite(4, HIGH);
    digitalWrite(2, LOW);
    }
    else{
      digitalWrite(2, HIGH);
      digitalWrite(4, LOW);
      } 

//DATOS DE COMUNICACION ENTRE TELEGRAM Y ARDUINO

  //Recogemos datos del sensor y la procesamos
  Temperatura = analogRead(A0);
  gradosCentigrados = ((5*Temperatura*10)/1024);
  //Serial.println(gradosCentigrados);

/*  if (Serial.available()>=0) {
    int temp = Serial.read();
    if (temp == 'T') {
    Serial.println(gradosCentigrados);
    }
  }
*/
 

//Codigo del Chatbot
if(digitalRead(pinPulsador)){
  Serial.println("H");
  }
  
  if (Serial.available()) {
    char Letra = Serial.read();
    if (Letra == 'H') {
      digitalWrite(pinRele, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(2, LOW);
      lcd.clear();
      lcd.backlight();
      lcd.setCursor(0,0);
      lcd.print(" Regando....");
      delay(duracionRiego);
    }else if (Serial.available()>=0) {
    int temp = Serial.read();
    if (temp == 'T') {
    Serial.println(String(temp));
    }
  }
}}
