// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <array>

#include "Utils/config.h"
#include "ScriptShared/ILogger.h"



namespace Engine::Utils{
enum LogType{
  INFO = 0,
  WARN = 1,
  ERRO = 2,
};



struct Log{
  LogType type;
  std::string module;
  std::string text;

  Log(LogType type, const std::string& module, const std::string& text) noexcept;
  std::string getText() const noexcept;

  std::string getTypeText() const noexcept;
  std::array<float, 4> getLogColor() const noexcept;
};



class Logger : public UW::ILogger {
private:
  std::vector<Engine::Utils::Log> data;
  size_t current_lines = 0;

public:
  static Logger& get() noexcept;

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;

private:
  Logger() noexcept;
  ~Logger() noexcept = default;
  
public:
  void info(const std::string& module, const std::string& text) noexcept;
  void warn(const std::string& module, const std::string& text) noexcept;
  void erro(const std::string& module, const std::string& text) noexcept;

  const std::vector<Log>& getLogs() const noexcept;

private:
  void checkAndTrimLog() noexcept;
  void calculateInitialLineCount() noexcept;
  void log_to_file(Log log) noexcept;
};
}; // namespace Engine::Utils
