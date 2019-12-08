#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti mWifi;
HTTPClient http;

const int empty_coffee_button = 16; // D0
const int enough_coffee_button = 5; // D1
const int brewing_coffee_button = 4; // D2
const int ready_coffee_button = 2; // D4

const int send_led = 14; // D5
const int connection_led = 12; // D6

int empty_coffee = 0;
int enough_coffee = 0;
int brewing_coffee = 0;
int ready_coffee = 0;

const char* coffeePostUrl = "***";

const char* fingerprint = "2C BC 06 10 0A E0 6E B0 9E 60 E5 96 BA 72 C5 63 93 23 54 B3";

void setup() {
  Serial.begin(9600);
  setupWiFi();

  pinMode(empty_coffee_button, INPUT);
  pinMode(enough_coffee_button, INPUT);
  pinMode(brewing_coffee_button, INPUT);
  pinMode(ready_coffee_button, INPUT);

  pinMode(connection_led, OUTPUT);
  pinMode(send_led, OUTPUT);
}

void loop() {
  empty_coffee = digitalRead(empty_coffee_button);
  enough_coffee = digitalRead(enough_coffee_button);
  brewing_coffee = digitalRead(brewing_coffee_button);
  ready_coffee = digitalRead(ready_coffee_button);

  if (empty_coffee == HIGH) {
    Serial.println("Empty");
    coffeeSend("empty");
  } else if (enough_coffee == HIGH) {
    Serial.println("Enough");
    coffeeSend("enough");
  } else if (brewing_coffee == HIGH) {
    Serial.println("Brewing");
    coffeeSend("brewing");
  } else if (ready_coffee == HIGH) {
    Serial.println("Ready");
    coffeeSend("ready");
  }
  delay(500);
}

void coffeeSend(String statusCode) {
  digitalWrite(send_led, HIGH);
  String postData = "status=" + statusCode;
  
  http.begin(coffeePostUrl);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
  http.end();
  digitalWrite(send_led, LOW);
}

void setupWiFi() {
  mWifi.addAP("VODAFONE_D905", "***");
  mWifi.addAP("AndroidAPdev", "***");
  mWifi.addAP("Pisano-2G", "***");
  mWifi.addAP("Pisano DEV-2G", "***");

  Serial.println("Connecting ...");
  
  // Wait for the Wi-Fi to connect
  while (mWifi.run() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  
  // When it connected
  Serial.println('\n');
  Serial.println("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.println("IP address:\t");
  Serial.println(WiFi.localIP());
  Serial.println('\n');
  digitalWrite(connection_led, HIGH);
}
