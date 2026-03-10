#include <PluginInfo.h>
#include "TTLEventGenerator.h" // <---- 이 부분을 "VisualizerPlugin.h"에서 바꿨어!

#include <string>

#ifdef WIN32
#include <Windows.h>
#define EXPORT __declspec (dllexport)
#else
#define EXPORT __attribute__ ((visibility ("default")))
#endif

using namespace Plugin;

#define NUM_PLUGINS 1 

extern "C" EXPORT void getLibInfo(Plugin::LibraryInfo* info)
{
   /* API version, defined by the GUI source.
   Should not be changed to ensure it is always equal to the one used in the latest codebase.
   The GUI refuses to load plugins with mismatched API versions */
   info->apiVersion = PLUGIN_API_VER;
   info->name = "TTL Event Generator"; // 가이드대로 업데이트
   info->libVersion = "0.1.0"; 
   info->numPlugins = NUM_PLUGINS;
}

extern "C" EXPORT int getPluginInfo(int index, Plugin::PluginInfo* info)
{
   switch (index)
   {
   case 0:
      info->type = Plugin::Type::PROCESSOR;
      info->processor.name = "TTL Event Generator"; // 가이드대로 업데이트
      info->processor.type = Processor::Type::FILTER; // 가이드대로 업데이트
      info->processor.creator = &(Plugin::createProcessor<TTLEventGenerator>); // 가이드대로 업데이트
      break;
   default:
      return -1;
      break;
   }
   return 0;
}

#ifdef WIN32
BOOL WINAPI DllMain(IN HINSTANCE hDllHandle, IN DWORD nReason, IN LPVOID Reserved)
{
   return TRUE;
}
#endif
