#include "logger.hpp"

#include <cmath>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>

Xenon::Logger& Xenon::Logger::getInstance() { 
	static Logger logger(false);
	return logger; 
}

Xenon::Logger& Xenon::Logger::getInstanceCore() { 
	static Logger logger(true);
	return logger; 
}

void Xenon::Logger::setColors(XN_COLOR entryColor, XN_COLOR infoColor, XN_COLOR warningColor, XN_COLOR errorColor, XN_COLOR debugColor, XN_COLOR traceColor)
{
	m_colors[0] = (debugColor.empty())   ? XN_LOG_CYAN			: debugColor;
	m_colors[1] = (traceColor.empty())   ? XN_LOG_LIGHT_GRAY	: traceColor;
	m_colors[2] = (entryColor.empty())   ? XN_LOG_WHITE			: entryColor;
	m_colors[3] = (infoColor.empty())    ? XN_LOG_GREEN			: infoColor;
	m_colors[4] = (warningColor.empty()) ? XN_LOG_YELLOW		: warningColor;
	m_colors[5] = (errorColor.empty())   ? XN_LOG_RED			: errorColor;
}
void Xenon::Logger::setFilePath(const std::string& filePath)
{ m_filepath = filePath; m_toFile = true; }


std::string Xenon::Logger::getTime()
{
	const int time = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_timeStart).count());
	const int hours = time / 3600;
	const int minutes = (time - hours * 3600) / 60;
	const int seconds = time - hours * 3600 - minutes * 60;
	std::stringstream result;
	result << "[" << ((hours < 10) ? "0" : "") << std::to_string(hours) << ":" << ((minutes < 10) ? "0" : "") << 
		std::to_string(minutes) << ":" << ((seconds < 10) ? "0" : "") << std::to_string(seconds) << "]";
	return result.str();
}

size_t Xenon::Logger::findToken(const char* string)
{
	if (string[0] == '\0') { return std::string::npos; }
	if (string[1] == '\0') { return std::string::npos; }
	for (size_t i = 0; string[i + 2] != '\0'; i++) {
		if (string[i] == '{' && string[i + 2] == '}')
			return i;
	}
	return std::string::npos;
}

void Xenon::Logger::proccesToken(char token, const std::string& arg)
{
	if (token == '0') { m_msg << arg; }
	if (token == 'q') { m_msg << "\"" << arg << "\""; }
	else { m_msg << "{" << token << "}"; }
}
void Xenon::Logger::proccesToken(char token, const char* arg)
{
	if (token == '0') { m_msg << arg; }
	if (token == 'q') { m_msg << "\"" << arg << "\""; }
	else { m_msg << "{" << token << "}"; }
}
void Xenon::Logger::proccesToken(char token, char* arg)
{
	if (token == '0') { m_msg << arg; }
	if (token == 'q') { m_msg << "\"" << arg << "\""; }
	//it makes no fucking sense what so ever and i dont care to fix it since it will never be used more then once 
	else if(token != '0' && token != 'q') { m_msg << "{" << token << "}"; } 
}

Xenon::Logger::~Logger() {
	try {
		if (m_toFile && !m_msg.str().empty()) {
			const auto lTime = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() }.get_local_time();
			const auto ld = floor<std::chrono::days>(lTime);
			const std::chrono::year_month_day ymd{ ld };

			std::stringstream ss;
			ss << ymd << "-";
			m_filepath += ss.str();

			for (int i = 1; true; ++i) {
				try {
					if (std::filesystem::exists(m_filepath + std::to_string(i)) && std::filesystem::is_regular_file(m_filepath + std::to_string(i))) {
						continue;
					}
					m_filepath += std::to_string(i) + ".txt";
					break;
				}
				catch (const std::filesystem::filesystem_error& e) {
					std::cerr << "Logger: Filesystem error: " << e.what() << "\n";
					return;
				}
			}
			try {
				std::ofstream out(m_filepath, std::ios::out);
				out << m_msg.str();
				out.close();
			}
			catch (const std::ofstream::failure& e) {
				std::cerr << "Logger: File operation error: " << e.what() << '\n';
			}
		}
	} catch (std::exception& e) {
		try {
			std::cout << "Logger couldn't save logs to file: " << e.what();
		} catch (std::exception& e2) { /* Holy fuck that is even worse lol */ }
	}
}

void Xenon::Logger::breakLine(logMode mode)
{
	log(mode, "=========================================================================");
}

