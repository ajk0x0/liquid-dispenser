#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
namespace wifi{
  const char* ssid = "ajk"; 
  const char* password = "ajkajkajk"; 

  String serverName = "http://127.0.0.1/user-access";

  void initialize(){
      WiFi.begin(ssid, password);
      while(WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
      }
      Serial.println("");
      Serial.println("Connected to WiFi");
  }

  String getDetails(String uid){
      if(WiFi.status()== WL_CONNECTED){
        WiFiClient client;
        HTTPClient http;
        String url = serverName + "?uid=" + uid;
        http.begin(client, serverPath.c_str());
        int httpResponseCode = http.GET();
        String payload =  http.getString();
        http.end();
        return payload;
      }
      else {
        Serial.println("WiFi Disconnected");
        return NULL;
      }
  }
}