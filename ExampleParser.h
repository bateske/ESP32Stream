#pragma once

#include "JsonListener.h"




  


class ExampleListener: public JsonListener {

  int takeNextValueId, takeNextValueUser, takeNextValueRaw;

  String postId, userName, postText;

  public:
    virtual void whitespace(char c);
  
    virtual void startDocument();

    virtual void key(String key);

    virtual void value(String value);

    virtual void endArray();

    virtual void endObject();

    virtual void endDocument();

    virtual void startArray();

    virtual void startObject();

    String getPostId();

    String getUserName();

    String getPostText();

     void reset();
    
};
