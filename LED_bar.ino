#include <WiFi.h>
#include <esp_now.h>
#include <FastLED.h>
#define NUM_LEDS 144
//GPIO6, GPIO7, GPIO8, GPIO9, GPIO10, and GPIO11 are all unavailable
#define DATA_PIN  10


CRGB leds[NUM_LEDS];

unsigned long previousMillis = 0;
const long interval = 10;

typedef struct struct_message {//matching data structure for mic side
  byte a;
  byte b = 0;
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

DEFINE_GRADIENT_PALETTE( uscYaFuck ) { //gold -> cardinal
  0,   255,  204,  0, //gold
  128,   255, 0, 0, //cardinal
  255,   255,  204,  0 // gold
};

bool musicReactive = HIGH;
int animationSpeed = 250;
CRGBPalette16 currentPalette;
uint8_t paletteIndex = 0;


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  //Serial.println(myData.a);


}
void setup() {
  // put your setup code here, to run once:
  //Add LEDs
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(115200);
  FastLED.setBrightness(175);
  FastLED.clear();
  FastLED.show();
  WiFi.mode(WIFI_STA);//sets wifi mode as a station
  currentPalette = RainbowColors_p;

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } 
    Serial.println("success");
    esp_now_register_recv_cb(OnDataRecv);
  
}
void loop() {
  // put your main code here, to run repeatedly:
  fill_palette(leds, myData.a, paletteIndex, 255 / NUM_LEDS, currentPalette, 175, LINEARBLEND);
  EVERY_N_MILLISECONDS(10) {
    fadeToBlackBy(leds, NUM_LEDS, 48);
  }
  EVERY_N_MILLISECONDS(animationSpeed) {
    paletteIndex++;
  }
  FastLED.show();
  delay(0);
}
