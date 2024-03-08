#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#define ssid "ESP32_AP"
#define password "12345678"

#define BlueLED 2
#define GreenLLED 22
#define RedLED 23


IPAddress local_ip(192, 168, 1, 1);
IPAddress gateaway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

String LEDstatus = "Connect";

void handleOnConnect();
void handleLedblink();
void handleLedalternate();
void handleLedOff();
void handleNotFound();
String getHTML();

// put function declarations here:


void setup() {
  Serial.begin(115200);
  pinMode(BlueLED, OUTPUT);
  pinMode(GreenLLED, OUTPUT);
  pinMode(RedLED, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateaway, subnet);
  delay(1000);

  server.on("/", handleOnConnect);
  server.on("/blink", handleLedblink);
  server.on("/alternate", handleLedalternate);
  server.on("/ledoff", handleLedOff);
  server.onNotFound( handleNotFound);
  server.begin();
  digitalWrite(GreenLLED,HIGH);
  Serial.println("HTTP Server Started");


  // put your setup code here, to run once:
  
}

void loop() {
  server.handleClient();
  if (LEDstatus == "Blinking"){
    digitalWrite(BlueLED, !digitalRead(BlueLED));
    digitalWrite(RedLED, !digitalRead(RedLED));
    delay(500);
  }
  else if (LEDstatus == "Alternate"){
    digitalWrite(BlueLED, !digitalRead(BlueLED));
    digitalWrite(RedLED, !digitalRead(BlueLED));
    delay(500);
  }
  else if (LEDstatus == "OFF"){
    digitalWrite(BlueLED, LOW);
    digitalWrite(RedLED, LOW);
  }
  // put your main code here, to run repeatedly:
}

String getHTML() {
  String htmlcode = "";
  htmlcode += "<!DOCTYPE html> <html>\n";
  htmlcode += "<head>\n";
  htmlcode += "<meta charset=\"utf-8\">\n";
  htmlcode += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">\n";
  htmlcode += "<title>ESP32 Webserver</title>\n";
  htmlcode += "<style>\n";
  htmlcode += "body { font-family: Arial, sans-serif; text-align: center; }\n";
  htmlcode += "h1 { margin-top: 50px; }\n";
  htmlcode += ".button { background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; border-radius: 5px; }\n";
  htmlcode += ".button-off { background-color: #f44336; }\n";
  htmlcode += "</style>\n";
  htmlcode += "</head>\n";
  htmlcode += "<body>\n";
  htmlcode += "<h1>ESP32 WEBSERVER</h1>\n";
  htmlcode += "<h3>A Sample Code Using Access Point Mode</h3>\n";

  if (LEDstatus =="Connect") {
    htmlcode += "<p> LED Status:OFF</p>\n";
    htmlcode += "<a href=\"/blink\" class=\"button button-off\">Blink LEDs</a>\n";
    htmlcode += "<a href=\"/alternate\" class=\"button button-off\">Alternate LEDs</a>\n";
    htmlcode += "<a href=\"/ledoff\" class=\"button button-off\">Turn it OFF</a>\n";
  }
  else if (LEDstatus =="OFF") {
    htmlcode += "<p> LED Status:OFF</p>\n";
    htmlcode += "<a href=\"/blink\" class=\"button button-off\">Blink LEDs</a>\n";
    htmlcode += "<a href=\"/alternate\" class=\"button button-off\">Alternate LEDs</a>\n";
    htmlcode += "<a href=\"/ledoff\" class=\"button button-off\">Turn it OFF</a>\n";
    
  }
  else if (LEDstatus == "Blinking") {
    htmlcode += "<p> LED Status:Blinking</p>\n";
    htmlcode += "<a href=\"/blink\" class=\"button\">Blink LEDs</a>\n";
    htmlcode += "<a href=\"/alternate\" class=\"button button-off\">Alternate LEDs</a>\n";
    htmlcode += "<a href=\"/ledoff\" class=\"button button-off\">Turn it OFF</a>\n";
  }
  else if (LEDstatus =="Alternate") {
    htmlcode += "<p> LED Status:Alternate</p>\n";
    htmlcode += "<a href=\"/blink\" class=\"button button-off\">Blink LEDs</a>\n";
    htmlcode += "<a href=\"/alternate\" class=\"button\">Alternate LEDs</a>\n";
    htmlcode += "<a href=\"/ledoff\" class=\"button button-off\">Turn it OFF</a>\n";
  }
   
  htmlcode += "</body>\n";
  htmlcode += "</html>\n";

  return htmlcode;
}
 void handleOnConnect(){
  LEDstatus = "OFF";
  Serial.println("LED Status: OFF");
  server.send(200, "text/html", getHTML());
  }

  void handleLedblink(){
  LEDstatus = "Blinking";
  Serial.println("LED Status: Blinking");
  server.send(200, "text/html", getHTML());
  }

  void handleLedalternate(){
  LEDstatus = "Alternate";
  Serial.println("LED Status: Alternate");
  server.send(200, "text/html", getHTML());
  }
  void handleLedOff(){
  LEDstatus = "OFF";
  Serial.println("LED Status: OFF");
  server.send(200, "text/html", getHTML());
  }


  void handleNotFound(){
    server.send(404, "text/plain", "NotFound");
  }
// put function definitions here:
