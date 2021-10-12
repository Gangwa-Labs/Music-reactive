#include <ESP8266WiFi.h>
#include <espnow.h>

//Parameters
const int micPin  = A0;
const int buttonPin = 16;
const int bottomLimit = 0;
const int topLimit = 130;
const int NUM_LEDS = 144;

uint8_t broadcastAddress1[] = {0xc4, 0x5b, 0xbe, 0x63, 0x4f, 0xe9}; //replace with the MAC addresses of the lamps (microcontrollers attached the lamps)
uint8_t broadcastAddress2[] = {0xc4, 0x5b, 0xbe, 0x71, 0x5d, 0xaa}; //replace with the MAC addresses of the lamps (microcontrollers attached the lamps)
uint8_t broadcastAddress3[] = {0xc4, 0x5b, 0xbe, 0x70, 0x58, 0xd8}; //replace with the MAC addresses of the lamps (microcontrollers attached the lamps)
uint8_t broadcastAddress4[] = {0x30, 0x83, 0x98, 0x82, 0xe8, 0x12}; //replace with the MAC addresses of the lamps (microcontrollers attached the lamps)

int buttonState = 0;
int preState = 0;
//place holder variables rn because I don't know what I will need
typedef struct struct_message {
  int a;
  int b = 0;
} struct_message;

struct_message myData;

//callback for when data is sent to see whether data was successful recieved, I will ultimately remove this in the end because serial outputs tend to slow down the program and here timing is very important
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  char macStr[18];
  //Serial.print("Packet to:");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  //Serial.print(macStr);
  //Serial.print(" send status: ");
  //if (sendStatus == 0) {
    //Serial.println("Delivery success");
  //}
  //else {
    //Serial.println("Delivery fail");
  //}
}

unsigned long previousMillis = 0;
const long interval = 25;
const int sampleSize = 5;
int readings[sampleSize];
int readIndex = 0;
int total = 0;
int micVal = 0;
double mean = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  //Init Serial USB
  //Serial.begin(115200);
  //Init Microphone
  pinMode(micPin, INPUT);
  //Enable wifi mode
  WiFi.mode(WIFI_STA);

  //check to see if esp-now was initalized correctly
  if (esp_now_init() != 0) {
    //Serial.println("Error initializing ESP-NOW");
    return;
  }

  //establish the role of the master for esp_now
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  //Register a peer
  esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress3, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress4, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  if ((millis() - previousMillis) >= interval) {
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH & preState == 0) {
      myData.b++;
      preState = 1;
    } else if (buttonState == LOW) {
      preState = 0;
    }
    if (myData.b >= 7) {
      myData.b = 0;
    }
    micVal = analogRead(micPin);
    micVal -= 542;
    total = total - readings[readIndex];
    readings[readIndex] = pow(micVal, 2);
    total = total + readings[readIndex];
    readIndex += 1;
    if (readIndex >= sampleSize) {
      readIndex = 0;
    }
    mean = total / sampleSize;
    mean = sqrt(mean);
    myData.a = (mean / topLimit) * NUM_LEDS;
    if (myData.a > 144) {
      myData.a = 144;
    }
    //Serial.println(myData.a);
    esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
    previousMillis = millis();
    delay(0);
  }
}
