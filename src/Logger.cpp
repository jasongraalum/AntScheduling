/*
 *  Log.cpp
 *
 *  Jason Graalum
 *  January 17, 2017
 *
 * Class for logging Error, Warning and Info messages - sure there are available packages...but why not!
 *
 *  Logger
 *
 */
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <fstream>
#include "Logger.h"

namespace Logger 
{

    Logger::Logger()
    {
        this->_debug_on = 0;
        this->_debug_level = DebugLevel::LOW;
    };
    
    void Logger::disableDebugLogging()
    {
        this->_debug_on = 0;
    }

    void Logger::enableDebugLogging()
    {
        this->_debug_on = 1;
    }

    void Logger::setDebugLevel( DebugLevel debug_level)
    {
        this->_debug_level = debug_level;
    }

    void Logger::DebugMsg( DebugLevel level, const std::string &str)
    {
        if( (this->_debug_on == 1) && (this->_debug_level >= level) )
        {
            std::cout << str << std::endl;;
        }
    }

    void Logger::InfoMsg(const std::string &str)
    {
        std::cout << str << std::endl;;
    }

    void Logger::DebugMsg( DebugLevel level, const char *format, ...)
    {
        if( (this->_debug_on == 1) && (this->_debug_level >= level) )
        {
            va_list args;
            va_start(args, format);
            vprintf (format, args);
            va_end(args);
        }
    }

    void Logger::InfoMsg(const char *format, ...)
    {
        va_list args;
        va_start(args, format);
        vprintf (format, args);
        va_end(args);
    }
}

