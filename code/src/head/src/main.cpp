#include <Arduino.h>
#include <esp_now.h>
#include <FastLED.h>
#include <WiFi.h>

#define STATUS_LED_PIN 8

typedef struct struct_message_payload {
  uint8_t brightness;
  bool holoprojector;
  bool psi;
} struct_message_payload;

volatile long last_recv_millis = 0;

CRGB status_led[1];

void ledGreen();
void ledRed();

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  struct_message_payload message_payload;
  memcpy(&message_payload, incomingData, sizeof(message_payload));
  Serial.print("Brightness: ");
  Serial.print(message_payload.brightness);
  Serial.print(" PSI: ");
  Serial.print(message_payload.psi);
  Serial.print(" Holo: ");
  Serial.println(message_payload.holoprojector);
  last_recv_millis = millis();
  ledGreen();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  pinMode(STATUS_LED_PIN, OUTPUT);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
  }

  FastLED.addLeds<WS2812B, STATUS_LED_PIN, GRB>(status_led, 1);
  FastLED.setBrightness(255);
  ledRed();

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  if((millis() - last_recv_millis) > 1000){
    ledRed();
  }
}

void ledGreen() {
  status_led[0] = CRGB::Green;
  FastLED.show();
}

void ledRed() {
  status_led[0] = CRGB::Red;
  FastLED.show();
}