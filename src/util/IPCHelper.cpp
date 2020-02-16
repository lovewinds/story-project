#include <vector>
#include <string>
#include <cstring>
#include <thread>
#include <memory>
#include <functional>

#include "util/LogHelper.hpp"
#include "util/IPCHelper.hpp"

/* Temporary feature */
#define FEATURE_IPC_ENABLED

/**************************************************************
 * IPC Server functions
 **************************************************************/
IPCServer::IPCServer() :
  context(1),
  socket_server(context, ZMQ_REP)
{
  
}

void * IPCServer::CreateIPC()
{
#if defined(FEATURE_IPC_ENABLED)
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
#else
  return NULL;
#endif
}

void IPCServer::DestroyIPC()
{
}

unsigned long IPCServer::SendIPC(void * hPipe, const void* pData, size_t tDataSize)
{
#if defined(FEATURE_IPC_ENABLED)
  zmq::message_t request;

  try {
    /*
     * REQ / REP pair server should send response when data receives
     */
    memcpy(request.data(), "Hello", 5);

    LOG_DBG("Sending Hello ...");
    socket_server.send(request);
  } catch(zmq::error_t e) {
    LOG_ERR("[ZMQ] %s", e.what());
  }

  return 0;
#else
  return 0;
#endif
}

unsigned long IPCServer::RecvIPC(void * hPipe, void* pData, size_t tDataSize)
{
#if defined(FEATURE_IPC_ENABLED)
  zmq::message_t request;

  try {
    auto len = socket_server.recv(&request, ZMQ_NOBLOCK);
    // LOG_DBG("Received Hello !");

    if (len > 0) {
      LOG_DBG("Sending Hello ...");
      memcpy(request.data(), "Hello", 5);
      socket_server.send(request);
    }
  } catch(zmq::error_t e) {
    LOG_ERR("[ZMQ] %s", e.what());
  }

  return 0;
#else
  return 0;
#endif
}



/**************************************************************
 * IPC Client functions
 **************************************************************/
IPCClient::IPCClient() :
  context(1),
  socket_client(context, ZMQ_REQ)
{
  
}

void * IPCClient::OpenIPC()
{
#if defined(FEATURE_IPC_ENABLED)
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
#else
  return NULL;
#endif
}

void IPCClient::CloseIPC()
{
}

unsigned long IPCClient::SendIPC(void * hPipe, const void* pData, size_t tDataSize)
{
#if defined(FEATURE_IPC_ENABLED)
  zmq::message_t request;

  try {
    memcpy(request.data(), "Hello", 5);

    LOG_DBG("Sending Hello ...");
    socket_client.send(request);
  } catch(zmq::error_t e) {
    LOG_ERR("[ZMQ] %s", e.what());
  }

  return 0;
#else
  return 0;
#endif
}

//////////////////////////////////////////////////////////////////////////
unsigned long IPCClient::RecvIPC(void * hPipe, void* pData, size_t tDataSize)
{
#if defined(FEATURE_IPC_ENABLED)
  zmq::message_t request;

  try {
    socket_client.recv(&request);
    LOG_DBG("Received Hello !");
  } catch(zmq::error_t e) {
    LOG_ERR("[ZMQ] %s", e.what());
  }

  return 0;
#else
  return 0;
#endif
}
