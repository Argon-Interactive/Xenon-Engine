#include"logger.hpp"
#include<fstream>
#include<chrono>
#include<iomanip>
#include<filesystem>

Xenon::Logger Xenon::Logger::s_LogCore;
Xenon::Logger Xenon::Logger::s_LogClient;

Xenon::Logger& Xenon::Logger::getInstance(int DO_NOT_SPECIFY) {
	if (DO_NOT_SPECIFY) return s_LogClient;
	return s_LogCore;
}


void Xenon::Logger::setColors(XN_COLOR entryColor, XN_COLOR infoColor, XN_COLOR warningColor, XN_COLOR errorColor, XN_COLOR debugColor, XN_COLOR traceColor)
{
	m_colors[0] = (debugColor	 == "\0") ? XN_LOG_CYAN			: debugColor;
	m_colors[1] = (traceColor	 == "\0") ? XN_LOG_LIGHT_GRAY	: traceColor;
	m_colors[2] = (entryColor	 == "\0") ? XN_LOG_WHITE		: entryColor;
	m_colors[3] = (infoColor	 == "\0") ? XN_LOG_GREEN		: infoColor;
	m_colors[4] = (warningColor	 == "\0") ? XN_LOG_YELLOW		: warningColor;
	m_colors[5] = (errorColor	 == "\0") ? XN_LOG_RED			: errorColor;
}
void Xenon::Logger::setFilePath(const std::string& filePath)
{ m_filepath = filePath; m_toFile = true; }


std::string Xenon::Logger::getTime()
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
size_t Xenon::Logger::findToken(const char* string) const
{
	if (string[0] == '\0') { return std::string::npos; }
	if (string[1] == '\0') { return std::string::npos; }
	size_t i = 0;
	while (string[i + 2] != '\0')
	{ if (string[i] == '{' && string[i + 2] == '}')  return i; i++; }
	return std::string::npos;
}

void Xenon::Logger::proccesToken(char token, const std::string& arg)
{
	if (token == '0') m_msg << arg;
	else m_msg << '{' << token << '}';
}
void Xenon::Logger::proccesToken(char token, const char* arg)
{
	if (token == '0') m_msg << arg;
	else m_msg << '{' << token << '}';
}

Xenon::Logger::~Logger() {
	if (m_toFile && !m_msg.str().empty()) {
	const auto now = std::chrono::system_clock::now();
	const auto in_time_t = std::chrono::system_clock::to_time_t(now);
	std::stringstream output_stream;
	struct tm time_info;
	const auto errno_value = localtime_s(&time_info, &in_time_t);
	output_stream << std::put_time(&time_info, "%Y-%m-%d");

	m_filepath = m_filepath + output_stream.str() + "-";

	for (int i = 1; true; ++i) {
		if (std::filesystem::exists(m_filepath + std::to_string(i)) && std::filesystem::is_regular_file(m_filepath + std::to_string(i))) {
			continue;
		}
		m_filepath += std::to_string(i) + ".txt";
		break;
	}

		std::ofstream out(m_filepath ,std::ios::out);
		out << m_msg.str(); out.close();
	}
}

void Xenon::Logger::breakLine(logMode mode)
{
	log(mode, "=========================================================================");
}

