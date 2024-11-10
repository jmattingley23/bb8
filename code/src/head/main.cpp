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
CRGB psi_leds[4];
CRGB holo_leds[8];
CRGB logic_leds[56];

uint8_t brightness = 255;

void setStatusLed(CRGB color);
void setLogicLeds();

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
    setStatusLed(CRGB::Green);
    message_payload.brightness = map(message_payload.brightness, 0, 255, 35, 255); //brightness values under 35 look weird and flickery
    if(brightness != message_payload.brightness) {
        brightness = message_payload.brightness;
        setLogicLeds();
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    pinMode(STATUS_LED_PIN, OUTPUT);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
    }

    FastLED.addLeds<WS2812B, STATUS_LED_PIN, GRB>(status_led, 1);
    FastLED.addLeds<WS2812B, 16, GRB>(logic_leds, 56).setCorrection(TypicalSMD5050).setTemperature(Tungsten40W);
    FastLED.addLeds<SK6812, 17, GRB>(psi_leds, 4).setRgbw(RgbwDefault()).setCorrection(TypicalSMD5050).setTemperature(CarbonArc);
    FastLED.addLeds<SK6812, 18, GRB>(holo_leds, 8).setRgbw(Rgbw(
        kRGBWDefaultColorTemp,
        kRGBWMaxBrightness,
        W3
    ));
    FastLED.setBrightness(255);
    setStatusLed(CRGB::Red);
    setLogicLeds();

    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
    if((millis() - last_recv_millis) > 1000){
        setStatusLed(CRGB::Red);
    }
}

void setStatusLed(CRGB color) {
    status_led[0] = color;
    FastLED.show();
}

void setLogicLeds() {
    // front logic top
    for(int i = 0; i < 12; i++){
        // logic_leds[i] = CRGB::White;
        logic_leds[i] = CRGB(255, 255, 255);
    }

    // // front logic bottom
    // for(int i = 22; i < 34; i++){
    //     logic_leds[i] = CRGB::White;
    // }

    // rear logic
    for(int i = 46; i < 56; i++){
        logic_leds[i] = CRGB::Blue;
    }
    nscale8(&logic_leds[0], 56, dim8_raw(brightness)); //scale brightness with gamma correction and dithering
    FastLED.show();
}