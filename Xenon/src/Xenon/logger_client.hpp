#ifndef LOGGER_CLIENT_HPP
#define LOGGER_CLIENT_HPP

#define _LOGGER_CLIENT_
#include"logger.hpp"

namespace XNTools {
	Logger& getLogger();
}

#ifdef DISABLE_BR
	#define XN_LOG_BR(color)
#else
	#define XN_LOG_BR(color) XNTools::Logger::getInstance().breakLine(color)
#endif

#ifdef DISABLE_LOG_DEB
	#define XN_LOG_DEB(...)
#else
	#if defined __COMPILER_GCC__ || __COMPILER_CLANG__ || __COMPILER_CLANG_CL__
		#define XN_LOG_DEB(first, ...) XNTools::Logger::getInstance().log(XNTools::Logger::logMode::degub, first __VA_OPT__(,) __VA_ARGS__)
	#elif defined __COMPILER_CL__ 
		#define XN_LOG_DEB(first, ...) XNTools::Logger::getInstance().log(XNTools::Logger::logMode::debug, first, ## __VA_ARGS__)
	#else 
		#error Compiler not supported by the logger
	#endif
#endif

#ifdef DISABLE_LOG_TRC
	#define XN_LOG_TRC(...)
#else
	#if defined __COMPILER_GCC__ || __COMPILER_CLANG__ || __COMPILER_CLANG_CL__
		#define XN_LOG_TRC(first, ...) XNTools::Logger::getInstance().log(XNTools::Logger::logMode::trace, first __VA_OPT__(,) __VA_ARGS__)
	#elif defined __COMPILER_CL__ 
		#define XN_LOG_TRC(first, ...) XNTools::Logger::getInstance().log(XNTools::Logger::logMode::trace, first, ## __VA_ARGS__)
	#else 
		#error Compiler not supported by the logger
	#endif
#endif

#ifdef DISABLE_LOG_ENT
	#define XN_LOG_ENT(...)
#else
	#if defined __COMPILER_GCC__ || __COMPILER_CLANG__ || __COMPILER_CLANG_CL__
		#define XN_LOG_ENT(first, ...) XNTools::Logger::getInstance().log(XNTools::Logger::logMode::entry, first __VA_OPT__(,) __VA_ARGS__)
	#elif defined __COMPILER_CL__ 
		#define XN_LOG_ENT(first, ...) XNTools::Logger::getInstance().log(XNTools::Logger::logMode::entry, first, ## __VA_ARGS__)
	#else 
		#error Compiler not supported by the logger
	#endif
#endif

#ifdef DISABLE_LOG_INF
	#define XN_LOG_INF(...)
#else
	#if defined __COMPILER_GCC__ || __COMPILER_CLANG__ || __COMPILER_CLANG_CL__
		#define XN_LOG_INF(first, ...) XNTools::Logger::getInstance().log(XNTools::Logger::logMode::info, first __VA_OPT__(,) __VA_ARGS__)
	#elif defined __COMPILER_CL__ 
		#define XN_LOG_INF(first, ...) XNTools::Logger::getInstance().log(XNTools::Logger::logMode::info, first, ## __VA_ARGS__)
	#else 
		#error Compiler not supported by the logger
	#endif
#endif

#ifdef DISABLE_LOG_WAR
	#define XN_LOG_WAR(...)
#else
	#if defined __COMPILER_GCC__ || __COMPILER_CLANG__ || __COMPILER_CLANG_CL__
		#define XN_LOG_WAR(first, ...) XNTools::Logger::getInstance().log(XNTools::Logger::logMode::warning, first __VA_OPT__(,) __VA_ARGS__)
	#elif defined __COMPILER_CL__ 
		#define XN_LOG_WAR(first, ...) XNTools::Logger::getInstance().log(XNTools::Logger::logMode::warning, first, ## __VA_ARGS__)
	#else 
		#error Compiler not supported by the logger
	#endif
#endif

#ifdef DISABLE_LOG_ERR
	#define XN_LOG_ERR(...)
#else
	#if defined __COMPILER_GCC__ || __COMPILER_CLANG__ || __COMPILER_CLANG_CL__
		#define XN_LOG_ERR(first, ...) XNTools::Logger::getInstance().log(XNTools::Logger::logMode::error, first __VA_OPT__(,) __VA_ARGS__)
	#elif defined __COMPILER_CL__ 
		#define XN_LOG_ERR(first, ...) XNTools::Logger::getInstance().log(XNTools::Logger::logMode::error, first, ## __VA_ARGS__)
	#else 
		#error Compiler not supported by the logger
	#endif
#endif

#endif
