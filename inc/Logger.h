// 
// Logger Class
//
// Message Types: Error, Warning, Info, Debug0, Debug1, Debug3
//  


#ifndef LOGGER_H
#define LOGGER_H

namespace Logger {

    enum class DebugLevel 
    { 
        LOW, 
        MED, 
        HIGH 
    };

    class Logger {

        public:


            void setDebugLevel( DebugLevel );
            /*
            void logInfo();
            void logWarning();
            void logError();
            */
            void InfoMsg(const std::string &);
            void InfoMsg(const char *, ... );
            void DebugMsg(DebugLevel , const std::string &);
            void DebugMsg(DebugLevel , const char *, ... );
            void enableDebugLogging();
            void disableDebugLogging();
            Logger();
        private:
            DebugLevel _debug_level;
            int _debug_on;
    };
}
#endif
