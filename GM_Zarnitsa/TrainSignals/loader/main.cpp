#include "main.h"

#include <Windows.h>
#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/FactoryLoader.hpp>
#include <Color.h>
#include "vmthook.h"

namespace GM = GarrysMod::Lua;

std::vector<VMTHookData_t> g_VMTHooks;
GarrysMod::Lua::ILuaShared*    g_IfaceGMODLua = nullptr;
GarrysMod::Lua::ILuaInterface* g_IfaceLuaClient = nullptr;

RunStringExFn        g_GMODRunStringEx = nullptr;
CreateLuaInterfaceFn g_GMODCreateLuaInterface = nullptr;
CloseLuaInterfaceFn  g_GMODCloseLuaInterface = nullptr;

bool g_MetrostroiLuaLoaded = false;

std::string GMOD_FileRead(GarrysMod::Lua::ILuaBase* LUA, const char* fileName, const char* pathID)
{
	std::string data;
	int top = LUA->Top();
	LUA->PushSpecial(GM::SPECIAL_GLOB);
	{
		LUA->GetField(-1, "file");
		if (LUA->IsType(-1, GM::Type::Table))
		{
			LUA->GetField(-1, "Read");
			{
				if (LUA->IsType(-1, GM::Type::Function))
				{
					LUA->PushString(fileName);
					LUA->PushString(pathID);
					LUA->Call(2, 1);
					if (LUA->IsType(-1, GM::Type::String))
					{
						data = LUA->GetString(-1);
					}
				}
			}
		}
	}
	LUA->Pop(LUA->Top() - top);

	return data;
}

void GMOD_Include(GarrysMod::Lua::ILuaBase* LUA, const char* filename)
{
	static GM::CFunc include = nullptr;
	if (include == nullptr)
	{
		LUA->PushSpecial(GM::SPECIAL_GLOB);
		{
			LUA->GetField(-1, "include");
			if (LUA->IsType(-1, GM::Type::Function))
			{
				include = LUA->GetCFunction(-1);
			}
			LUA->Pop();
		}
		LUA->Pop();
	}

	if (include == nullptr)
		return;

	LUA->PushCFunction(include);
	LUA->PushString(filename);
	LUA->Call(1, 0);
}

void GMOD_LoadLuaFiles(GarrysMod::Lua::ILuaInterface* LUA)
{
	// Check if files already loaded
	bool trainSignalsLoaded = false;
	LUA->PushSpecial(GM::SPECIAL_GLOB);
	{
		LUA->GetField(-1, "TrainSignals");
		trainSignalsLoaded = LUA->IsType(-1, GM::Type::Table);
		LUA->Pop();
	}
	LUA->Pop();

	if (trainSignalsLoaded)
		return;

	PRINT_MSG("Include Lua files...\n");
	for (const char* file : includeLuaFiles)
	{
		std::string data = GMOD_FileRead(LUA, file, "lcl");
		if (!data.empty())
			LUA->RunStringEx(file, "", data.c_str(), true, true, true, true);
	}
}

GarrysMod::Lua::ILuaInterface* __fastcall hkCreateLuaInterface(GarrysMod::Lua::ILuaShared* _this, unsigned char luaState, bool renew)
{
	auto newLuaIface = g_GMODCreateLuaInterface(_this, luaState, renew);

	if (luaState == GM::State::CLIENT && newLuaIface != nullptr)
	{
		g_IfaceLuaClient = newLuaIface;
		if (g_GMODRunStringEx == nullptr)
		{
			PRINT_MSG("Hook LUA->RunStringEx()\n");
			g_GMODRunStringEx = VMTHook<RunStringExFn>((PVOID**)g_IfaceLuaClient, (PVOID)hkRunStringEx, 111);
		}
	}

	return newLuaIface;
}

int __fastcall hkCloseLuaInterface(GarrysMod::Lua::ILuaInterface* _this, GarrysMod::Lua::ILuaInterface* luaInterface)
{
	if (luaInterface == g_IfaceLuaClient)
	{
		PRINT_MSG("Restore LUA->RunStringEx()\n");
		VMTHook<RunStringExFn>((PVOID**)g_IfaceLuaClient, (PVOID)g_GMODRunStringEx, 111, true); // Restore original function
		g_IfaceLuaClient = nullptr;
		g_GMODRunStringEx = nullptr;
		g_MetrostroiLuaLoaded = false;
	}

	return g_GMODCloseLuaInterface(_this, luaInterface);
}

bool __fastcall hkRunStringEx(GarrysMod::Lua::ILuaInterface* LUA, const char* filename, const char* path, const char* stringToRun,
	bool run, bool printErrors, bool dontPushErrors, bool noReturns)
{
	if (!g_MetrostroiLuaLoaded)
	{
		std::string strFilename(filename);
		if (strFilename.find_last_of("lua/autorun/metrostroi.lua") != std::string::npos)
		{
			std::string strStringToRun(stringToRun);
			if (strStringToRun.find("Metrostroi = {}") != std::string::npos) // Ensure we are found real metrostroi.lua
			{
				g_MetrostroiLuaLoaded = true;
				bool loadRes = g_GMODRunStringEx(LUA, filename, path, strStringToRun.c_str(), run, printErrors, dontPushErrors, noReturns);

				if (loadRes)
					GMOD_LoadLuaFiles(LUA);

				return loadRes;
			}
		}
	}

	return g_GMODRunStringEx(LUA, filename, path, stringToRun, run, printErrors, dontPushErrors, noReturns);
}

void HookLuaFunctions()
{
	if (g_IfaceGMODLua != nullptr)
		return;

	SourceSDK::FactoryLoader lua_shared_loader("lua_shared");
	g_IfaceGMODLua = lua_shared_loader.GetInterface<GM::ILuaShared>(GMOD_LUASHARED_INTERFACE);
	if (g_IfaceGMODLua != nullptr)
	{
		g_GMODCreateLuaInterface = VMTHook<CreateLuaInterfaceFn>((PVOID**)g_IfaceGMODLua, (PVOID)hkCreateLuaInterface, 4);
		g_GMODCloseLuaInterface = VMTHook<CloseLuaInterfaceFn>((PVOID**)g_IfaceGMODLua, (PVOID)hkCloseLuaInterface, 5);

		g_IfaceLuaClient = g_IfaceGMODLua->GetLuaInterface(GM::State::CLIENT);
		if (g_IfaceLuaClient != nullptr)
		{
			g_GMODRunStringEx = VMTHook<RunStringExFn>((PVOID**)g_IfaceLuaClient, (PVOID)hkRunStringEx, 111);
			GMOD_LoadLuaFiles(g_IfaceLuaClient);
		}

		PRINT_MSG("Module loaded successfully.\n");
		PRINT_MSG("Bulded %s %s with MSVC %d\n", __DATE__, __TIME__, _MSC_VER);
	}
	else
	{
		PRINT_MSG_ERROR("Fail to get \"" GMOD_LUASHARED_INTERFACE "\" interface");
	}
}

GMOD_MODULE_OPEN()
{
	HookLuaFunctions();
	return 0;
}

GMOD_MODULE_CLOSE()
{
	RestoreVMTHooks();
	return 0;
}

BOOL WINAPI DllMain([[maybe_unused]] HINSTANCE hinstDLL, [[maybe_unused]] DWORD fdwReason, [[maybe_unused]] LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		HookLuaFunctions();
		break;
	case DLL_PROCESS_DETACH:
		RestoreVMTHooks();
		break;
	default:
		break;
	}
	return TRUE;
}
