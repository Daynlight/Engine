#include "Logger.h"



UW::Log::Log(UW::LogType type, const std::string module, const std::string text)
  :type(type), module(module), text(text){};



std::string UW::Log::getText() const {
  return "["+ getTypeText() +"] ("+ module +"): " + text;
};



std::string UW::Log::getTypeText() const{
  switch (type){
  case UW::LogType::INFO:
    return "INFO";
  case UW::LogType::WARN:
    return "WARN";
  case UW::LogType::ERRO:
    return "ERRO";  
  default:
    return "NO TYPE";
  };

  return "NO TYPE";
};



ImVec4 UW::Log::getLogColor() const{
  switch(type){
  case UW::LogType::INFO:
    return ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
  case UW::LogType::WARN:
    return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
  case UW::LogType::ERRO:
    return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
  default:
    return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); 
  };

  return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
};






UW::Logger &UW::Logger::get(){
  static Logger instance;
  return instance;
};



void UW::Logger::info(const std::string module, const std::string text){
  data.emplace_back(UW::LogType::INFO, module, text);
};



void UW::Logger::warn(const std::string module, const std::string text){
  data.emplace_back(UW::LogType::WARN, module, text);
};



void UW::Logger::erro(const std::string module, const std::string text){
  data.emplace_back(UW::LogType::ERRO, module, text);
};



const std::vector<UW::Log>& UW::Logger::getLogs() const {
  return data;
};
