#include "worker.h"
#include "globals.h"
#include "czmqpp/czmqpp.h"
#include <iostream>
using namespace std;

int s_worker_pipe(zloop_t* loop, zsock_t* reader, void* arg) {

	cout << "worker pipe cmd" << endl;
	auto msg = zstr_recv(reader);
	if (msg == nullptr) return -1;

	if (streq(msg, "$TERM")) return -1;

	zstr_free(&msg);

	return 0;
}

int s_worker(zloop_t* loop, zsock_t* reader, void* arg) {

	auto msg = zstr_recv(reader);
	cout << "worker cmd: " << msg << endl;
	zstr_free(&msg);

	return 0;
}

void worker(zsock_t* pipe, void* args) {
	cout << "Starting worker" << endl;

	zsock_ptr worker(zsock_new_pull(nullptr));

	auto ret = zsock_connect(worker.get(), endpoint);
	
	cout << "connect: " << ret << endl;

	auto loop = zloop_new();

	zloop_reader(loop, pipe, s_worker_pipe, nullptr);
	//	zloop_reader_set_tolerant(loop, pipe);
	zloop_reader(loop, worker.get(), s_worker, nullptr);

	zloop_set_verbose(loop, true);

	zsock_signal(pipe, 0);
	zloop_start(loop);
}
