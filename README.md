# SITE EN RED

## Presentación 
<img src="https://github.com/mreyess/SiteEnRed/blob/master/Imagenes/Presentacion.PNG">

## Introducción
El siguiente proyecto trata de un sistema de seguridad de un web site el cual simulara
alertas producidas por sensores tanto de humedad, temperatura, así como
un sensor ultrasonico el cual detectara el movimiento de personal no autorizado.

El sistema actual cumple con las siguientes requisitos:
* Se debe incluir al menos tres sensores y un actuador.
* Se deben identificar puertos y protocolos de comunicación.
* El sistema de control debe estar conectado en red.
* El sistema de control debe de mostrar estadísticas de los sensores y tomar acciones en consecuencia.

Caracteristicas principales del sistema:
* La aplicación desarrollada es en web, usando HTML
* Para la comunicación entre placas y la aplicación, se utilizo el protocolo TCP/IP
* Enviara notifcaciones de alerta según el senso que se active
* El sistema envia dichas notificaciones de forma automatizada
* Los resultados para el usuario son mostrados en una página web
* Si la puerta principal sel web site es abierta madnda una notificación de que alguien a ingresado

## Descripción de tecnologias nuevas

PLACA SHIELD ETHERNET W5100

El shield provee un conector ethernet estándar RJ45. La ethernet shield dispone de unos conectores que
permiten conectar a su vez otras placas encima y apilarlas sobre la placa Arduino.

Arduino usa los pines digitales 10, 11, 12, y 13 (SPI) para comunicarse con el W5100 en la ethernet shield.
Estos pines no pueden ser usados para e/s genéricas.

El botón de reset en la shield resetea ambos, el W5100 y la placa Arduino.

La shield contiene varios LEDs para información:

* ON: indica que la placa y la shield están alimentadas
* LINK: indica la presencia de un enlace de red y parpadea cuando la shield envía o recibe datos
* 100M: indica la presencia de una conexión de red de 100 Mb/s (de forma opuesta a una de 10Mb/s)
* RX: parpadea cuando el shield recibe datos
* TX: parpadea cuando el shield envía datos

SENSOR ULTRASONICO HCSR04

El HC-SR04 es un sensor de distancias por ultrasonidos capaz de detectar objetos y calcular la distancia a
la que se encuentra en un rango de 2 a 450 cm. El sensor funciona por ultrasonidos y contiene toda la electrónica
encargada de hacer la medición. Su uso es tan sencillo como enviar el pulso de arranque y medir la anchura del pulso
de retorno. De muy pequeño tamaño, el HC-SR04 se destaca por su bajo consumo, gran precisión y bajo precio por lo
que esta reemplazando a los sensores polaroid en los robots mas recientes.

Caracteristicas:

* Dimensiones del circuito: 43 x 20 x 17 mm
* Tensión de alimentación: 5 Vcc
* Frecuencia de trabajo: 40 KHz
* Rango máximo: 4.5 m
* Rango mínimo: 1.7 cm
* Duración mínima del pulso de disparo (nivel TTL): 10 μS.
* Duración del pulso eco de salida (nivel TTL): 100-25000 μS.
* Tiempo mínimo de espera entre una medida y el inicio de otra 20 mS.

## Maeriales
De Hardware

* Protoboard
* Arduino Uno
* Placa shield etherneT w5100
* 1 servo motor sg90
* Sensor de humedad dht11
* Sensor de temperatura lm35
* Sensor ultrasonico HC-SR04
* Leds de diferentes colores
* Resistencias de 330 homs
* 1 buzzer
* Cables Dupont(Jumpers) macho macho
* Cables Dupont(Jumpers) hembra macho
* Cable ethernet con conectores RJ-45
* Cable USB para arduino
* Computadora
* Maqueta simuladora de web site

De Software

* Arduino IDE
* Libreria SPI
* Libreria Ethernet
* Libreria DTH
* Libreria Servo
* Controlador de versiones GIT

## Imagenes del Proyecto

### Maqueta Vista Lateral
<img width="400px" height="400px" src="https://github.com/mreyess/SiteEnRed/blob/master/Imagenes/VistaLateral.jpeg">

### Maqueta Vista Superior
<img width="400px" height="400px" src="https://github.com/mreyess/SiteEnRed/blob/master/Imagenes/VistaInclinada.jpeg">

### Conexión de sensores y placas.
<img width="400px" height="400px" src="https://github.com/mreyess/SiteEnRed/blob/master/Imagenes/EsquemaConexión1.jpeg">

### Conexión Circuito con Maqueta
<img width="400px" height="400px" src="https://github.com/mreyess/SiteEnRed/blob/master/Imagenes/EsquemaConexión1.jpeg">
