#include "ExampleParser.h"
#include "JsonListener.h"


void ExampleListener::whitespace(char c) {
  Serial.println("whitespace");
}

void ExampleListener::startDocument() {
  Serial.println("start document");
}

void ExampleListener::key(String key) {
  Serial.println("key: " + key);
  if(key=="id"){
    Serial.println("FOUND IT!");
    takeNextValue = 1;
  }
}

void ExampleListener::value(String value) {
  Serial.println("value: " + value);
  if(takeNextValue==1){
    postId = value;
    takeNextValue=0;
  }
}

String ExampleListener::showId(){
  return postId;
}

void ExampleListener::endArray() {
  Serial.println("end array. ");
}

void ExampleListener::endObject() {
  Serial.println("end object. ");
}

void ExampleListener::endDocument() {
  Serial.println("end document. ");
}

void ExampleListener::startArray() {
   Serial.println("start array. ");
}

void ExampleListener::startObject() {
   Serial.println("start object. ");
}
