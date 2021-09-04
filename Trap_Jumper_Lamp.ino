#include <ESP8266WiFi.h>
#include <espnow.h>
#include <FastLED.h>
#define NUM_LEDS 60
//GPIO6, GPIO7, GPIO8, GPIO9, GPIO10, and GPIO11 are all unavailable
#define DATA_PIN  2

CRGB leds[NUM_LEDS];

typedef struct struct_message {//matching data structure for mic side
  int a;
} struct_message;

struct_message myData;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.println("Music Val: ");
  Serial.println(myData.a);
}

void setup() {
  // put your setup code here, to run once:
  //Add LEDs
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  //fill Strip with rainbow
  fill_rainbow( leds, NUM_LEDS, 0, 255 / NUM_LEDS );
  FastLED.show();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);//sets wifi mode as a station
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:

}
