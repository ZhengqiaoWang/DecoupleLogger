#include "decouple_logger.h"
#include <unordered_map>
using namespace Joger::DecoupleLogger;

class Table : public LoggerBase {
public:
  void testLog() { callLog(LogLevel::INFO, "this is the %s's log", "Table"); }
};

class Chair : public LoggerBase {
public:
  void testLog() {
    callLog(LogLevel::WARNING, "this is the %s's log", "Chair");
  }
};

class ClassRoom : public LoggerBase {
public:
  ClassRoom() {
    registLoggerSubClass(&tb);
    registLoggerSubClass(&ch);
  }

  void testLog() {
    callLog(LogLevel::ERROR, "this is the ClassRoom's log");
    tb.testLog();
    ch.testLog();
  }

private:
  Table tb;
  Chair ch;
};

/**
 * @brief Use unorder_map to make the code look better
 * 
 */
static const std::unordered_map<LogLevel,
                                std::function<void(const std::string &)>>
    g_log_func{std::make_pair(LogLevel::ERROR,
                              [](const std::string &msg) {
                                printf("ERROR: %s\n", msg.c_str());
                              }),
               std::make_pair(LogLevel::WARNING,
                              [](const std::string &msg) {
                                printf("WARNING: %s\n", msg.c_str());
                              }),
               std::make_pair(LogLevel::INFO,
                              [](const std::string &msg) {
                                printf("INFO: %s\n", msg.c_str());
                              }),
               std::make_pair(LogLevel::DEBUG, [](const std::string &msg) {
                 printf("DEBUG: %s\n", msg.c_str());
               })};

int main(int argc, char const *argv[]) {
  ClassRoom cr;

  // setLoggerFunc to call logs
  cr.setLoggerFunc([](const LogLevel &log_level, const std::string &msg) {
    printf("example 1:\n");
    if (LogLevel::ERROR == log_level) {
      printf("ERROR: ");
    } else if (LogLevel::WARNING == log_level) {
      printf("WARNING: ");
    } else if (LogLevel::INFO == log_level) {
      printf("INFO: ");
    }
    printf("%s\n", msg.c_str());
  });

  // or you can write like this
  cr.setLoggerFunc([&](const LogLevel &log_level, const std::string &msg) {
    printf("example 2:\n");
    auto iter = g_log_func.find(log_level);
    if (iter == g_log_func.end()) {
      return;
    }
    iter->second(msg);
  });

  cr.testLog();
  return 0;
}
