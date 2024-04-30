#include "main.h"

using namespace GarrysMod::Lua;

LUA_FUNCTION(API_Start)
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif // SHOW_CONSOLE

	LUA->CheckType(1, Type::Number);

	if (g_Device.isConnected())
	{
		PRINT_MSG("Already connected to COM%d\n", g_Device.getPortNumber());
		return 0;
	}

	int inPortnumber = (int)LUA->GetNumber(-1);
	if (inPortnumber < 1 || inPortnumber > 254)
	{
		PRINT_MSG_ERROR("TrainSignals: Enter valid COM port number (1-254)\n");
		return 0;
	}

	g_Device.start(inPortnumber);
	return 0;
}

LUA_FUNCTION(API_Stop)
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif // SHOW_CONSOLE

	if (!g_Device.isConnected())
	{
		PRINT_MSG("Not connected\n");
		return 0;
	}
	g_Device.stop();

	PRINT_MSG("Set disconnected.\n");
	return 0;
}

LUA_FUNCTION(API_DataExchange)
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif // SHOW_CONSOLE

	if (!g_Device.isConnected())
		return 0;

	auto& tableOut = g_Device.m_NW2VarTableOutput.VarTable;
	auto& tableIn = g_Device.m_NW2VarTableInput.VarTable;

	const char* asotpStr = nullptr;
	const char* asnpStr = nullptr;

	LUA->CheckType(1, Type::Entity);		// [Train]

	// Get ASOTP text
	LUA->GetField(-1, "ASOTPText");			// [Train,ASOTPText]
	if (LUA->GetType(-1) == Type::String)
	{
		asotpStr = LUA->GetString(-1);      // [Train,ASOTPText]
	}
	else
	{
		PRINT_MSG_DBG("Fail to get IGLA string.\n");
	}
	LUA->Pop();								// [Train]

	// Get ASNP text
	LUA->GetField(-1, "ASNPText");			// [Train,ASNPText]
	if (LUA->GetType(-1) == Type::String)
	{
		asnpStr = LUA->GetString(-1);		// [Train,ASNPText]
	}
	else
	{
		PRINT_MSG_DBG("Fail to get ASNP string.\n");
	}
	LUA->Pop();								// [Train]

	// Get NW2 vars table
	LUA->GetField(-1, "GetNW2VarTable");	// [Train,ENT:GetNW2VarTable()]
	LUA->Push(1);							// [Train,ENT:GetNW2VarTable(),Train]
	LUA->Call(1, 1);						// [Train,NW2VarTable]
	LUA->CheckType(-1, Type::Table);		// [Train,NW2VarTable]
	LUA->PushNil();							// [Train,NW2VarTable,Nil]

	// -2 - key, -1 - value
	EnterCriticalSection(g_CriticalSection);
	if (asotpStr != nullptr)
	{
		g_Device.m_ASOTPText = asotpStr;
	}
	if (asnpStr != nullptr)
	{
		g_Device.m_ASNPText = asnpStr;
	}
	while (LUA->Next(-2) != 0)
	{
		NW2VarTable::ControlItem control;
		auto NW2VarName = LUA->GetString(-2);
		LUA->CheckType(-1, Type::Table);	    // [Train,NW2VarTable,Key(String),Value(Table)]
			LUA->GetField(-1, "value");			// [Train,NW2VarTable,Key(String),Value(Table),Value(Table).value]
				int valType = LUA->GetType(-1);
				if (valType == Type::Bool)
				{
					control.val = LUA->GetBool(-1);
					control.type = valType;
				}
				else if (valType == Type::Number)
				{
					control.val = (int)LUA->GetNumber(-1);
					control.type = valType;
				}
			LUA->Pop();						// [Train,NW2VarTable,Key(String),Value(Table)]
		LUA->Pop();							// [Train,NW2VarTable,Key(String)]
		LUA->CheckType(-1, Type::String);

		tableOut[NW2VarName] = control;
	}
	LeaveCriticalSection(g_CriticalSection);

	LUA->CreateTable();
	EnterCriticalSection(g_CriticalSection);
	for (auto const& [k, v] : tableIn)
	{
		if (tableOut[k].val != v.val)
		{
			LUA->PushString(k.c_str());
			if (v.type == Type::Bool)
				LUA->PushBool(v.val);
			else if (v.type == Type::Number)
				LUA->PushNumber(v.val);

			LUA->RawSet(-3);
		}
	}
	LeaveCriticalSection(g_CriticalSection);

	return 1;
}

LUA_FUNCTION(API_ForceStop)
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif // SHOW_CONSOLE
	g_Device.stop(true);
	PRINT_MSG("Set force disconnected.\n");
	return 0;
}

LUA_FUNCTION(API_LoadCalibraions)
{
	if (g_Device.isConnected())
	{
		PRINT_MSG_ERROR("Disconnect before reload calibrations!\n");
		return 0;
	}
	g_Device.loadCalibartions(LUA->GetBool(1));
	PRINT_MSG("Calibrations loaded!\n");
	return 0;
}

LUA_FUNCTION(API_IsConnected)
{
	LUA->PushBool(g_Device.isConnected());
	return 1;
}

LUA_FUNCTION(API_Version)
{
	PRINT_MSG("Bulded %s %s with MSVC %d\n", __DATE__, __TIME__, _MSC_VER);
	return 0;
}

GMOD_MODULE_OPEN()
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif // SHOW_CONSOLE
	g_CriticalSection = g_Device.getCriticalSection();
	if (!g_CriticalSection)
	{
		PRINT_MSG_ERROR("Failed to get g_CriticalSection.\n");
		return 0;
	}

	// 1 = Full path to DLL (aka "d:\games\steamlibrary\steamapps\common\garrysmod\garrysmod\lua\bin\gmcl_trainsignals_win32.dll")
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);// 2
		LUA->GetField(-1, "TrainSignals"); // 3
		LUA->CheckType(-1, Type::Table);
		LUA->CreateTable();
			PushCFunc(API_Start, "Start");
			PushCFunc(API_Stop, "Stop");
			PushCFunc(API_ForceStop, "ForceStop");
			PushCFunc(API_DataExchange, "DataExchange");
			PushCFunc(API_LoadCalibraions, "LoadCalibrations");
			PushCFunc(API_IsConnected, "IsConnected");
			PushCFunc(API_Version, "Version");
			PushStr(TRAIN_CLASSNAME, "TargetTrain");
		LUA->SetField(-2, "Module");
	LUA->Pop(2);

#ifdef SHOW_CONSOLE
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	std::cout << "TrainSignals: Console initialized!\n";
#endif

	PRINT_MSG("Module loaded successfully.\n");
	API_Version__Imp(LUA);

	return 0;
}

GMOD_MODULE_CLOSE()
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif // SHOW_CONSOLE

	g_Device.stop();

	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "TrainSignals");
			LUA->PushNil();
			LUA->SetField(-2, "Module");
		LUA->Pop();
	LUA->Pop();

#ifdef SHOW_CONSOLE
	std::cout << "TrainSignals: DLL unloaded. You can close this window.\n";
	FreeConsole();
#endif

	PRINT_MSG("Module unloaded!\n");
	if (g_Device.isConnected())
	{
		PRINT_MSG_ERROR("Warning! Thread is running.\n");
	}


	return 0;
}