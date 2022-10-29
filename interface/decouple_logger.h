#ifndef DECOUPLE_LOGGER_H
#define DECOUPLE_LOGGER_H

#include <functional>
#include <vector>

namespace Joger {
namespace DecoupleLogger {

// You can change it if your logs are much more longer
constexpr unsigned long LOG_MAX_LENGTH{1024}; ///< supported max log length.

/**
 * @brief Log Levels
 *
 */
enum class LogLevel { ERROR, WARNING, INFO, DEBUG };

/**
 * @brief Log func type
 *
 */
using LogFuncType = std::function<void(const LogLevel &, const std::string &)>;

class LoggerBase {
public:
  void setLoggerFunc(LogFuncType log_func) noexcept {
    m_logger_func = log_func;
    m_has_set_logger = true;
    for (auto sub_cls : m_sub_log_cls_vec) {
      if (nullptr == sub_cls) {
        continue;
      }
      sub_cls->setLoggerFunc(log_func);
    }
  }

protected:
  /**
   * @brief execute callLog if you want to show log by DecoupleLogger
   *
   * @param log_level log level in LogLevel
   * @param log_msg string message
   */
  inline void callLog(const LogLevel &log_level, const std::string &log_msg) {
    if (m_has_set_logger == false) {
      printf("* WARN: LoggerBase class hasn't setLoggerFunc yet!\n");
    }
    m_logger_func(log_level, log_msg);
  }

  /**
   * @brief execute callLog if you want to show log by DecoupleLogger
   *
   * @tparam Args arguments
   * @param log_level log level in LogLevel
   * @param fmt string format
   * @param args string args
   */
  template <typename... Args>
  inline void callLog(const LogLevel &log_level, const char *fmt,
                      const Args &...args) {
    char tmp_str[LOG_MAX_LENGTH]{0};
    snprintf(tmp_str, LOG_MAX_LENGTH, fmt, args...);
    callLog(log_level, std::string(tmp_str));
  }

  template <typename ClassType>
  void registLoggerSubClass(ClassType *sub_cls) noexcept {
    m_sub_log_cls_vec.emplace_back(dynamic_cast<LoggerBase *>(sub_cls));
  }

protected:
  bool m_has_set_logger{false};
  LogFuncType m_logger_func{[](const LogLevel &, const std::string &) {}};
  std::vector<LoggerBase *> m_sub_log_cls_vec; ///< 让setLoggerFunc传递
};

}; // namespace DecoupleLogger
}; // namespace Joger

#endif