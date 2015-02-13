#pragma once

#include <memory>
#include <czmq.h>
#include <iostream>

#define ZMQ_UNIQUE_PTR(name) class __##name##_deleter {\
  public: \
    void operator() ( name##_t * p ) { \
      name##_destroy(&p); \
    } \
  };\
  typedef std::unique_ptr<name##_t, __##name##_deleter> name##_ptr;

ZMQ_UNIQUE_PTR(zsock)
ZMQ_UNIQUE_PTR(zactor)
ZMQ_UNIQUE_PTR(zpoller)
ZMQ_UNIQUE_PTR(zmsg)
ZMQ_UNIQUE_PTR(zframe)

class actor {
public:
  actor();

private:
  zactor_ptr p;
};

class proxy {
public:
  proxy();
  void frontend(const char* type, const char * endpoint);
  void backend(const char* type, const char * endpoint);

private:

  zactor_ptr p;
};

std::string zmsg_popstring(zmsg_t *self);
std::string zstring_recv(void *source);


