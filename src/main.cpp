// Copied from IRsendDemo.ino in the IRremoteESP8266 project
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "secrets.h"

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

uint64_t projectorOn = 0x4CB340BF;
uint64_t projectorOff = 0x4CB3748B;

ESP8266WebServer server(80);

void setupServerRouting();

void setup() {
  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);

  Serial.println("\n\n\n\n");
  Serial.printf("Connecting to wifi (%s). MAC address is %s ...", SSID, WiFi.macAddress().c_str());
  WiFi.begin(SSID, wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("connected! IP address is %s\n", WiFi.localIP().toString().c_str());

  setupServerRouting();
  server.begin();
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}


void handle_receiver(String requestURI) {
  Serial.printf("Handling AV receiver command %s\n", requestURI.c_str());
  if (requestURI.endsWith("power/on")) {
    irsend.sendDenon(denonPowerOn, 48, 2);
  } else if (requestURI.endsWith("power/off")) {
    irsend.sendDenon(denonPowerOn, 48, 2);
  } else if (requestURI.endsWith("volume/up")) {
    irsend.sendDenon(denonVolumeUp, 48, 2);
  } else if (requestURI.endsWith("volume/down")) {
    irsend.sendDenon(denonVolumeDown, 48, 2);
  } else if (requestURI.endsWith("volume/mute")) {
    irsend.sendDenon(denonMute, 48, 2);
  } else if (requestURI.endsWith("source/cblsat")) {
    irsend.sendDenon(denonCbl_Sat, 48, 2);
  } else if (requestURI.endsWith("source/dvd_bluray")) {
    irsend.sendDenon(denonDVD_BluRay, 48, 2);
  } else if (requestURI.endsWith("source/bluray")) {
    irsend.sendDenon(denonBluRay, 48, 2);
  } else if (requestURI.endsWith("source/game")) {
    irsend.sendDenon(denonGame, 48, 2);
  } else if (requestURI.endsWith("source/mediaplayer")) {
    irsend.sendDenon(denonMediaPlayer, 48, 2);
  }
  server.send(200, "text/plain", "");
}

// https://stackoverflow.com/questions/35816454/setting-a-wildcard-in-esp8266webserver
class AVReceiverHandler : public RequestHandler {

  bool canHandle(HTTPMethod method, String uri) {
    return uri.startsWith("/receiver");
  }

  bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestURI) {    
    handle_receiver(requestURI);
    return true;
  }

} avReceiverHandler;


void handle_projector(String requestURI) {
  Serial.printf("Handling projector command %s\n", requestURI.c_str());
if (requestURI.endsWith("power/on")) {
    irsend.sendNEC(projectorOn);
  } else if (requestURI.endsWith("power/off")) {
    irsend.sendNEC(projectorOff);
    delay(500);
    irsend.sendNEC(projectorOff);
  }
  server.send(200, "text/plain", "");
}

class ProjectorHandler : public RequestHandler {

  bool canHandle(HTTPMethod method, String uri) {
    return uri.startsWith("/projector");
  }

  bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestURI) {    
    handle_projector(requestURI);
    return true;
  }

} projectorHandler;

void setupServerRouting() {
  server.on("/", HTTP_GET, []() {
        server.send(200, F("text/html"),
            F("Welcome to the REST Web Server"));
    });
  server.addHandler(&avReceiverHandler);
  server.addHandler(&projectorHandler);
  server.onNotFound(handle_NotFound);
}

void loop() {
  server.handleClient();
}