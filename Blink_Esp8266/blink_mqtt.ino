#include <ESP8266WiFi.h> // Adciona a Biblioteca que permite que o ESP Conecte a o WIFI
#include <PubSubClient.h> // Adciona a Biblioteca que Permite que o ESP conecte a o MQTT

// WiFi
const char *ssid = "Nome do Wifi"; // Cria a variavel ssid onde sera armarzenado o nome do WIFI que o ESP se conectara
const char *password = "Senha do Wifi";  // Cria a variavel password onde sera armarzenado a sebga do WIFI que o ESP se conectara

// MQTT Broker
const char *mqtt_broker = "Url do Broker MQTT"; // Cria a variavel mqtt_broker onde fica armazenado o URL do Broker
const char *topic = "Topico do Usuario"; // Cria a variavel topic onde fica armazendo o primeiro Topico a qual o ESP se inscrevera no Broker
const char *mqtt_username = "Nome de Usuario/Login"; // Cria a variavel mqtt_username onde fica armazenado o login que sera usado para conectar no Broker
const char *mqtt_password = "Senha do Usuario"; // Cria a variavel mqtt_password onde fica armazenado a senha que sera usado para conectar no Broker
const int mqtt_port = 1883; // Criar a variavel mqtt_port onde ficara a armazenado a porta onde o Broker estar localizada

bool estadoled = 0; // variavel de controle do estado do led

int LED = 16; // declara a pino de saida como o D0 que e tem o GPO 16
String msg; // Criar a variavel do tipo String msg

WiFiClient espClient; // Criar a variavel que sera usada para conectar no WIFI
PubSubClient client(espClient); // Criar a variavel que sera usada para conectar no Broker

void setup() {
  pinMode(LED, OUTPUT); // Define o led como um output

  Serial.begin(115200); // Define que o monitor serial sera iniciado na Configuração 115200
  
  WiFi.begin(ssid, password); // Inicia a conexão com o WIFI 
  while (WiFi.status() != WL_CONNECTED) { // Aguarda ate o Wifi estar conectado 
      delay(500); // Espera 500 milisegundo
      Serial.println("Iniciado conexão..."); // Mostrar no monitor serial o texto Iniciado conexão.
  }
  Serial.println("WIFI Conectado"); // Quando o WIFI conectar mostrar no monitor serial o texto WIFI Conectado

  client.setServer(mqtt_broker, mqtt_port); // Inicia a conexão com o Broker
  client.setCallback(callback); // Verifica a resposta 
  while (!client.connected()) { // Aguarda ate o Broker estar conectado  
      String client_id = "esp8266-client-dw"; // Define o ID que o ESP usara no Broker
      client_id += String(WiFi.macAddress()); // Criar um ID usando o MAC address para que nao aja conflito de ids
      Serial.printf("Conectando a Broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) { // coloca o mqtt_username e o mqtt_password para que seja feito o login no Broker
           Serial.println("Broker Conectado"); // Mostrar no monitor serial o texto Broker Conectado
      } else { // Caso nao consiga se conectar a o Broker
          Serial.print("Falha na conexão"); // Mostrar no monitor serial o texto Falha na conexão
          Serial.print(client.state()); // Mostrar no monitor serial o Status da conexão
          delay(2000); // Aguarda dois segundos para uma nova conexão
      }
  }
  
  client.publish(topic, "Ola Aqui e o ESP"); // Publica no topico defino A mesagem Ola Null/ESP
  client.subscribe(topic); // Se inscreve no Topico para receber futuras mensagem de resposta
}

void callback(char *topic, byte *payload, unsigned int length) { // Caso tenha uma mensagem do broker 
  Serial.print("Mensagem publica no Topico: "); // Mostrar no monitor serial o texto Mensagem publica no Topico:
  Serial.println(topic); // Mostrar no monitor serial o Topico
  Serial.print("Mesagem:"); // Mostrar no monitor serial o texto Mensagem:

  String msg; // Reseta a messagem para que nao haja erro
  
  for (int i = 0; i < length; i++) { // Cria um For para receber a messagem
      Serial.print((char) payload[i]); // Mostra no monitor seril cada letra da mesagem
      char c = ((char) payload[i]); // Amarzena as letras na variavel c
      msg += c; // Armazena as letras para criar a mesagem
  }
  
  if(msg == "Led"){ // Verifica se a messagem e "Led"
    estadoled = !estadoled; // troca o estado do LED 
    digitalWrite(LED, estadoled); // Liga ou Desliga o led
  }
  
  Serial.println(); // Pula uma linha
}

void loop() {
  client.loop(); //Inicia o loop de conexão  
}
