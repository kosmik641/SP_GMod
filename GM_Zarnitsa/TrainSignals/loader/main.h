#pragma once
#include <string>
#include <array>
#include <GarrysMod/Lua/LuaInterface.h>
#include <GarrysMod/Lua/LuaShared.h>

#define MSG_COLOR Color(200, 255, 100, 255)
#define MSG_COLOR_NORMAL Color(255, 255, 255, 255)
#define MSG_COLOR_ERROR Color(255, 0, 0, 255)

#define PRINT_MSG(_msg,...) ConColorMsg(MSG_COLOR, "TrainSignals loader: "); ConColorMsg(MSG_COLOR_NORMAL, _msg,__VA_ARGS__)
#define PRINT_MSG_ERROR(_msg,...) ConColorMsg(MSG_COLOR, "TrainSignals loader: "); ConColorMsg(MSG_COLOR_ERROR, _msg,__VA_ARGS__)
//#define PRINT_MSG_DBG(_msg,...) DevMsg("TrainSignals loader: "); DevMsg(_msg,__VA_ARGS__)

// Files to include via RunStringEx (TODO: use file.Find)
std::array<const char*, 7> includeLuaFiles = {
	"metrostroi/trainsignals/cl_campos.lua",
	"metrostroi/trainsignals/cl_displays_text.lua",
	"metrostroi/trainsignals/cl_mirrors.lua",
	"metrostroi/trainsignals/cl_mute_sounds.lua",
	"metrostroi/trainsignals/cl_nocircle.lua",
	"metrostroi/trainsignals/cl_trainsignals_convars.lua",
	"metrostroi/trainsignals/cl_trainsignals_module.lua",
};

// GMod functions
std::string GMOD_FileRead(GarrysMod::Lua::ILuaBase* LUA, const char* fileName, const char* pathID);
void GMOD_Include(GarrysMod::Lua::ILuaBase* LUA, const char* filename);
void GMOD_LoadLuaFiles(GarrysMod::Lua::ILuaInterface* LUA);

// Hooked functions
typedef bool(__thiscall* RunStringExFn)(GarrysMod::Lua::ILuaInterface* _this, const char* filename, const char* path, const char* stringToRun, bool run, bool printErrors, bool dontPushErrors, bool noReturns);
typedef GarrysMod::Lua::ILuaInterface* (__thiscall* CreateLuaInterfaceFn)(GarrysMod::Lua::ILuaShared* _this, unsigned char luaType, bool renew);
typedef int(__thiscall* CloseLuaInterfaceFn)(GarrysMod::Lua::ILuaInterface* _this, GarrysMod::Lua::ILuaInterface* luaInterface);

GarrysMod::Lua::ILuaInterface* __fastcall hkCreateLuaInterface(GarrysMod::Lua::ILuaShared* _this, unsigned char luaState, bool renew);
int __fastcall hkCloseLuaInterface(GarrysMod::Lua::ILuaInterface* _this, GarrysMod::Lua::ILuaInterface* luaInterface);
bool __fastcall hkRunStringEx(GarrysMod::Lua::ILuaInterface* LUA, const char* filename, const char* path, const char* stringToRun,
	bool run, bool printErrors, bool dontPushErrors, bool noReturns);
