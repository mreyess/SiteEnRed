#include <SPI.h>
#include <Ethernet.h> //Declaración de la direcciones MAC e IP. También del puerto 80 
#include <DHT.h>
#include <Servo.h>

#define DHTTYPE DHT11
#define rojo 4
#define verde 7
#define sensorT A2
#define buzz 9

const int DHTPin = 2;       //Pin de entrada para el sensor de temperatura y humedad
const int EchoPin = 5;
const int TriggerPin = 6;

float hum;                  //Definimos la variable de la temperatura

int cm = 0;

char c;

int LED;

float voltaje = 0;
float temperatura = 0;

DHT dht(DHTPin, DHTTYPE);   //Inicializamos el sensor de temperatura definiendo el tipo de sensor y el pin de entrada
Servo servoMotor;


byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; //MAC
IPAddress ip(192, 168, 0, 19); //IP
EthernetServer servidor(80);
int PIN_LED = 8;
String readString = String(30); //lee los caracteres de una secuencia en una cadena.
//Los strings se representan como arrays de caracteres (tipo char)
String state = String(3);

boolean acceso = false;
boolean lineaenblanco;

void setup() {
  Serial.begin(9600);
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(rojo, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(buzz, OUTPUT);

  digitalWrite(rojo, LOW);
  digitalWrite(verde, HIGH);
  digitalWrite(buzz, LOW);

  dht.begin();             //Iniciamos con el funcionamiento del sensor de temperatura
  servoMotor.attach(3);
  servoMotor.write(0);

  Ethernet.begin(mac, ip); //Inicializamos con las direcciones asignadas
  servidor.begin();
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  state = "OFF";
}
void loop() {
  hum = dht.readHumidity();  //Leemos la humedad del sensor

  voltaje = analogRead(sensorT) * 3.3 / 1023;
  temperatura = voltaje * 100;

  cm = ping(TriggerPin, EchoPin);

  if (temperatura > 25) {
    digitalWrite(buzz, HIGH);
    Serial.println(temperatura);
  } else {
    digitalWrite(buzz, LOW);
    Serial.println(temperatura);
  }


  if (cm <= 5) {
    digitalWrite(rojo, HIGH);
    digitalWrite(verde, LOW);
    servoMotor.write(90);
    acceso = true;
  } else {
    digitalWrite(rojo, LOW);
    digitalWrite(verde, HIGH);
    servoMotor.write(0);
  }

  delay(250);

  //EthernetClient Crea un cliente que se puede conectar a
  //una dirección específica de Internet IP
  EthernetClient cliente = servidor.available();
  if (cliente) {
    lineaenblanco = true;
    while (cliente.connected()) {
      if (cliente.available()) {
        c = cliente.read();
        if (readString.length() < 30) {
          readString.concat(c);
          //Cliente conectado
          //Leemos petición HTTP caracter a caracter
          //Almacenar los caracteres en la variable readString
        }
        if (c == '\n' && lineaenblanco) //Si la petición HTTP ha finalizado
        {
          LED = readString.indexOf("LED=");

          if (readString.substring(LED, LED + 5) == "LED=T") {
            digitalWrite(PIN_LED, HIGH);
            state = "Encendido";
          }
          else if (readString.substring(LED, LED + 5) == "LED=F") {
            digitalWrite(PIN_LED, LOW);
            state = "Apagado";
          }

          //Cabecera HTTP estándar
          cliente.println("HTTP/1.1 200 OK");
          cliente.println("Content-Type: text/html");
          cliente.println(); //Página Web en HTML
          cliente.println("<html style='background-image:linear-gradient(blue, white); font:oblique bold 120% cursive'>");
          cliente.println("<head>");
          cliente.println("<meta http-equiv='refresh' content='3'");
          cliente.println("</head>");
          cliente.println("<body width=100% height=100%>");
          cliente.println("<center>");
          cliente.println("<h1 style='color: white;'>SISTEMA DE CONTROL DE ESTADO DEL SITE</h1>");
          cliente.print("<h3 style='color: yellow;'>Estado de la lampara: ");
          cliente.print(state);
          cliente.print("</h3><br><br><h3 style='color: yellow;'>");
          if (temperatura > 25) {
            cliente.print("ALERTA EL SITE presenta una alta temperatura de:");
          } else {
            cliente.print("Temperatura del SITE: ");
          }
          cliente.print(temperatura);
          cliente.print("</h3><br><br>");
          cliente.print("<h3 style='color: yellow;'>Humedad del SITE: ");
          cliente.print(hum);

          if(acceso){
          cliente.print("</h3><br><br><h3 style='color: red;'>ALERTA, Se detecto una entrada en el SITE.");
          }
          cliente.print("</h3><br><br><br><br>");
          cliente.println("<input type=submit value='Encender SITE' style='width:200px;height:75px;background-color:#249c04; border-radius:6px; color:white;' onClick=location.href='./?LED=T' />");
          cliente.println("<input type=submit value='Apagar SITE' style='width:200px;height:75px;background-color:red; border-radius:6px; color:white;' onClick=location.href='./?LED=F' />");
          cliente.println("</center>");
          cliente.println("</body>");
          cliente.println("</html>");
          cliente.stop();
          //Cierro conexión con el cliente
          readString = "";
        }
      }
    }
  }

  acceso = false;
}

int ping(int TriggerPin, int EchoPin) {
  long duration, distanceCm;

  digitalWrite(TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
  delayMicroseconds(4);
  digitalWrite(TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);

  duration = pulseIn(EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos

  distanceCm = duration * 10 / 292 / 2;  //convertimos a distancia, en cm
  return distanceCm;
}
