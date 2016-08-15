#include <vector>
#include <string>
#include <cstring>
#include <thread>
#include <memory>
#include <functional>

#include "util/LogHelper.hpp"
#include "util/IPCHelper.hpp"

zmq::context_t IPCServer::context(1);
zmq::context_t IPCClient::context(1);
zmq::socket_t IPCServer::socket_server(context, ZMQ_REP);
zmq::socket_t IPCClient::socket_client(context, ZMQ_REQ);

/**************************************************************
 * IPC Server functions
 **************************************************************/
void * IPCServer::CreateIPC()
{
	try {
		LOG_DBG("Waiting for IPC client...");
		/* INPROC only supports inter-thread communication (same process) */
		//socket_server.bind("inproc://story.ipc");
		/* IPC supports inter-process communication, but not working in Windows */
		//socket_server.bind("ipc:///tmp/story.ipc");
		/* TCP supports communication via network, even different platform */
		socket_server.bind("tcp://*:3740");
		LOG_DBG("  Bind success.");
	} catch(zmq::error_t e) {
		LOG_ERR("[ZMQ] %s", e.what());
	}

	return socket_server;
}

void IPCServer::DestroyIPC()
{
}

unsigned long IPCServer::SendIPC(void * hPipe, const void* pData, size_t tDataSize)
{
	zmq::message_t request;

	try {
		memcpy(request.data(), "Hello", 5);

		LOG_DBG("Sending Hello ...");
		socket_server.send(request);
	} catch(zmq::error_t e) {
		LOG_ERR("[ZMQ] %s", e.what());
	}

	return 0;
}

unsigned long IPCServer::RecvIPC(void * hPipe, void* pData, size_t tDataSize)
{
	zmq::message_t request;

	try {
		socket_server.recv(&request);
		LOG_DBG("Received Hello !");
	} catch(zmq::error_t e) {
		LOG_ERR("[ZMQ] %s", e.what());
	}

	return 0;
}



/**************************************************************
 * IPC Client functions
 **************************************************************/
void * IPCClient::OpenIPC()
{
	try {
		LOG_DBG("Connecting to IPC server...");
		//socket_client.connect("inproc://story.ipc");
		//socket_client.connect("ipc:///tmp/story.ipc");
		socket_client.connect("tcp://localhost:3740");
		LOG_DBG("  Connected.");
	} catch(zmq::error_t e) {
		LOG_ERR("[ZMQ] %s", e.what());
		LOG_ERR("tcp");
	}

	return socket_client;
}

void IPCClient::CloseIPC()
{
}

unsigned long IPCClient::SendIPC(void * hPipe, const void* pData, size_t tDataSize)
{
	zmq::message_t request;

	try {
		memcpy(request.data(), "Hello", 5);

		LOG_DBG("Sending Hello ...");
		socket_client.send(request);
	} catch(zmq::error_t e) {
		LOG_ERR("[ZMQ] %s", e.what());
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
unsigned long IPCClient::RecvIPC(void * hPipe, void* pData, size_t tDataSize)
{
	zmq::message_t request;

	try {
		socket_client.recv(&request);
		LOG_DBG("Received Hello !");
	} catch(zmq::error_t e) {
		LOG_ERR("[ZMQ] %s", e.what());
	}

	return 0;
}
