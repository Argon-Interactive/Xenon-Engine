#pragma once

#include"logger.hpp"
#include"timer.hpp"

namespace Xenon {
	void setLogColors(XN_COLOR entrtColor, XN_COLOR infoColor, XN_COLOR warrningColor, XN_COLOR errorColor);
	void setLogToFile(const std::string& path);
	void LogBR(XN_COLOR color);
	void LogBR(Core::Logger::logMode mode);
}

//TODO This is awfull rewrite it asap