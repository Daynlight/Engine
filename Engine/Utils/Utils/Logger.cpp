// Help me I'am Under The Water
// Copyright 2026 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#include "Logger.h"



Engine::Utils::Log::Log(Engine::Utils::LogType type, const std::string& module, const std::string& text) noexcept
  :type(type), module(module), text(text){};



std::string Engine::Utils::Log::getText() const noexcept {
  return "["+ getTypeText() +"] ("+ module +"): " + text;
};



std::string Engine::Utils::Log::getTypeText() const noexcept {
  #ifndef PRODUCTION
  switch (type){
    case Engine::Utils::LogType::INFO:
      return "INFO";
    case Engine::Utils::LogType::WARN:
      return "WARN";
    case Engine::Utils::LogType::ERRO:
      return "ERRO";  
    default:
      return "NO TYPE";
  };
  #endif

  return "NO TYPE";
};



std::array<float, 4> Engine::Utils::Log::getLogColor() const noexcept {
  #ifndef PRODUCTION
  switch(type){
    case Engine::Utils::LogType::INFO:
      return {0.0f, 0.0f, 1.0f, 1.0f};
    case Engine::Utils::LogType::WARN:
      return {1.0f, 1.0f, 0.0f, 1.0f};
    case Engine::Utils::LogType::ERRO:
      return {1.0f, 0.0f, 0.0f, 1.0f};
    default:
      return {1.0f, 1.0f, 1.0f, 1.0f}; 
  };
  #endif

  return {1.0f, 1.0f, 1.0f, 1.0f};
};






Engine::Utils::Logger &Engine::Utils::Logger::get() noexcept {
  static Logger instance;
  return instance;
};



Engine::Utils::Logger::Logger() noexcept {
  calculateInitialLineCount();
};



void Engine::Utils::Logger::info(const std::string& module, const std::string& text) noexcept {
#ifndef PRODUCTION
  data.emplace_back(Engine::Utils::LogType::INFO, module, text);
  log_to_file(data[data.size() - 1]);
#endif
};



void Engine::Utils::Logger::warn(const std::string& module, const std::string& text) noexcept {
#ifndef PRODUCTION
  data.emplace_back(Engine::Utils::LogType::WARN, module, text);
  log_to_file(data[data.size() - 1]);
#endif
};



void Engine::Utils::Logger::erro(const std::string& module, const std::string& text) noexcept {
#ifndef PRODUCTION
  data.emplace_back(Engine::Utils::LogType::ERRO, module, text);
  log_to_file(data[data.size() - 1]);
#endif
};



const std::vector<Engine::Utils::Log>& Engine::Utils::Logger::getLogs() const noexcept {
  return data;
};



void Engine::Utils::Logger::checkAndTrimLog() noexcept {
  std::ifstream infile(Engine::Config::LOG_FILE_PATH);
  if (!infile.is_open()) return;

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(infile, line)) {
    lines.push_back(line);
  }
  infile.close();

  if (lines.size() >= Engine::Config::LOGS_MAX_LINES) {
    std::ofstream outfile(Engine::Config::LOG_FILE_PATH, std::ios::trunc);
    if (outfile.is_open()) {
      size_t start_index = lines.size() - Engine::Config::LOGS_TARGET_TRIM_LINES;
      for (size_t i = start_index; i < lines.size(); ++i) {
        outfile << lines[i] << "\n";
      }
      current_lines = Engine::Config::LOGS_TARGET_TRIM_LINES;
    };
  };
};



void Engine::Utils::Logger::calculateInitialLineCount() noexcept {
  if (!std::filesystem::exists(Engine::Config::LOG_FILE_PATH)) return;
  std::ifstream infile(Engine::Config::LOG_FILE_PATH);
  std::string line;
  while (std::getline(infile, line)) {
    current_lines++;
  };
};



void Engine::Utils::Logger::log_to_file(Log log) noexcept {
  std::ofstream log_file(Engine::Config::LOG_FILE_PATH, std::ios::app);
  if (log_file.is_open()) {
    log_file << log.getText() << "\n";
    current_lines++;
    log_file.close();

    if (current_lines >= Engine::Config::LOGS_MAX_LINES) {
      checkAndTrimLog();
    };
  };
};
