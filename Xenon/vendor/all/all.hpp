//NOLINTBEGIN
#ifndef ALL
#define ALL

#include<iostream>
#include<sstream>
#include<fstream>
#include<chrono>
#include<vector>
#include<mutex>

#ifndef ALL_DISABLE_ENT
	#define ALL_ENT(first, ...) all::Logger::getInstance().log(all::Logger::logMode::ent, first, ## __VA_ARGS__)
#else
	#define ALL_ENT(...)
#endif
#ifndef ALL_DISABLE_INF
	#define ALL_INF(first, ...) all::Logger::getInstance().log(all::Logger::logMode::inf, first, ## __VA_ARGS__)
#else
	#define ALL_INF(...)
#endif
#ifndef ALL_DISABLE_WAR
	#define ALL_WAR(first, ...) all::Logger::getInstance().log(all::Logger::logMode::war, first, ## __VA_ARGS__)
#else
	#define ALL_WAR(...)
#endif
#ifndef ALL_DISABLE_ERR
	#define ALL_ERR(first, ...) all::Logger::getInstance().log(all::Logger::logMode::err, first, ## __VA_ARGS__)
#else
	#define ALL_ERR(...)
#endif

#define ALL_LOG all::Logger::getInstance()

#define ALL_BLACK			"\033[0;30m"
#define ALL_DARK_GRAY		"\033[1;30m"
#define ALL_BLUE			"\033[0;34m"
#define ALL_LIGHT_BLUE		"\033[1;34m"
#define ALL_GREEN			"\033[0;32m"
#define ALL_LIGHT_GREEN		"\033[1;32m"
#define ALL_CYAN			"\033[0;36m"
#define ALL_LIGHT_CYAN		"\033[1;36m"
#define ALL_RED				"\033[0;31m"
#define ALL_LIGHT_RED		"\033[1;31m"
#define ALL_PURPLE			"\033[0;35m"
#define ALL_LIGHT_PURPLE	"\033[1;35m"
#define ALL_BROWN			"\033[0;33m"
#define ALL_YELLOW			"\033[1;33m"
#define ALL_LIGHT_GRAY		"\033[0;37m"
#define ALL_WHITE			"\033[1;37m"
#define ALL_DEFAULT			"\0"

#define ALL_LOG_MODE_ENT	all::Logger::logMode::ent
#define ALL_LOG_MODE_INF	all::Logger::logMode::inf
#define ALL_LOG_MODE_WAR	all::Logger::logMode::war
#define ALL_LOG_MODE_ERR	all::Logger::logMode::err

typedef std::string all_color;

namespace all {
class Logger
{
public:
	~Logger() {
		if (m_toFile) {
			std::ofstream out(m_filepath ,std::ios::out);
			out << m_msg.str(); out.close();
		}
	}
	Logger(const Logger&) = delete;
	Logger& operator = (const Logger&) = delete;
	static Logger& getInstance()
	{ static Logger s; return s; }
	enum struct logMode {ent, inf, war, err};
	//===============================================================================
	// logging
	//===============================================================================
	template<typename T, typename ...Types>
	void log(logMode mode, T first, Types&& ... args)
	{
		const std::lock_guard<std::mutex> lg(m_mutex);
		if (!m_toFile) m_msg << m_colors[static_cast<int>(mode)];
		m_msg << getTime() << m_type[static_cast<int>(mode)];
		output(first, args...);
		m_msg << '\n';
		if (!m_toFile) { 
			m_msg << "\033[0m";
			if (mode != logMode::err) std::cout << m_msg.str(); 
			else std::cerr << m_msg.str();
			m_msg = std::stringstream();
		}
	}
	//===============================================================================
	// settings
	//===============================================================================
	void setColors(all_color entryColor, all_color infoColor, all_color warningColor, all_color errorColor)
	{
		m_colors[0] = (entryColor	 == "\0") ? ALL_WHITE : entryColor;
		m_colors[1] = (infoColor	 == "\0") ? ALL_GREEN : infoColor;
		m_colors[2] = (warningColor	 == "\0") ? ALL_YELLOW : warningColor;
		m_colors[3] = (errorColor	 == "\0") ? ALL_RED : errorColor;
	}
	void setFilePath(const std::string& filePath) 
	{ m_filepath = filePath; m_toFile = true; }
	void setFilePath(const char* filePath)
	{ m_filepath = filePath; m_toFile = true; }
private:
	Logger() :m_timeStart(std::chrono::steady_clock::now()) {}
	const std::chrono::steady_clock::time_point m_timeStart;
	bool m_toFile = false;
	std::string m_filepath;
	const std::string m_type[4] = { "[ENT] ", "[INF] ", "[WAR] ", "[ERR] "};
	std::string m_colors[4] = { ALL_WHITE, ALL_GREEN, ALL_YELLOW, ALL_RED};
	std::stringstream m_msg;
	std::mutex m_mutex;
	//===============================================================================
	// helper funcions
	//===============================================================================
	std::string getTime()
	{
		int time = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_timeStart).count());
		int hours = time / 3600;
		int minutes = (time - hours*3600) / 60;
		int seconds = time - hours *3600 - minutes * 60;
		std::stringstream result;
		result << "[" << ((hours < 10) ? "0" : "") << std::to_string(hours) << ":" << ((minutes < 10) ? "0" : "") << 
			std::to_string(minutes) << ":" << ((seconds < 10) ? "0" : "") << std::to_string(seconds) << "]";
		return result.str();
	}
	size_t findToken(const char* string) const
	{
		if (string[0] == '\0') { return std::string::npos; }
		if (string[1] == '\0') { return std::string::npos; }
		size_t i = 0;
		while (string[i + 2] != '\0')
		{ if (string[i] == '{' && string[i + 2] == '}')  return i; i++; }
		return std::string::npos;
	}
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
	void proccesToken(char token, const std::string& arg)
	{
		if (token == '0') m_msg << arg;
		else m_msg << '{' << token << '}';
	}
	void proccesToken(char token, const char* arg)
	{
		if (token == '0') m_msg << arg;
		else m_msg << '{' << token << '}';
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
#endif
//NOLINTEND