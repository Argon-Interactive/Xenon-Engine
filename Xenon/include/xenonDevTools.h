#include"../src/core.h"
#include"../src/devTools/logger.hpp"
#include"../src/devTools/timer.hpp"

namespace Xenon
{
	template<typename ...Types>
	void LogEnt(Types&& ... Args)
	{ Core::Logger::getInstanceClient().log(Core::Logger::logMode::ent, Args...); }
	template<typename ...Types>
	void LogInf(Types&& ... Args)
	{ Core::Logger::getInstanceClient().log(Core::Logger::logMode::inf, Args...); }
	template<typename ...Types>
	void LogWar(Types&& ... Args)
	{ Core::Logger::getInstanceClient().log(Core::Logger::logMode::war, Args...); }
	template<typename ...Types>
	void LogErr(Types&& ... Args)
	{ Core::Logger::getInstanceClient().log(Core::Logger::logMode::err, Args...); }
	void XAPI setLogColors(XN_COLOR entrtColor, XN_COLOR infoColor, XN_COLOR warrningColor, XN_COLOR errorColor);
	void XAPI setLogToFile(const std::string& path);
	void XAPI LogBR(XN_COLOR color = XN_LOG_WHITE);
	void XAPI LogBR(Core::Logger::logMode mode);
}

