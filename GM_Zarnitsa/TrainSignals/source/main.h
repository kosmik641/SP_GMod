#pragma once
#include "params.h"

#ifdef DEBUG
#define _PREFAST_
#endif // DEBUG

#include <Windows.h>
#include <iostream>
#include <GarrysMod/Lua/Interface.h>
#include "TrainIODevice.h"

#define PushCFunc(_function,_name) LUA->PushCFunction(_function); LUA->SetField(-2, _name)
#define PushStr(_string,_name) LUA->PushString(_string); LUA->SetField(-2, _name)

extern TrainIODevice g_Device;
CRITICAL_SECTION* g_CriticalSection = nullptr;