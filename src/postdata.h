/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

*/

#include <WiFiClient.h>
#include <HTTPClient.h>


// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://localhost/Sensor/post-esp-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "your api key";
String sensorName = "DHT11";
String sensorLocation = "SERVER_ROOM";
float readTemperature = 0;
uint8_t readHumidity = 0;
uint8_t readPressure = 0;


#define SEALEVELPRESSURE_HPA (1013.25)


void postSensor(float sTemp, float sHum) {
    readTemperature = sTemp;
    readHumidity = sHum;
    Serial.println("-------------------------");
    //Serial.println(sTemp);
    //Serial.println(sHum);
    if(sTemp > 25){
      enviaEmail(sHum,sTemp, 0);
    }
    if(sHum < 35 || sHum > 65){
      enviaEmail(sHum, sTemp, 0);
    }
 //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient *client = new WiFiClient;
    //client->setInsecure(); //don't use SSL certificate
    HTTPClient https;
    
    // Your Domain name with URL path or IP address with path
    https.begin(*client, serverName);
    
    // Specify content-type header
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                          + "&location=" + sensorLocation + "&value1=" + String(readTemperature)
                          + "&value2=" + String(readHumidity) + "&value3=" + String(readPressure) + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = https.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //https.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = https.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //https.addHeader("Content-Type", "application/json");
    //int httpResponseCode = https.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      if (httpResponseCode > 400){
              enviaEmail(sHum,sTemp, httpResponseCode);
      }
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    https.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }

}