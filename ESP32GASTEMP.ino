#include "WiFi.h"
#include <PubSubClient.h>
#include "DHT.h"

// ANDERSON ARRUDA, JOSÉ EMILIO CATANHO DA SILVA JUNIOR IFSP CATANDUVA-SP
// ESP32 COM SENSOR DE TEMPARATURA E POTENCIOMETRO SIMULANDO UM SENSOR DE GAS
// SE A TEMPERATURA OU O SENSOR (SIMULADO PELO POTENCIOMENTRO) ESTIVER ACIMA
// DO NIVEL 50, ACIONA O LED VERMELHO E MANDA MENSAGEM DE ATENÇÃO



// WiFi Credential
const char *ssid = "Wokwi-GUEST";
const char *password = "";


// MQTT Credential
//const char *mqtt_broker = "broker.hivemq.com";
const char *mqtt_broker = "ec2-3-83-102-68.compute-1.amazonaws.com";
const char *mqtt_username = "";
const char *mqtt_password = "";
const int mqtt_port = 1883;

#define DHTPIN 32
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float temp, hum;
int ledPin = 23;
float gas = 0;

// MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Task for GPS Dual Core
TaskHandle_t Task0;

void wifiSetup() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the Wi-Fi network");
  //   connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  checkConnection();
  //  Publish and subscribe
  //  client.subscribe(sub_topic);

}

void checkConnection() {
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
    //    client.subscribe(sub_topic);
  }
}

void callback(char *topic, byte * payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
}





void setup()
{
  Serial.begin(115200);
  wifiSetup();
  dht.begin();

   pinMode(ledPin, OUTPUT); // set pin #8 as OUTPUT pin

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
    mqttTask,   /* Task function. */
    "Task0",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task0,      /* Task handle to keep track of created task */
    1);          /* pin task to core 0 */
  delay(500);
}

// Network Task
void mqttTask( void * pvParameters ) {
  while (1) {
    // Serial.print("Network Task running on core ");
    // Serial.println(xPortGetCoreID());
    checkConnection();
    client.publish("esp32temperatura", String(temp).c_str());
    client.publish("esp32umidade", String(hum).c_str());
    client.publish("esp32gas", String(gas).c_str());
    client.loop();
    delay(100);
  }
}

void loop() {
  gas = analogRead(33);
  gas = gas*100/4033;

  if(temp > 50 || gas > 50) {
    digitalWrite(ledPin, HIGH);
    Serial.println("PERIGO, DETECTADO TAXAS ALTAS DE GAS OU TEMPERATURA"); 
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("TAXAS DE GAS E TEMPERATURA EM NIVEIS ACEITAVEIS"); 
  }
    
  Serial.println(gas); 
  

  getDht();
  delay(100);
 
}

void getDht() {
  temp = dht.readTemperature();
  hum = dht.readHumidity();
   Serial.print("Temperatura : " + String(temp));
   Serial.println("Humidade  : " + String(hum));
}