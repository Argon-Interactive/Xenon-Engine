#ifndef LOGGER_CLIENT_HPP
#define LOGGER_CLIENT_HPP

#define _LOGGER_CLIENT_
#include"logger.hpp"

namespace Xenon {
	Logger& getLogger();
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

#endif
