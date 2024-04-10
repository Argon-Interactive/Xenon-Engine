#pragma once

#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<mutex>

#include "../core/core.h"

#if defined __DEBUG__ || __RELESE__
	#define XN_LOG_ENT(first, ...) Core::Logger::getInstanceCore().log(Core::Logger::logMode::ent, first, ## __VA_ARGS__)
	#define XN_LOG_INF(first, ...) Core::Logger::getInstanceCore().log(Core::Logger::logMode::inf, first, ## __VA_ARGS__)
	#define XN_LOG_WAR(first, ...) Core::Logger::getInstanceCore().log(Core::Logger::logMode::war, first, ## __VA_ARGS__)
	#define XN_LOG_ERR(first, ...) Core::Logger::getInstanceCore().log(Core::Logger::logMode::err, first, ## __VA_ARGS__)
	#define XN_LOG_BR Core::Logger::getInstanceCore().breakLine() 
	#define XN_LOG_TO_FILE(path)
#else
	#define XN_LOG_ENT(...)
	#define XN_LOG_INF(...)
	#define XN_LOG_WAR(...)
	#define XN_LOG_BR
	#define XN_LOG_ERR(first, ...) Core::Logger::getInstanceCore().log(Core::Logger::logMode::err, first, ## __VA_ARGS__)
	#define XN_LOG_TO_FILE(path) Core::Logger::getInstanceCore().setFilePath(path)
#endif

#define XN_LOG_CORE Core::Logger::getInstanceCore()
#define XN_LOG_CLIENT Core::Logger::getInstanceClient()

#define XN_LOG_BLACK			"\033[0;30m"
#define XN_LOG_DARK_GRAY		"\033[1;30m"
#define XN_LOG_BLUE				"\033[0;34m"
#define XN_LOG_LIGHT_BLUE		"\033[1;34m"
#define XN_LOG_GREEN			"\033[0;32m"
#define XN_LOG_LIGHT_GREEN		"\033[1;32m"
#define XN_LOG_CYAN				"\033[0;36m"
#define XN_LOG_LIGHT_CYAN		"\033[1;36m"
#define XN_LOG_RED				"\033[0;31m"
#define XN_LOG_LIGHT_RED		"\033[1;31m"
#define XN_LOG_PURPLE			"\033[0;35m"
#define XN_LOG_LIGHT_PURPLE		"\033[1;35m"
#define XN_LOG_BROWN			"\033[0;33m"
#define XN_LOG_YELLOW			"\033[1;33m"
#define XN_LOG_LIGHT_GRAY		"\033[0;37m"
#define XN_LOG_WHITE			"\033[1;37m"
#define XN_LOG_DEFAULT			"\0"

#define XN_LOG_MODE_ENT	Core::Logger::logMode::ent
#define XN_LOG_MODE_INF	Core::Logger::logMode::inf
#define XN_LOG_MODE_WAR	Core::Logger::logMode::war
#define XN_LOG_MODE_ERR	Core::Logger::logMode::err

typedef std::string XN_COLOR;

namespace Core {
	class Logger
	{
	public:
		~Logger();
		Logger(const Logger&) = delete;
		Logger& operator = (const Logger&) = delete;
		static Logger& getInstanceCore();
		XAPI static Logger& getInstanceClient();
		enum struct logMode {ent, inf, war, err};
		//===============================================================================
		// logging
		//===============================================================================

		template<typename T, typename ...Types>
		void log(logMode mode, T first, Types&& ... args)
		{
			const std::lock_guard<std::mutex> lg(m_mutex);
			if (!m_toFile) m_msg << m_colors[static_cast<int>(mode)];
			m_msg << getTime();
			switch (mode) {
			case logMode::ent:
				m_msg << "[ENT] "; break;
			case logMode::inf:
				m_msg << "[INF] "; break;
			case logMode::war:
				m_msg << "[WAR] "; break;
			case logMode::err:
				m_msg << "[ERR] "; break;
			}
			output(first, args...);
			m_msg << '\n';
			if (!m_toFile) { 
				m_msg << "\033[0m";
				if (mode != logMode::err) std::cout << m_msg.str(); 
				else std::cerr << m_msg.str();
				m_msg = std::stringstream();
			}
		}
		void XAPI breakLine(XN_COLOR color = XN_LOG_WHITE);
		void XAPI breakLine(logMode mode);
		//===============================================================================
		// settings
		//===============================================================================

