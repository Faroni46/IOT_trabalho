# Iot_Trabalho_Sensores
Temperatura, Humidade, Gás
>Projeto de trabalho de conclusão da disciplina de Plataformas de Prototipação para IoT (E2PPT) IFSP CTD.
# Visão Geral
Este projeto consitutui de medidores de IoT de temperatura, gás e umidade que estão sendo usados nos dispositivos conectados para monitoramento remoto em tempo real. Através desta coleta de dados que está sendo realida neste monitoramento quando o dispositivo atingir um nivel acima de 50 sera emitido um alerta para tomar as devidas providencias para solucionar possíveis problemas. E estes dados que são originados nos sensores são publicados em um tópico via MQTT. 
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
create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
    mqttTask,   /* Task function. */
    "Task0",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task0,      /* Task handle to keep track of created task */
    1);          /* pin task to core 0 */
  delay(500);












