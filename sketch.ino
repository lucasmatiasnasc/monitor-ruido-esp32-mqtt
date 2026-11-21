//PROJETO DE MONITORAMENTO DE RUIDO - MICROCONTROLADORES.
// Natan estevão, Lucas matias e Daniel asafe.
#include <WiFi.h>
#include <PubSubClient.h>
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "biblioteca/ruidoLAN";
#define SENSOR_SOM 34
#define LED_VERDE 25
#define LED_VERMELHO 26
#define BUZZER 27
const int limiteRuido = 2000;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long ultimaPublicacao = 0;
const unsigned long intervaloPublicacao = 3000;

bool estadoAnterior = false; 

void conectarWiFi() {
  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.println(WiFi.localIP());
}
void conectarMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    String clientId = "ESP32_Ruido_";
    clientId += String((uint32_t)ESP.getEfuseMac(), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println(" conectado!");
    } else {
      Serial.print(" falhou. Estado: ");
      Serial.println(client.state());
      delay(3000);
    }
  }
}

void controlarIndicadores(bool ruidoAlto) {
  digitalWrite(LED_VERDE, ruidoAlto ? LOW : HIGH);
  digitalWrite(LED_VERMELHO, ruidoAlto ? HIGH : LOW);

 
  if (ruidoAlto && !estadoAnterior) {
    tone(BUZZER, 1000);   
  } else if (!ruidoAlto && estadoAnterior) {
    noTone(BUZZER);    
  }
  estadoAnterior = ruidoAlto;
}

void publicarMQTT(int nivel, bool ruidoAlto) {
  char mensagem[100];
  snprintf(mensagem, sizeof(mensagem),
           "{\"status\":\"%s\",\"nivel\":%d}",
           ruidoAlto ? "alto" : "normal", nivel);

  bool enviado = client.publish(mqtt_topic, mensagem);
  Serial.print(enviado ? "MQTT enviado: " : "Falha ao publicar: ");
  Serial.println(mensagem);
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  analogReadResolution(12);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(BUZZER, LOW);

  client.setServer(mqtt_server, mqtt_port);
  conectarWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    conectarWiFi();
  }

  if (!client.connected()) {
    conectarMQTT();
  }
  client.loop();

  int nivel = analogRead(SENSOR_SOM);
  bool ruidoAlto = (nivel > limiteRuido);

  controlarIndicadores(ruidoAlto);

  Serial.print("Nivel simulado: ");
  Serial.print(nivel);
  Serial.println(ruidoAlto ? " | RUIDO ALTO" : " | NORMAL");

  if (millis() - ultimaPublicacao >= intervaloPublicacao) {
    publicarMQTT(nivel, ruidoAlto);
    ultimaPublicacao = millis();
  }

  delay(100);
}
