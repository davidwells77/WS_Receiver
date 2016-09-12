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

void processMessage(char *buf, int buflen, int *temperature, int *humidity, int *pressure, int *heading) {

  int i = 0, last = 0, j = 0;

  for(i = 0; i <= buflen; i++) {
    if(buf[i] == ';' || i == buflen) {
      int k = 0;
      char *numero = (char *)malloc(((i - last) - 1) * sizeof(char));
      for(j = last + 2; j < i ; j++) {
        // printf("%c", prueba[j]);
        numero[k] = buf[j];
        k++;
      }
      numero[k] = '\0';
      switch (buf[last]) {
        case 'T':
          *temperature = atoi(numero);
          break;
        case 'H':
          *humidity = atoi(numero);
          break;
        case 'P':
          *pressure = atoi(numero);
          break;
        case 'D':
          *heading = atoi(numero);
          break;
      }
      free(numero);
      last = i + 1;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  static bool led = true;
  unsigned long currentMillis = millis();
  static unsigned long blinker = currentMillis;
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);
  static int temperature = 0, humidity = 0,  pressure = 0, heading = 0, RPM = 0;

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
    processMessage((char *)buf, (int)buflen, &temperature, &humidity, &pressure, &heading);
    char buffer[255];
    sprintf(buffer, "%d %d %d %d", temperature, humidity, pressure, heading);
    Serial.println(buffer);
  }
}
