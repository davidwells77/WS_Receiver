#include <RH_ASK.h>

#define RXPIN 7
#define TXPIN 6

RH_ASK ask(2000, RXPIN, TXPIN);

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  if(!ask.init()) {
    Serial.println("No ASK transmitter detected");
    while(1);
  }
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  static bool led = true;
  unsigned long currentMillis = millis();
  static unsigned long blinker = currentMillis;
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if(currentMillis - blinker >= 1000) {
    if(led) {
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    led = !led;
    blinker = currentMillis;
  }
  if (ask.recv(buf, &buflen)) {
    //int i;
    // Message with a good checksum received, dump it.
    ask.printBuffer("Got:", buf, buflen);
  }
}

