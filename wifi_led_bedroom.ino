#include "config.h"

AdafruitIO_Feed *counter = io.feed("bedroom light");
AdafruitIO_Feed *indicator = io.feed("bedroom indicator");
bool light_state = false;

void setup() {
  pinMode(D1, OUTPUT);
  pinMode(D4, OUTPUT);
  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // start MQTT connection to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  counter->onMessage(handleMessage);

  // wait for an MQTT connection
  // NOTE: when blending the HTTP and MQTT API, always use the mqttStatus
  // method to check on MQTT connection status specifically
  while(io.mqttStatus() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  digitalWrite(D4, HIGH);
  // Because Adafruit IO doesn't support the MQTT retain flag, we can use the
  // get() function to ask IO to resend the last value for this feed to just
  // this MQTT client after the io client is connected.
  counter->get();

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {
  io.run();
  
  if( light_state != digitalRead(D1)){
    light_state = digitalRead(D1);
    Serial.print("sending -> ");
    Serial.println(light_state);
    if (digitalRead(D1))
      indicator->save(1);
    else
      indicator->save(0);
  }
}

void handleMessage(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  Serial.println(data->value());
  int state = data->toInt();
  if(state == 1)
    digitalWrite(D1, HIGH);
  else
    digitalWrite(D1, LOW);
}
