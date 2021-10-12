#include <ESP8266WiFi.h>
#include <espnow.h>
#include <FastLED.h>
#define NUM_LEDS 144
//GPIO6, GPIO7, GPIO8, GPIO9, GPIO10, and GPIO11 are all unavailable
#define DATA_PIN  2


CRGB leds[NUM_LEDS];

typedef struct struct_message {//matching data structure for mic side
  int a;
  int b = 0;
} struct_message;

struct_message myData;

DEFINE_GRADIENT_PALETTE( red_blue ) { //blue -> red
  0,   0,  0,  255,  
  128,   255, 0, 0,
  255,   0, 0, 255
};

DEFINE_GRADIENT_PALETTE( green_purple ) { //green -> purple
  0,   0,  255,  0,   
  128,   255, 0, 255,
  255,   0, 255, 0
};

CRGBPalette16 currentPalette;
uint8_t paletteIndex = 0;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  //Serial.println(myData.b);
  if ( myData.b == 0) {
    currentPalette = RainbowColors_p;
    fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, currentPalette, 255, LINEARBLEND);
  } else if ( myData.b == 1) {
    fill_palette(leds, myData.a, paletteIndex, 255 / NUM_LEDS, currentPalette, 255, LINEARBLEND);
  } else if (myData.b == 2) {
    currentPalette = red_blue;
    fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, currentPalette, 255, LINEARBLEND);
  } else if (myData.b == 3) {
    fill_palette(leds, myData.a, paletteIndex, 255 / NUM_LEDS, currentPalette, 255, LINEARBLEND);
  } else if (myData.b == 4) {
    currentPalette = green_purple;
    fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, currentPalette, 255, LINEARBLEND);
  } else if (myData.b == 5) {
    fill_palette(leds, myData.a, paletteIndex, 255 / NUM_LEDS, currentPalette, 255, LINEARBLEND);
  } else if (myData.b == 6){
    paletteIndex = 0
  }
  FastLED.show();
}
void setup() {
  // put your setup code here, to run once:
  //Add LEDs
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  //Serial.begin(115200);
  FastLED.setBrightness(200);
  WiFi.mode(WIFI_STA);//sets wifi mode as a station
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    //Serial.println("Error initializing ESP-NOW");
    return;
  }else{
    //Serial.println("sucess");
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:
  EVERY_N_MILLISECONDS(10) {
    fadeToBlackBy(leds, NUM_LEDS, 48);
  }
  EVERY_N_MILLISECONDS(100){
    paletteIndex--;
  }
}
