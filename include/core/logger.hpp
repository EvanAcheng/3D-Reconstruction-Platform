#pragma once
// 分级日志。所有模块只通过这里输出，便于统一格式与关闭调试输出。
// 对应需求：FR-M1-04（运行期基础设施）

#include <string>

namespace recon {

enum class LogLevel { Debug = 0, Info = 1, Warn = 2, Error = 3 };

class Logger {
 public:
  static Logger& instance();

  void setLevel(LogLevel level);
  LogLevel level() const;

  // 线程安全地写一行日志：时间戳 [级别] [tag] message
  void write(LogLevel level, const std::string& tag, const std::string& message);

 private:
  Logger() = default;
  LogLevel level_ = LogLevel::Info;
};

// 便捷入口
void logDebug(const std::string& tag, const std::string& message);
void logInfo(const std::string& tag, const std::string& message);
void logWarn(const std::string& tag, const std::string& message);
void logError(const std::string& tag, const std::string& message);

}  // namespace recon
