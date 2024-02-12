#define XENON
#include"logger.hpp"
#include<chrono>

Core::Logger Core::Logger::m_LogCore;
Core::Logger Core::Logger::m_LogClient;

Core::Logger& Core::Logger::getInstanceCore() { return m_LogCore; }
Core::Logger& Core::Logger::getInstanceClient() { return m_LogClient; }

void Core::Logger::setColors(XN_COLOR entryColor, XN_COLOR infoColor, XN_COLOR warningColor, XN_COLOR errorColor)
{
	m_colors[0] = (entryColor	 == "\0") ? XN_LOG_WHITE : entryColor;
	m_colors[1] = (infoColor	 == "\0") ? XN_LOG_GREEN : infoColor;
	m_colors[2] = (warningColor	 == "\0") ? XN_LOG_YELLOW : warningColor;
	m_colors[3] = (errorColor	 == "\0") ? XN_LOG_RED : errorColor;
}
void Core::Logger::setFilePath(const std::string& filePath)
{ m_filepath = filePath; m_toFile = true; }
void Core::Logger::setFilePath(const char* filePath)
{ m_filepath = filePath; m_toFile = true; }

std::string Core::Logger::getTime()
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
size_t Core::Logger::findToken(const char* string) const
{
	if (string[0] == '\0') { return std::string::npos; }
	if (string[1] == '\0') { return std::string::npos; }
	size_t i = 0;
	while (string[i + 2] != '\0')
	{ if (string[i] == '{' && string[i + 2] == '}')  return i; i++; }
	return std::string::npos;
}

void Core::Logger::proccesToken(char token, const std::string& arg)
{
	if (token == '0') m_msg << arg;
	else m_msg << '{' << token << '}';
}
void Core::Logger::proccesToken(char token, const char* arg)
{
	if (token == '0') m_msg << arg;
	else m_msg << '{' << token << '}';
}

Core::Logger::~Logger() {
	if (m_toFile) {
		std::ofstream out(m_filepath ,std::ios::out);
		out << m_msg.str(); out.close();
	}
}

void Core::Logger::breakLine(XN_COLOR color)
{ std::cout << color << "=========================================================================\033[0m\n"; }

void Core::Logger::breakLine(logMode mode)
{ std::cout << m_colors[static_cast<int>(mode)] << "=========================================================================\033[0m\n"; }

