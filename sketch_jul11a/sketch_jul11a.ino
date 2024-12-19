#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "SGP200W-59A8-bg";
const char* password = "53ccNz9M";

const char* serverName = "http://192.168.1.110:8000/decrement_points";

const int buttonPin = 32;
bool lastButtonState = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  
  // WiFiに接続
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  bool buttonState = digitalRead(buttonPin);

  // ボタンが押されたかどうかをチェック
  if (buttonState == LOW && lastButtonState == HIGH) {
    Serial.println("Button pressed");
    sendRequest();
    delay(100); // デバウンス処理
  }

  lastButtonState = buttonState;
}

void sendRequest() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    
    String httpRequestData = "{\"value\":1}";
    int httpResponseCode = http.POST(httpRequestData);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
}