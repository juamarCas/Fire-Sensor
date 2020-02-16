#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "DHT.h"

#define dhtpin 2
#define DHTTYPE DHT22

#define smokePin A0
#define readBattPin A1

#define maxTemp 36
#define minHum 30
#define maxSmk 125
#define minBatt 291

#define batteryLevelPin 5
#define sentConfirmationPin 4

DHT dht(dhtpin, DHTTYPE);

RF24 radio(7, 8);                   // nRF24L01(+) radio attached using Getting Started board

RF24Network network(radio);          // Network uses that radio

const uint16_t this_node = 012;        // Address of our node in Octal format
const uint16_t other_node = 00;       // Address of the other node in Octal format

const unsigned long interval = 1000; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already


struct payload_t {                  // Structure of our payload
  float temperature;
  float smoke;
  float humidity;
  float bat;
  bool state;
  char text[32];
};

void setup(void)
{
  Serial.begin(9600);
  Serial.println("RF24Network/examples/helloworld_tx/");
  pinMode(batteryLevelPin, OUTPUT);
  pinMode(sentConfirmationPin, OUTPUT);
  SPI.begin();
  dht.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);


  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  float smk = analogRead(smokePin);
  float battery = analogRead(readBattPin);
  bool states = false;
  network.update();                          // Check the network regularly

  if((smk >= maxSmk) || (temp >= maxTemp && hum < minHum) || (temp >= (maxTemp + 4))){
    states = true;
  }else{
    states = false; 
  }

  if(battery <= minBatt){
    digitalWrite(batteryLevelPin, HIGH); 
  }else{
    digitalWrite(batteryLevelPin, LOW);
  }
  

  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - last_sent >= interval  )
  {
    last_sent = now;
    payload_t payload = { temp, hum, smk, battery, states, "1012"};
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header, &payload, sizeof(payload));
    if (ok) {
      digitalWrite(sentConfirmationPin, HIGH);
    }
    else {
      digitalWrite(sentConfirmationPin, LOW);
    }
  }
}
