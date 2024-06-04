#ifndef _XENON_XENON_LOGGER_
#define _XENON_XENON_LOGGER_

#include <string>
#include <memory>
#include "api.h"

#define XN_LOG_BLACK		"\033[0;30m"
#define XN_LOG_DARK_GRAY	"\033[1;30m"
#define XN_LOG_BLUE			"\033[0;34m"
#define XN_LOG_LIGHT_BLUE	"\033[1;34m"
#define XN_LOG_GREEN		"\033[0;32m"
#define XN_LOG_LIGHT_GREEN	"\033[1;32m"
#define XN_LOG_CYAN			"\033[0;36m"
#define XN_LOG_LIGHT_CYAN	"\033[1;36m"
#define XN_LOG_RED			"\033[0;31m"
#define XN_LOG_LIGHT_RED	"\033[1;31m"
#define XN_LOG_PURPLE		"\033[0;35m"
#define XN_LOG_LIGHT_PURPLE	"\033[1;35m"
#define XN_LOG_BROWN		"\033[0;33m"
#define XN_LOG_YELLOW		"\033[1;33m"
#define XN_LOG_LIGHT_GRAY	"\033[0;37m"
#define XN_LOG_WHITE		"\033[1;37m"
#define XN_LOG_DEFAULT		"\0"

using XN_COLOR = std::string;

namespace XNtools { class Logger; }

namespace Xenon {

class XAPI Logger {

public:
	~Logger();
	Logger(Logger &&) = delete;
	Logger &operator=(Logger &&) = delete;
	Logger(const Logger &) = delete;
	Logger& operator = (const Logger&) = delete;

	enum logMode { degub, trace, entry, info, warning, error};

	static XAPI Logger& getInstance();

	template<typename T, typename ...Types>
	void XAPI log(logMode mode, T first, Types&& ... args);

	void XAPI breakLine(logMode mode = logMode::entry);

	void XAPI setColors(XN_COLOR entryColor, XN_COLOR infoColor, XN_COLOR warningColor, XN_COLOR errorColor, XN_COLOR debugColor, XN_COLOR traceColor);
	void XAPI setFilePath(const char* filePath);

private:
	Logger();
	std::unique_ptr<XNtools::Logger> m_impl;

};

}

#ifdef DISABLE_BR
	#define XN_LOG_BR(color)
#else
	#define XN_LOG_BR(color) Xenon::Logger::getInstance().breakLine(color)
#endif

#ifdef DISABLE_LOG_DEB
	#define XN_LOG_DEB(...)
#else
	#if defined __COMPILER_GCC__ || __COMPILER_CLANG__ || __COMPILER_CLANG_CL__
		#define XN_LOG_DEB(first, ...) Xenon::Logger::getInstance().log(Xenon::Logger::logMode::degub, first __VA_OPT__(,) __VA_ARGS__)
	#elif defined __COMPILER_CL__ 
		#define XN_LOG_DEB(first, ...) Xenon::Logger::getInstance().log(Xenon::Logger::logMode::debug, first, ## __VA_ARGS__)
	#else 
		#error Compiler not supported by the logger
	#endif
#endif

#ifdef DISABLE_LOG_TRC
	#define XN_LOG_TRC(...)
#else
	#if defined __COMPILER_GCC__ || __COMPILER_CLANG__ || __COMPILER_CLANG_CL__
		#define XN_LOG_TRC(first, ...) Xenon::Logger::getInstance().log(Xenon::Logger::logMode::trace, first __VA_OPT__(,) __VA_ARGS__)
	#elif defined __COMPILER_CL__ 
		#define XN_LOG_TRC(first, ...) Xenon::Logger::getInstance().log(Xenon::Logger::logMode::trace, first, ## __VA_ARGS__)
	#else 
		#error Compiler not supported by the logger
	#endif
#endif

#ifdef DISABLE_LOG_ENT
	#define XN_LOG_ENT(...)
#else
	#if defined __COMPILER_GCC__ || __COMPILER_CLANG__ || __COMPILER_CLANG_CL__
		#define XN_LOG_ENT(first, ...) Xenon::Logger::getInstance().log(Xenon::Logger::logMode::entry, first __VA_OPT__(,) __VA_ARGS__)
	#elif defined __COMPILER_CL__ 
		#define XN_LOG_ENT(first, ...) Xenon::Logger::getInstance().log(Xenon::Logger::logMode::entry, first, ## __VA_ARGS__)
	#else 
		#error Compiler not supported by the logger
	#endif
#endif

#ifdef DISABLE_LOG_INF
	#define XN_LOG_INF(...)
#else
	#if defined __COMPILER_GCC__ || __COMPILER_CLANG__ || __COMPILER_CLANG_CL__
		#define XN_LOG_INF(first, ...) Xenon::Logger::getInstance().log(Xenon::Logger::logMode::info, first __VA_OPT__(,) __VA_ARGS__)
	#elif defined __COMPILER_CL__ 
		#define XN_LOG_INF(first, ...) Xenon::Logger::getInstance().log(Xenon::Logger::logMode::info, first, ## __VA_ARGS__)
	#else 
		#error Compiler not supported by the logger
	#endif
#endif

#ifdef DISABLE_LOG_WAR
	#define XN_LOG_WAR(...)
#else
	#if defined __COMPILER_GCC__ || __COMPILER_CLANG__ || __COMPILER_CLANG_CL__
		#define XN_LOG_WAR(first, ...) Xenon::Logger::getInstance().log(Xenon::Logger::logMode::warning, first __VA_OPT__(,) __VA_ARGS__)
	#elif defined __COMPILER_CL__ 
		#define XN_LOG_WAR(first, ...) Xenon::Logger::getInstance().log(Xenon::Logger::logMode::warning, first, ## __VA_ARGS__)
	#else 
		#error Compiler not supported by the logger
	#endif
#endif

#ifdef DISABLE_LOG_ERR
	#define XN_LOG_ERR(...)
#else
	#if defined __COMPILER_GCC__ || __COMPILER_CLANG__ || __COMPILER_CLANG_CL__
		#define XN_LOG_ERR(first, ...) Xenon::Logger::getInstance().log(Xenon::Logger::logMode::error, first __VA_OPT__(,) __VA_ARGS__)
	#elif defined __COMPILER_CL__ 
		#define XN_LOG_ERR(first, ...) Xenon::Logger::getInstance().log(Xenon::Logger::logMode::error, first, ## __VA_ARGS__)
	#else 
		#error Compiler not supported by the logger
	#endif
#endif

#endif // !_XENON_XENON_LOGGER_
