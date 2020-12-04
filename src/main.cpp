// Copied from IRsendDemo.ino in the IRremoteESP8266 project
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = D5;  // ESP8266 GPIO pin to use.

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

uint64_t denonPowerOn = 0x2A4C028A0088;
uint64_t denonVolumeUp = 0x2A4C0280E86A;
uint64_t denonVolumeDown = 0x2A4C0288E862;
uint64_t denonMute = 0x2A4C0284E86E;
uint64_t denonCbl_Sat = 0x2A4C028CB43A;
uint64_t denonDVD_BluRay = 0x2A4C0280B436;
uint64_t denonBluRay = 0x2A4C0288B43E;
uint64_t denonGame = 0x2A4C028AB43C;
uint64_t denonMediaPlayer = 0x2A4C0286B430;

void setup() {
  irsend.begin();
#if ESP8266
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(115200, SERIAL_8N1);
#endif  // ESP8266
}

void loop() {
  //Serial.println("Power On");
  //irsend.sendDenon(0x2A4C028A0088, 48, 1);
  //delay(2000);
  Serial.println("Volume Up x 3");
  irsend.sendDenon(0x2A4C0280E86A, 48, 3);
  delay(1000);
  Serial.println("Volume Down x 3");
  irsend.sendDenon(0x2A4C0288E862, 48, 3);
  delay(1000);
}