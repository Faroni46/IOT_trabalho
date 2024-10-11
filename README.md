# Iot_Trabalho_Sensores
Temperatura, Umidade, Gás
>Projeto de trabalho de conclusão da disciplina de Plataformas de Prototipação para IoT (E2PPT) IFSP CTD. Realizado pelos alunos: Anderson Arruda e José Emilio.
# Visão Geral
Este projeto consitutui de medidores de IoT de Temperatura, Gás e Umidade que estão sendo usados nos dispositivos conectados para monitoramento remoto em tempo real. Através desta coleta de dados que está sendo realizada neste monitoramento quando o dispositivo atingir um nivel acima de 50 sera emitido um alerta para tomar as devidas providencias para solucionar possíveis problemas. E estes dados que são originados nos sensores são publicados em um tópico via MQTT. 
# Quais equipamentos são utilizados
1 x ESP32 WROOM Devkit V1 <br>
1 x LED (red) indicação de conectado <br>
1 x Sensor de Umidade e Temperatura DHT22 <br>
1 x Potenciômetro <br>

![esquema montagem](https://github.com/user-attachments/assets/4e2f8697-deda-4cc3-9e72-c98532141ccc) <br>




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
Com o MQTT_IOT em conjunto com MySQL é possível monitorar a comunicação entre os componentes e também mostrar graficamente os dados para um melhor entendindo para analise como demonstrado a seguir: <br>


![leitura sensores](https://github.com/user-attachments/assets/bdb49683-46fc-4a63-bbd2-3363b08939ba) <br>
![graficos](https://github.com/user-attachments/assets/082e59d1-9c6f-4f65-b8de-80ccf1c4a22e) <br>
![tabela bd](https://github.com/user-attachments/assets/6d06127e-41a6-4948-8d5a-2b9b46274127) <br>

Agora o fluxo Node -RED implementado pode ser obtido neste [Gist](http://3.83.102.68:1880/#flow/80794394386acbb8) , cuja representação na interface da plataforma se parece com a captura da tela: 

![imagem fluxo nodered](https://github.com/user-attachments/assets/e2febd08-7b3a-41c7-b1e8-33a8a5ae1043) <br>


# Simulação do Circuito
Os circuitos estão simulados na plataforma [Wokwi](https://wokwi.com/) e cada um dos módulos pode ser acessado nos links abaixo:<br>
[Unidade de Monitoramento](http://3.83.102.68:1880/ui) <br>
[Unidade de Simulação](https://wokwi.com/projects/410688670007925761)<br>



    











