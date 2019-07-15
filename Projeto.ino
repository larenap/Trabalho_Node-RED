#include "PubSubClient.h"
#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN 2     
#define DHTTYPE DHT22   
#define HOST "127.0.0.1"
#define PORT "1883"
#define TOPIC "Tulio/Energia"
#define TOPIC_WRITE "Tulio/Temperatura"
#define ssid "login"
#define password "senha"
#define CLIENT_ID ""

//Inicializacao cliente MQTT
WifiClient espClient;
PubSubClient mqttClient(espClient);

void setup()
{
    //Configura pinos do ESP8266
    pinMode(1, OUTPUT);//Pino da acao do rele

    //Tenta conectar ao Wifi
    setup_wifi();
 
    //Conecta ao servidor MQTT
    mqttClient.setServer(HOST, PORT);
    mqttClient.setCallback(callback);
}


void loop()
{
    //Checa se desconectou e tenta reconnectar
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    //Le a temperatura do sensor
    float t = dht.readTemperature();
    char my_payload [20];

    my_payload = String(t);
    mqttClient.publish(TOPIC_WRITE,my_payload);

    delay(500);
}

void reconnect()
{
 while (!mqttClient.connected()) {
    // Attempt to connect
    if (mqttClient.connect(CLIENT_ID)) {
      mqttClient.subscribe(TOPICO, 0);
    }
    else {
      delay(5000);
    }
  }
}

void callback(char* topico, byte* payload, unsigned int length)
{
  String payloadStr = "";
  for (int i = 0; i < length; i++) 
  {
   payloadStr += (char)payload[i];
  }
  if(String(topico).equals(TOPIC))
  {
      if(payloadStr == "1")
      {
          DigitalWrite(1,HIGH);
      }
      if(payloadStr == "0")
      {
          DigitalWrite(1, LOW);
      }
  }
}

void setup_wifi()
{

  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
}

