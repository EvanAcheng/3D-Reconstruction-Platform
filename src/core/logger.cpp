#include "core/logger.hpp"

#include <mutex>

namespace recon {

namespace {
std::mutex g_log_mutex;
}  // namespace

Logger& Logger::instance() {
  static Logger logger;
  return logger;
}

void Logger::setLevel(LogLevel level) {
  level_ = level;
}

LogLevel Logger::level() const {
  return level_;
}

void Logger::write(LogLevel level, const std::string& tag, const std::string& message) {
  (void)level;
  (void)tag;
  (void)message;
  // TODO(FR-M1-04): 加锁（g_log_mutex），输出 "HH:MM:SS.mmm [LEVEL] [tag] message"
  //   - 用 std::chrono::system_clock + std::put_time 取时间戳
  //   - 级别低于 level_ 的直接 return
  //   - Error 级别同时写 stderr，便于重定向时仍能看到
}

void logDebug(const std::string& tag, const std::string& message) {
  Logger::instance().write(LogLevel::Debug, tag, message);
}

void logInfo(const std::string& tag, const std::string& message) {
  Logger::instance().write(LogLevel::Info, tag, message);
}

void logWarn(const std::string& tag, const std::string& message) {
  Logger::instance().write(LogLevel::Warn, tag, message);
}

void logError(const std::string& tag, const std::string& message) {
  Logger::instance().write(LogLevel::Error, tag, message);
}

}  // namespace recon
