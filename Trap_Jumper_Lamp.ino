#include <ESP8266WiFi.h>
#include <espnow.h>
#include <FastLED.h>
#define NUM_LEDS 144
//GPIO6, GPIO7, GPIO8, GPIO9, GPIO10, and GPIO11 are all unavailable
#define DATA_PIN  2


CRGB leds[NUM_LEDS];

typedef struct struct_message {//matching data structure for mic side
  int a;
} struct_message;

struct_message myData;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println(myData.a);
  fill_rainbow( leds, myData.a, 0, 255/NUM_LEDS);
  FastLED.show();
  EVERY_N_MILLISECONDS(10){
    fadeToBlackBy(leds, NUM_LEDS, 64);
    FastLED.show();
    delay(0);
  }
}

void setup() {
  // put your setup code here, to run once:
  //Add LEDs
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  Serial.begin(115200);
  FastLED.setBrightness(65);
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