		void XAPI setColors(XN_COLOR entryColor, XN_COLOR infoColor, XN_COLOR warningColor, XN_COLOR errorColor);
		void XAPI setFilePath(const std::string& filePath);
		void XAPI setFilePath(const char* filePath);
	private:
		Logger() :m_timeStart(std::chrono::steady_clock::now()), m_toFile(false), m_colors{ XN_LOG_WHITE, XN_LOG_GREEN, XN_LOG_YELLOW, XN_LOG_RED} {}
		const std::chrono::steady_clock::time_point m_timeStart;
		bool m_toFile;
		std::string m_colors[4];
		std::string m_filepath;
		std::stringstream m_msg;
		std::mutex m_mutex;
		XAPI static Logger m_LogCore;
		XAPI static Logger m_LogClient;
		//===============================================================================
		// helper funcions
		//===============================================================================

		std::string XAPI getTime();
		size_t XAPI findToken(const char* string) const;
		//===============================================================================
		// recursive output
		//===============================================================================

		template<typename T>
		void output(T arg)
		{ m_msg << arg; }
		template<typename T>
		void outputnostr(T arg)
		{ m_msg << arg; }
		template<typename T, typename ...Types>
		void output(const char* text, T&& arg, Types&& ...args)
		{
			const size_t tlc = findToken(text);
			if (tlc == std::string::npos) { m_msg << text; return; }
			m_msg.write(text, tlc);	
			proccesToken(text[tlc + 1], arg);
			output(&text[tlc+3], args...);
		}
		template<typename T, typename ...Types>
		void output(T&& arg, Types&& ...args)
		{ m_msg << arg << ", "; outputnostr(args...); }
		template<typename T, typename ...Types>
		void outputnostr(T&& arg, Types&& ...args)
		{ m_msg << arg << ", "; outputnostr(args...); }
		//===============================================================================
		// tokens processing
		//===============================================================================

		void XAPI proccesToken(char token, const std::string& arg);
		void XAPI proccesToken(char token, const char* arg);
		template<typename T>
		void proccesToken(char token, T arg)
		{ 
			if(token == '0')
				m_msg << arg;
			else if (token == 'o')
				m_msg << std::oct << arg << std::dec;
			else if (token == 's')
				m_msg << std::scientific << arg;
			else if (token == 'x')
				m_msg << std::uppercase << std::hex << arg << std::dec << std::nouppercase;
			else
				m_msg << '{' << token << '}';
		}
		template<typename T>
		void proccesToken(char token, const std::vector<T>& arg)
		{
			if (token == '0') {
				for (int i = 0; i < arg.size() - 1; ++i) 
				{ m_msg << arg[i] << ", "; } 
				m_msg << arg[arg.size()-1];
			}
			else if (token == 'i') {
				for (int i = 0; i < arg.size() - 1; ++i) 
				{ m_msg << i << ": " << arg[i] << ", "; }
				m_msg << arg.size() - 1 << ": " << arg[arg.size() - 1];
			}
			else if (token == 's') {
				m_msg << "\n";
				for (int i = 0; i < arg.size(); ++i) 
				{ m_msg << arg[i] << "\n"; }
			}
			else if (token == 'l') {
				m_msg << "\n";
				for (int i = 0; i < arg.size(); ++i) 
				{ m_msg << i << ". " << arg[i] << "\n"; }
			}
			else m_msg << '{' << token << '}';
		}
	};
}
