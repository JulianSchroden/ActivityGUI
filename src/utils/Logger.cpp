#include "Logger.h"

#include <stdarg.h>
#include <stdio.h>

#include <chrono>


namespace ActivityGUI
{
const char* toString(LogLevel level)
{
#define CASE(level_)      \
   case LogLevel::level_: \
      return #level_;
   switch (level)
   {
      CASE(debug);
      CASE(info);
      CASE(warning);
      CASE(error);
      CASE(noLogs);
   }
#undef CASE
   return "";
}

void log(LogLevel level, const char* file, int line, const char* format, ...)
{
   if (level >= minimumLogLevel)
   {
      using namespace std::chrono;
      auto nanosecondsSinceStart = steady_clock::now().time_since_epoch();
      auto secondsSinceStart = duration_cast<seconds>(nanosecondsSinceStart);
      nanosecondsSinceStart -= secondsSinceStart;
      printf("[%04lld.%09lld] ",
             secondsSinceStart.count(),
             nanosecondsSinceStart.count());
      printf("[%7s] %s:%d ", toString(level), file, line);

      va_list args;
      va_start(args, format);
      vprintf(format, args);
      va_end(args);

      printf("\n");
   }
}

}  // namespace ActivityGUI