#include "logger.hpp"
#include <memory>
#include <utility>

#include "devTools/logger/logger.hpp"

namespace Xenon {

Logger::Logger()
	:m_impl(std::make_unique<XNtools::Logger>(false)) {}

Logger::~Logger() = default;

Logger& Logger::getInstance() {
	static Logger instance;
	return instance;
}

template<typename T, typename ...Types>
void Logger::log(logMode mode, T first, Types&& ... args){
	m_impl->log(mode, std::forward<T>(first), std::forward<Types>(args)...);
}

void Logger::breakLine(logMode mode) {
	m_impl->breakLine(static_cast<XNtools::Logger::logMode>(mode));
}

void Logger::setColors(XN_COLOR entryColor, XN_COLOR infoColor, XN_COLOR warningColor, XN_COLOR errorColor, XN_COLOR debugColor, XN_COLOR traceColor) {
	m_impl->setColors(entryColor, infoColor, warningColor, errorColor, debugColor, traceColor);
}

void Logger::setFilePath(const char* filePath) {
	m_impl->setFilePath(filePath);
}

}
