#include <ESP8266WiFi.h>
#include <espnow.h>

//Parameters
const int micPin  = A0;
const int bottomLimit = 550;
const int topLimit = 575;

uint8_t broadcastAddress[] = {0x30, 0x83, 0x98, 0x82, 0xe8, 0x12}; //replace with the MAC addresses of the lamps (microcontrollers attached the lamps)

//place holder variables rn because I don't know what I will need
typedef struct struct_message {
  int a;
} struct_message;

struct_message myData;

//callback for when data is sent to see whether data was successful recieved, I will ultimately remove this in the end because serial outputs tend to slow down the program and here timing is very important
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  //Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  }
  else {
    Serial.println("Delivery fail");
  }
}

unsigned long prevMillis = 0;
unsigned long interval = 50;
unsigned long averageInterval = 25;
float micVal = 0;
const int samples = 5;
int readings[samples];
int readIndex = 0;
int total = 0;
int average = 0;

void setup() {
  //Init Serial USB
  Serial.begin(115200);
  //Init Microphone
  pinMode(micPin, INPUT);
  //Enable wifi mode
  WiFi.mode(WIFI_STA);
  for (int thisReading = 0; thisReading < samples; thisReading++) {
    readings[thisReading] = 0;
  }
  //check to see if esp-now was initalized correctly
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //establish the role of the master for esp_now
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  //Register a peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  if ((millis() - prevMillis) >= interval) {
    total = total - readings[readIndex];
    // read from the sensor:
    readings[readIndex] = analogRead(micPin);
    // add the reading to the total:
    total = total + readings[readIndex];
    // advance to the next position in the array:
    readIndex = readIndex + 1;

    // if we're at the end of the array...
    if (readIndex >= samples) {
      // ...wrap around to the beginning:
      readIndex = 0;
    }
    // calculate the average:
    average = total / samples;
    // send it to the computer as ASCII digits
    micVal = average;//do you need to pole the sensor even if the data isn't going to be used?
    //set mic data point to the current value of the mic from the timer sequence
    myData.a = abs (round (((micVal - bottomLimit) / (topLimit - bottomLimit)) * 144));
    if (myData.a > 144) {
      myData.a = 144;
    }
    //Serial.println(myData.a);
    //send the message with ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    prevMillis = millis();
    delay(0);
  }
}
