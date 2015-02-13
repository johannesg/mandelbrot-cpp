#include "czmqpp.h"
#include <string>

std::string zstring_recv(void *source) {

	assert(source);
	void *handle = zsock_resolve(source);

	zmq_msg_t message;
	zmq_msg_init(&message);
	if (zmq_recvmsg(handle, &message, 0) < 0) 
		return std::string();

	size_t size = zmq_msg_size(&message);

	std::string string ((const char*)zmq_msg_data(&message), size);

	zmq_msg_close(&message);
	return string;
}
