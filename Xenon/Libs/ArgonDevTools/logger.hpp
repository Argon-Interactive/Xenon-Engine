#ifndef _XENON_DEVTOOLS_LOGGER_
#define _XENON_DEVTOOLS_LOGGER_

#include<iostream>
#include<iomanip>
#include<sstream>
#include<vector>
#include<mutex>
#include<format>

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

namespace XNTools {
	class Logger
	{
	public:
		~Logger();
		Logger(Logger &&) = delete;
		Logger &operator=(Logger &&) = delete;
		Logger(const Logger &) = delete;
		Logger& operator = (const Logger&) = delete;

		enum logMode { debug, trace, entry, info, warning, error};
		//===============================================================================
		// logging
		//===============================================================================

		static Logger& getInstance();

		/*
		0 - default
		Numbers:
		b - base 2
		o - base 8
		x - base 16
		s - scientific notation
		Strings:
		q - quote
		Vectors and Arrays:
		0 - horizontal
		i - indexed, horizontal
		s - vertical
		l - indexed, vertical  
		*/
		template<typename T, typename ...Types>
		void log(logMode mode, T first, Types&& ... args)
		{
			const std::lock_guard<std::mutex> lg(m_mutex);
			if (!m_toFile) m_msg << m_colors[mode];
			m_msg << getTime();
			m_msg << m_name << s_labels[mode] << " ";
			output(first, args...);
			m_msg << '\n';
			if (!m_toFile) { 
				m_msg << "\033[0m";
				if (mode != logMode::error) std::cout << m_msg.str(); 
				else std::cerr << m_msg.str();
				m_msg = std::stringstream();
			}
		}

		void breakLine(logMode mode = logMode::entry);

		//===============================================================================
		// settings
		//===============================================================================

		void setColors(XN_COLOR entryColor, XN_COLOR infoColor, XN_COLOR warningColor, XN_COLOR errorColor, XN_COLOR debugColor, XN_COLOR traceColor);
		void setFilePath(const std::string& filePath);
		void setName(const char* name);
	private:
		explicit Logger() :m_timeStart(std::chrono::steady_clock::now()) {}
		const std::chrono::steady_clock::time_point m_timeStart;
		bool m_toFile = false;
		std::string m_name = "[ENGINE]";
		bool m_namechange = true;
		static constexpr const char* s_labels[6] = { "[DEB]", "[TRC]", "[ENT]", "[INF]", "[WAR]", "[ERR]" };
		std::string m_colors[6] = { XN_LOG_CYAN, XN_LOG_LIGHT_GRAY,	XN_LOG_WHITE, XN_LOG_GREEN, XN_LOG_YELLOW, XN_LOG_RED};
		std::string m_filepath;
		std::stringstream m_msg;
		std::mutex m_mutex;
		//===============================================================================
		// helper funcions
		//===============================================================================

		std::string getTime();
		static size_t findToken(const char* string);
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
			const size_t tokenLocation = findToken(text);
			if (tokenLocation == std::string::npos) { m_msg << text; return; }
			m_msg.write(text, static_cast<int64_t>(tokenLocation));	
			proccesToken(text[tokenLocation + 1], arg);
			output(&text[tokenLocation + 3], args...);
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

		void proccesToken(char token, const std::string& arg);
		void proccesToken(char token, const char* arg);
		void proccesToken(char token, char* arg);
		template<typename T>
		void proccesToken(char /*token*/, T* arg) { m_msg << std::hex << arg << std::dec; }
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
			else if (token == 'b')
				m_msg << std::setfill('0') << std::setw(sizeof(T) * 8) << std::format("{:b}", arg);
			else
				m_msg << '{' << token << '}';
		}
		void proccesToken(char token, float arg)
		{ 
			if(token == '0')
				m_msg << arg;
			else if (token == 's')
				m_msg << std::scientific << arg;
			else
				m_msg << '{' << token << '}';
		}
		void proccesToken(char token, double arg)
		{ 
			if(token == '0')
				m_msg << arg;
			else if (token == 's')
				m_msg << std::scientific << arg;
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
		template<typename T, size_t S>
		void proccesToken(char token, const std::array<T, S>& arg)
		{
			if (token == '0') {
				for (int i = 0; i < S - 1; ++i) 
				{ m_msg << arg[i] << ", "; } 
				m_msg << arg[S - 1];
			}
			else if (token == 'i') {
				for (int i = 0; i < S - 1; ++i) 
				{ m_msg << i << ": " << arg[i] << ", "; }
				m_msg << S - 1 << ": " << arg[S - 1];
			}
			else if (token == 's') {
				m_msg << "\n";
				for (int i = 0; i < S; ++i) 
				{ m_msg << arg[i] << "\n"; }
			}
			else if (token == 'l') {
				m_msg << "\n";
				for (int i = 0; i < S; ++i) 
				{ m_msg << i << ". " << arg[i] << "\n"; }
			}
			else m_msg << '{' << token << '}';
		}
	};
}

#endif // !_XENON_DEVTOOLS_LOGGER_
