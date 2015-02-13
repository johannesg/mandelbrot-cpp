#include "stdafx.h"
#include "zmqpp.h"

proxy::proxy() : p(zactor_new(zproxy, nullptr))
{
}

void proxy::frontend(const char* type, const char * endpoint) {
  zstr_sendx(p.get(), "FRONTEND", type, endpoint, NULL);
  zsock_wait(p.get());
}

void proxy::backend(const char* type, const char * endpoint) {
  zstr_sendx(p.get(), "BACKEND", type, endpoint, NULL);
  zsock_wait(p.get());
}
