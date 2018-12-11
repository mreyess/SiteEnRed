#include <SPI.h>		// Importaciòn de la libreria para la comunicacion en red
#include <Ethernet.h> 	// Importacion de la linreria para controlar la placa Ethernet Shield
#include <DHT.h>		// Importacion de la libreria para la medicion de la humedad
#include <Servo.h>		// Importacion de la libreria para el manejo del servomotor (Puerta automatica)

#define DHTTYPE DHT11	// Definimos el tipo de sensor de humedad que se usara
#define rojo 4			// Definicion de pines por los cuales se s´ñalara cuando la puerta esta abierta o cerrada
#define verde 7
#define sensorT A2		// Declaracion de la entrada analogica para el sensor de temperatura
#define buzz 9

const int DHTPin = 2;       //Pin de entrada para el sensor de temperatura y humedad
const int EchoPin = 5;		//Pines que se usaran para el sensor ultrasonico
const int TriggerPin = 6;

float hum;                  //Definimos la variable de la temperatura

int cm = 0;					// Definicion de la variable que guardara los centimetros en el sensor ultrasonico

char c;

int LED;

float voltaje = 0;
float temperatura = 0;

DHT dht(DHTPin, DHTTYPE);   //Inicializamos el sensor de temperatura definiendo el tipo de sensor y el pin de entrada
Servo servoMotor;			// Declaracion de la variable tipo servo (Puerta automatica)


byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // Asignacion de la direccion  MAC a la placa Shield
IPAddress ip(192, 168, 0, 19); // Asignacion de la direccion IP para la placa shield
EthernetServer servidor(80);//Especificacion del puerto por el cual se va a trabajar
int PIN_LED = 8;			//Variable para encender las luces de manera remota
String readString = String(30); //lee los caracteres de una secuencia en una cadena.
//Los strings se representan como arrays de caracteres (tipo char)
String state = String(3); //Variable que guardara el estado de las luces del SITE

boolean acceso = false; //Variable que guarda su hubo un acceso enel SITE
boolean lineaenblanco; //VAriable para saber si es el fin de la URL que se leyó

void setup() {
  Serial.begin(9600); //Iniciamos la comunicacion Serial en 9600
  pinMode(TriggerPin, OUTPUT);  //Establecemos el modo de entrada y salida del sensor ultrasocnico
  pinMode(EchoPin, INPUT);      
  pinMode(rojo, OUTPUT);        //Establecemos los modos de salida delas luces de entrada y la alarma
  pinMode(verde, OUTPUT);       /
  pinMode(buzz, OUTPUT);

// Definimos los estados iniciales de cada componente
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

  //Calculamos la temperatura captada por el sensor
  voltaje = analogRead(sensorT) * 3.3 / 1023;
  temperatura = voltaje * 100;

  cm = ping(TriggerPin, EchoPin); // Asignamos la distencia calculada en base a los datos registrados por el sensor en el metodo Ping

  //Si la temperatura sobre pasa de los 25 grados entonces que suene la alarma
  if (temperatura > 25) {
    digitalWrite(buzz, HIGH);
    Serial.println(temperatura);
  } else {
    digitalWrite(buzz, LOW);
    Serial.println(temperatura);
  }

//Si el sensor ultrasonico capta movimiento en menos de 5 centimetros entonces manda una alerta a la pagina web y se activa el acceso automatico al SITE
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
          cliente.println("Content-Type: text/html"); //Definimos que el contenido sera codigo HTML
          cliente.println(); //Página Web en HTML
          cliente.println("<html style='background-image:linear-gradient(blue, white); font:oblique bold 120% cursive'>"); //Establecemos el color del fondo, el inicio de pagina y el tipo y tamaño de letra
          cliente.println("<head>");  
          cliente.println("<meta http-equiv='refresh' content='3'"); //Actualizamos la pagina cada 3 segundos
          cliente.println("</head>");
          cliente.println("<body width=100% height=100%>"); //Definimos los tamaños del cuerpo de la pagina
          cliente.println("<center>");
          cliente.println("<h1 style='color: white;'>SISTEMA DE CONTROL DE ESTADO DEL SITE</h1>"); //Establecemos el titulo de la pagina
          cliente.print("<h3 style='color: yellow;'>Estado de la lampara: "); //Definimos los colores y tipos de letra a usar
          cliente.print(state);
          cliente.print("</h3><br><br><h3 style='color: yellow;'>");
          if (temperatura > 25) { //Si se registro una temperatura alta, entonces manda una alerta a la pagina
            cliente.print("ALERTA EL SITE presenta una alta temperatura de:");
          } else {
            cliente.print("Temperatura del SITE: ");
          }
          cliente.print(temperatura);
          cliente.print("</h3><br><br>");
          cliente.print("<h3 style='color: yellow;'>Humedad del SITE: "); //Mostramos la humedad del SITE
          cliente.print(hum);

          //Si se presento un acceso al SITE, se notifica que lo hubo en la pagina web
          if(acceso){
          cliente.print("</h3><br><br><h3 style='color: red;'>ALERTA, Se detecto una entrada en el SITE.");
          }
          cliente.print("</h3><br><br><br><br>");

          //Definimos los botones para controlar la luz del Site de manera remota
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
