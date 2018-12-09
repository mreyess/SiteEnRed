#include <DHT.h> 
#include <Servo.h>

#define DHTTYPE DHT11
#define rojo 4
#define verde 7
#define sensorT A2
#define buzz 8

const int EchoPin = 5;
const int TriggerPin = 6;
const int LedPin = 13;

const int DHTPin = 2;       //Pin de entrada para el sensor de temperatura y humedad

float hum;                  //Definimos la variable de la temperatura

String humedad= "Humedad: ";

int cm= 0;

float voltaje = 0;
float temperatura = 0;




DHT dht(DHTPin, DHTTYPE);   //Inicializamos el sensor de temperatura definiendo el tipo de sensor y el pin de entrada
Servo servoMotor;

void setup() {
  Serial.begin(115200);
   pinMode(LedPin, OUTPUT);
   pinMode(TriggerPin, OUTPUT);
   pinMode(EchoPin, INPUT);
   pinMode(rojo, OUTPUT);
   pinMode(verde, OUTPUT);
   pinMode(buzz, OUTPUT);
   digitalWrite(rojo, LOW);
   digitalWrite(verde, HIGH);
   digitalWrite(buzz, LOW);
   servoMotor.attach(3);
   dht.begin();             //Iniciamos con el funcionamiento del sensor de temperatura
   servoMotor.write(0);

}

void loop() {
  hum = dht.readHumidity();  //Leemos la humedad del sensor
  
  voltaje = analogRead(sensorT)*3.3/1023;
  temperatura = voltaje * 100; 

  if (isnan(hum)) {
    Serial.println("Error al intentar leer las mediciones");
   }else{
   humedad="Humedad: ";
   humedad.concat(hum);
   Serial.println(humedad);
   
      
      
      }
   cm = ping(TriggerPin, EchoPin);

   if(temperatura > 25){
      digitalWrite(buzz, HIGH);
      Serial.println(temperatura);
    }else{
      digitalWrite(buzz, LOW);
      Serial.println(temperatura);
      }

   if(cm <= 5){
      digitalWrite(rojo, HIGH);
      digitalWrite(verde, LOW);
      servoMotor.write(90);
    }else{
      digitalWrite(rojo, LOW);
      digitalWrite(verde, HIGH);
      servoMotor.write(0);
      }
    
    delay(250);
    temperatura= 0;

}

int ping(int TriggerPin, int EchoPin) {
   long duration, distanceCm;
   
   digitalWrite(TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
   delayMicroseconds(4);
   digitalWrite(TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
   delayMicroseconds(10);
   digitalWrite(TriggerPin, LOW);
   
   duration = pulseIn(EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
   
   distanceCm = duration * 10 / 292/ 2;   //convertimos a distancia, en cm
   return distanceCm;
}
