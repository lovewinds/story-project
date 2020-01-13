#include "util/LogHelper.hpp"

#ifndef USE_SDL_LOG
Log* Log::logger = nullptr;
//std::unique_ptr<g3::LogWorker> Log::logworker{ g3::LogWorker::createLogWorker() };

struct CustomSink {
	// Linux xterm color
	// http://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
	enum FG_Color { YELLOW = 33, RED = 41, GREEN = 32, BLUE = 36, WHITE = 97 };

	FG_Color GetColor(const LEVELS level) const {
		if (level.value == WARNING.value) { return RED; }
		// Windows (DEBUG->DBUG)
		else if (level.value == DBUG.value) { return GREEN; }
		/* Custom log level for python */
		else if (level.value == WARNING.value + 1) { return GREEN; }
		else if (g3::internal::wasFatal(level)) { return BLUE; }

		return WHITE;
	}

	void ReceiveLogMessage(g3::LogMessageMover logEntry) {
		auto level = logEntry.get()._level;
		auto color = GetColor(level);

		logEntry.get().timestamp("[%m/%d %H:%M:%S]");

		if (level.value == WARNING.value + 1) {
			/* Custom log level for python */
#if !defined(PLATFORM_IOS)
			std::cout << "\033[" << color << "m";
#endif
			std::cout << "[" << logEntry.get().timestamp("%m/%d %H:%M:%S") << "]"
				<< std::setw(4);
			std::cout.width(7);
			std::cout << "[Python] " << logEntry.get().message();
#if !defined(PLATFORM_IOS)
			std::cout << "\033[m";
#endif
			std::cout << std::endl;
		} else {
			/* Normal log strings */
#if !defined(PLATFORM_IOS)
			if (level.value == WARNING.value)
				std::cout << "\033[41m" << "\033[97m";
			else
				std::cout << "\033[" << color << "m";
#endif
			std::cout << "[" << logEntry.get().timestamp("%m/%d %H:%M:%S") << "]"
				<< std::setw(4);
			std::cout.width(7);
			std::cout << logEntry.get().level()
				<< " [" << logEntry.get().file()
				<< ":" << logEntry.get().line() << "]"
				<< " " << logEntry.get().message();
#if !defined(PLATFORM_IOS)
			std::cout << "\033[m";
#endif
			std::cout << std::endl;
		}
	}
};

Log::Log()
{
}

Log::~Log()
{
	//deinit();
}

void Log::dbg()
{
	// LOGF(PY_LOG, "Python debug Message !!!");
}

void Log::init()
{
	if (logger == nullptr) {
		logger = new Log();

/*
	// Default file sink
	auto worker = g3::LogWorker::createLogWorker();
	auto handle = worker->addDefaultLogger("test", "./");

	g3::initializeLogging(worker.get());
*/

		logger->logworker = g3::LogWorker::createLogWorker();
		static auto sinkHandle = logger->logworker->addSink(std2::make_unique<CustomSink>(),
			&CustomSink::ReceiveLogMessage);
		g3::initializeLogging(logger->logworker.get());

	//std::future<void> received = sinkHandle->call(&CustomSink::Foo, NULL);

	//shutDownLogging();
	//	std::future<std::string> log_file_name = handle->call(&g3::FileSink::fileName);
	//	std::cout << "Log filename :[" << log_file_name.get() << std::endl;
	}
}

void Log::deinit()
{
	if (logger != nullptr) {
		std::cout << "Logger destroyed" << std::endl;

		g3::internal::shutDownLogging();
		delete logger;
		logger = nullptr;
	}
}
#endif /*#ifndef USE_SDL_LOG*/
