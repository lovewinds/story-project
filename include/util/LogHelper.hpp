#pragma once
#include <string>
#include <iostream>
#include <thread>
#include <iomanip>
#include <memory>

#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>
#include <g3log/logmessage.hpp>

#include "EScreenManager.h"
#include "texture/ETexture.h"

#ifdef USE_SDL_LOG
#define ERROR(...)	SDL_LogError(SDL_LOG_CATEGORY_ERROR, __VA_ARGS__);
#define DEBUG(...)	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__);
#define INFO(...)	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__);
#else
#define LOG_ERR(fmt, ...)	LOGF(WARNING, fmt, ##__VA_ARGS__);
#define LOG_DBG(fmt, ...)	LOGF(DBUG, fmt, ##__VA_ARGS__);
#define LOG_INFO(fmt, ...)	LOGF(INFO, fmt, ##__VA_ARGS__);
#endif

class Log
{
public:
	~Log();
	//static Log*	getInstance();

	static void init();
	static void deinit();

private:
	Log();

	static Log*	logger;
	std::unique_ptr<g3::LogWorker> logworker;
	//auto sinkHandle;
};
