#pragma once

#include "JsonListener.h"




  


class ExampleListener: public JsonListener {

  bool takeNextValue;

  String postId;

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

    String showId();
};
