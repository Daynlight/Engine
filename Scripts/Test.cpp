// Help me I'am Under The Water
// Copyright 2025 Daynlight
// Licensed under the GNU General, Version 3.0.
// See LICENSE file for details.



#pragma once
#define SCRIPTNAME Script_Test
#define BUILDING_SCRIPT_DLL

#include "GameObjectScriptInterface.h"
#include <cstdio>


namespace UW{
class SCRIPTNAME : public GameObjectScriptInterface {
public:
  ~Script_Test() = default;
  
  void OnLoad(){
  };
  
  void OnUpdate(){
  };
  
  void OnDestroy(){

  };

};
};



extern "C" UW::GameObjectScriptInterface* SCRIPT_API GetScript() {
  UW::SCRIPTNAME* script = new UW::SCRIPTNAME();
  return (UW::GameObjectScriptInterface*)script;
};



extern "C" void SCRIPT_API DeleteScript(UW::GameObjectScriptInterface* script) {
  UW::SCRIPTNAME* temp_script = (UW::SCRIPTNAME*)script;
  delete temp_script;
};
