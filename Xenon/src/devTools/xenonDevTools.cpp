#include"xenonDevTools.h"
#include"timer.hpp"

void Xenon::setLogColors(XN_COLOR entrtColor, XN_COLOR infoColor, XN_COLOR warrningColor, XN_COLOR errorColor)
{ Core::Logger::getInstanceClient().setColors(entrtColor, infoColor, warrningColor, errorColor); }
void Xenon::setLogToFile(const std::string& path)
{ Core::Logger::getInstanceClient().setFilePath(path); }
void Xenon::LogBR()
{ Core::Logger::getInstanceClient().breakLine(); }