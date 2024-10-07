[Bibliotecas](https://wokwi.com/projects/410688670007925761)<br>
# Iot_Trabalho_Sensores
Temperatura, Umidade, Gás
>Projeto de trabalho de conclusão da disciplina de Plataformas de Prototipação para IoT (E2PPT) IFSP CTD.
# Visão Geral
Este projeto consitutui de medidores de IoT de Temperatura, Gás e Umidade que estão sendo usados nos dispositivos conectados para monitoramento remoto em tempo real. Através desta coleta de dados que está sendo realida neste monitoramento quando o dispositivo atingir um nivel acima de 50 sera emitido um alerta para tomar as devidas providencias para solucionar possíveis problemas. E estes dados que são originados nos sensores são publicados em um tópico via MQTT. 
# Quais equipamentos são utilizados
1 x ESP32 WROOM Devkit V1 <br>
1 x LED (red) indicação de conectado <br>
1 x Sensor de Umidade e Temperatura DHT22 <br>
1 x Potentiometer <br>

![image](https://github.com/user-attachments/assets/a0c0f7a6-b3bf-407c-b9fe-e42fbd92d29e) <br>
# Topicos do MQTT
MQTT<br>
WiFiClient espClient;<br>
PubSubClient client(espClient);<br>
Network Task<br>
void mqttTask( void * pvParameters ) {
  while (1) {
    // Serial.print("Network Task running on core ");<br>
    // Serial.println(xPortGetCoreID());<br>
    checkConnection();<br>
    client.publish("esp32/temperatura", String(temp).c_str());<br>
    client.publish("esp32/humidade", String(hum).c_str());<br>
    client.publish("esp32/gas", String(gas).c_str());<br>
    client.loop();<br>
    delay(100);<br>
 # Monitoramento e Logging
 O referido projeto consta com uma [infraestrutura em nuvem](https://www.redhat.com/pt-br/topics/cloud-computing/what-is-cloud-infrastructure) que possibilita o monitoramento e controle remoto dos contêineres mensurados sendo via internet através de um navegador em um PC ou smartphone. Os serviços disponíveis através de uma [instância EC2](https://aws.amazon.com/pt/ec2/) do [tipo t2.micro](https://aws.amazon.com/pt/ec2/instance-types/t2/) onde estão virtualizados os serviços através de [containers Docker](https://docs.docker.com/). Os serviços instalados são:<br>
 Servidor [MySQL](https://www.mysql.com/) <br>
 Servidor [Eclipse Mosquitto](https://mosquitto.org/download/)<br>
 Servidor [Node-RED v3.0.2](https://hub.docker.com/r/nodered/node-red/) <br>
 Servidor [Apache HTTP Server Project](https://httpd.apache.org/download.cgi)<br>
Com o Node-RED é possível monitorar a comunicação entre os componentes IoT que compõem o projeto, registrar as leituras em bancos de dados MySQL e fornecer os dados como mostra a seguir: ![image](https://i.sstatic.net/ScBty.png) <br>
![image](![download](https://github.com/user-attachments/assets/ee72ea8f-acb7-4aba-89a7-cf3c1e36a33b)<br>

Agora o fluxo Node -RED implementado pode ser obtido neste [Gist](http://54.172.232.108:1880/#flow/80794394386acbb8) , cuja representação na interface da plataforma se parece com a captura da tela 
http://54.172.232.108:1880/#flow/80794394386acbb8
# Simulação do Circuito
Os circuitos estão simulados na plataforma [Wokwi](https://wokwi.com/) e cada um dos módulos pode ser acessado nos links abaixo:<br>
[Unidade de Monitoramento](http://54.172.232.108:1880/ui/) <br>
[Unidade de Simulação](https://wokwi.com/projects/410688670007925761)<br>



    











