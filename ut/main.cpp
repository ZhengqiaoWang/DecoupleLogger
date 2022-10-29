#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// for unit test
#include "decouple_logger.h"
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

TEST_CASE("test for DecoupleLogger hasn't set") {
  ClassRoom cr;
  cr.testLog();
}

TEST_CASE("test for DecoupleLogger") {
  ClassRoom cr;
  cr.setLoggerFunc([](const LogLevel &log_level, const std::string &msg) {
    if (LogLevel::ERROR == log_level) {
      // ClassRoom
      CHECK(msg == "this is the ClassRoom's log");
    } else if (LogLevel::WARNING == log_level) {
      // Chair
      CHECK(msg == "this is the Chair's log");
    } else if (LogLevel::INFO == log_level) {
      // Chair
      CHECK(msg == "this is the Table's log");
    }
  });
  cr.testLog();
}