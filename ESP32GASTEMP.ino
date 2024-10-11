#include "WiFi.h"
#include <PubSubClient.h>
#include "DHT.h"

// ANDERSON ARRUDA, JOSÉ EMILIO CATANHO DA SILVA JUNIOR IFSP CATANDUVA-SP
// ESP32 COM SENSOR DE TEMPERATURA E POTENCIÔMETRO SIMULANDO UM SENSOR DE GÁS
// SE A TEMPERATURA OU O SENSOR (SIMULADO PELO POTENCIÔMETRO) ESTIVER ACIMA
// DO NÍVEL 50, ACIONA O LED VERMELHO E ENVIA MENSAGEM DE ATENÇÃO

// Credenciais de WiFi
const char *ssid = "Wokwi-GUEST"; // Nome da rede WiFi
const char *password = ""; // Senha da rede WiFi (nesse caso, vazio)

// Credenciais de MQTT
const char *mqtt_broker = "ec2-3-83-102-68.compute-1.amazonaws.com"; // Endereço do broker MQTT
const char *mqtt_username = ""; // Nome de usuário do MQTT (vazio)
const char *mqtt_password = ""; // Senha do MQTT (vazio)
const int mqtt_port = 1883; // Porta do servidor MQTT

// Configuração do sensor DHT
#define DHTPIN 32 // Pino onde o sensor DHT22 está conectado
#define DHTTYPE DHT22 // Tipo do sensor DHT (DHT22)
DHT dht(DHTPIN, DHTTYPE); // Instanciação do sensor DHT
float temp, hum; // Variáveis para armazenar temperatura e umidade
int ledPin = 23; // Pino para o LED de alerta
float gas = 0; // Variável para armazenar a leitura simulada do sensor de gás

// Configuração do MQTT
WiFiClient espClient; // Cliente WiFi
PubSubClient client(espClient); // Cliente MQTT usando o cliente WiFi

// Tarefa para execução em dois núcleos
TaskHandle_t Task0;

// Função de configuração do WiFi
void wifiSetup() {
  WiFi.begin(ssid, password); // Conecta-se à rede WiFi
  while (WiFi.status() != WL_CONNECTED) { // Enquanto não estiver conectado
    delay(500); // Atraso de 500ms
    Serial.println("Connecting to WiFi.."); // Mostra que está tentando conectar
  }
  Serial.println("Connected to the Wi-Fi network"); // Mostra que está conectado

  // Conexão ao broker MQTT
  client.setServer(mqtt_broker, mqtt_port); // Define o servidor MQTT
  client.setCallback(callback); // Define a função de callback para mensagens recebidas
  checkConnection(); // Verifica e mantém a conexão
}

// Função para verificar a conexão MQTT
void checkConnection() {
  while (!client.connected()) { // Enquanto o cliente MQTT não estiver conectado
    String client_id = "esp32-client-"; // Cria um ID de cliente
    client_id += String(WiFi.macAddress()); // Adiciona o endereço MAC ao ID do cliente
    Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str()); // Mostra o ID do cliente
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) { // Tenta conectar ao broker MQTT
      Serial.println("Broker connected"); // Mostra que conectou com sucesso
    } else {
      Serial.print("failed with state "); // Mostra que falhou ao conectar
      Serial.print(client.state()); // Mostra o estado da conexão
      delay(2000); // Atraso de 2 segundos antes de tentar novamente
    }
  }
}

// Função de callback para lidar com mensagens recebidas no MQTT
void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: "); // Mostra o tópico da mensagem recebida
  Serial.println(topic);
  Serial.print("Message:"); // Mostra o conteúdo da mensagem
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]); // Exibe cada caractere da mensagem
  }
  Serial.println();
}

// Função de configuração inicial do ESP32
void setup() {
  Serial.begin(115200); // Inicia a comunicação serial
  wifiSetup(); // Configura a conexão WiFi
  dht.begin(); // Inicia o sensor DHT

  pinMode(ledPin, OUTPUT); // Define o pino do LED como saída

  // Cria uma tarefa que será executada na função mqttTask() com prioridade 1 no core 0
  xTaskCreatePinnedToCore(
    mqttTask,   /* Função da tarefa. */
    "Task0",    /* Nome da tarefa. */
    10000,      /* Tamanho da pilha da tarefa */
    NULL,       /* Parâmetro da tarefa */
    1,          /* Prioridade da tarefa */
    &Task0,     /* Handle da tarefa para rastrear a tarefa criada */
    1);         /* Atribui a tarefa ao core 0 */
  delay(500); // Pequeno atraso para garantir que a tarefa seja criada
}

// Função que representa a tarefa MQTT em segundo plano
void mqttTask(void * pvParameters) {
  while (1) { // Loop infinito da tarefa
    checkConnection(); // Verifica a conexão MQTT
    client.publish("esp32temperatura", String(temp).c_str()); // Publica a temperatura no tópico MQTT
    client.publish("esp32umidade", String(hum).c_str()); // Publica a umidade no tópico MQTT
    client.publish("esp32gas", String(gas).c_str()); // Publica o nível de gás no tópico MQTT
    client.loop(); // Mantém o loop do cliente MQTT
    delay(100); // Atraso de 100ms entre as execuções
  }
}

// Função principal (loop)
void loop() {
  gas = analogRead(33); // Lê o valor do pino 33 (potenciômetro simulando sensor de gás)
  gas = gas * 100 / 4033; // Normaliza o valor do sensor de gás para uma escala de 0 a 100

  if (temp > 50 || gas > 50) { // Se a temperatura ou o nível de gás estiver acima de 50
    digitalWrite(ledPin, HIGH); // Acende o LED de alerta
    Serial.println("PERIGO, DETECTADO TAXAS ALTAS DE GAS OU TEMPERATURA"); // Exibe mensagem de perigo
  } else {
    digitalWrite(ledPin, LOW); // Apaga o LED de alerta
    Serial.println("TAXAS DE GAS E TEMPERATURA EM NIVEIS ACEITAVEIS"); // Exibe mensagem de que está tudo ok
  }

  Serial.println(gas); // Mostra o valor do gás no monitor serial

  getDht(); // Chama a função para ler os dados do sensor DHT
  delay(100); // Atraso de 100ms
}

// Função para obter dados do sensor DHT
void getDht() {
  temp = dht.readTemperature(); // Lê a temperatura do sensor DHT
  hum = dht.readHumidity(); // Lê a umidade do sensor DHT
  Serial.print("Temperatura : " + String(temp)); // Exibe a temperatura
  Serial.println("Humidade : " + String(hum)); // Exibe a umidade
}
