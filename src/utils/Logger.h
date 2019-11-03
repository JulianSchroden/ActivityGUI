#ifndef _LOGGER_h
#define _LOGGER_h

namespace ActivityGUI
{
//! Log Level of a message
enum class LogLevel
{
   debug,
   info,
   warning,
   error,
   noLogs,
};

//!
//! Get string representation of LogLevel \a level.
//!
const char* toString(LogLevel level);

//!
//! The \a minimumLogLevel defines, which debug messages are going to be print
//! out. To disable all logs, set \a minimumLogLevel to \a LogLevel::noLogs.
//!
constexpr LogLevel minimumLogLevel = LogLevel::info;

//!
//! Forward one log message to stdout.
//!
//! Don't call \a log() directly. Instead use the LOG_X macros, so the
//! \a file and \a line parameters are filled out automatically.
//!
void log(LogLevel level, const char* file, int line, const char* format, ...);

//!
//! Log a debug message in printf style.
//! A timestamp, logLevel, file and line are prepended to the message.
//!
#define LOG_D(format_, ...)                       \
   ActivityGUI::log(ActivityGUI::LogLevel::debug, \
                    __FILE__,                     \
                    __LINE__,                     \
                    (format_),                    \
                    ##__VA_ARGS__)

//!
//! Log a info message in printf style.
//! A timestamp, logLevel, file and line are prepended to the message.
//!
#define LOG_I(format_, ...)                      \
   ActivityGUI::log(ActivityGUI::LogLevel::info, \
                    __FILE__,                    \
                    __LINE__,                    \
                    (format_),                   \
                    ##__VA_ARGS__)

//!
//! Log a warning message in printf style.
//! A timestamp, logLevel, file and line are prepended to the message.
//!
#define LOG_W(format_, ...)                         \
   ActivityGUI::log(ActivityGUI::LogLevel::warning, \
                    __FILE__,                       \
                    __LINE__,                       \
                    (format_),                      \
                    ##__VA_ARGS__)

//!
//! Log a error message in printf style.
//! A timestamp, logLevel, file and line are prepended to the message.
//!
#define LOG_E(format_, ...)                       \
   ActivityGUI::log(ActivityGUI::LogLevel::error, \
                    __FILE__,                     \
                    __LINE__,                     \
                    (format_),                    \
                    ##__VA_ARGS__)

}  // namespace ActivityGUI
#endif