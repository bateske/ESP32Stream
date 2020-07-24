/**
   StreamHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#include <WiFi.h>
#include <WiFiMulti.h>

#include <HTTPClient.h>

#include "JsonStreamingParser.h"
#include "JsonListener.h"
#include "ExampleParser.h"


#define TFT_DC     2       // register select (stands for Data Control perhaps!)
#define TFT_RST   4         // Display reset pin, you can also connect this to the ESP32 reset
// in which case, set this #define pin to -1!
#define TFT_CS   5       // Display enable (Chip select), if not enabled will not talk on SPI bus

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

JsonStreamingParser parser;
ExampleListener listener;

WiFiMulti WiFiMulti;


  //HTTPClient http; //must be declared after WiFiClient for correct destruction order, because used by http.begin(client,...)


void setup() {

  Serial.begin(115200);

  tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab

  tft.fillScreen(ST77XX_BLACK);

  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(true);

  parser.setListener(&listener);
  Serial.println();
  Serial.println();
  Serial.println();
  /*

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    tft.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  */
//delay(1000);
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("biscuits", "biscuits");
    Serial.println("[WIFI] CONNECTING ...");
    tft.println("[WIFI] CONNECTING ...");


    
}


String firstPostUrl;

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    requestFirst();

    requestSecond();



  }

  delay(60000);
}


void requestFirst() {

  //WiFiClient client;
  HTTPClient http; //must be declared after WiFiClient for correct destruction order, because used by http.begin(client,...)

  // ======== First GET ======
  // Getting the list of posts
  
  // =========================
  Serial.print("[MEM] heap: ");
  Serial.println(ESP.getFreeHeap());
  Serial.print("[HTTP] begin: ");
  String firstUrl = "https://community.arduboy.com/posts.json";
  Serial.println(firstUrl);
  http.begin(firstUrl);

  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);

  tft.println("[HTTP] GET");
  tft.println(firstUrl);

  Serial.print("[HTTP] FIRST GET...\n");
  int httpCode = http.GET();



  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {

      int len = http.getSize();
      uint8_t buff[128] = { 0 };

#if 0
      Serial.println(http.getString()); // with API
#else

      // or "by hand"

      WiFiClient * stream = http.getStreamPtr(); // get tcp stream


      if (http.connected() && (len > 0 || len == -1)) {  // read all data from server

        int c = stream->readBytes(buff, std::min((size_t)len, sizeof(buff))); // read up to 128 byte
        Serial.printf("readBytes: %d\n", c);
        if (!c) {
          Serial.println("read timeout");
        }

        Serial.write(buff, c); // write it to Serial
        if (len > 0) {
          len -= c; //process the next stream count
        }

        for (int i = 0; i < sizeof(buff); i++) {
          parser.parse(buff[i]); // parse JSON all 128 bytes of the buffer
        }

      }

#endif


      // manipulating the URL
      // ===============================================
      // =====================================
      Serial.print("PostID: ");
      String firstPostId = listener.getPostId();
      Serial.println( firstPostId );
      firstPostUrl = "https://community.arduboy.com/posts/" + firstPostId + ".json";
      //firstPostUrl = "http://www.arduboy.com/a/test.json";
      Serial.println(firstPostUrl);

      parser.reset();
      listener.reset();
      Serial.println();
      Serial.print("[HTTP] connection closed or file end.\n");
    }
  } else {
    // HTTP Error
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();



}

void requestSecond() {

  //WiFiClient client;
  HTTPClient http; //must be declared after WiFiClient for correct destruction order, because used by http.begin(client,...)

  // ======== First GET ======
  // Getting the list of posts
  // =========================

  Serial.print("[MEM] heap: ");
  Serial.println(ESP.getFreeHeap());
  Serial.print("[HTTP] begin: ");
  http.begin(firstPostUrl);
  Serial.println(firstPostUrl);

  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.println("[HTTP] GET");
  tft.println(firstPostUrl);

  Serial.print("[HTTP] SECOND GET...\n");
  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {

      int len = http.getSize();
      uint8_t buff[128] = { 0 };

#if 0
      String httpResult = http.getString();
      Serial.println(httpResult ); // with API
      for (int i = 0; i < sizeof(httpResult); i++) {
        parser.parse(httpResult[i]); // parse JSON all 128 bytes of the buffer
      }

#else

      // or "by hand"

      WiFiClient * stream = http.getStreamPtr(); // get tcp stream

      size_t sizeOfStream = stream->available();
      len = sizeOfStream;

      while (http.connected() && (len > 0 || len == -1)) {  // read all data from server

        size_t size = stream->available();
          Serial.print("SIZE OF HEAP----> ");
        Serial.println(ESP.getFreeHeap());
        Serial.print("SIZE OF STREAM----> ");
        Serial.println(size);
        Serial.print("SIZE OF LEN----> ");
        Serial.println(len);
        if (size) {

          int c = stream->readBytes(buff, std::min((size_t)len, sizeof(buff))); // read up to 128 byte
          Serial.printf("readBytes: %d\n", c);
          if (!c) {
            Serial.println("read timeout");
          }

          Serial.write(buff, c); // write it to Serial
          if (len > 0) {
            len -= c; //process the next stream count
          }


          for (int i = 0; i < sizeof(buff); i++) {
            parser.parse(buff[i]); // parse JSON all 128 bytes of the buffer
          }
        }
        delay(1);

      }

#endif





      // manipulating the URL
      // ===============================================
      // =====================================
      Serial.print("PostID: ");
      String firstPostId = listener.getPostId();
      Serial.println( firstPostId );

      Serial.print("Username: ");
      String postUserName = listener.getUserName();
      Serial.println( postUserName );

      Serial.print("Text: ");
      String postText = listener.getPostText();
      Serial.println( postText );


      tft.fillScreen(ST77XX_BLACK);
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println(postUserName);
      tft.setTextSize(1);
      tft.println(postText);

      parser.reset();
      listener.reset();

      Serial.println();
      Serial.print("[HTTP] connection closed or file end.\n");
    }
  } else {
    // HTTP Error
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }


  http.end();
  //delete http;
}
