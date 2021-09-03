#include <ESP8266WiFi.h>
#include <espnow.h>

//Parameters
const int micPin  = A0;

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //replace with the MAC addresses of the lamps (microcontrollers attached the lamps)

//place holder variables rn because I don't know what I will need
typedef struct struct_message {
  int a;
} struct_message;

struct_message myData;

//callback for when data is sent to see whether data was successful recieved, I will ultimately remove this in the end because serial outputs tend to slow down the program and here timing is very important
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  }
  else {
    Serial.println("Delivery fail");
  }
}

int micVal  = 0;
unsigned long prevMillis = 0;
unsigned long interval = 10;

void setup() {
  //Init Serial USB
  Serial.begin(115200);
  //Init Microphone
  pinMode(micPin, INPUT);
  //Enable wifi mode
  WiFi.mode(WIFI_STA);

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
  unsigned long currentMillis = millis();
  micVal = analogRead(micPin);//do you need to pole the sensor even if the data isn't going to be used?
  if (currentMillis - prevMillis >= interval) {
    prevMillis = currentMillis; //reset the counter for the next cycle timer
    Serial.println(micVal);
    //set mic data point to the current value of the mic from the timer sequence
    myData.a = micVal;

    //send the message with ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  }
}
