#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <zmq.hpp>

/* TODO: Change into single instance */
class IPCServer
{
public:
	IPCServer();

	void *CreateIPC();
	void DestroyIPC();

	unsigned long SendIPC(void *handle, const void* pData, size_t tDataSize);
	unsigned long RecvIPC(void *handle, void* pData, size_t tDataSize);

private:
	zmq::context_t context;
	zmq::socket_t socket_server;
};

class IPCClient
{
public:
	IPCClient();

	void *OpenIPC();
	void CloseIPC();

	unsigned long SendIPC(void *handle, const void* pData, size_t tDataSize);
	unsigned long RecvIPC(void *handle, void* pData, size_t tDataSize);

private:
	zmq::context_t context;
	zmq::socket_t socket_client;
};
