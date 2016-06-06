#pragma once
#include <string>
#include <iostream>
#include <thread>
#include <iomanip>
#include <memory>

#ifndef USE_SDL_LOG
#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>
#include <g3log/logmessage.hpp>
#endif

#ifdef USE_SDL_LOG
#define LOG_ERR(fmt, ...)	SDL_LogError(SDL_LOG_CATEGORY_ERROR, fmt, ##__VA_ARGS__)
#define LOG_DBG(fmt, ...)	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, fmt, ##__VA_ARGS__)
#else
#define LOG_ERR(fmt, ...)	LOGF(WARNING, fmt, ##__VA_ARGS__)
#define LOG_DBG(fmt, ...)	LOGF(DBUG, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)	LOGF(INFO, fmt, ##__VA_ARGS__)
#endif

#ifndef USE_SDL_LOG
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
#endif
