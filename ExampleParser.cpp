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
  if(key=="id" && takeNextValueId==0){
    Serial.println("FOUND ID!");
    takeNextValueId = 1;
  }
  else if(key=="username"&& takeNextValueUser==0){
    Serial.println("FOUND USERNAME!");    
    takeNextValueUser = 1;
  }
  else if(key=="raw"&& takeNextValueRaw==0){
    Serial.println("FOUND RAW!");    
    takeNextValueRaw = 1;
  }
}

void ExampleListener::value(String value) {
  Serial.println("value: " + value);
  if(takeNextValueId==1){
    postId = value;
    takeNextValueId=2; 
  }
  if(takeNextValueUser==1){
    userName = value;
    takeNextValueUser=2;
  }
  if(takeNextValueRaw==1){
    postText = value;
    takeNextValueRaw=2;  
  }
}


String ExampleListener::getPostId(){
  return postId;
}

String ExampleListener::getUserName(){
  return userName;
}

String ExampleListener::getPostText(){
  return postText;
}

void ExampleListener::reset(){
    takeNextValueId=0;
    takeNextValueUser=0;
    takeNextValueRaw=0;     
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
