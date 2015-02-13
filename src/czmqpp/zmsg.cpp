#include "czmqpp.h"

std::string zmsg_popstring(zmsg_t *self) {
  assert(self);
  assert(zmsg_is(self));

  auto frame = zframe_ptr(zmsg_pop(self));

  auto size = zframe_size(frame.get());
  auto data = zframe_data(frame.get());

  return std::string(data, data + size);
}

