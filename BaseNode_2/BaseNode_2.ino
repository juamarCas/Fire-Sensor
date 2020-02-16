#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <ArduinoJson.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>


// el espacio necesitado en el nodo mcu para el json es 148 mínimo
//para AVR es 100
RF24 radio(2, 15);               // nRF24L01(+) radio attached using Getting Started board

RF24Network network(radio);      // Network uses that radio
//agregar más nodos en caso de tener más de la siguiente manera:
// const uint16_t other_node(num) = xx; // xx es un número en forma octal diferente a los anteriores
const uint16_t this_node = 00;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t other_node1 = 01;
const uint16_t other_node2 = 02;// Address of the other node in Octal format
const uint16_t other_node3 = 03;
const uint16_t other_node4 = 04;
const uint16_t other_node5 = 05;

struct payload_t {                  // Structure of our payload
  float temperature;
  float smoke;
  float humidity;
  float batteryLevel;
  bool state; // falso = normal, verdadero = alerta
  char text[32];
};

int interval = 10000; //10 segundos por defecto

void setup(void)
{
  Serial.begin(9600);
  Serial.println("RF24Network/examples/helloworld_rx/");

  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  network.begin(/*channel*/ 90, /*node address*/ this_node);

  //Serial connection
  WiFi.begin("", "");   //WiFi connection

  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion

    delay(500);
    Serial.println("Waiting for connection");

  }
}

void loop(void) {

  // Check the network regularly

  network.update();
  while ( network.available() ) {     // Is there anything ready for us?

    if (WiFi.status() == WL_CONNECTED) {
      const size_t capacity = JSON_OBJECT_SIZE(6);
      DynamicJsonBuffer jsonBuffer(capacity);
      JsonObject& root = jsonBuffer.createObject();

      payload_t payload;
      RF24NetworkHeader header;
      
      /* RF24NetworkHeader header4(other_node4);
        RF24NetworkHeader header5(other_node5);
        RF24NetworkHeader header6(other_node6);
        RF24NetworkHeader header7(other_node7);*/
      network.read(header, &payload, sizeof(payload));
      if (header.from_node == 01) {
        root["id"] = payload.text;
        root["temperature"] = payload.temperature;
        root["humidity"] = payload.humidity;
        root["smoke"] = payload.smoke;
        root["battery"] = payload.batteryLevel;
        root["state"] = payload.state;
      }
      if (header.from_node == 02) {      
        root["id"] = payload.text;
        root["temperature"] = payload.temperature;
        root["humidity"] = payload.humidity;
        root["smoke"] = payload.smoke;
        root["battery"] = payload.batteryLevel;
        root["state"] = payload.state;
      }
      if (header.from_node == 03) {
       
        root["id"] = payload.text;
        root["temperature"] = payload.temperature;
        root["humidity"] = payload.humidity;
        root["smoke"] = payload.smoke;
        root["battery"] = payload.batteryLevel;
        root["state"] = payload.state;
      }
      
      if (header.from_node == 04) {
       
        root["id"] = payload.text;
        root["temperature"] = payload.temperature;
        root["humidity"] = payload.humidity;
        root["smoke"] = payload.smoke;
        root["battery"] = payload.batteryLevel;
        root["state"] = payload.state;
      }
      
      if (header.from_node == 05) {
       
        root["id"] = payload.text;
        root["temperature"] = payload.temperature;
        root["humidity"] = payload.humidity;
        root["smoke"] = payload.smoke;
        root["battery"] = payload.batteryLevel;
        root["state"] = payload.state;
      }

      if (header.from_node == 012) {
       
        root["id"] = payload.text;
        root["temperature"] = payload.temperature;
        root["humidity"] = payload.humidity;
        root["smoke"] = payload.smoke;
        root["battery"] = payload.batteryLevel;
        root["state"] = payload.state;
      }
      

      if(payload.state){
        interval = 1000; //3 segundos
      }
    
      char JSONmessageBuffer[500];
      root.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
      Serial.println(JSONmessageBuffer);

      HTTPClient http;    //Declare object of class HTTPClient
      http.begin("http://ddb461bb.ngrok.io/api/status/post/");      //Specify request destination
      http.addHeader("Content-Type", "application/json");  //Specify content-type header
      int httpCode = http.POST(JSONmessageBuffer);   //Send the request
      String payloads = http.getString();                                        //Get the response payload
      Serial.println(httpCode);   //Print HTTP return code
      Serial.println(payloads);    //Print request response payload
      http.end();  //Close connection
      Serial.println();



      for(int i = 0; i < interval; i++){
        delay(1);
      }
    } else {
      Serial.println("Error in WiFi connection");
    }
  }
}
