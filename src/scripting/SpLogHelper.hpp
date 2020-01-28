#pragma once

class SpLog
{
public:
  SpLog();
  ~SpLog();

  void dbg();
  void connect();
  void send();

// private:
  // void *ipc;
};
